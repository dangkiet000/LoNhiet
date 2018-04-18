/*******************************************************************************
 * @file     DataFlash.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 31/12/2016 11:47p $
 * @brief    Data Flash driver source file
 *
 * @note
 * Copyright (C) 2016 DangKiet Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __DATAFLASH_H__
#define __DATAFLASH_H__

/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "NUC200Series.h"
#include "Std_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 **                       Type definitions                                    **
 ******************************************************************************/
/* Definition pages type. */
typedef enum ETag_Fls_PageType
{
  FLS_PAGE_ONE = 0,
  FLS_PAGE_ONE,
  FLS_PAGE_TWO,
  FLS_PAGE_THREE,
  FLS_PAGE_FOUR,
  FLS_PAGE_FIVE,
  FLS_PAGE_SIX,
  FLS_PAGE_SEVEN,
  FLS_PAGE_EIGHT,
  FLS_MAX_PAGE_NUMBER
} Fls_PageType;

/* Used as address offset from the configured flash base address to access a
   certain flash memory area. */
typedef uint32_t Fls_AddressType;

/* Specifies the number of bytes to read/write/erase/compare. */
typedef uint32_t Fls_LengthType;

/* Definition flash data type(8/16/32). It depend on MCU platform */
typedef uint16_t Fls_DataType;

/*----------------------------------------------------------------------------*/
/*                          USER CONFIGURATION                                */
/*----------------------------------------------------------------------------*/
#define FMC_APROM_BASE          0x00000000UL    /*!< APROM  Base Address      */
#define FMC_LDROM_BASE          0x00100000UL    /*!< LDROM  Base Address      */
#define FMC_CONFIG_BASE         0x00300000UL    /*!< CONFIG Base Address      */

#define FMC_FLASH_PAGE_SIZE     0x200U       /*!< Flash Page Size (512 Bytes) */
#define FMC_LDROM_SIZE          0x1000U      /*!< LDROM Size (4K Bytes)       */



#define DATA_FLS_START_ADDR     0x0001F000UL /*!< Start address of data Flash */
#define DATA_FLS_END_ADDR       0x0001FFFFUL /*!< End address of data Flash   */

#define DATA_FLS_PAGE_ONE     DATA_FLS_START_ADDR
#define DATA_FLS_PAGE_TWO     (DATA_FLS_START_ADDR + FMC_FLASH_PAGE_SIZE)
#define DATA_FLS_PAGE_THREE   (DATA_FLS_START_ADDR + FMC_FLASH_PAGE_SIZE*2)
#define DATA_FLS_PAGE_FOUR    (DATA_FLS_START_ADDR + FMC_FLASH_PAGE_SIZE*3)
#define DATA_FLS_PAGE_FIVE    (DATA_FLS_START_ADDR + FMC_FLASH_PAGE_SIZE*4)
#define DATA_FLS_PAGE_SIX     (DATA_FLS_START_ADDR + FMC_FLASH_PAGE_SIZE*5)
#define DATA_FLS_PAGE_SEVEN   (DATA_FLS_START_ADDR + FMC_FLASH_PAGE_SIZE*6)
#define DATA_FLS_PAGE_EIGHT   (DATA_FLS_START_ADDR + FMC_FLASH_PAGE_SIZE*7)

#define FLS_PAGE_DATASIZE       (FMC_FLASH_PAGE_SIZE/4)U

/*----------------------------------------------------------------------------*/
/*                          END CONFIGURATION                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*  ISPCON constant definitions                                               */
/*----------------------------------------------------------------------------*/
/*!< ISPCON setting to select to boot from LDROM */
#define FMC_ISPCON_BS_LDROM     0x2
/*!< ISPCON setting to select to boot from APROM */
#define FMC_ISPCON_BS_APROM     0x0

/*----------------------------------------------------------------------------*/
/*  ISPCMD constant definitions                                               */
/*----------------------------------------------------------------------------*/
#define FMC_ISPCMD_READ        0x00     /*!< ISP Command: Read Flash          */
#define FMC_ISPCMD_PROGRAM     0x21     /*!< ISP Command: Program Flash       */
#define FMC_ISPCMD_PAGE_ERASE  0x22     /*!< ISP Command: Page Erase Flash    */
#define FMC_ISPCMD_VECMAP      0x2e     /*!< ISP Command: Set VECMAP          */
#define FMC_ISPCMD_READ_UID    0x04     /*!< ISP Command: Read Unique ID      */
#define FMC_ISPCMD_READ_CID    0x0B     /*!< ISP Command: Read Company ID     */
#define FMC_ISPCMD_READ_DID    0x0C     /*!< ISP Command: Read Device ID      */


/*@}*/ /* end of group FMC_EXPORTED_CONSTANTS */

/** @addtogroup FMC_EXPORTED_FUNCTIONS FMC Exported Functions
  @{
*/

/*----------------------------------------------------------------------------*/
/*  FMC Macro Definitions                                                     */
/*----------------------------------------------------------------------------*/
/**
 * @brief      Enable ISP Function
 *
 * @param      None
 *
 * @return     None
 *
 * @details    This function will set ISPEN bit of ISPCON control register to
 *             enable ISP function.
 *
 */
#define FMC_ENABLE_ISP()          (FMC->ISPCON |=  FMC_ISPCON_ISPEN_Msk)


/**
 * @brief      Disable ISP Function
 *
 * @param      None
 *
 * @return     None
 *
 * @details    This function will clear ISPEN bit of ISPCON control register to
 *             disable ISP function.
 *
 */
#define FMC_DISABLE_ISP()         (FMC->ISPCON &= ~FMC_ISPCON_ISPEN_Msk)


/**
 * @brief      Get ISP fail flag
 *
 * @param      None
 *
 * @retval     0 Previous ISP command execution result is successful
 * @retval     1 Previous ISP command execution result is fail
 *
 * @details    ISPFF flag of ISPCON is used to indicate ISP command success or
 *             fail.
 *             This function will return the ISPFF flag to identify ISP command
 *             OK or fail.
 *
 */
#define FMC_GET_FAIL_FLAG()       ((FMC->ISPCON & FMC_ISPCON_ISPFF_Msk) ? 1 : 0)


#define FLS_GET_PAGE_ADDR(PageNumber)       (DATA_FLS_START_ADDR + \
                                             (PageNumber*FMC_FLASH_PAGE_SIZE))

/*******************************************************************************
**                      Internal Functions                                    **
*******************************************************************************/
extern void FMC_Open(void);
extern void FMC_Close(void);
Std_ReturnType FMC_Erase(uint32_t u32addr);
uint32_t FMC_Read(uint32_t u32addr);
void FMC_Write(uint32_t u32addr, uint32_t u32data);

/*******************************************************************************
**                      API Functions                                         **
*******************************************************************************/
extern Std_ReturnType DataFlash_Write(Fls_PageType PageID, \
                         uint32_t TargetAddress, uint32_t *SourceAddressPtr, \
                         uint16_t Lenght);

extern Std_ReturnType DataFlash_Read(Fls_PageType PageID, \
                         uint32_t SourceAddress, uint32_t* TargetAddressPtr, \
                         uint16_t Lenght);

#ifdef __cplusplus
}
#endif


#endif

