/**************************************************************************//**
 * @file     LoNhiet_DevCfg.h
 * @version  V1.00
 * $Revision: 1
 * $Date: 08/05/2018
 * @brief    Chỉ dành cho developer. Đây là file cấu hình cho lò nhiệt.
 *
 * @Encoding: UTF-8 without signature
 * @note
 *
 ******************************************************************************/
#ifndef __LONHIET_DEVCFG_H__
#define __LONHIET_DEVCFG_H__
/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "LoNhiet_UserCfg.h"
#include "Compiler.h"
#include "NUC200Series.h"

/*******************************************************************************
**                      Developer configuration                               **
*******************************************************************************/

/* Trial time theo đơn vị phút, thời gian lưu trong flash memory có đơn vị là
   phút. */
#define TRIAL_TIME_IN_MIN     (TRIAL_TIME*60)

#define DATA_LOGGING              STD_OFF

/* System clock define */
#define PLL_CLOCK                 50000000


/* Led test define */
#define LED_TEST                  PA13
#define LED_TEST_PORT             PA
#define LED_TEST_BIT              BIT13

/* Triac control define */
#define TRIAC_PORT                PA
#define TRIAC_PIN                 PA12
#define TRIAC_BIT                 BIT12
#define TRIAC_ON                  0U
#define TRIAC_OFF                 1U


/* Speaker control define */
#define SPEAKER_ON                0U
#define SPEAKER_OFF               1U
#define SPEAKER_PIN               PB8

#define HEATER_TIMEOUT_TRUE      TRUE
#define HEATER_TIMEOUT_FALSE     FALSE

#define MAX_HEATER_TEMPERATURE       725U

/* Maximum of ones */
#define MAX_ONES_TEMPERATURE         ((uint8)(MAX_HEATER_TEMPERATURE%10))
#define MAX_TENS_TEMPERATURE         ((uint8)((MAX_HEATER_TEMPERATURE/10)%10))
#define MAX_HUNDREDS_TEMPERATURE     ((uint8)((MAX_HEATER_TEMPERATURE/100)%10))
#define MAX_THOUSANDS_TEMPERATURE    ((uint8)(MAX_HEATER_TEMPERATURE/1000))

INLINE void LED_TEST_ON(void)
{
  LED_TEST = 1;
}

INLINE void LED_TEST_OFF(void)
{
  LED_TEST = 0;
}

INLINE void LED_TEST_TOOGLE(void)
{
  LED_TEST ^= 1;
}

#endif //__LONHIET_DEVCFG_H__
