/**************************************************************************//**
 * @file     Button_Processing.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 27/12/2016 22:52p $
 * @brief    Button Processing library.
 *
 * @note
 * Copyright (C) 2016 DangKiet Technology Corp. All rights reserved.
*****************************************************************************/
#include "Button_Processing_Cfg.h"

/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/
Btn_ConfigType Btn_ConfigSet[] = {
  /* BTRU_ID */
  {
    /* ulStartHoldTime */
    0,
    /* usHoldThresTime */
    3000,
    /* enStatus */
    BTN_IDLE,
    /* pfnFunction */
    &BTRU_Release_Event,
    /* pfnHoldEvent1 */
    &BSET_BTRU_HoldToThres_Event,
    /* pfnHoldEvent2 */
    &BTRU_HoldToThres_Event
  },
  /* BCONG_ID */
  {
    /* ulStartHoldTime */
    0,
    /* usHoldThresTime */
    3000,
    /* enStatus */
    BTN_IDLE,
    /* pfnFunction */
    &BCONG_Release_Event,
    /* pfnHoldEvent1 */
    &BSET_BCONG_HoldToThres_Event,
    /* pfnHoldEvent2 */
    NULL_PTR
  },
  /* BSET_ID */
  {
    /* ulStartHoldTime */
    0,
    /* usHoldThresTime */
    3000,
    /* enStatus */
    BTN_IDLE,
    /* pfnFunction */
    &BSET_Release_Event,
    /* pfnHoldEvent1 */
    &BSET_HoldToThres_Event,
    /* pfnHoldEvent2 */
    NULL_PTR
  }
};


/*** (C) COPYRIGHT 2016 DangKiet Technology Corp. ***/



