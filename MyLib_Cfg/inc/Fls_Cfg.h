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
/* Used as address offset from the configured flash base address to access a
   certain flash memory area. */
typedef uint32 Fls_AddressType;

/* Specifies the number of bytes to read/write/erase/compare. */
typedef uint32 Fls_LengthType;

/* Definition flash data type(8/16/32). It depend on MCU platform */
typedef uint32 Fls_DataType;

/* Definition flash data ID */
typedef uint8 Fls_DataIdType;

typedef struct STag_Fls_DataConfigType
{
  /* Lenght of flash data. */
  Fls_LengthType ddLen;
  
  /* Address offset of flash memory area. */
  Fls_AddressType ddAddr; 
  
  /* Page Address. */
  uint32 ulPageAddr;
}Fls_DataConfigType;

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



/* Definition of Data flash index in configuration set. */
#define FLS_SETPOINT        0U
#define FLS_WORKINGTIME     1U
#define FLS_ACTILOCKSTATUS  2U
#define FLS_NGAY            3U
#define FLS_THANG           4U
#define FLS_NAM             5U
#define FLS_DATEPRODUCTSTATUS      6U
#define FLS_SERIAL_NUMBER   7U

#define MAX_FLS_ID          8U

/* Definition of configuration set ID */
#define FLS_CONFIG               (&Fls_GaaConfig[0])

#endif /* __FLS_CFG_H__ */

