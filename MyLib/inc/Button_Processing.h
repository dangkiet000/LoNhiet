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
  
#define BSET_INDEX         2U
#define BCONG_INDEX        1U
#define BTRU_INDEX         0U
  
#define NUM_BUTTON_EVENTS  3U

/*******************************************************************************
**                      TYPE DEFINITION                                       **
*******************************************************************************/
/* Definition of event type */
typedef enum ETag_Btn_EventType
{
  PRESS = 0,
  RELEASE,
  HOLD
} Btn_EventType;
 
/* Definition of status type */
typedef enum ETag_Btn_StatusType
{
  IDLE = 0,
  JUST_RELEASED,
  ONHOLD,
  JUST_PRESSED
} Btn_StatusType;
//*****************************************************************************
//
//! The structure defining a function which the scheduler will call
//! periodically.
//
//*****************************************************************************
typedef struct STag_tNotification
{
   /* Tick count when this function was last called.  This field is updated
   * by the scheduler.
   */
  uint32_t ulStartHoldTime;

  /* Miliseconds
   */
  uint16_t usHoldThresTime;

  /* A flag indicating whether or not this task is active.  If true, the
   * function will be called periodically.  If false, the function is
   * disabled and will not be called.
   */
  Btn_EventType enEventType;
  
  Btn_StatusType enStatus;
  
  /* A function pointer to the user function which is to be called 
   * periodically by the Btn_MainFunction function. 
   */
  void (*pfnFunction)(void);
  /* A function pointer to the user function which is to be called 
   * periodically by the Btn_MainFunction function. 
   */
  void (*pfnHoldFunction)(void);
}tNotification;

//*****************************************************************************
//
//! This global table must be populated by the client and contains information
//! on each function that the scheduler is to call.
//
//*****************************************************************************
extern tNotification Gaa_NotificationTable[];

//*****************************************************************************
//
//! This global variable must be exported by the client.  It must contain the
//! number of entries in the Gaa_NotificationTable array.
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

