/**************************************************************************//**
 * @file     Std_Common.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 12/08/2016 3:58p $
 * @brief    Standard library.
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __STD_COMMON_H__
#define __STD_COMMON_H__

#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define ONE_MILISECOND      5000
  
/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/

/* STD011
  The Std_ReturnType (STD005) may be used with the following values (STD006):
  E_OK:     0
  E_NOT_OK: 1
*/
typedef unsigned char Std_ReturnType;
#define E_NOT_OK    1U  
#define E_OK      0U

#define STD_ON      1U
#define STD_OFF     0U

#define HIGH      1U
#define LOW     0U
typedef unsigned char       boolean;        /* for use with TRUE/FALSE       */

#ifndef TRUE                                /* conditional check */
  #define TRUE      1
#endif

#ifndef FALSE                               /* conditional check */
  #define FALSE     0
#endif

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
void Delay_ms(uint32_t nTime);
uint32_t millis(void);
void SYSTick_Init(uint32_t Lulticks);
void DelaySystemTick_ms(uint32_t LulMilisecond);

#ifdef __cplusplus
}
#endif

#endif /* End __STD_COMMON_H__ */

