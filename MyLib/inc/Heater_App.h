/**************************************************************************//**
 * @file     Heater_App.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 26/05/2018 3:59p $
 * @brief    Heater application header file.
 *
 * @note
*****************************************************************************/
#ifndef __HEATER_APP_H__
#define __HEATER_APP_H__

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include <stdio.h>
#include "SystemTick_App.h"

#include "Temperature_Conversion.h"
#include "Get_ADC_Value.h"
#include "Scheduler.h"
#include "LED7Segment.h"
#include "Button_Processing.h"
#include "Fls.h"
#include "TimeOut.h"
#include "DateTime_Validation.h"

#include "Heater_Types.h"
#include "LoNhiet_UserCfg.h"
#include "LoNhiet_DevCfg.h"


/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/

#define LIMIT_MAX_SETPOINT(SPvar)  ((SPvar > MAX_TEMP_TYPE_K) ? MAX_TEMP_TYPE_K : SPvar)

#define HEATER_IS_NOT_SETTING_MODE(HeaterMode)     \
         ((HeaterMode != HEATER_UPDATE_SETPOINT) && \
          (HeaterMode != HEATER_ENTER_PASSWORD) && \
          (HeaterMode != HEATER_SETUP_DAY) && \
          (HeaterMode != HEATER_SETUP_MON))
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/* Lo Nhiet Global structure */
extern HeaterType Heater;

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
void SYS_Init(void);
void PORT_Init(void);
void Buttons_Init(void);
void Timer0_Init(void);

/* Read Heater data which store in data flash memory. */
void Heater_ReadFlsData(HeaterType *pHeater, Fls_DataIdType FlsId);

/* Store Heater data to flash memory. */
void Heater_StoreFlsData(HeaterType *pHeater, Fls_DataIdType FlsId);

/* Blinking LED7-Seg in milisecond synchronously. */
void BlinkingAllLED7_Synchronous(uint32 duration);

/* Check date of product is configured or not. */
boolean Heater_DateProductIsConfigured(void);


void Enter_HEATER_UPDATE_SETPOINT_mode(void);
void Exit_HEATER_UPDATE_SETPOINT_mode(boolean IsTimeOut);

void Enter_HEATER_ENTER_PASSWORD_mode(void);
void Exit_HEATER_ENTER_PASSWORD_mode(boolean IsTimeOut);

void Enter_HEATER_SETUP_MON_mode(void);
void Exit_HEATER_SETUP_MON_mode(boolean IsTimeOut);

void Enter_HEATER_SETUP_DAY_mode(void);
void Exit_HEATER_SETUP_DAY_mode(boolean IsTimeOut);

/* Button call-back function prototypes */
void BSET_HoldToThres_Event(void);
void BSET_BCONG_HoldToThres_Event(void);
void BSET_BTRU_HoldToThres_Event(void);
void BCONG_Release_Event(void);
void BTRU_Release_Event(void);
void BSET_Release_Event(void);


#if (DEBUG_MODE == STD_ON)
Std_ReturnType SYS_CheckResetSrc(void);
void UART1_Init(void);

#if (DATA_LOGGING == STD_ON)
void Send_ADCtoPC(uint16_t *Data, uint16_t Len);
void Send_TemptoPC(void);
#endif

#endif


#if (TEST_MODE == STD_ON)
void SpeakerTest(void);
void TriacTest(void);
void Led7segTest(void);
#endif
#ifdef __cplusplus
extern "C"
{
#endif


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

#endif //__HEATER_APP_H__

