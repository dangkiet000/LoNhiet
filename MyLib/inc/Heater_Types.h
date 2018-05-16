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


/*******************************************************************************
**                      Types Definition                                      **
*******************************************************************************/

/* Lo Nhiet status */
typedef enum ETag_Heater_StatusType
{
  /* State which LoNhiet doesn't any task, TRIAC is not working */
  HEATER_IDLE = 0,
  /* State which LoNhiet is working, TRIAC is controlled by MCU */
  HEATER_BUSY,
  /* State which user is setting setpoint */
  HEATER_UPDATE_SETPOINT,
  /* State which user is setting date-time */
  HEATER_SETUP_DATETIME,
  /* State which user is typing password */
  HEATER_ENTER_PASSWORD,
  
}Heater_StatusType;

/* Definition of LoNhiet Activation Lock Status */
typedef enum ETag_Heater_ActiLockStatusType
{
  /* State which LoNhiet is in trial time */
  LONHIET_TRIAL = 0,
  /* State which TRIAC is not working, user must correct password to UNLOCKED */
  LONHIET_LOCKED,
  /* State which user can use full feature. */
  LONHIET_UNLOCKED
}Heater_ActiLockStatusType;

typedef struct STag_HeaterType
{
  /* Working-time of LoNhiet (Unit: minutes) */
  uint32 ulWorkingTime;
  
  /* LoNhiet operation status */
  Heater_StatusType enOpStatus;
  
  /* LoNhiet Activation Lock Status */
  Heater_ActiLockStatusType enActiLockStatus;
  
  /* Set-point of temperature */
  sint32 slSetPoint; 
  
}HeaterType;



#endif //__HEATER_TYPES_H__

/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
