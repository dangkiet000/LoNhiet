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
//*****************************************************************************

#include "Scheduler.h"


//*****************************************************************************
//
//! \addtogroup scheduler_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Instructs the scheduler to update its task table and make calls to
//! functions needing called.
//!
//! This function must be called periodically by the client to allow the
//! scheduler to make calls to any configured task functions if it is their
//! time to be called.  The call must be made at least as frequently as the
//! most frequent task configured in the Sch_GaaTable array.
//!
//! Although the scheduler makes use of the SysTick interrupt, all calls to
//! functions configured in \e Sch_GaaTable are made in the context of
//! Sch_MainFunction().
//!
//! \return None.
//
//*****************************************************************************
void Sch_MainFunction(void)
{
  uint8_t LucCnt;
  uint32_t LulNowTick;
  tSchedulerTask *pTask;
  
  /* Get a pointer to the task information. */
  pTask = &Sch_GaaTable[0];
  
  /* Get current time */
  LulNowTick = millis();
  
  /* Loop through each task in the task table. */
  for(LucCnt = 0; LucCnt < MAX_TASKS; LucCnt++)
  {
    /* Is this task active and, if so, is it time to call it's function? */
    if((pTask->enStatus == TASK_ENABLE) && 
       (LulNowTick - pTask->ulLastTick >= pTask->ulInterval))
    {
      /* Remember the timestamp at which we make the function call. */
      pTask->ulLastTick = LulNowTick;

      /* Call the task function, passing the provided parameter */
      pTask->pfnFunction();
    }
    pTask++;
  }
}

//*****************************************************************************
//
//! Enables a task and allows the scheduler to call it periodically.
//!
//! \param ui32Index is the index of the task which is to be enabled in the
//! global \e Sch_GaaTable array.
//! \param bRunNow is \b true if the task is to be run on the next call to
//! Sch_MainFunction() or \b false if one whole period is to elapse before the task
//! is run.
//!
//! This function marks one of the configured tasks as enabled and causes
//! Sch_MainFunction() to call that task periodically.  The caller may choose to
//! have the enabled task run for the first time on the next call to
//! Sch_MainFunction() or to wait one full task period before making the first
//! call.
//!
//! \return None.
//
//*****************************************************************************
Std_ReturnType Sch_TaskEnable(Sch_TaskIdType ucTaskId, boolean blRunNow)
{
    /* Is the task index passed valid? */
  if(ucTaskId < MAX_TASKS)
  {
    /* Yes - mark the task as active. */
    Sch_GaaTable[ucTaskId].enStatus = TASK_ENABLE;

    /* Set the last call time to ensure that the function is called either
     * next time the scheduler is run or after the desired number of ticks
     * depending upon the value of the blRunNow parameter. */
    
    if(blRunNow == SCH_RUN_NOW)
    {
      /* Cause the task to run on the next call to Sch_MainFunction(). */
      Sch_GaaTable[ucTaskId].ulLastTick =
          (millis() - Sch_GaaTable[ucTaskId].ulInterval);
    }
    else
    {
      /* Cause the task to run after one full time period. */
      Sch_GaaTable[ucTaskId].ulLastTick = millis();
    }
    
    return E_OK;
  }
  else
  {
    return E_NOT_OK;
  }
}

//*****************************************************************************
//
//! Disables a task and prevents the scheduler from calling it.
//!
//! \param ui32Index is the index of the task which is to be disabled in the
//! global \e Sch_GaaTable array.
//!
//! This function marks one of the configured tasks as inactive and prevents
//! Sch_MainFunction() from calling it.  The task may be reenabled by calling
//! Sch_TaskEnable().
//!
//! \return None.
//
//*****************************************************************************
Std_ReturnType Sch_TaskDisable(Sch_TaskIdType ucTaskId)
{
  /* Is the task index passed valid? */
  if(ucTaskId < MAX_TASKS)
  {
    /* Yes - mark the task as inactive. */
    Sch_GaaTable[ucTaskId].enStatus = TASK_DISABLE;
    
    return E_OK;
  }
  else
  {
    return E_NOT_OK;
  }
}

Sch_TaskStatusType Sch_GetStatus(Sch_TaskIdType ucTaskId)
{
  /* Is the task index passed valid? */
  if(ucTaskId < MAX_TASKS)
  {
    /* Yes - mark the task as inactive. */
    return Sch_GaaTable[ucTaskId].enStatus;
  }
  else
  {
    return TASK_UNKNOWN;
  }
}

void Sch_SetInterval(Sch_TaskIdType ucTaskId, uint32_t ulIntervalTime)
{
  /* Is the task index passed valid? */
  if(ucTaskId < MAX_TASKS)
  {
    /* Yes - set interval time. */
    Sch_GaaTable[ucTaskId].ulInterval = ulIntervalTime;
  }
  else
  {
    /* Do nothing */
  }
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
