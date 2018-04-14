/*******************************************************************************
 * @file     Std_Types.h
 * @version  V1.00
 * $Revision: 1
 * $Date: 13/04/2018
 * @brief    This is standard types.
 *
 * @Encoding: UTF-8 without signature
 * @note
 *
 ******************************************************************************/



/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*
 * V1.0.0:  13-04-2018  : Initial Version
 */
/******************************************************************************/
#ifndef STD_TYPES_H
#define STD_TYPES_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Compiler.h"                  /* mapping compiler specific keywords */
#include "Platform_Types.h"            /* platform specific type definitions */

/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/
/*
 * AUTOSAR specification version information
 */
#define STD_TYPES_AR_RELEASE_MAJOR_VERSION     4
#define STD_TYPES_AR_RELEASE_MINOR_VERSION     0
#define STD_TYPES_AR_RELEASE_REVISION_VERSION  1

/*
 * File version information
 */
#define STD_TYPES_SW_MAJOR_VERSION  1
#define STD_TYPES_SW_MINOR_VERSION  0
#define STD_TYPES_SW_PATCH_VERSION  0

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/

/* for OSEK compliance this typedef has been added */
#ifndef STATUSTYPEDEFINED
  #define STATUSTYPEDEFINED
  #define E_OK      0U
  typedef unsigned char StatusType;
#endif

/* STD011
  The Std_ReturnType (STD005) may be used with the following values (STD006):
  E_OK:     0
  E_NOT_OK: 1
*/
typedef uint8 Std_ReturnType;
#define E_NOT_OK    1U

typedef struct
{
  uint16  vendorID;
  uint16  moduleID;
  uint8  sw_major_version;
  uint8  sw_minor_version;
  uint8  sw_patch_version;
} Std_VersionInfoType; /* STD015                                */


#define STD_HIGH    1U  /* Physical state 5V or 3.3V             */
#define STD_LOW     0U  /* Physical state 0V                     */

#define STD_ACTIVE  1U  /* Logical state active                  */
#define STD_IDLE    0U  /* Logical state idle                    */

#define STD_ON      1U
#define STD_OFF     0U

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

#endif  /* STD_TYPES_H */

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
