/**************************************************************************//**
 * @file     Button_Processing.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 27/12/2016 22:52p $
 * @brief    Button Processing library.
 *
 * @note
 * Copyright (C) 2016 DangKiet Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __BUTTON_PROCESSING_CFG_H__
#define __BUTTON_PROCESSING_CFG_H__

/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "Button_Processing.h"

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define Button_ISR         GPAB_IRQHandler

#define ButtonBaseAddr     PA

#define BSET_PIN           PA4
#define BCONG_PIN          PA2
#define BTRU_PIN           PA3

#define BSET_BIT           BIT4
#define BCONG_BIT          BIT2
#define BTRU_BIT           BIT3

#define MAX_BUTTON_ID      3U
#define BTRU_ID            0U
#define BCONG_ID           1U
#define BSET_ID            2U

/* Button call-back function prototypes */
extern void BSET_Release_Event(void);
extern void BSET_Hold_3s_Event(void);
extern void BSET_BCONG_Hold_3s_Event(void);
extern void BSET_BTRU_Hold_3s_Event(void);

extern void BCONG_Release_Event(void);

extern void BTRU_Release_Event(void);
extern void BTRU_Hold_6s_Event(void);

/*******************************************************************************
**                      TYPE DEFINITION                                       **
*******************************************************************************/

#endif //__BUTTON_PROCESSING_CFG_H__

/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/

