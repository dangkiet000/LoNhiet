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
#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "NUC200Series.h"
#include "Std_Types.h"

#include "Scheduler_Cfg.h"


//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! \addtogroup scheduler_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Prototype of a function that the scheduler can call periodically.
//
//*****************************************************************************
typedef void (*tSchedulerFunction)(void *pvParam);

/* Definition of event type */
typedef enum ETag_Sch_StatusType
{
  TASK_ENABLE = 1,
  TASK_DISABLE,
  TASK_UNKNOWN
} Sch_TaskStatusType;

#define SCH_RUN_NOW    TRUE
#define SCH_RUN_LATER  FALSE

typedef uint8_t Sch_TaskIdType;

//*****************************************************************************
//
//! The structure defining a function which the scheduler will call
//! periodically.
//
//*****************************************************************************
typedef struct STag_tSchedulerTask
{
  /* A function pointer to the user function which is to be called 
   * periodically by the scheduler. 
   */
  void (*const pfnFunction)(void);

  /* The parameter which is to be passed to this function when it is called. */
  //void *pvParam;

  /* The frequency the function is to be called expressed in terms of system
   * ticks.  If this value is 0, the function will be called on every call
   * to Sch_MainFunction.
   */
  uint32_t ulInterval;

  /* Tick count when this function was last called.  This field is updated
   * by the scheduler.
   */
  uint32_t ulLastTick;

  /* A flag indicating whether or not this task is active.  If true, the
   * function will be called periodically.  If false, the function is
   * disabled and will not be called.
   */
  Sch_TaskStatusType enStatus;
}
tSchedulerTask;




//*****************************************************************************
//
//! This global table must be populated by the client and contains information
//! on each function that the scheduler is to call.
//
//*****************************************************************************
extern tSchedulerTask Sch_GaaTable[];

//*****************************************************************************
//
//! This global variable must be exported by the client.  It must contain the
//! number of entries in the Sch_GaaTable array.
//
//*****************************************************************************


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Public function prototypes
//
//*****************************************************************************
extern void Sch_MainFunction(void);

extern Std_ReturnType Sch_TaskEnable(Sch_TaskIdType ucTaskId, 
                                          boolean blRunNow);
extern Std_ReturnType Sch_TaskDisable(Sch_TaskIdType ucTaskId);

extern Sch_TaskStatusType Sch_GetStatus(Sch_TaskIdType ucTaskId);

extern void Sch_SetInterval(Sch_TaskIdType ucTaskId, uint32_t ulIntervalTime);

extern uint32_t millis(void);
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __ SCHEDULER_H_
