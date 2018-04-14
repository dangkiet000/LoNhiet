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
#include <stdio.h>
#include "NUC200Series.h"
#include "DataFlash.h"

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
 * @param[in]  LulPage  Flash address of Data Flash
 *             List values : DATA_FLS_PAGE_ONE
 *                           ...
 *                           DATA_FLS_PAGE_EIGHT
 * @param[in]  *LpData  32-bit pointer data point to source data array
 * @param[in]  LusLen  lenght of source data array
 * @retval     E_OK Success
 * @retval     E_NOT_OK Failed
 *
 * @details    Program 32-bit data into specified address of flash
 *             We only have 8 pages (512 bytes) t store data.
 */
Std_ReturnType DataFlash_Write(uint32_t LulPageAddr, uint32_t *LpData, 
                               uint16_t LusLen)
{
  uint16_t LusDataIndex;
  
  if((LulPageAddr < DATA_FLS_PAGE_ONE) ||
     (LulPageAddr > DATA_FLS_PAGE_EIGHT))
  {
    return E_NOT_OK;
  }
  /* Disable register write-protection function */
  SYS_UnlockReg();
  
  /* Enable FMC ISP functions */ 
  FMC_Open(); 

  /* Erase all data in page (512 bytes) */
  FMC_Erase(LulPageAddr);
  
  for(LusDataIndex = 0; LusDataIndex < LusLen; LusDataIndex++)
  {
    FMC_Write(LulPageAddr + LusDataIndex*4, *LpData);
    LpData++;
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
 * @param[in]  LulPage  Flash address of Data Flash
 *             List values : DATA_FLS_PAGE_ONE
 *                           ...
 *                           DATA_FLS_PAGE_EIGHT
 * @param[in]  *LpData  32-bit pointer data point to source data array
 * @param[in]  LusLen  lenght of source data array
 * @retval     E_OK Success
 * @retval     E_NOT_OK Failed
 *
 * @details    Read data flash from address of data flash
 *             We only have 8 pages (512 bytes) t store data.
 */
Std_ReturnType DataFlash_Read(uint32_t LulPageAddr, uint32_t *LpDesData, 
                               uint16_t LusLen)
{
  uint16_t LusDataIndex;
  
  if((LulPageAddr < DATA_FLS_PAGE_ONE) ||
     (LulPageAddr > DATA_FLS_PAGE_EIGHT))
  {
    return E_NOT_OK;
  }
  
  /* Disable register write-protection function */
  SYS_UnlockReg();
  
  /* Enable FMC ISP functions */ 
  FMC_Open(); 
  
  for(LusDataIndex = 0; LusDataIndex < LusLen; LusDataIndex++)
  {
    *LpDesData = FMC_Read(LulPageAddr + LusDataIndex*4);
    LpDesData++;
  }

  /* Disable FMC ISP function */
  FMC_Close();
  
  /* Lock protected registers */
  SYS_LockReg();
  
  return E_OK;
}
/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/


