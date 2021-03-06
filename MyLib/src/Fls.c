/******************************************************************************
 * @file     DataFlash.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 31/12/2016 11:47p $
 * @brief    Data Flash driver source file
 *
 * @note
 * Copyright (C) 2016 DangKiet Technology Corp. All rights reserved.
 *****************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

#include "Fls.h"

/*******************************************************************************
**                      Global data                                           **
*******************************************************************************/
/* Global pointer point to constant flash configuration. */
const Fls_DataConfigType *Fls_GpConfig;

/*******************************************************************************
**                      Internal Functions                                    **
*******************************************************************************/
/**
 * @brief      Program 32-bit data into specified address of flash
 *
 * @param[in]  u32addr  Flash address include APROM, LDROM, Data Flash
 *                      and CONFIG
 * @param[in]  u32data  32-bit Data to program
 *
 * @return     None
 *
 * @details    To program word data into Flash include APROM, LDROM, Data Flash
 *             and CONFIG.
 *             The corresponding functions in CONFIG are listed in FMC section
 *             of Technical Reference Manual.
 */
void FMC_Write(uint32_t u32addr, uint32_t u32data)
{
  FMC->ISPCMD = FMC_ISPCMD_PROGRAM;   /* Set ISP Command Code */
  /* Set Target ROM Address. The address must be word alignment. */
  FMC->ISPADR = u32addr;
  FMC->ISPDAT = u32data;              /* Set Data to Program */
  FMC->ISPTRG = 0x1;                  /* Trigger to start ISP procedure */
  /* To make sure ISP/CPU be Synchronized */
  __ISB();
  while(FMC->ISPTRG);                 /* Waiting for ISP Done */
}

/**
 * @brief       Read 32-bit Data from specified address of flash
 *
 * @param[in]   u32addr  Flash address include APROM, LDROM, Data Flash and
 *              CONFIG
 *
 * @return      The data of specified address
 *
 * @details     To read word data from Flash include APROM, LDROM, Data
 *              Flash, and CONFIG.
 */
uint32_t FMC_Read(uint32_t u32addr)
{
  FMC->ISPCMD = FMC_ISPCMD_READ; /* Set ISP Command Code */
  /* Set Target ROM Address. The address must be word alignment. */
  FMC->ISPADR = u32addr;
  FMC->ISPTRG = 0x1;             /* Trigger to start ISP procedure */
  __ISB();                       /* To make sure ISP/CPU be Synchronized */
  while(FMC->ISPTRG);            /* Waiting for ISP Done */

  return FMC->ISPDAT;
}


/**
 * @brief      Flash page erase
 *
 * @param[in]  u32addr  Flash address including APROM, LDROM, Data Flash and
 *                      CONFIG
 *
 * @details    To do flash page erase. The target address could be APROM,
 *             LDROM, Data Flash, or CONFIG.
 *             The page size is 512 bytes.
 *
 * @retval     E_OK Success
 * @retval     E_NOT_OK Erase failed
 *
 */
Std_ReturnType FMC_Erase(uint32_t u32addr)
{
  FMC->ISPCMD = FMC_ISPCMD_PAGE_ERASE; /* Set ISP Command Code */
  /* Set Target ROM Address. The address must be page alignment. */
  FMC->ISPADR = u32addr;
  FMC->ISPTRG = 0x1;                   /* Trigger to start ISP procedure */
  /* To make sure ISP/CPU be Synchronized */
  __ISB();
  while(FMC->ISPTRG);                  /* Waiting for ISP Done */

  /* Check ISPFF flag to know whether erase OK or fail. */
  if(FMC->ISPCON & FMC_ISPCON_ISPFF_Msk)
  {
    FMC->ISPCON = FMC_ISPCON_ISPFF_Msk;
    return E_NOT_OK;
  }
  return E_OK;
}

/**
  * @brief    Disable ISP Functions
  *
  * @param    None
  *
  * @return   None
  *
  * @details  This function will clear ISPEN bit of ISPCON to disable
  *           ISP function.
  */
void FMC_Close(void)
{
    FMC->ISPCON &= ~FMC_ISPCON_ISPEN_Msk;
}


/**
  * @brief    Enable FMC ISP function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  ISPEN bit of ISPCON must be set before we can use ISP commands.
  *           Therefore, To use all FMC function APIs, user needs to call
  *           FMC_Open() first to enable ISP functions.
  *
  * @note     ISP functions are write-protected. user also needs to unlock it
  *           by calling SYS_UnlockReg() before using all ISP functions.
  *
  */
void FMC_Open(void)
{
    FMC->ISPCON |=  FMC_ISPCON_ISPEN_Msk;
}

/*******************************************************************************
**                      API Functions                                         **
*******************************************************************************/
/**
 * @brief      Program 32-bit data into specified address of flash
 *
 * @param[in]  FlsId: ID of flash data in configuration.

 * @param[out] *SourceAddressPtr: Pointer to source data buffer.
 * @retval     E_OK Success
 * @retval     E_NOT_OK Failed
 *
 * @details    Program 32-bit data into specified address of flash
 *             We only have 8 pages (512 bytes) t store data.
 */
Std_ReturnType Fls_Write(Fls_DataIdType FlsId,
                         const Fls_DataType *SourceAddressPtr)
{
  /* Pointer to constant */
  const Fls_DataConfigType *pFlsData;
  Fls_LengthType LusCnt;
  Fls_DataType LaaPageData[FLS_PAGE_DATASIZE];
  
  pFlsData = Fls_GpConfig + FlsId;

  if(pFlsData->ddAddr > (FLS_PAGE_DATASIZE - 1))
  {
    return E_NOT_OK;
  }

  if(pFlsData->ddLen > (FLS_PAGE_DATASIZE - pFlsData->ddAddr))
  {
    return E_NOT_OK;
  }


  /* Disable register write-protection function */
  SYS_UnlockReg();

  /* Enable FMC ISP functions */
  FMC_Open();

  /* Read all data of page to backup */
  for(LusCnt = 0; LusCnt < FLS_PAGE_DATASIZE; LusCnt++)
  {
    LaaPageData[LusCnt] = FMC_Read(pFlsData->ulPageAddr + LusCnt*4);
  }

  /* Write new data to LaaPageData */
  for(LusCnt = pFlsData->ddAddr; \
      LusCnt < (pFlsData->ddAddr + pFlsData->ddLen); LusCnt++)
  {
    LaaPageData[LusCnt] = *SourceAddressPtr;
    SourceAddressPtr++;
  }

  /* Erase all data in page (512 bytes) */
  FMC_Erase(pFlsData->ulPageAddr);

  /* Write whole LaaPageData to FLASH PAGE MEMORY */
  for(LusCnt = 0; LusCnt < FLS_PAGE_DATASIZE; LusCnt++)
  {
    FMC_Write(pFlsData->ulPageAddr + (LusCnt*4), LaaPageData[LusCnt]);
  }

  /* Disable FMC ISP function */
  FMC_Close();

  /* Lock protected registers */
  SYS_LockReg();

  return E_OK;
}

/**
 * @brief      Read data flash from address of data flash to array
 *
 * @param[in]  FlsId: ID of flash data in configuration.
 * @param[out] *TargetAddressPtr: Pointer to target data buffer.
 * @retval     E_OK Success
 * @retval     E_NOT_OK Failed
 *
 * @details    Read data flash from address of data flash
 *             We only have 8 pages (512 bytes) t store data.
 */
Std_ReturnType Fls_Read(Fls_DataIdType FlsId, \
                        Fls_DataType* TargetAddressPtr)
{
  /* Pointer to constant */
  const Fls_DataConfigType *pFlsData;
  Fls_LengthType LusCnt;

  pFlsData = Fls_GpConfig + FlsId;
  
  if(pFlsData->ddAddr > (FLS_PAGE_DATASIZE - 1))
  {
    return E_NOT_OK;
  }
  else
  {
    /* Do Nothing */
  }

  if(pFlsData->ddLen > (FLS_PAGE_DATASIZE - pFlsData->ddAddr))
  {
    return E_NOT_OK;
  }
  else
  {
    /* Do Nothing */
  }
  
  /* Disable register write-protection function */
  SYS_UnlockReg();

  /* Enable FMC ISP functions */
  FMC_Open();

  for(LusCnt = pFlsData->ddAddr; \
      LusCnt < (pFlsData->ddAddr + pFlsData->ddLen); LusCnt++)
  {
    *TargetAddressPtr = FMC_Read(pFlsData->ulPageAddr + LusCnt*4);
    TargetAddressPtr++;
  }

  /* Disable FMC ISP function */
  FMC_Close();

  /* Lock protected registers */
  SYS_LockReg();

  return E_OK;
}

/**
 * @brief      Initialize flash configuration.
 *
 * @param[in]  *Configset: Pointer to target data buffer.
 * @param[out] None.
 * @retval     None.
 *
 * @details    Initialize flash configuration.
 */
void Fls_Init(const Fls_DataConfigType *Configset)
{
  Fls_GpConfig = Configset;
}
/**
 * @brief      DeInitialize flash configuration.
 *
 * @param[in]  None.
 * @param[out] None.
 * @retval     None.
 *
 * @details    Initialize flash configuration.
 */
void Fls_DeInit(void)
{
  Fls_GpConfig = NULL_PTR;
}

/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/


