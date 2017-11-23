/**************************************************************************//**
 * @file     LED7Segment.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 27/12/2016 22:52p $
 * @brief    LED7Segment library.
 *
 * @note
 * Copyright (C) 2016 DangKiet Technology Corp. All rights reserved.
*****************************************************************************/

#include "NUC200Series.h"
#include "LED7Segment.h"

/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/
uint8_t GaaLEDValue[NoOfLED];
boolean GblAllLedIsOff = FALSE;
static uint8_t Guccount = 0;
/*******************************************************************************
**                      Internal Functions                                    **
*******************************************************************************/
/**
  * @brief  Interrupt service routine displays LED-7segs.
  * @param[in] None.
  * @return None.
  * @details  Interrupt service routine displays LED-7segs.
  */
void TimerLED_ISR(void)
{
  if(TIMER_GetIntFlag(TimerBaseAddr) == 1)
  {
    if(GblAllLedIsOff == FALSE)
    {
      Switch_to_LED(Guccount);
      
      LED_7Seg_Decode(GaaLEDValue[Guccount]);
      
      if(Guccount >= NoOfLED-1)
      {
        Guccount = 0;
      }
      else
      {
        Guccount++;
      }
    }
    else
    {
      TurnOffLED();
    }
    
    /* Clear TimerLED time-out interrupt flag */
    TIMER_ClearIntFlag(TimerBaseAddr);
  }
}


/**
  * @brief  Init LED-7seg.
  * @param[in] LusFrequence: 0-9.
  * @return E_OK/E_NOT_OK.
  * @details  Decode LED-7seg to display digits.
  */
Std_ReturnType Timer_LED_Init(uint16_t LusFrequence)
{
  uint32_t LulCompareValue;
  uint8_t  LucPrescaler;
  
  /* 1. Enable Timer 3 module IP clock */
  CLK_EnableModuleClock(TMR3_MODULE);

  /* 2. Select Timer 3 module clock source: HXT = 12MHz */
  CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3_S_HXT, NULL);

  /* 3. Open Timer3 in periodic mode, enable interrupt */
  TIMER_Open(TimerBaseAddr, TIMER_PERIODIC_MODE, 1);

  /*
    FreqTimer (Hz)
    Period : seconds

    Period(s) = (Perscaler +1)*(TCMP Value) / (FreqTimer)
  => TCMP Value = (Period)*(FreqTimer)/ (Perscaler +1)
     TCMP Value = (FreqTimer)/ ((Perscaler +1)*(FreqReal))
  */
  /* To find  */
  LucPrescaler = 0;
  do
  {
    LucPrescaler++;
    LulCompareValue = PERIPHERAL_CLOCK_LEDTIMER/((LucPrescaler+1)*LusFrequence);
  
    if(LucPrescaler >= MAX_PRESCALER_VALUE)
    {
      return E_NOT_OK;
    }
  }
  while((LulCompareValue < 2) || (LulCompareValue >= MAX_COMPARE_VALUE));
  
  /* 4. Clock input is divided by (prescale + 1)
     Timer Clock = 12000000/(31+1) = 375000
  */
  TIMER_SET_PRESCALE_VALUE(TimerBaseAddr, LucPrescaler);

  /* 5. Set Timer Compare Value
     FREQ = 100Hz de quet led
     => Cmp value = 375000/100 = 3750
  */
  TIMER_SET_CMP_VALUE(TimerBaseAddr, LulCompareValue);

  /* 6. Enable Timer Time-out Interrupt Feature */
  TIMER_EnableInt(TimerBaseAddr);

  /* 7. Enable interrupt in NVIC */
  NVIC_EnableIRQ(TMR3_IRQn);
  NVIC_SetPriority(TMR3_IRQn, 1);
  
  /* 8. Start count */
  TIMER_Start(TimerBaseAddr);
  
  return E_OK;
}


/**
  * @brief  Decode LED-7seg.
  * @param[in] LucLedNumber: 0-9.
  * @return None.
  * @details  Decode LED-7seg to display digits.
  */
Std_ReturnType LED_7Seg_Decode(uint8_t LedNumber)
{
  uint8_t LucLedNumber;
  
  LucLedNumber= LedNumber;
  
  switch(LucLedNumber)
  {
    case 0:
    {
      PB->DOUT |=  (BIT3);
      PB->DOUT &=~ (BIT0|BIT1|BIT2);
      PC->DOUT &=~ (BIT0|BIT2|BIT3);
      return E_OK;
    }
    case 1:
    {
      PB->DOUT |=  (BIT0|BIT1|BIT3);
      PB->DOUT &=~ (BIT2);
      PC->DOUT |=  (BIT2|BIT3);
      PC->DOUT &=~ (BIT0);
      return E_OK;
    }
    case 2:
    {
      PB->DOUT |=  (BIT1);
      PB->DOUT &=~ (BIT0|BIT2|BIT3);
      PC->DOUT |=  (BIT0);
      PC->DOUT &=~ (BIT2|BIT3);
      return E_OK;
    }
    case 3:
    {
      PB->DOUT |=  (BIT1);
      PB->DOUT &=~ (BIT0|BIT2|BIT3);
      PC->DOUT |=  (BIT2);
      PC->DOUT &=~ (BIT0|BIT3);
      return E_OK;
    }
    case 4:
    {
      PB->DOUT |=  (BIT0);
      PB->DOUT &=~ (BIT1|BIT2|BIT3);
      PC->DOUT |=  (BIT2|BIT3);
      PC->DOUT &=~ (BIT0);
      return E_OK;
    }
    case 5:
    {
      PB->DOUT |=  (BIT2);
      PB->DOUT &=~ (BIT0|BIT1|BIT3);
      PC->DOUT |=  (BIT2);
      PC->DOUT &=~ (BIT0|BIT3);
      return E_OK;
    }
    case 6:
    {
      PB->DOUT |=  (BIT2);
      PB->DOUT &=~ (BIT0|BIT1|BIT3);
      PC->DOUT &=~ (BIT0|BIT3|BIT2);
      return E_OK;
    }
    case 7:
    {
      PB->DOUT |=  (BIT1|BIT3);
      PB->DOUT &=~ (BIT0|BIT2);
      PC->DOUT |=  (BIT2|BIT3);
      PC->DOUT &=~ (BIT0);
      return E_OK;
    }
    case 8:
    {
      PB->DOUT &=~ (BIT0|BIT1|BIT2|BIT3);
      PC->DOUT &=~ (BIT0|BIT2|BIT3);
      return E_OK;
    }
    case 9:
    {
      PB->DOUT &=~ (BIT0|BIT1|BIT2|BIT3);
      PC->DOUT &=~ (BIT0|BIT2|BIT3);
      PC->DOUT |=  (BIT2);
      return E_OK;
    }
    case LED_OFF_VALUE:
    {
      /* No display */
      PB->DOUT |= (BIT0|BIT1|BIT2|BIT3);
      PC->DOUT |= (BIT0|BIT2|BIT3);
    }
    default: return E_NOT_OK;
  }
}

/**
  * @brief  Display one LED-7seg and turn off the others.
  * @param[in] LucLedPosition: 0 -> (NoOfLED - 1).
  * @return E_OK/E_NOT_OK.
  * @details  Display one LED-7seg and turn off the others.
  */
Std_ReturnType Switch_to_LED(uint8_t LucLedPosition)
{
  if(LucLedPosition < NoOfLED)
  {
    /* Turn off all LEDs */
    PA->DOUT &=~ 0x0F00;
    /* Turn on only one LED */
    PA->DOUT |= (1 << (LucLedPosition + 8));
    return E_OK;
  }
  else
  {
    return E_NOT_OK;
  }
}


/**
  * @brief  Convert int to character array.
  * @param[in] LusNumber: integer.
  * @param[in] *LpLEDValue: pointer point to character array.
  * @return None.
  * @details  Ham tach so nguyen (khong dau) thanh ung chu 
  * so rieng bietva luu vao mang.
  */
void Int_to_Array(uint16_t  LusIntNumber, uint8_t *LpLEDValue)
{
  uint16_t LusNumber;
  uint8_t LED0, LED1, LED2, LED3;
  
  LusNumber = LusIntNumber;
  LED0 = LpLEDValue[0];
  LED1 = LpLEDValue[1];
  LED2 = LpLEDValue[2];
  LED3 = LpLEDValue[3];
  
  if(LusNumber < 100) //So Number co 2 chu so
  {
    LED0 = 10;
    LED1 = 10;
    LED2 = (uint8_t) (LusNumber/10);
    LED3 = (uint8_t) (LusNumber%10);
  }
  else if(LusNumber < 1000)  //So Number co 3 chu so
  {
    LED0 = 10;
    LED1 = (uint8_t) (LusNumber/100);
    LusNumber = (LusNumber - (LED1*100));
    LED2 = (uint8_t) (LusNumber/10);
    LED3 = (uint8_t) (LusNumber%10);
  }
  else if(LusNumber < 9999)  //So Number co 4 chu so
  {
    LED0 = (uint8_t) (LusNumber/1000);
    LusNumber = LusNumber - (LED0*1000);
    LED1 = (uint8_t) (LusNumber/100);
    LusNumber = LusNumber - (LED1*100);
    LED2 = (uint8_t) (LusNumber/10);
    LED3 = (uint8_t) (LusNumber%10);
  }
  else
  { 
    LED0 = 10;
    LED1 = 10;
    LED2 = 10;
    LED3 = 10;
  }
  LpLEDValue[0] = LED0;
  LpLEDValue[1] = LED1;
  LpLEDValue[2] = LED2;
  LpLEDValue[3] = LED3;
}

/*******************************************************************************
**                      API Functions                                         **
*******************************************************************************/
/**
  * @brief  Initialize GPIO and Timer to control LED-7seg.
  * @param[in] None.
  * @return None.
  * @details  Initialize GPIO and Timer to control LED-7seg.
  */
void LED_7Seg_Init(void)
{
  /* Configure all pins to control LED-7seg */
  GPIO_SetMode(PA, BIT8|BIT9|BIT10|BIT11, GPIO_PMD_OUTPUT);
  GPIO_SetMode(PB, BIT0|BIT1|BIT2|BIT3, GPIO_PMD_OUTPUT);
  GPIO_SetMode(PC, BIT0|BIT1|BIT2|BIT3, GPIO_PMD_OUTPUT);
  
  /* Dot in Led7seg is off */
  LEDDOT = 1;
  
  /* Configure Time 150Hz use for display LED7-Seg */
  Timer_LED_Init(200);
}


/**
  * @brief  Display value by LED7segment.
  * @param[in] LusDisplayValue: 0 -> (2^NoOfLED-1).
  * @return None.
  * @details  Display value by LED7segment.
  */
void LED7Seg_Show(uint16_t  LusDisplayValue)
{
  uint16_t Lus_DisplayValue;
  
  Lus_DisplayValue = LusDisplayValue;
  
  Int_to_Array(Lus_DisplayValue, GaaLEDValue);
}
/**
  * @brief  Turn off all LED7segment.
  * @param[in] None.
  * @return None.
  * @details  Turn off all LED7segment.
  */
void TurnOffLED(void)
{
  /* Turn off all LEDs */
  PA->DOUT &=~ 0x0F00;
  LED_7Seg_Decode(LED_OFF_VALUE);
  GblAllLedIsOff = TRUE;
}
/**
  * @brief  Turn on all LED7segment.
  * @param[in] None.
  * @return None.
  * @details  Turn on all LED7segment.
  */
void TurnOnLED(void)
{
  GblAllLedIsOff = FALSE;
}
void BlinkingLED(boolean LblEnable)
{
  static uint8_t LucBlink = 0;
  if(LblEnable == STD_ON)
  {
    if(LucBlink != 0)
    {
      TurnOnLED();
    }
    else
    {
      TurnOffLED();
    }
    LucBlink ^= 1;
  }
  else
  {
    TurnOnLED();
    LucBlink = 0;
  } 
}
/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/



