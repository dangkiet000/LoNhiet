/**************************************************************************//**
 * @file     HeaterTypes.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 14/05/2018 
 * @brief    Definition Heater Types.
 *
 * @note
 *
 *
 ******************************************************************************/
#ifndef __HEATER_TYPES_H__
#define __HEATER_TYPES_H__

#include "Std_Types.h"


/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define HEATER_DATEPRODUCT_IS_CONFIGURED       24358UL
#define HEATER_DATEPRODUCT_IS_NOT_CONFIGURED   0xFFFFFFFFUL


/*******************************************************************************
**                      Types Definition                                      **
*******************************************************************************/

/* Lo Nhiet status */
typedef enum ETag_Heater_StatusType
{
  /* State which LoNhiet doesn't any task, TRIAC is not working. */
  HEATER_IDLE = 0,
  /* State which LoNhiet is working, TRIAC is controlled by MCU. */
  HEATER_BUSY,
  /* State which user is setting setpoint. */
  HEATER_UPDATE_SETPOINT,
  /* State which user is setting day. */
  HEATER_SETUP_DAY,
  /* State which user is setting month. */
  HEATER_SETUP_MON,
  /* State which Heater is waiting user setting DateTime. */
  HEATER_WAIITING_USER_SETUP_DATETIME,
  /* State which user is typing password. */
  HEATER_ENTER_PASSWORD,
  /* State which heater display working time with unit as day. */
  HEATER_WORINGTIME_DISPLAY,
  /* State which triac is died. */
  HEATER_TRIAC_DIED,
  /* State which thermal resistor is died. */
  HEATER_RESISTOR_DIED,
}Heater_StatusType;

/* Definition of LoNhiet Activation Lock Status */
typedef enum ETag_Heater_ActiLockStatusType
{
  /* State which LoNhiet is in trial time */
  LONHIET_TRIAL = 5,
  /* State which TRIAC is not working, user must correct password to UNLOCKED */
  LONHIET_LOCKED,
  /* State which user can use full feature. */
  LONHIET_UNLOCKED
}Heater_ActiLockStatusType;


/* Definition of LoNhiet Active Status Type. */
typedef enum ETag_Heater_ActiStatusType
{
  /* State which LoNhiet is in trial time */
  HEATER_ENABLE = 0,
  /* State which TRIAC is not working, user must correct password to UNLOCKED */
  HEATER_DISABLE
}Heater_ActiStatusType;

typedef struct STag_Heater_DateTimeType
{
  /* Date of product. */
  uint8 ucDay;
  
  /* Month of product. */
  uint8 ucMonth;
  
  /* Year of product. */
  uint16 usYear;
}Heater_DateTimeType;


typedef struct STag_HeaterType
{
  /* Working-time of LoNhiet (Unit: minutes) */
  uint32 ulWorkingTime;
  
  /* LoNhiet operation status */
  Heater_StatusType enOpStatus;
  
  /* LoNhiet Activation Lock Status */
  Heater_ActiLockStatusType enActiLockStatus;
  
  /* Set-point of temperature */
  uint16 usSetPoint; 
  
  /* Temperature of Thermo-Couple */
  uint16 usTempTHC;
  
  /* Store password which user type. */
  uint16 usUserPassword;
  
  /* Index of Blink LED7-Segment */
  uint8 ucBlinkLED7Idx;
  
  /* Manufacture date. */
  Heater_DateTimeType MDate;
  
  /* Heater status is disable or enable. */
  Heater_ActiStatusType enActiStatus;
  
}HeaterType;



#endif //__HEATER_TYPES_H__

/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
