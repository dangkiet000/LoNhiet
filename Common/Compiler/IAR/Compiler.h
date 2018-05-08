/*******************************************************************************
 * @file     Compiler.h
 * @version  V1.00
 * $Revision: 1
 * $Date: 13/04/2018
 * @brief    This is compiler.
 *
 * @Encoding: UTF-8 without signature
 * @note
 *
 ******************************************************************************/

#ifndef COMPILER_H
#define COMPILER_H

/*******************************************************************************
 *  Includes
 ******************************************************************************/

//#include "Compiler_Cfg.h"

/***************************************************************************/
/* Version Check                                                           */
/***************************************************************************/
#define COMPILER_VENDOR_ID    (0x9999u) 

/* AUTOSAR Software Specification Document Version Information */
#define COMPILER_AR_MAJOR_VERSION       (4u)
#define COMPILER_AR_MINOR_VERSION       (0u)
#define COMPILER_AR_PATCH_VERSION       (0u)

/* Component Version Information */
#define COMPILER_SW_MAJOR_VERSION       (1u)
#define COMPILER_SW_MINOR_VERSION       (1u)
#define COMPILER_SW_PATCH_VERSION       (0u)

/*******************************************************************************
 *  MISRA Violations - Summary and analysis.
 ******************************************************************************/

#if defined ( __ICCARM__ ) /* IAR Embedded Workbench */

#elif defined ( __CC_ARM ) /* KeilC */
  #define INLINE         __inline                                   /*!< inline keyword for ARM Compiler       */
  #define STATIC         static
#endif

#ifndef NULL_PTR
  #define NULL_PTR  ((void *)0)
#endif
 
/*  ----------------------------------------------------------------------------
*  REVISION HISTORY
*  -----------------------------------------------------------------------------
*  Version   Date    Author   Description
*  -----------------------------------------------------------------------------
* 01.00.00     -      SD      	First Version
* ----------------------------------------------------------------------------*/
#endif /* COMPILER_H */
/*---------------------- End of File -----------------------------------------*/

