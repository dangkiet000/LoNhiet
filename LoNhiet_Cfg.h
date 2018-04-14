/**************************************************************************//**
 * @file     LoNhiet_Configuration.h
 * @version  V1.00
 * $Revision: 1
 * $Date: 11/04/2018
 * @brief    Đây là file cấu hình cho lò nhiệt.
 *
 * @Encoding: UTF-8 without signature
 * @note
 *
 ******************************************************************************/
#ifndef __LONHIET_CONFIGURATION_H__
#define __LONHIET_CONFIGURATION_H__
/*******************************************************************************
**                      Include                                               **
*******************************************************************************/

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
/* Đây là thời gian mà người dùng được phép dùng thử sản phẩm, quá thời gian
   này, mạch sẽ bi khóa, không dùng được.
	 Đơn vị: giờ.
*/
#define TRIAL_TIME		  240U

/* Đây là số serial của sản phẩm, số này phải là số có bốn chữ số */
#define SERIAL_NUMBER		9000U

/* Đây là số đặc biệt của sản phẩm, hết hợp với số serial tạo thành mật khẩu */
#define SPECIAL_NUMBER  2U

#define PASSWORD (SERIAL_NUMBER + SPECIAL_NUMBER)

/* Đây là năm sản xuất của sản phẩm */
#define MANUFACTURE_YEAR 2018U




/* Đây là chế độ debug, khi ON thì sẽ xuất ra nhiều thông tin cần thiết qua 
   UART1 */
#define DEBUG_MODE                STD_ON
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/


/*******************************************************************************
**                      API Functions                                         **
*******************************************************************************/


#endif //__LONHIET_CONFIGURATION_H__
