/**************************************************************************//**
 * @file     Button_Processing.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 27/12/2016 22:52p $
 * @brief    Button Processing library.
 *
 * @note
 * Copyright (C) 2016 DangKiet Technology Corp. All rights reserved.
*****************************************************************************/

#include "NUC200Series.h"
#include "Button_Processing.h"

/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/
tNotification Gaa_NotificationTable[NUM_BUTTON_EVENTS];


/*******************************************************************************
**                      Internal Functions                                    **
*******************************************************************************/
/**
  * @brief  Interrupt service routine is happen when buton is released or 
  *         pressed.
  * @param[in] None.
  * @return None.
  * @details  Interrupt service routine is happen when buton is released or 
  *           pressed.
  */
void Button_ISR(void)
{
  /* To check if Button SET interrupt occurred */
  if(GPIO_GET_INT_FLAG(ButtonBaseAddr, BSET_BIT))
  {
    /* To check if BSET is pressed <=> falling edge */
    if(BSET == LOW)
    {
      Gaa_NotificationTable[BSET_INDEX].ulStartHoldTime = millis();
      Gaa_NotificationTable[BSET_INDEX].enStatus = ONHOLD;
    }
    /* To check if BSET is released <=> rising edge */
    else
    {
      /* To check if BSET Hold Event is happened or not yet */
      if(Gaa_NotificationTable[BSET_INDEX].enStatus == ONHOLD)
      {
        Gaa_NotificationTable[BSET_INDEX].enStatus = JUST_RELEASED;
      }
      else 
      {
        /* Reset status */
        Gaa_NotificationTable[BSET_INDEX].enStatus = IDLE;
      }
    }
    
    GPIO_CLR_INT_FLAG(ButtonBaseAddr, BSET_BIT);
  }
  /* To check if Button CONG interrupt occurred */
  else if(GPIO_GET_INT_FLAG(ButtonBaseAddr, BCONG_BIT))
  {
    /* To check if BCONG is pressed <=> falling edge */
    if(BCONG == LOW)
    {
      
    }
    /* To check if BCONG is released <=> rising edge */
    else
    {
      Gaa_NotificationTable[BCONG_INDEX].enStatus = JUST_RELEASED;
    }
    
    GPIO_CLR_INT_FLAG(ButtonBaseAddr, BCONG_BIT);
  }
  /* To check if Button TRU interrupt occurred */
  else if(GPIO_GET_INT_FLAG(ButtonBaseAddr, BTRU_BIT))
  {
    /* To check if BTRU is pressed <=> falling edge */
    if(BTRU == LOW)
    {
      
    }
    /* To check if BTRU is released <=> rising edge */
    else
    {
      Gaa_NotificationTable[BTRU_INDEX].enStatus = JUST_RELEASED;
    }
    
    GPIO_CLR_INT_FLAG(ButtonBaseAddr, BTRU_BIT);
  }
}


void HW_Interrupt_GPIO_Init(void)
{
  /* Configure PA.2/3/4 as Input mode */
  GPIO_SetMode(ButtonBaseAddr, BTRU_BIT|BCONG_BIT|BSET_BIT, GPIO_PMD_INPUT);
  
  /* Enable the interrupt de-bounce function */
  GPIO_ENABLE_DEBOUNCE(ButtonBaseAddr, BTRU_BIT|BCONG_BIT|BSET_BIT);
  
  /* Set the interrupt de-bounce sampling cycle time */
  GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_256);
  
  /* Enable interrupt by rising and falling edge trigger */
  GPIO_EnableInt(ButtonBaseAddr, 2, GPIO_INT_BOTH_EDGE);
  GPIO_EnableInt(ButtonBaseAddr, 3, GPIO_INT_BOTH_EDGE);
  GPIO_EnableInt(ButtonBaseAddr, 4, GPIO_INT_BOTH_EDGE);
  NVIC_EnableIRQ(GPAB_IRQn);
  
  NVIC_SetPriority(GPAB_IRQn, 2);
}



/*******************************************************************************
**                      API Functions                                         **
*******************************************************************************/
/**
  * @brief  Initialize to configure button as input and interrupt service 
  *         routine.
  * @param[in] None.
  * @return None.
  * @details  Initialize to configure button as input and interrupt service 
  *           routine.
  */
void ButtonsProcessing_Init(void)
{
  uint8_t LucNotifyIndex;
  tNotification *pEvent;
  
  for(LucNotifyIndex = 0; LucNotifyIndex < NUM_BUTTON_EVENTS; LucNotifyIndex++)
  {
    /* Get a pointer to the task information. */
    pEvent = &Gaa_NotificationTable[LucNotifyIndex];

    pEvent->enStatus = IDLE;
  }
  HW_Interrupt_GPIO_Init();
}

/**
  * @brief  Check status button to call notification function.
  * @param[in] None.
  * @return None.
  * @details  Check status button to call notification function.
  *           This function must be called periodically by the user. 
  */
void ButtonsPoll(void)
{
  uint8_t LucNotifyIndex;
  uint32_t LulNowTick;
  tNotification *pEvent;

  /* Get current time */
  LulNowTick = millis();

  /* Loop through each call-back function in the notification table */
  for(LucNotifyIndex = 0; LucNotifyIndex < NUM_BUTTON_EVENTS; LucNotifyIndex++)
  {
    /* Get a pointer to the notification information */
    pEvent = &Gaa_NotificationTable[LucNotifyIndex];

    /* To check event type and status button */
    if((pEvent->enEventType == HOLD) && (pEvent->enStatus == ONHOLD) &&
       (LulNowTick - pEvent->ulStartHoldTime > pEvent->usHoldThresTime) ||
    ((pEvent->enStatus == JUST_RELEASED) && (pEvent->enEventType == RELEASE)) ||
    ((pEvent->enStatus == JUST_PRESSED) && (pEvent->enEventType == PRESS)))
    {
      /* Call the event */
      pEvent->pfnFunction();
      /* Reset status */
      pEvent->enStatus = IDLE;
    }
    else if((pEvent->enEventType == HOLD) && (pEvent->enStatus == RELEASE))
    {
      /* Call the event */
      pEvent->pfnHoldFunction();
      /* Reset status */
      pEvent->enStatus = IDLE;
    }
  }
}
/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/



