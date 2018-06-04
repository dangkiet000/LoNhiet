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



/*-------------------- Specific hardware configuration. ----------------------*/

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
