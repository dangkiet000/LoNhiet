/**************************************************************************//**
 * @file     LoNhiet_UserCfg.h
 * @version  V1.00
 * $Revision: 1
 * $Date: 11/04/2018
 * @brief    Đây là file cấu hình cho lò nhiệt.
 *
 * @Encoding: UTF-8 without signature
 * @note
 *
 ******************************************************************************/
#ifndef __LONHIET_USERCFG_H__
#define __LONHIET_USERCFG_H__

#include "Std_Types.h"
/*******************************************************************************
**                      Include                                               **
*******************************************************************************/

/*******************************************************************************
**                      Uuer configuration                                    **
*******************************************************************************/
/* Thời gian mà người dùng được phép dùng thử sản phẩm, quá thời gian
   này, mạch sẽ bi khóa, không dùng được.
	 Đơn vị: giờ.
*/
#define THOI_GIAN_DUNG_THU		  1U

/* Số serial của sản phẩm, số này phải là số có bốn chữ số */
#define SO_SERI             		2000U

/* Đây là số đặc biệt của sản phẩm, hết hợp với số serial tạo thành mật khẩu */
#define SO_DAC_BIET             2U

/* Năm sản xuất của sản phẩm */
#define NAM_SAN_XUAT            2018U

/* Thời gian hiển thị số ngày hoạt động của lò nhiệt
   Đơn vị: mili giây.
*/
#define THOI_GIAN_HIEN_THI_SO_NGAY_LAM_VIEC_CUA_LO_NHIET  5000U

/* Sai số hiện thị so với nhiệt độ cài đặt. Đây là khoảng nhiệt mà lò sẽ hiển 
   thị nhiệt độ cài đặt thay vì nhiệt độ thực tế của lò. */
#define SAI_SO_HIEN_THI_NHIET_DO_CAI_DAT    5U

/* Nhiệt độ cao nhất mà lò có thể đạt được. */
#define NHIET_DO_MAX            1350U

/* Thời gian cập nhật để hiển thị nhiệt độ. Đơn vị: mili giây. */
#define THOI_GIAN_CAP_NHAT_HIEN_THI_NHIET_DO 1000

/* Đây là loại LED7 đoạn
LED7_TYPE = 0: Anode chung(dương chung).
LED7_TYPE = 1: Cathode chung(âm chung).
*/
#define LED7_TYPE           0

/* Đây là vị trí mã font không hiển thị trong mảng LED7_NumberFont */
#define LED7_FONT_NO_DISPLAY 11

/* Số mã lỗi hiển thị trên LED7. */
#define NUMBER_OF_ERROR_CODE 4

/* Chế độ debug, khi ON thì sẽ xuất ra nhiều thông tin cần thiết qua UART1 */
#define DEBUG_MODE              STD_ON

/* Chế độ dùng để test mạch, khi ON thì sẽ xuất ra nhiều thông tin cần 
   thiết qua và test các thành phần cơ bản của mạch UART1 */
#define TEST_MODE               STD_OFF

/* Chế độ này nếu STD_ON thì heater sẽ gửi data đến máy tính qua UART1
   Máy tính sẽ dùng phần mềm để hiển thị biểu đồ nhiệt độ theo thời gian.
*/
#define DATA_LOGGING            STD_OFF


/*
Chú ý: 
1. Khi Cặp nhiệt không kết nối với mạch thì LED sẽ hiển thị: ERR0.
2. Khi TRIAC mở thì LED xanh dương không sáng, khi TRIAC đóng thì LED xanh dương
sáng.
*/

#endif //__LONHIET_USERCFG_H__
