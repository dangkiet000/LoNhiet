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
#define THOI_GIAN_DUNG_THU		  240U

/* Số serial của sản phẩm, số này phải là số có bốn chữ số */
#define SO_SERI             		8020U

/* Đây là số đặc biệt của sản phẩm, hết hợp với số serial tạo thành mật khẩu */
#define SO_DAC_BIET             2U

/* Năm sản xuất của sản phẩm */
#define NAM_SAN_XUAT            2018U

/* Thời gian hiển thị số ngày hoạt động của lò nhiệt
   Đơn vị: mili giây.
*/
#define THOI_GIAN_HIEN_THI_SO_NGAY_LAM_VIEC_CUA_LO_NHIET  5000U

/* Nhiệt độ cao nhất mà lò có thể đạt được. */
#define NHIET_DO_MAX            725U


/* Chế độ debug, khi ON thì sẽ xuất ra nhiều thông tin cần thiết qua UART1 */
#define DEBUG_MODE              STD_ON

/* Chế độ dùng để test mạch, khi ON thì sẽ xuất ra nhiều thông tin cần 
   thiết qua và test các thành phần cơ bản của mạch UART1 */
#define TEST_MODE               STD_OFF

/* Chế độ này nếu STD_ON thì heater sẽ gửi data đến máy tính qua UART1
   Máy tính sẽ dùng phần mềm để hiển thị biểu đồ nhiệt độ theo thời gian.
*/
#define DATA_LOGGING            STD_OFF


#endif //__LONHIET_USERCFG_H__
