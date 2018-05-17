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

#include "Button_Processing.h"

/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/
Btn_ConfigType Btn_ConfigSet[NUM_BUTTON_EVENTS];


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
    if(BSET == STD_LOW)
    {
      Btn_ConfigSet[BSET_ID].ulStartHoldTime = millis();
      Btn_ConfigSet[BSET_ID].enStatus = BTN_ONHOLD;
    }
    /* To check if BSET is released <=> rising edge */
    else
    {   
      /* To check if BSET Hold Event is happened or not yet */
      if(Btn_ConfigSet[BSET_ID].enStatus == BTN_ONHOLD)
      {
        /* This is button released event */
        Btn_ConfigSet[BSET_ID].enStatus = BTN_RELEASED;
      }
      else 
      {
        /* Reset status */
        Btn_ConfigSet[BSET_ID].enStatus = BTN_IDLE;
      }
    }
    
    GPIO_CLR_INT_FLAG(ButtonBaseAddr, BSET_BIT);
  }
  /* To check if Button CONG interrupt occurred */
  else if(GPIO_GET_INT_FLAG(ButtonBaseAddr, BCONG_BIT))
  {
    /* To check if BCONG is pressed <=> falling edge */
    if(BCONG == STD_LOW)
    {
      Btn_ConfigSet[BCONG_ID].ulStartHoldTime = millis();
      Btn_ConfigSet[BCONG_ID].enStatus = BTN_ONHOLD;
    }
    /* To check if BCONG is released <=> rising edge */
    else
    {
      /* To check if BCONG Hold Event is happened or not yet */
      if(Btn_ConfigSet[BCONG_ID].enStatus == BTN_ONHOLD)
      {
        Btn_ConfigSet[BCONG_ID].enStatus = BTN_RELEASED;
      }
      else 
      {
        /* Reset status */
        Btn_ConfigSet[BCONG_ID].enStatus = BTN_IDLE;
      }
    }
    
    GPIO_CLR_INT_FLAG(ButtonBaseAddr, BCONG_BIT);
  }
  /* To check if Button TRU interrupt occurred */
  else if(GPIO_GET_INT_FLAG(ButtonBaseAddr, BTRU_BIT))
  {
    /* To check if BTRU is pressed <=> falling edge */
    if(BTRU == STD_LOW)
    {
      
    }
    /* To check if BTRU is released <=> rising edge */
    else
    {
      Btn_ConfigSet[BTRU_ID].enStatus = BTN_RELEASED;
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
void Btn_Init(void)
{
  uint8_t LucBtnID;
  Btn_ConfigType *pEvent;
  
  for(LucBtnID = 0; LucBtnID < NUM_BUTTON_EVENTS; LucBtnID++)
  {
    /* Get a pointer to the task information. */
    pEvent = &Btn_ConfigSet[LucBtnID];

    pEvent->enStatus = BTN_IDLE;
  }
  
  /* Enable interrupt by rising and falling edge trigger. */
  HW_Interrupt_GPIO_Init();
}

/**
  * @brief  Check status button to call notification function.
  * @param[in] None.
  * @return None.
  * @details  Check status button to call notification function.
  *           This function must be called periodically by the user. 
  */
void Btn_MainFunction(void)
{
  uint32_t LulNowTick;

  /* Get current time */
  LulNowTick = millis();
  
  if(Btn_ConfigSet[BTRU_ID].enStatus == BTN_RELEASED)
  {
    Btn_ConfigSet[BTRU_ID].pfnFunction();
    /* Reset status */
    Btn_ConfigSet[BTRU_ID].enStatus = BTN_IDLE;
  }
  else
  {
  
  }
  
  if(Btn_ConfigSet[BCONG_ID].enStatus == BTN_RELEASED)
  {
    Btn_ConfigSet[BCONG_ID].pfnFunction();
    /* Reset status */
    Btn_ConfigSet[BCONG_ID].enStatus = BTN_IDLE;
  }
  else
  {
  
  }
  /* To check if BSET_ID pressed and on-hold for > usHoldThresTime */
  if((BTN_ONHOLD == Btn_ConfigSet[BSET_ID].enStatus) && \
         (LulNowTick - Btn_ConfigSet[BSET_ID].ulStartHoldTime > \
                             Btn_ConfigSet[BSET_ID].usHoldThresTime))
  {
    if(BTN_ONHOLD != Btn_ConfigSet[BCONG_ID].enStatus)
    {
      Btn_ConfigSet[BSET_ID].pfnHoldEvent1();
      /* Reset status */
      Btn_ConfigSet[BSET_ID].enStatus = BTN_IDLE;
    }
    /* To check if BCONG_ID pressed and on-hold for > usHoldThresTime */
    else if((BTN_ONHOLD == Btn_ConfigSet[BCONG_ID].enStatus) && \
         (LulNowTick - Btn_ConfigSet[BCONG_ID].ulStartHoldTime > \
                             Btn_ConfigSet[BCONG_ID].usHoldThresTime))
    {
      /* Call the event */
      Btn_ConfigSet[BSET_ID].pfnHoldEvent2();
      /* Reset status */
      Btn_ConfigSet[BSET_ID].enStatus = BTN_IDLE;
    }
    else
    {
      /* Do Nothing */
    }
  }
  else if(BTN_RELEASED == Btn_ConfigSet[BSET_ID].enStatus)
  {
    /* Call the event */
    Btn_ConfigSet[BSET_ID].pfnFunction();
    /* Reset status */
    Btn_ConfigSet[BSET_ID].enStatus = BTN_IDLE;
  }
  else
  {
    /* Do Nothing */
  }
}
/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/



