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
#define Button_ISR         GPAB_IRQHandler
  
#define ButtonBaseAddr     PA 
  
#define BSET               PA4
#define BCONG              PA2
#define BTRU               PA3
  
#define BSET_BIT           BIT4
#define BCONG_BIT          BIT2
#define BTRU_BIT           BIT3
  
#define BSET_ID         2U
#define BCONG_ID        1U
#define BTRU_ID         0U
  
#define NUM_BUTTON_EVENTS  3U

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

  /* If button is hold longer this time, call-back pfnHoldFunction2 was called */
  uint16_t usHoldThresTime;

  /* This type of button event */
  Btn_EventType enEventType;
  
  /* State of button */
  Btn_StatusType enStatus;
  
  /* Call-back function */
  void (*pfnFunction)(void);
  
  /* Call-back function was called if button is hold longer usHoldThresTime */
  void (*pfnHoldFunction1)(void);
  
  /* Call-back function was called if button is hold longer usHoldThresTime */
  void (*pfnHoldFunction2)(void);
}Btn_ConfigType;

//*****************************************************************************
//
//! This global table must be populated by the client and contains information
//! on each function that the scheduler is to call.
//
//*****************************************************************************
extern Btn_ConfigType Btn_ConfigSet[];

//*****************************************************************************
//
//! This global variable must be exported by the client.  It must contain the
//! number of entries in the Btn_ConfigSet array.
//
//*****************************************************************************

  
  
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

void HW_Interrupt_GPIO_Init(void);
/*******************************************************************************
**                      API Function Prototypes                               **
*******************************************************************************/
void Btn_Init(void);
void Btn_MainFunction(void);

extern uint32_t millis(void);

#endif //__BUTTON_PROCESSING_H__

/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/

