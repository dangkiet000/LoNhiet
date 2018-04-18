/*******************************************************************************
 * @file     Fls_Cfg.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 19/04/2018 11:47p $
 * @brief    Data Flash configuration
 *
 * @note
 * Copyright (C) 2018 DangKiet Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __FLS_CFG_H__
#define __FLS_CFG_H__


/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "Std_Types.h"

/*******************************************************************************
 **                       Type definitions                                    **
 ******************************************************************************/
/* Definition pages type. */
typedef enum ETag_Fls_PageType
{
  FLS_PAGE_ONE = 0,
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
typedef uint32 Fls_AddressType;

/* Specifies the number of bytes to read/write/erase/compare. */
typedef uint32 Fls_LengthType;

/* Definition flash data type(8/16/32). It depend on MCU platform */
typedef uint32 Fls_DataType;

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

#define FLS_PAGE_DATASIZE     (FMC_FLASH_PAGE_SIZE/4U)


#endif /* __FLS_CFG_H__ */

