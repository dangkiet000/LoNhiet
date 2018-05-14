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
typedef enum ETag_LoNhietStatusType
{
  READING_INFO_SYSTEM = 0,
  
  /* State which LoNhiet doesn't any task, TRIAC is not working */
  LONHIET_IDLE,
  /* State which LoNhiet is working, TRIAC is controlled by MCU */
  LONHIET_BUSY,
  /* State which user is setting setpoint */
  LONHIET_UPDATE_SETPOINT,
  /* State which user is setting date-time */
  LONHIET_SETUP_DATETIME,
  /* State which user is typing password */
  LONHIET_ENTER_PASSWORD,
  
}LoNhietStatusType;

/* Definition of LoNhiet Activation Lock Status */
typedef enum ETag_LoNhiet_ActivationLockStatusType
{
  /* State which LoNhiet is in trial time */
  LONHIET_TRIAL = 0,
  /* State which TRIAC is not working, user must correct password to UNLOCKED */
  LONHIET_LOCKED,
  /* State which user can use full feature. */
  LONHIET_UNLOCKED
}LoNhiet_ActiLockStatusType;

typedef struct STag_LoNhietType
{
  /* Working-time of LoNhiet (Unit: minutes) */
  uint32 ulWorkingTime;
  
  /* LoNhiet operation status */
  LoNhietStatusType enOpStatus;
  
  /* LoNhiet Activation Lock Status */
  LoNhiet_ActiLockStatusType enActiLockStatus;
  
  /* Set-point of temperature */
  sint32 slSetPoint; 
  
}LoNhietType;



#endif //__HEATER_TYPES_H__

/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
