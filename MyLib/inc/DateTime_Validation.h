/**************************************************************************//**
 * @file     DateTime_Validation.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 26/05/2018 3:59p $
 * @brief    DateTime Validation header file.
 *
 * @note
*****************************************************************************/
#ifndef __DATETIME_VALIDATION_H__
#define __DATETIME_VALIDATION_H__

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Heater_Types.h"



/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define MIN_DAY_ONES       1U
#define MIN_DAY_TENS       0U
#define MIN_MON_ONES       1U
#define MIN_MON_TENS       0U

#define THOUSANDS      0U
#define HUNDREDS       1U
#define TENS           2U
#define ONES           3U

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/


/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
void Heater_MonPlus(Heater_DateTimeType *DateTime, uint8 Place);
void Heater_MonMinus(Heater_DateTimeType *DateTime, uint8 Place);
void Heater_DayPlus(Heater_DateTimeType *DateTime, uint8 Place);
void Heater_DayMinus(Heater_DateTimeType *DateTime, uint8 Place);

boolean DateTime_CheckLeapYear(uint16 year);
uint8 Heater_GetMaxDay(Heater_DateTimeType DateTime);

/** @addtogroup None
  @{
*/

/** @addtogroup None
  @{
*/

/** @addtogroup None
  @{
*/


/*@}*/ /* None */

/*@}*/ /* None */

/*@}*/ /* None */

#ifdef __cplusplus
}
#endif

#endif //__DATETIME_VALIDATION_H__

