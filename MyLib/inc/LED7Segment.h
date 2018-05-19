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

#include "NUC200Series.h"
#include "Std_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
typedef uint8_t LED7_IdType;

/* Definition of position LED */
typedef enum ETag_LED7_StatusType
{
  LED7_UNINIT = 0,
  LED7_NORMAL,
  LED7_BLINKING,
} LED7_StatusType;

/* Definition of blink LED7 state */
typedef enum ETag_LED7_BlinkStateType
{
  LED7_BLINK_IS_OFF = 0,
  LED7_BLINK_IS_ON
} LED7_BlinkStateType;

/* Definition of position LED */
typedef enum ETag_LED7_DisplayType
{
  LED7_DISPLAY_NUMBER = 0,
  LED7_DISPLAY_NUMBER_LEADING_ZEROS,
} LED7_DisplayType;

/* Definition of position LED */
typedef struct STag_LED7_DriverType
{
  /* This variable store status LED7 driver */
  LED7_StatusType enDriverStatus;

  /* This variable store blinking status LED7s
   * BIT[n] <=> LED7-[n]
   * n = 0..3
   *   LED7-3 | LED7-2 | LED7-1 | LED7-0
   *   BIT3   | BIT2   | BIT1   | BIT0
   *
   * BIT0 = 1 : LED7-0 is displayed normally.
   * BIT0 = 0 : LED7-0 is turned off.
   */
  LED7_IdType    ucBlinkLED;
  
  LED7_BlinkStateType  ucBlinkLEDStatus;
  
  /* Store blinking time */
  uint32          ulBlinkTime;
  
  /* Tick count when blinking was enabled */
  uint32          ulStartTick;
} LED7_DriverType;

/*******************************************************************************
**                      Configuration                                         **
*******************************************************************************/
/* Definition of position LED */
#define  LED7_0   0x01U
#define  LED7_1   0x02U
#define  LED7_2   0x04U
#define  LED7_3   0x08U
#define  LED7_ALL 0x0FU

/* Maximum number of LED7segment */
#define MAX_NUM_LED7      4U

/* LED off value code. */
#define LED7_NODISPLAY_CODE     10U
#define LEDDOT            PC1

/* Timer Led define */  
#define TimerLED_ISR              TMR3_IRQHandler
#define TimerBaseAddr             TIMER3
#define PERIPHERAL_CLOCK_LEDTIMER 12000000UL
#define MAX_COMPARE_VALUE         0xFFFFFFUL
#define MAX_PRESCALER_VALUE       0xFFU



#define INCREASE_LED7VAL(val)  ((val >= 9) ? (val = 0) : (val++))
#define DECREASE_LED7VAL(val)  ((val == 0) ? (val = 9) : (val--))

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
extern uint32_t millis(void);

void LED_7Seg_Decode(uint8 LedNumber);
Std_ReturnType Timer_LED_Init(uint16 LusFrequence);
void Int_to_Array(uint16  LusIntNumber, uint8 *LpLEDValue, \
                              LED7_DisplayType DisplayType);
uint16 Array_To_Int(uint8 *Array);
/*******************************************************************************
**                      API Function Prototypes                               **
*******************************************************************************/
/* Initialize GPIO and Timer to control LED-7seg. */
void LED_7Seg_Init(void);

/* Display number with adding zeros on LED7segment. */
void LED7_DisplayLeadingZeros(uint16 DisplayValue);

/* Display number on LED7segment. */
void LED7_DisplayNumber(uint16 DisplayValue);

/* Turn off LED7segment without effect another LED7s. */
void LED7_TurnOff(LED7_IdType LEDpos);

/* Turn on LED7segment without effect another LED7s. */
void LED7_TurnOn(LED7_IdType LEDpos);

/* Enable blinking LED7s. */
void LED7_EnableBlinking(LED7_IdType LEDpos, uint32 Blinktime);

/* Disable blinking LED7s. */
void LED7_DisableBlinking(void);

/* Scheduler function */
void LED7_MainFunction(void);

LED7_IdType NUMBER_TO_LEDID(uint8 num);

/* Increase value of the LED7. */
void LED7_IncreaseLED7(LED7_IdType LEDpos, uint16 *DisplayValue);

/* Decrease value of the LED7. */
void LED7_DecreaseLED7(LED7_IdType LEDpos, uint16 *DisplayValue);

#ifdef __cplusplus
}
#endif

#endif //__LED7SEGMENT_H__

/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/

