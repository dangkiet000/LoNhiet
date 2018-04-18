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
 * @param[in]  PageID: Flash address of Data Flash
 *             List values:  DATA_FLS_PAGE_ONE
 *                           ...
 *                           DATA_FLS_PAGE_EIGHT
 * @param[in]  TargetAddress: target address in PAGE flash memory. This address
 *             offset will be  added to the PAGE flash memory base address.
 *                           Min.: 0
 *                           Max.: FLS_PAGE_DATASIZE - 1
 * @param[in]  SourceAddressPtr: Pointer to source data buffer.
 * @param[in]  Lenght: lenght of source data array
 * @retval     E_OK Success
 * @retval     E_NOT_OK Failed
 *
 * @details    Program 32-bit data into specified address of flash
 *             We only have 8 pages (512 bytes) t store data.
 */
Std_ReturnType Fls_Write(Fls_PageType PageID, \
               Fls_AddressType TargetAddress, Fls_DataType *SourceAddressPtr, \
                 Fls_LengthType Lenght)
{
  Fls_LengthType LusCnt;
  Fls_DataType LaaPageData[FLS_PAGE_DATASIZE];
  Fls_AddressType LddPageAddr;

  if(PageID >= FLS_MAX_PAGE_NUMBER)
  {
    return E_NOT_OK;
  }

  if(TargetAddress > (FLS_PAGE_DATASIZE - 1))
  {
    return E_NOT_OK;
  }

  if(Lenght > (FLS_PAGE_DATASIZE - TargetAddress))
  {
    return E_NOT_OK;
  }

  LddPageAddr = FLS_GET_PAGE_ADDR(PageID);

  /* Disable register write-protection function */
  SYS_UnlockReg();

  /* Enable FMC ISP functions */
  FMC_Open();

  /* Read all data of page to backup */
  for(LusCnt = 0; LusCnt < FLS_PAGE_DATASIZE; LusCnt++)
  {
    LaaPageData[LusCnt] = FMC_Read(LddPageAddr + LusCnt*4);
  }

  /* Write new data to LaaPageData */
  for(LusCnt = TargetAddress; LusCnt < Lenght; LusCnt++)
  {
    LaaPageData[LusCnt] = *SourceAddressPtr;
    SourceAddressPtr++;
  }

  /* Erase all data in page (512 bytes) */
  FMC_Erase(LddPageAddr);

  /* Write whole LaaPageData to FLASH PAGE MEMORY */
  for(LusCnt = 0; LusCnt < FLS_PAGE_DATASIZE; LusCnt++)
  {
    FMC_Write(LddPageAddr + (LusCnt*4), LaaPageData[LusCnt]);
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
 * @param[in]  PageID: Flash address of Data Flash
 *             List values:  DATA_FLS_PAGE_ONE
 *                           ...
 *                           DATA_FLS_PAGE_EIGHT
 * @param[in]  SourceAddress: data point to target data array.
 *             Range: 0..FLS_PAGE_DATASIZE - 1
 * @param[out] *TargetAddressPtr: Pointer to target data buffer.
 * @param[in]  Lenght: lenght of source data array.
 *             Range: 1..FLS_PAGE_DATASIZE - SourceAddress
 * @retval     E_OK Success
 * @retval     E_NOT_OK Failed
 *
 * @details    Read data flash from address of data flash
 *             We only have 8 pages (512 bytes) t store data.
 */
Std_ReturnType Fls_Read(Fls_PageType PageID, \
               Fls_AddressType SourceAddress, Fls_DataType* TargetAddressPtr, \
                 Fls_LengthType Lenght)
{
  Fls_LengthType LusCnt;
  Fls_AddressType LddPageAddr;

  if(PageID >= FLS_MAX_PAGE_NUMBER)
  {
    return E_NOT_OK;
  }
  else
  {
    /* Do Nothing */
  }

  if(SourceAddress > (FLS_PAGE_DATASIZE - 1))
  {
    return E_NOT_OK;
  }
  else
  {
    /* Do Nothing */
  }

  if(Lenght > (FLS_PAGE_DATASIZE - SourceAddress))
  {
    return E_NOT_OK;
  }
  else
  {
    /* Do Nothing */
  }

  LddPageAddr = FLS_GET_PAGE_ADDR(PageID);
  
  /* Disable register write-protection function */
  SYS_UnlockReg();

  /* Enable FMC ISP functions */
  FMC_Open();

  for(LusCnt = SourceAddress; LusCnt < Lenght; LusCnt++)
  {
    *TargetAddressPtr = FMC_Read(LddPageAddr + LusCnt*4);
    TargetAddressPtr++;
  }

  /* Disable FMC ISP function */
  FMC_Close();

  /* Lock protected registers */
  SYS_LockReg();

  return E_OK;
}

/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/


