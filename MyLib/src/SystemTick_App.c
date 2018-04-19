/**************************************************************************//**
 * @file     Std_Common.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 12/08/2016 3:58p $
 * @brief    Standard Common library .
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "SystemTick_App.h"

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
static volatile uint32_t GulSystemTickCount = 1;
/*******************************************************************************
**                      Interrupt Service Routine                             **
*******************************************************************************/
void SysTick_Handler(void)
{
  if((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0)
  {
    GulSystemTickCount++;  
  }
}
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
/**
  * @brief  Software delay in miliseconds.
  * @param[in] nTime: number of loop count.
  * @return None.
  * @details  Delay in miliseconds.
  */
void Delay_ms(uint32_t nTime)
{
  uint32_t i=0;
  
  for(i=0; i< nTime*ONE_MILISECOND; i++);
}

void SYSTick_Init(uint32_t Lulticks)
{
  /* Set SysTick clock source: 12MHz/2 = 6MHz */
  CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HXT_DIV2);
  
  /* Set reload register */
  SysTick->LOAD = (Lulticks & SysTick_LOAD_RELOAD_Msk) - 1; 

  /* Set Priority for Systick Interrupt */  
  NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  
  
  /* Load the SysTick Counter Value */
  SysTick->VAL = 0;        
  
  /* Select clock source SystemTick: External reference clock */
  SysTick->CTRL &=~ SysTick_CTRL_CLKSOURCE_Msk;
  
  /* Enable SysTick IRQ and SysTick Timer */
  SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;                    
                                              
}

uint32_t millis(void)
{
  uint32_t LulSystemTickCount;
  
  LulSystemTickCount = GulSystemTickCount;
  
  return (LulSystemTickCount*1000) - ((SysTick->VAL)/ONE_MS_SYSTICK);
}


void DelaySystemTick_ms(uint32_t LulMilisecond)
{
  uint32_t LulNow;
  LulNow = millis();
  
  while(millis() - LulNow < LulMilisecond);
}

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/



