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

#define LED7_TYPE           0
#define LED7_COMMON_ANODE   0
#define LED7_COMMON_CATHODE 1

#define NUMBER_OF_ERROR_CODE 4
/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/
/* 
Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 |  Bit1 | Bit0
DOT     G      F      E      D      C       B      A

Bit7 = 1 <=> LEDDOT is ON
*/

static const uint8 LED7_NumberFont[11] = {
  0x3F, /* Digit 0 */
	0x06, /* Digit 1 */
	0x5B, /* Digit 2 */
	0x4F, /* Digit 3 */
	0x66, /* Digit 4 */
  0x6D, /* Digit 5 */
	0x7D, /* Digit 6 */
	0x07, /* Digit 7 */
	0x7F, /* Digit 8 */
	0x6F, /* Digit 9 */
  0x00  /* No Display */
};

static const uint8 LED7_ErrorFont[NUMBER_OF_ERROR_CODE][4] = {
  {0x79,  0x77,  0x77, 0x3F}, // {'E','R','R','0'}
  {0x79,  0x77,  0x77, 0x06}, // {'E','R','R','1'}
  {0x79,  0x77,  0x77, 0x5B}, // {'E','R','R','2'}
  {0x79,  0x77,  0x77, 0x4F}, // {'E','R','R','3'}
};

static const uint8 LED7_ResultPASS_Font[4] = {
  0x73,  0x77,  0x6D, 0x6D // {'P','A','S','S'}
};

static const uint8 LED7_ResultFAIL_Font[4] = {
  0x71,  0x77,  0x30, 0x38 // {'F','A','I','L'}
};


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
STATIC Std_ReturnType Timer_LED_Init(uint16 LusFrequence)
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
  * @param[in] RawValue: 8bit LED7. If bitx = 1, LED is ON. Otherwise, LED is OFF
  * @return  None.
  * @details  Decode LED-7seg to display digits.
  */

static void Int_to_Array(uint16  LusIntNumber, uint8 *LpArray, \
                                    uint8 NoMeaningValue)
{
  LpArray[0] = (uint8) (LusIntNumber/1000);
  LpArray[1] = (uint8) ((LusIntNumber/100)%10);
  LpArray[2] = (uint8) ((LusIntNumber/10)%10);
  LpArray[3] = (uint8) (LusIntNumber%10);
  
  if(LusIntNumber < 10) /* Nếu là số có 1 chữ số */
  {
    LpArray[0] = NoMeaningValue;
    LpArray[1] = NoMeaningValue;
    LpArray[2] = NoMeaningValue;
  }
  else if(LusIntNumber < 100) /* Nếu là số có 2 chữ số */
  {
    LpArray[0] = NoMeaningValue;
    LpArray[1] = NoMeaningValue;
  }
  else if(LusIntNumber < 1000)  /* Nếu là số có 3 chữ số */
  {
    LpArray[0] = NoMeaningValue;
  }
  else if(LusIntNumber < 9999)  /* Nếu là số có 4 chữ số */
  {
    
  }
  else
  { 
    LpArray[0] = NoMeaningValue;
    LpArray[1] = NoMeaningValue;
    LpArray[2] = NoMeaningValue;
    LpArray[3] = NoMeaningValue;
  }
}

STATIC void LED_7Seg_Decode(uint8 RawValue)
{
  if(LED7_TYPE == LED7_COMMON_ANODE)
  {
    RawValue = ~RawValue;
  }
  LED7_SEG_A_PIN = (BIT0 & RawValue);
  LED7_SEG_B_PIN = (BIT1 & RawValue)>>1;
  LED7_SEG_C_PIN = (BIT2 & RawValue)>>2;
  LED7_SEG_D_PIN = (BIT3 & RawValue)>>3;
  LED7_SEG_E_PIN = (BIT4 & RawValue)>>4;
  LED7_SEG_F_PIN = (BIT5 & RawValue)>>5;
  LED7_SEG_G_PIN = (BIT6 & RawValue)>>6;
LED7_SEG_DOT_PIN = (BIT7 & RawValue)>>7;
}

/**
  * @brief  Convert int to character array.
  * @param[in] LusNumber: integer.
  * @param[in] *LpLEDValue: pointer point to character array.
  * @return  None.
  * @details  Ham tach so nguyen (khong dau) thanh ung chu 
  * so rieng biet va luu vao mang.
  */
void LED7_IntToLED7Code(uint16  LusIntNumber, uint8 *LpLEDValue, \
                                    LED7_DisplayType DisplayType)
{
  uint16 LusNumber;
  uint8 LED0, LED1, LED2, LED3;
  uint8 LucNoMeaningDigit;
  
  if(DisplayType == LED7_DISPLAY_NUMBER_LEADING_ZEROS)
  {
    LucNoMeaningDigit = 0;
  }
  else
  {
    LucNoMeaningDigit = LED7_NODISPLAY_CODE;
  }
    
  LusNumber = LusIntNumber;
  
  LED0 = (uint8) (LusNumber/1000);
  LED1 = (uint8) ((LusNumber/100)%10);
  LED2 = (uint8) ((LusNumber/10)%10);
  LED3 = (uint8) (LusNumber%10);
  
  if(LusNumber < 10) /* Nếu là số có 1 chữ số */
  {
    LED0 = LucNoMeaningDigit;
    LED1 = LucNoMeaningDigit;
    LED2 = LucNoMeaningDigit;
  }
  else if(LusNumber < 100) /* Nếu là số có 2 chữ số */
  {
    LED0 = LucNoMeaningDigit;
    LED1 = LucNoMeaningDigit;
  }
  else if(LusNumber < 1000)  /* Nếu là số có 3 chữ số */
  {
    LED0 = LucNoMeaningDigit;
  }
  else if(LusNumber < 9999)  /* Nếu là số có 4 chữ số */
  {
    
  }
  else
  { 
    LED0 = LED7_NODISPLAY_CODE;
    LED1 = LED7_NODISPLAY_CODE;
    LED2 = LED7_NODISPLAY_CODE;
    LED3 = LED7_NODISPLAY_CODE;
  }
  LpLEDValue[0] = LED7_NumberFont[LED0];
  LpLEDValue[1] = LED7_NumberFont[LED1];
  LpLEDValue[2] = LED7_NumberFont[LED2];
  LpLEDValue[3] = LED7_NumberFont[LED3];
}

/**
  * @brief  Convert array to int.
  * @param[in] *Array: pointer point to character array.
  * @return  value of array as int.
  * @details  None
  */
static uint16 Array_To_Int(uint8 *Array)
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
  LED7_IntToLED7Code(DisplayValue, GaaLED7Value, LED7_DISPLAY_NUMBER);
}
/**
  * @brief  Display number with adding zeros on LED7segment.
  * @param[in] LusDisplayValue: 0 -> (2^MAX_NUM_LED7-1).
  * @return None.
  * @details  Display value on LED7segment.
  */
void LED7_DisplayLeadingZeros(uint16  DisplayValue)
{
  LED7_IntToLED7Code(DisplayValue, GaaLED7Value, \
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
  GaaLED7Value[0] = LED7_ErrorFont[ErrorCode][0];
  GaaLED7Value[1] = LED7_ErrorFont[ErrorCode][1];
  GaaLED7Value[2] = LED7_ErrorFont[ErrorCode][2];
  GaaLED7Value[3] = LED7_ErrorFont[ErrorCode][3];
}

/**
  * @brief  Display PASS/FAIL on LED7segment.
  * @param[in] ErrorCode: .
  * @return None.
  * @details  Display PASS/FAIL on LED7segment.
  */
void LED7_DisplayResult(uint8  ResultCode)
{
  switch(ResultCode)
  {
    case LED7_PASS:
    {
      GaaLED7Value[0] = LED7_ResultPASS_Font[0];
      GaaLED7Value[1] = LED7_ResultPASS_Font[1];
      GaaLED7Value[2] = LED7_ResultPASS_Font[2];
      GaaLED7Value[3] = LED7_ResultPASS_Font[3];
      break;
    }
    case LED7_FAIL:
    {
      GaaLED7Value[0] = LED7_ResultFAIL_Font[0];
      GaaLED7Value[1] = LED7_ResultFAIL_Font[1];
      GaaLED7Value[2] = LED7_ResultFAIL_Font[2];
      GaaLED7Value[3] = LED7_ResultFAIL_Font[3];
      break;
    }
    default: break;
  }
}

/**
  * @brief  Display Day on LED7segment.
  * @param[in] Day: Day value.
  * @return None.
  * @details  None.
  */
void LED7_DisplayDay(uint8  Day)
{
  GaaLED7Value[0] = LED7_NumberFont[LED7_NODISPLAY_CODE];
  GaaLED7Value[1] = LED7_NumberFont[LED7_NODISPLAY_CODE];
  GaaLED7Value[2] = LED7_NumberFont[Day/10];
  GaaLED7Value[3] = LED7_NumberFont[Day%10];
}
/**
  * @brief  Display Month on LED7segment.
  * @param[in] Mon: Month value.
  * @return None.
  * @details  None.
  */
void LED7_DisplayMon(uint8  Mon)
{
  GaaLED7Value[0] = LED7_NumberFont[LED7_NODISPLAY_CODE];
  GaaLED7Value[1] = LED7_NumberFont[LED7_NODISPLAY_CODE];
  GaaLED7Value[2] = LED7_NumberFont[Mon/10];
  GaaLED7Value[3] = LED7_NumberFont[Mon%10];
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
void LED7_IncreaseLED7(uint8 LEDIdx, uint16 *DisplayValue)
{
  uint8  LaaLED7Value[4];
  
  /* Convert display value(uint16) to array. */
  Int_to_Array(*DisplayValue, LaaLED7Value, 0);
  
  INCREASE_LED7VAL(LaaLED7Value[LEDIdx]);

  /* Convert array display value(uint16) to . */
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
void LED7_DecreaseLED7(uint8 LEDIdx, uint16 *DisplayValue)
{
  uint8  LaaLED7Value[4];
  
  Int_to_Array(*DisplayValue, LaaLED7Value, 0);
  
  DECREASE_LED7VAL(LaaLED7Value[LEDIdx]);
  
  *DisplayValue = Array_To_Int(LaaLED7Value);
}




/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/



