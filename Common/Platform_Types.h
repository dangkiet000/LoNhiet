/*******************************************************************************
 * @file     Platform_Types.h
 * @version  V1.00
 * $Revision: 1
 * $Date: 13/04/2018
 * @brief    This is platform types.
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

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H


/*******************************************************************************
 *  Includes
 ******************************************************************************/

 /***************************************************************************/
/* Version Check                                                           */
/***************************************************************************/
#define PLATFORM_VENDOR_ID    (0x0099u)

/* AUTOSAR Software Specification Document Version Information */
#define PLATFORM_AR_MAJOR_VERSION       (4u)
#define PLATFORM_AR_MINOR_VERSION       (3u)
#define PLATFORM_AR_PATCH_VERSION       (1u)

/* Component Version Information */
#define PLATFORM_SW_MAJOR_VERSION       (1u)
#define PLATFORM_SW_MINOR_VERSION       (0u)
#define PLATFORM_SW_PATCH_VERSION       (0u)

/*******************************************************************************
 *  MISRA Violations - Summary and analysis.
 ******************************************************************************/

/*******************************************************************************
 *  Define & Macros
 ******************************************************************************/

#define CPU_TYPE_8       8
#define CPU_TYPE_16      16
#define CPU_TYPE_32      32

#define MSB_FIRST        0    /* big endian bit ordering */
#define LSB_FIRST        1    /* little endian bit ordering */

#define HIGH_BYTE_FIRST  0    /* big endian byte ordering */
#define LOW_BYTE_FIRST   1    /* little endian byte ordering */

#ifndef TRUE
   #define TRUE   (1u)
#endif

#ifndef FALSE
   #define FALSE  (0u)
#endif

#define CPU_TYPE         CPU_TYPE_32

#define CPU_BIT_ORDER    LSB_FIRST        /*little endian bit ordering*/

#define CPU_BYTE_ORDER   LOW_BYTE_FIRST   /*little endian byte ordering*/


/*******************************************************************************
 *  TYPEDEFS
 ******************************************************************************/
typedef unsigned char         boolean;       /*        TRUE .. FALSE          */

typedef signed char           sint8;         /*        -128 .. +127           */
typedef unsigned char         uint8;         /*           0 .. 255            */
typedef signed short          sint16;        /*      -32768 .. +32767         */
typedef unsigned short        uint16;        /*           0 .. 65535          */
typedef signed int            sint32;        /* -2147483648 .. +2147483647    */
typedef unsigned int          uint32;        /*           0 .. 4294967295     */

typedef signed char           sint8_least;   /* At least 7 bit + 1 bit sign   */
typedef unsigned char         uint8_least;   /* At least 8 bit                */
typedef signed short          sint16_least;  /* At least 15 bit + 1 bit sign  */
typedef unsigned short        uint16_least;  /* At least 16 bit               */
typedef signed int            sint32_least;  /* At least 31 bit + 1 bit sign  */
typedef unsigned int          uint32_least;  /* At least 32 bit               */

typedef float                 float32;
typedef double                float64;

/* An integer type large enough to be able to hold a pointer.
   This is optional, but always supported in IAR compilers. */
/* typedef __INTPTR_T_TYPE__   intptr; */
/* typedef __UINTPTR_T_TYPE__ uintptr; */

/*******************************************************************************
 *  GLOBAL VARIABLES
 ******************************************************************************/


/*******************************************************************************
 *  FUNCTION PROTOTYPES
 ******************************************************************************/

#endif  /* PLATFORM_TYPES_H */


/*---------------------- End of File -----------------------------------------*/
