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

/* Thời gian mà người dùng được phép dùng thử sản phẩm, quá thời gian
   này, mạch sẽ bi khóa, không dùng được.
	 Đơn vị: giờ.
*/
#define TRIAL_TIME              THOI_GIAN_DUNG_THU

/* Số serial của sản phẩm, số này phải là số có bốn chữ số */
#define SERIAL_NUMBER		        SO_SERI

/* Đây là số đặc biệt của sản phẩm, hết hợp với số serial tạo thành mật khẩu */
#define SPECIAL_NUMBER          SO_DAC_BIET

/* Năm sản xuất của sản phẩm */
#define MANUFACTURE_YEAR        NAM_SAN_XUAT

/* Thời gian hiển thị số ngày hoạt động của lò nhiệt
   Đơn vị: mili giây.
*/
#define HEATER_WORKING_DISPLAY_TIME  THOI_GIAN_HIEN_THI_SO_NGAY_LAM_VIEC_CUA_LO_NHIET

/* Nhiệt độ cao nhất mà lò có thể đạt được. */
#define MAX_HEATER_TEMPERATURE  NHIET_DO_MAX


/* Trial time theo đơn vị phút, thời gian lưu trong flash memory có đơn vị là
   phút. */
#define TRIAL_TIME_IN_MIN       (TRIAL_TIME*60)

/* Password của lò nhiệt. */
#define HEATER_PASSWORD         (SERIAL_NUMBER + SPECIAL_NUMBER)


/* Maximum of ones */
#define MAX_ONES_TEMPERATURE         ((uint8)(MAX_HEATER_TEMPERATURE%10))
#define MAX_TENS_TEMPERATURE         ((uint8)((MAX_HEATER_TEMPERATURE/10)%10))
#define MAX_HUNDREDS_TEMPERATURE     ((uint8)((MAX_HEATER_TEMPERATURE/100)%10))
#define MAX_THOUSANDS_TEMPERATURE    ((uint8)(MAX_HEATER_TEMPERATURE/1000))

/* Thời gian nhấp nháy LED7 lúc kết thúc một sự kiện. */
#define END_EVENT_BLINK_TIME         150U

/* Blinking LED7 time in setting modes. */
#define SETTING_MODE_BLINK_TIME      300U

/* Khoảng thời gian(phút) mà lò nhiệt lưu lại thời gian làm việc. */
#define STORE_WORKING_TIME_INTERVAL_IN_MIN  30U

#define NUM_MIN_PER_HOUR  60U
#define NUM_HOUR_PER_DAY  8U

/*-------------------- Specific hardware configuration. ----------------------*/
/*************    HardWare connection:    *************************************
* LED-7Seg:   a <-> PB0
*             f <-> PB1
*             b <-> PB2
*             g <-> PB3
*             c <-> PC0
*             dot <-> PC1
*             e <-> PC2
*             d <-> PC3
*          LED1 <-> PA8
*          LED2 <-> PA9
*          LED3 <-> PA10
*          LED4 <-> PA11
*
* Button:  Button+ <-> PA2
*          Button- <-> PA3
*          ButtonSET <-> PA4
* Triac:
* Triac control  <-> PA12
* Triac feedback <-> PA5
*
* Speaker pin    <-> PB8
*
* ADC Thermo-Couple <-> PA0
* ADC LM35          <-> PA1
*******************************************************************************/

/* System clock define */
#define PLL_CLOCK                 50000000


/* Led Status define */
#define LED_STATUS                PA13
#define LED_STATUS_PORT           PA
#define LED_STATUS_BIT            BIT13
#define LED_STATUS_ON_VALUE       1U
#define LED_STATUS_OFF_VALUE      0U

/* Triac control define */
#define TRIAC_PORT                PA
#define TRIAC_PIN                 PA12
#define TRIAC_BIT                 BIT12
#define TRIAC_ON_VALUE            0U
#define TRIAC_OFF_VALUE           1U


/* Speaker control define */
#define SPEAKER_ON                0U
#define SPEAKER_OFF               1U
#define SPEAKER_PIN               PB8

#define HEATER_TIMEOUT_TRUE      TRUE
#define HEATER_TIMEOUT_FALSE     FALSE


#define TRIAC_ON()               TRIAC_PIN = TRIAC_ON_VALUE
#define TRIAC_OFF()              TRIAC_PIN = TRIAC_OFF_VALUE

#define LED_STATUS_ON()          LED_STATUS = LED_STATUS_ON_VALUE

#define LED_STATUS_OFF()         LED_STATUS = LED_STATUS_OFF_VALUE

#define LED_STATUS_TOOGLE()      LED_STATUS ^= 1;

#endif //__LONHIET_DEVCFG_H__
