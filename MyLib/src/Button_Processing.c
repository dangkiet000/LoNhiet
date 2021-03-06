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
#include "Button_Processing_Cfg.h"

/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/


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
    /* To check if BSET_PIN is pressed <=> falling edge */
    if(BSET_PIN == STD_LOW)
    {
      Btn_ConfigSet[BSET_ID].ulStartHoldTime = millis();
      Btn_ConfigSet[BSET_ID].enStatus = BTN_ONHOLD;
    }
    /* To check if BSET_PIN is released <=> rising edge */
    else
    {   
      /* To check if BSET_PIN Hold Event is happened or not yet */
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
    /* To check if BCONG_PIN is pressed <=> falling edge */
    if(BCONG_PIN == STD_LOW)
    {
      Btn_ConfigSet[BCONG_ID].ulStartHoldTime = millis();
      Btn_ConfigSet[BCONG_ID].enStatus = BTN_ONHOLD;
    }
    /* To check if BCONG_PIN is released <=> rising edge */
    else
    {
      /* To check if BCONG_PIN Hold Event is happened or not yet */
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
    /* To check if BTRU_PIN is pressed <=> falling edge */
    if(BTRU_PIN == STD_LOW)
    {  
      Btn_ConfigSet[BTRU_ID].ulStartHoldTime = millis();
      Btn_ConfigSet[BTRU_ID].enStatus = BTN_ONHOLD;
    }
    /* To check if BTRU_PIN is released <=> rising edge */
    else
    {
      /* To check if BTRU_PIN Hold Event is happened or not yet */
      if(Btn_ConfigSet[BTRU_ID].enStatus == BTN_ONHOLD)
      {
        Btn_ConfigSet[BTRU_ID].enStatus = BTN_RELEASED;
      }
      else 
      {
        /* Reset status */
        Btn_ConfigSet[BTRU_ID].enStatus = BTN_IDLE;
      }
    }
    
    GPIO_CLR_INT_FLAG(ButtonBaseAddr, BTRU_BIT);
  }
}


STATIC void HW_Interrupt_GPIO_Init(void)
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
  
  if(BTN_RELEASED == Btn_ConfigSet[BTRU_ID].enStatus)
  {
    Btn_ConfigSet[BTRU_ID].pfnFunction();
    /* Reset status */
    Btn_ConfigSet[BTRU_ID].enStatus = BTN_IDLE;
  }
  else if(BTN_ONHOLD == Btn_ConfigSet[BTRU_ID].enStatus)
  {
    /* To check if BTRU onhold in 6 seconds. */
    if(LulNowTick - Btn_ConfigSet[BTRU_ID].ulStartHoldTime > \
                             (Btn_ConfigSet[BTRU_ID].usHoldThresTime*2))
    {
      Btn_ConfigSet[BTRU_ID].pfnHoldEvent2();
    }
  }
  
  if(BTN_RELEASED == Btn_ConfigSet[BCONG_ID].enStatus)
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
    if((BTN_ONHOLD != Btn_ConfigSet[BCONG_ID].enStatus) && \
       (BTN_ONHOLD != Btn_ConfigSet[BTRU_ID].enStatus))
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
      Btn_ConfigSet[BCONG_ID].pfnHoldEvent1();
      /* Reset status */
      Btn_ConfigSet[BSET_ID].enStatus = BTN_IDLE;
      
      Btn_ConfigSet[BCONG_ID].enStatus = BTN_IDLE;
    }
    /* To check if BCONG_ID pressed and on-hold for > usHoldThresTime */
    else if((BTN_ONHOLD == Btn_ConfigSet[BTRU_ID].enStatus) && \
         (LulNowTick - Btn_ConfigSet[BTRU_ID].ulStartHoldTime > \
                             Btn_ConfigSet[BTRU_ID].usHoldThresTime))
    {
      /* Call the event */
      Btn_ConfigSet[BTRU_ID].pfnHoldEvent1();
      /* Reset status */
      Btn_ConfigSet[BSET_ID].enStatus = BTN_IDLE;
      
      Btn_ConfigSet[BTRU_ID].enStatus = BTN_IDLE;
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



