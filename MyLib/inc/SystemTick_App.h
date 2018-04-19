/*******************************************************************************
 * @file     Std_Common.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 12/08/2016 3:58p $
 * @brief    Standard library.
 *
 * @note
 *
 ******************************************************************************/
#ifndef __STD_COMMON_H__
#define __STD_COMMON_H__

/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "NUC200Series.h"
#include "Std_Types.h"

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define ONE_MILISECOND      5000U
#define SYSTICK_CLOCK       6000000UL
/* Number of cycle = 1us */
#define ONE_US_SYSTICK      (SYSTICK_CLOCK/(1000000UL))
/* Number of cycle = 1ms */
#define ONE_MS_SYSTICK      (SYSTICK_CLOCK/(1000UL))
  
/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
void Delay_ms(uint32_t nTime);
extern uint32_t millis(void);
void SYSTick_Init(uint32_t Lulticks);
void DelaySystemTick_ms(uint32_t LulMilisecond);


#endif /* End __STD_COMMON_H__ */

