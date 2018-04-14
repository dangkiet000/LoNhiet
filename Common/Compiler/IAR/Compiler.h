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

#if defined ( __ICCARM__ )

#endif  /*  __ICCARM__ */
#endif  /* COMPILER_H */

/*  ----------------------------------------------------------------------------
*  REVISION HISTORY
*  -----------------------------------------------------------------------------
*  Version   Date    Author   Description
*  -----------------------------------------------------------------------------
* 01.00.00     -      SD      	First Version
* ----------------------------------------------------------------------------*/

/*---------------------- End of File -----------------------------------------*/

