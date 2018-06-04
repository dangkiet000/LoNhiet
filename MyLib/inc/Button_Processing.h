/**************************************************************************//**
 * @file     Button_Processing.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 27/12/2016 22:52p $
 * @brief    Button Processing library.
 *
 * @note
 * Copyright (C) 2016 DangKiet Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __BUTTON_PROCESSING_H__
#define __BUTTON_PROCESSING_H__

/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "NUC200Series.h"
#include "Std_Types.h"


/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/

/*******************************************************************************
**                      TYPE DEFINITION                                       **
*******************************************************************************/
/* Definition of event type */
typedef enum ETag_Btn_EventType
{
  BTN_PRESSED_EVENT = 0,
  BTN_RELEASED_EVENT,
  BTN_HOLD_EVENT
} Btn_EventType;
 
/* Definition of status type */
typedef enum ETag_Btn_StatusType
{
  BTN_IDLE = 0,
  BTN_RELEASED,
  BTN_ONHOLD,
  BTN_PRESSED
} Btn_StatusType;

//*****************************************************************************
//
//! The structure defining a function which the scheduler will call
//! periodically.
//
//*****************************************************************************
typedef struct STag_Btn_ConfigType
{
  /* To Store time at the moment which button have just been pressed */
  uint32_t ulStartHoldTime;

  /* If button is hold longer this time, call-back pfnHoldEvent2 was called */
  uint16_t usHoldThresTime;

  /* State of button */
  Btn_StatusType enStatus;
  
  /* Call-back function */
  void (*const pfnFunction)(void);
  
  /* Call-back function was called if button is hold longer usHoldThresTime */
  void (*const pfnHoldEvent1)(void);
  
  /* Call-back function was called if button is hold longer usHoldThresTime */
  void (*const pfnHoldEvent2)(void);
}Btn_ConfigType;

/*******************************************************************************
**                      Global definition                                     **
*******************************************************************************/
extern Btn_ConfigType Btn_ConfigSet[];

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
STATIC void HW_Interrupt_GPIO_Init(void);

/*******************************************************************************
**                      API OTHER MODULE                                      **
*******************************************************************************/
extern uint32_t millis(void);

/*******************************************************************************
**                      API Function Prototypes                               **
*******************************************************************************/
void Btn_Init(void);
void Btn_MainFunction(void);



#endif //__BUTTON_PROCESSING_H__

/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/

