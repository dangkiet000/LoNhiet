/**************************************************************************//**
 * @file     LED7Segment.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 27/12/2016 22:52p $
 * @brief    LED7Segment library.
 *
 * @note
 * Copyright (C) 2016 DangKiet Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __LED7SEGMENT_H__
#define __LED7SEGMENT_H__


#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define LED_OFF_VALUE     10U
#define LEDDOT            PC1
#define NoOfLED           4U
#define MAX_PRESCALER_VALUE       0xFFU 

/* Timer Led define */  
#define TimerLED_ISR      TMR3_IRQHandler
#define TimerBaseAddr     TIMER3
#define PERIPHERAL_CLOCK_LEDTIMER 12000000UL
#define MAX_COMPARE_VALUE         0xFFFFFFUL
#define MAX_PRESCALER_VALUE       0xFFU
  
  
extern boolean GblAllLedIsOff;
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
Std_ReturnType LED_7Seg_Decode(uint8_t LedNumber);
Std_ReturnType Timer_LED_Init(uint16_t LusFrequence);
Std_ReturnType Switch_to_LED(uint8_t LucLedPosition);
void Int_to_Array(uint16_t  LusIntNumber, uint8_t *LpLEDValue);

/*******************************************************************************
**                      API Function Prototypes                               **
*******************************************************************************/
void LED_7Seg_Init(void);
void LED7Seg_Show(uint16_t  LusDisplayValue);
void TurnOffLED(void);
void TurnOnLED(void);
void BlinkingLED(boolean LblEnable);

#ifdef __cplusplus
}
#endif

#endif //__LED7SEGMENT_H__

/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/

