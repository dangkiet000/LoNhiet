//*****************************************************************************
//
// scheduler.c - A simple task scheduler
//
// Copyright (c) 2010-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.1.0.12573 of the Tiva Utility Library.
//
//******************************************************************************

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Scheduler.h"

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
tSchedulerTask Sch_GaaTable[] =
{
  /* Index: 0 - SCH_UpdateADC_Task */
  {
    /* pfnFunction */
    &UpdateADCValue,
    /* ulInterval */
    250,
    /* ulLastTick */
    0,
    /* enStatus */
    TASK_ENABLE
  },
  /* Index: 1 - SCH_Display_Task */
  {
    /* pfnFunction */
    &DisplayTask,
    /* ulInterval */
    500,
    /* ulLastTick */
    0,
    /* enStatus */
    TASK_ENABLE
  },
  /* Index: 2 - SCH_BlinkingLED_Task */
  {
    /* pfnFunction */
    &BlinkingLed,
    /* ulInterval */
    400,
    /* ulLastTick */
    0,
    /* enStatus */
    TASK_DISABLE
  },
  /* Index: 3 - SCH_SendSetPoint_Task */
  {
    /* pfnFunction */
    &Send_SetPoint_to_PC,
    /* ulInterval */
    20000,
    /* ulLastTick */
    0,
    /* enStatus */
    TASK_DISABLE
  }
};

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
