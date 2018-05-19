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

#include "LED7Segment.h"

/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/
uint8 GaaLED7Value[MAX_NUM_LED7];
boolean GblAllLedIsOff = FALSE;
static uint8 Guccount = 0;

const LED7_IdType GaaMapNumberToLEDPos[MAX_NUM_LED7] = {
  LED7_0, 
  LED7_1,
  LED7_2, 
  LED7_3
};

/* Global struct: LED7 driver */
LED7_DriverType LED7_GstDriver =
{
  /* enDriverStatus */
  LED7_UNINIT,
  /* ucBlinkLED */
  0,
  /* ucBlinkLEDStatus */
  LED7_BLINK_IS_OFF,
  /* ulBlinkTime */
  0,
  /* ulStartTick */
  0
};

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
  LED7_IdType LEDPos;
  if(TIMER_GetIntFlag(TimerBaseAddr) == 1)
  {
    LEDPos = GaaMapNumberToLEDPos[Guccount];

    /* Turn off all LEDs first. */
    LED7_TurnOff(LED7_ALL);

    if(LED7_GstDriver.enDriverStatus == LED7_NORMAL)
    {
      /* Turn on LED */
      LED7_TurnOn(LEDPos);
    }
    else if(LED7_GstDriver.enDriverStatus == LED7_BLINKING)
    {
      /* To check if this LED7 is blink LED or not. */
      if((LEDPos & LED7_GstDriver.ucBlinkLED) == 0)
      {
        /* No. Turn on LED */
        LED7_TurnOn(LEDPos);
      }
      else /* Yes. This is blink LED. */
      {
        if(LED7_GstDriver.ucBlinkLEDStatus == LED7_BLINK_IS_ON)
        {
          /* NO. Turn on LED */
          LED7_TurnOn(LEDPos);
        }
        else
        {
          
        }
      }
      /* This LED7 is enabled blinking */
      LED7_MainFunction();
    }
    
    LED_7Seg_Decode(GaaLED7Value[Guccount]);
    
    /* Check to reset count. */
    if(Guccount >= (MAX_NUM_LED7 - 1))
    {
      Guccount = 0;
    }
    else
    {
      Guccount++;
    }
    
    /* Clear TimerLED time-out interrupt flag */
    TIMER_ClearIntFlag(TimerBaseAddr);
  }
}


/**
  * @brief  Init LED-7seg.
  * @param[in] LusFrequence: 0-9.
  * @return  E_OK/E_NOT_OK.
  * @details  Decode LED-7seg to display digits.
  */
Std_ReturnType Timer_LED_Init(uint16 LusFrequence)
{
  uint32_t LulCompareValue;
  uint8  LucPrescaler;
  
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
  * @return  None.
  * @details  Decode LED-7seg to display digits.
  */
void LED_7Seg_Decode(uint8 LedNumber)
{
  switch(LedNumber)
  {
    case 0:
    {
      PB->DOUT |=  (BIT3);
      PB->DOUT &=~ (BIT0|BIT1|BIT2);
      PC->DOUT &=~ (BIT0|BIT2|BIT3);
      break;
    }
    case 1:
    {
      PB->DOUT |=  (BIT0|BIT1|BIT3);
      PB->DOUT &=~ (BIT2);
      PC->DOUT |=  (BIT2|BIT3);
      PC->DOUT &=~ (BIT0);
      break;
    }
    case 2:
    {
      PB->DOUT |=  (BIT1);
      PB->DOUT &=~ (BIT0|BIT2|BIT3);
      PC->DOUT |=  (BIT0);
      PC->DOUT &=~ (BIT2|BIT3);
      break;
    }
    case 3:
    {
      PB->DOUT |=  (BIT1);
      PB->DOUT &=~ (BIT0|BIT2|BIT3);
      PC->DOUT |=  (BIT2);
      PC->DOUT &=~ (BIT0|BIT3);
      break;
    }
    case 4:
    {
      PB->DOUT |=  (BIT0);
      PB->DOUT &=~ (BIT1|BIT2|BIT3);
      PC->DOUT |=  (BIT2|BIT3);
      PC->DOUT &=~ (BIT0);
      break;
    }
    case 5:
    {
      PB->DOUT |=  (BIT2);
      PB->DOUT &=~ (BIT0|BIT1|BIT3);
      PC->DOUT |=  (BIT2);
      PC->DOUT &=~ (BIT0|BIT3);
      break;
    }
    case 6:
    {
      PB->DOUT |=  (BIT2);
      PB->DOUT &=~ (BIT0|BIT1|BIT3);
      PC->DOUT &=~ (BIT0|BIT3|BIT2);
      break;
    }
    case 7:
    {
      PB->DOUT |=  (BIT1|BIT3);
      PB->DOUT &=~ (BIT0|BIT2);
      PC->DOUT |=  (BIT2|BIT3);
      PC->DOUT &=~ (BIT0);
      break;
    }
    case 8:
    {
      PB->DOUT &=~ (BIT0|BIT1|BIT2|BIT3);
      PC->DOUT &=~ (BIT0|BIT2|BIT3);
      break;
    }
    case 9:
    {
      PB->DOUT &=~ (BIT0|BIT1|BIT2|BIT3);
      PC->DOUT &=~ (BIT0|BIT2|BIT3);
      PC->DOUT |=  (BIT2);
      break;
    }
    case LED7_NODISPLAY_CODE:
    {
      /* No display */
      PB->DOUT |= (BIT0|BIT1|BIT2|BIT3);
      PC->DOUT |= (BIT0|BIT2|BIT3);
      break;
    }
    case LED7_E_CHAR:
    {
      /* 'E' character */
      PB->DOUT &=~ (BIT0|BIT1|BIT3);
      PB->DOUT |= (BIT2);
      
      PC->DOUT &=~ (BIT2|BIT3);
      PC->DOUT |= (BIT0);
      break;
    }
    case LED7_R_CHAR:
    {
      /* 'R' character */
      PB->DOUT &=~ (BIT0|BIT1|BIT2|BIT3);
      
      PC->DOUT &=~ (BIT0|BIT2);
      PC->DOUT |= (BIT3);
      break;
    }
    default: break;
  }
}



/**
  * @brief  Convert int to character array.
  * @param[in] LusNumber: integer.
  * @param[in] *LpLEDValue: pointer point to character array.
  * @return  None.
  * @details  Ham tach so nguyen (khong dau) thanh ung chu 
  * so rieng biet va luu vao mang.
  */
void Int_to_Array(uint16  LusIntNumber, uint8 *LpLEDValue, \
                                    LED7_DisplayType DisplayType)
{
  uint16 LusNumber;
  uint8 LED0, LED1, LED2, LED3;
  
  LusNumber = LusIntNumber;
  LED0 = LpLEDValue[0];
  LED1 = LpLEDValue[1];
  LED2 = LpLEDValue[2];
  LED3 = LpLEDValue[3];
  
  if(LusNumber < 100) //So Number co 2 chu so
  {
    if(DisplayType == LED7_DISPLAY_NUMBER_LEADING_ZEROS)
    {
      LED0 = 0;
      LED1 = 0;
    }
    else
    {
      LED0 = 10;
      LED1 = 10;
    }
    
    LED2 = (uint8) (LusNumber/10);
    LED3 = (uint8) (LusNumber%10);
  }
  else if(LusNumber < 1000)  //So Number co 3 chu so
  {
    if(DisplayType == LED7_DISPLAY_NUMBER_LEADING_ZEROS)
    {
      LED0 = 0;
    }
    else
    {
      LED0 = 10;
    }
    LED1 = (uint8) (LusNumber/100);
    LusNumber = (LusNumber - (LED1*100));
    LED2 = (uint8) (LusNumber/10);
    LED3 = (uint8) (LusNumber%10);
  }
  else if(LusNumber < 9999)  //So Number co 4 chu so
  {
    LED0 = (uint8) (LusNumber/1000);
    LusNumber = LusNumber - (LED0*1000);
    LED1 = (uint8) (LusNumber/100);
    LusNumber = LusNumber - (LED1*100);
    LED2 = (uint8) (LusNumber/10);
    LED3 = (uint8) (LusNumber%10);
  }
  else
  { 
    if(DisplayType == LED7_DISPLAY_NUMBER_LEADING_ZEROS)
    {
      LED0 = 0;
      LED1 = 0;
      LED2 = 0;
      LED3 = 0;
    }
    else
    {
      LED0 = 10;
      LED1 = 10;
      LED2 = 10;
      LED3 = 10;
    }
    
  }
  LpLEDValue[0] = LED0;
  LpLEDValue[1] = LED1;
  LpLEDValue[2] = LED2;
  LpLEDValue[3] = LED3;
}

/**
  * @brief  Convert array to int.
  * @param[in] *Array: pointer point to character array.
  * @return  value of array as int.
  * @details  None
  */
uint16 Array_To_Int(uint8 *Array)
{
  uint16 IntNumber;
  
  IntNumber = Array[0]*1000 + Array[1]*100 + Array[2]*10 + + Array[3];

  return IntNumber;  
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
  
  LED7_GstDriver.enDriverStatus = LED7_NORMAL;
}


/**
  * @brief  Display number on LED7segment.
  * @param[in] LusDisplayValue: 0 -> (2^MAX_NUM_LED7-1).
  * @return None.
  * @details  Display value on LED7segment.
  */
void LED7_DisplayNumber(uint16  DisplayValue)
{
  Int_to_Array(DisplayValue, GaaLED7Value, LED7_DISPLAY_NUMBER);
}
/**
  * @brief  Display number with adding zeros on LED7segment.
  * @param[in] LusDisplayValue: 0 -> (2^MAX_NUM_LED7-1).
  * @return None.
  * @details  Display value on LED7segment.
  */
void LED7_DisplayLeadingZeros(uint16  DisplayValue)
{
  Int_to_Array(DisplayValue, GaaLED7Value, \
               LED7_DISPLAY_NUMBER_LEADING_ZEROS);
}

/**
  * @brief  Display error on LED7segment.
  * @param[in] ErrorCode: .
  * @return None.
  * @details  Display error on LED7segment.
  */
void LED7_DisplayError(uint8  ErrorCode)
{
  GaaLED7Value[0] = LED7_E_CHAR;
  GaaLED7Value[1] = LED7_R_CHAR;
  GaaLED7Value[2] = LED7_R_CHAR;
  
  switch(ErrorCode)
  {
    case LED7_ERR0:
    {
      GaaLED7Value[3] = 0;
      break;
    }
    case LED7_ERR1:
    {
      GaaLED7Value[3] = 1;
      break;
    }
    case LED7_ERR9:
    {
      GaaLED7Value[3] = 9;
      break;
    }
    default: break;
  }
  
}
/**
  * @brief  Turn off LED7segment without effect another LED7s.
  * @param[in] LEDpos: bit0 <=> LEd7seg 0
  *            LED7-3 | LED7-2 | LED7-1 | LED7-0
  *            BIT3   | BIT2   | BIT1   | BIT0
  * @return None.
  * @details  Turn off all LED7segment.
  */
void LED7_TurnOff(LED7_IdType LEDpos)
{
  /* Turn off LED7s */
  PA->DOUT &=~ (LEDpos << 8);

}
/**
  * @brief  Turn on LED7segment without effect another LED7s.
  * @param[in] LEDpos: bit0 <=> LEd7seg 0
  *            LED7-3 | LED7-2 | LED7-1 | LED7-0
  *            BIT3   | BIT2   | BIT1   | BIT0
  * @return None.
  * @details  Turn on all LED7segment.
  */
void LED7_TurnOn(LED7_IdType LEDpos)
{
  PA->DOUT |= (LEDpos << 8);
}

/**
  * @brief  Toogle LED7segment without effect another LED7s.
  * @param[in] LEDpos: bit0 <=> LEd7seg 0
  *            LED7-3 | LED7-2 | LED7-1 | LED7-0
  *            BIT3   | BIT2   | BIT1   | BIT0
  * @return None.
  */
void ToogleLED(LED7_IdType LEDpos)
{
  PA->DOUT ^= (LEDpos << 8);
}

/**
  * @brief  Enable blinking LED7s.
  * @param[in] LEDpos: position of Toogle LED.
  *            bit0 <=> LEd7seg 0
  *            LED7-3 | LED7-2 | LED7-1 | LED7-0
  *            BIT3   | BIT2   | BIT1   | BIT0
  * @param[in] Blinktime: in milliseconds.
  * @return None.
  * @details 
  */
void LED7_EnableBlinking(LED7_IdType LEDpos, uint32 Blinktime)
{
  LED7_GstDriver.ucBlinkLED = LEDpos;
  
  LED7_GstDriver.enDriverStatus = LED7_BLINKING;
  
  LED7_GstDriver.ulBlinkTime = Blinktime;
  
  LED7_GstDriver.ulStartTick = millis();
}

/**
  * @brief  Disable blinking LED7s.
  * @param[in] None.
  * @return None.
  * @details 
  */
void LED7_DisableBlinking(void)
{
  LED7_GstDriver.ucBlinkLED = 0;
  
  LED7_GstDriver.enDriverStatus = LED7_NORMAL;
}

void LED7_MainFunction(void)
{
  uint32_t LulNowTick;
  
  LulNowTick =  millis();
  
  if((LulNowTick - LED7_GstDriver.ulStartTick) > LED7_GstDriver.ulBlinkTime)
  {
    /* Change new state of blink LED */
    if(LED7_GstDriver.ucBlinkLEDStatus == LED7_BLINK_IS_ON)
    {
      LED7_GstDriver.ucBlinkLEDStatus = LED7_BLINK_IS_OFF;
    }
    else
    {
      LED7_GstDriver.ucBlinkLEDStatus = LED7_BLINK_IS_ON;
    }
    
    /* Reset Start Tick time */
    LED7_GstDriver.ulStartTick = LulNowTick;
  }
  else
  {
    /* Do Nothing. */
  }
}

LED7_IdType NUMBER_TO_LEDID(uint8 num)
{
  return GaaMapNumberToLEDPos[num];
}

/**
  * @brief  Increase value of the LED7.
  * @param[in] LEDpos: position of LED77.
  *            bit0 <=> LEd7seg 0
  *            LED7-3 | LED7-2 | LED7-1 | LED7-0
  *            BIT3   | BIT2   | BIT1   | BIT0
  * @return Value of Number after increase.
  * @details 
  */
void LED7_IncreaseLED7(LED7_IdType LEDpos, uint16 *DisplayValue)
{
  uint8  LaaLED7Value[4];
  
  Int_to_Array(*DisplayValue, LaaLED7Value, LED7_DISPLAY_NUMBER_LEADING_ZEROS);
  
  /* Increase LED7 value. */
  switch(LEDpos)
  {
    case LED7_0: /* LED7_0(Nghìn) is blinking */
    {
      INCREASE_LED7VAL(LaaLED7Value[0]);
      break;
    }
    case LED7_1: /* LED7_1(Trăm) is blinking */
    {
      INCREASE_LED7VAL(LaaLED7Value[1]);
      break;
    }
    case LED7_2: /* LED7_2(Chục) is blinking */
    {
      INCREASE_LED7VAL(LaaLED7Value[2]);
      break;
    }
    case LED7_3: /* LED7_3(Đơn vị) is blinking */
    {
      INCREASE_LED7VAL(LaaLED7Value[3]);
      break;
    }
    default: break;
  }
  
  *DisplayValue = Array_To_Int(LaaLED7Value);
}


/**
  * @brief  Decrease value of the LED7.
  * @param[in] LEDpos: position of LED7.
  *            bit0 <=> LEd7seg 0
  *            LED7-3 | LED7-2 | LED7-1 | LED7-0
  *            BIT3   | BIT2   | BIT1   | BIT0
  * @return Value of Number after decrease.
  * @details 
  */
void LED7_DecreaseLED7(LED7_IdType LEDpos, uint16 *DisplayValue)
{
  uint8  LaaLED7Value[4];
  
  Int_to_Array(*DisplayValue, LaaLED7Value, LED7_DISPLAY_NUMBER_LEADING_ZEROS);
  
  /* Decrease LED7 value. */
  switch(LEDpos)
  {
    case LED7_0: /* LED7_0(Nghìn) is blinking */
    {
      DECREASE_LED7VAL(LaaLED7Value[0]);
      break;
    }
    case LED7_1: /* LED7_1(Trăm) is blinking */
    {
      DECREASE_LED7VAL(LaaLED7Value[1]);
      break;
    }
    case LED7_2: /* LED7_2(Chục) is blinking */
    {
      DECREASE_LED7VAL(LaaLED7Value[2]);
      break;
    }
    case LED7_3: /* LED7_3(Đơn vị) is blinking */
    {
      DECREASE_LED7VAL(LaaLED7Value[3]);
      break;
    }
    default: break;
  }
  
  *DisplayValue = Array_To_Int(LaaLED7Value);
}




/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/



