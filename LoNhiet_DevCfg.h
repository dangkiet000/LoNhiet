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

/*******************************************************************************
**                      Developer configuration                               **
*******************************************************************************/
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

/* Data flash define */
#define DATA_FLS_LEN              10U
#define DATA_FLS_SETPOINT_ADDR     0U
#define DATA_FLS_WORKINGTIME_ADDR  1U
#define DATA_FLS_ACTILOCKSTATUS_ADDR  2U


__INLINE void LED_TEST_ON(void)
{
  LED_TEST = 1;
}

__INLINE void LED_TEST_OFF(void)
{
  LED_TEST = 0;
}

__INLINE void LED_TEST_TOOGLE(void)
{
  LED_TEST ^= 1;
}

#endif //__LONHIET_DEVCFG_H__
