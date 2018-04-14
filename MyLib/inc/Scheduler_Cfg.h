//*****************************************************************************
//
// scheduler.h - Public header for the simple timed function scheduler module.
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
//*****************************************************************************
#ifndef __SCHEDULER_CFG_H__
#define __SCHEDULER_CFG_H__

#define NUM_TASKS   4U

/* Scheduler define */
#define SCH_UpdateADC_Task            0U
#define SCH_Display_Task               1U
#define SCH_BlinkingLED_Task          2U
#define SCH_SendSetPoint_Task         3U

/* Scheduler call-back function prototypes */
extern void DisplayTask(void);
extern void BlinkingLed(void);
extern void Send_SetPoint_to_PC(void);
extern void UpdateADCValue(void);



#endif // __SCHEDULER_CFG_H__
