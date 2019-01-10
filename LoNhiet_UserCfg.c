/**************************************************************************//**
 * @file     LoNhiet_UserCfg.c
 * @version  V1.00
 * $Revision: 1
 * $Date: 11/04/2018
 * @brief    Đây là file cấu hình cho lò nhiệt.
 *
 * @Encoding: UTF-8 without signature
 * @note
 *
 ******************************************************************************/

/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "LoNhiet_UserCfg.h"

/*******************************************************************************
**                      Uuer configuration                                    **
*******************************************************************************/
/* 
Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 |  Bit1 | Bit0
DOT     G      F      E      D      C       B      A

Ví dụ: LED7_NumberFont[0] = 0x02
=> Bit1 = 1 <=> B is ON
*/
const uint8 LED7_NumberFont[11] = {
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

/* Đây là mã hiển thị khi xảy ra lỗi. */
const uint8 LED7_ErrorFont[NUMBER_OF_ERROR_CODE][4] = {
  {0x79,  0x77,  0x77, 0x3F}, // {'E','R','R','0'}
  {0x79,  0x77,  0x77, 0x06}, // {'E','R','R','1'}
  {0x79,  0x77,  0x77, 0x5B}, // {'E','R','R','2'}
  {0x79,  0x77,  0x77, 0x4F}, // {'E','R','R','3'}
};

/* Đây là mã hiển thị khi người dùng nhập pass đúng. */
const uint8 LED7_ResultPASS_Font[4] = {
  0x73,  0x77,  0x6D, 0x6D // {'P','A','S','S'}
};

/* Đây là mã hiển thị khi người dùng nhập pass sai. */
const uint8 LED7_ResultFAIL_Font[4] = {
  0x71,  0x77,  0x30, 0x38 // {'F','A','I','L'}
};

