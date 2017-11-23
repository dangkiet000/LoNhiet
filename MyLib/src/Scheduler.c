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

#include "NUC200Series.h"
#include "Scheduler.h"


//*****************************************************************************
//
//! \addtogroup scheduler_api
//! @{
//
//*****************************************************************************

tSchedulerTask Gaa_SchedulerTable[NUM_TASKS];

//*****************************************************************************
//
//! Instructs the scheduler to update its task table and make calls to
//! functions needing called.
//!
//! This function must be called periodically by the client to allow the
//! scheduler to make calls to any configured task functions if it is their
//! time to be called.  The call must be made at least as frequently as the
//! most frequent task configured in the Gaa_SchedulerTable array.
//!
//! Although the scheduler makes use of the SysTick interrupt, all calls to
//! functions configured in \e Gaa_SchedulerTable are made in the context of
//! SchedulerPoll().
//!
//! \return None.
//
//*****************************************************************************
void SchedulerPoll(void)
{
  uint8_t LucTaskIndex;
  uint32_t LulNowTick;
  tSchedulerTask *pTask;
  
  
  /* Get current time */
  LulNowTick = millis();
  
  /* Loop through each task in the task table. */
  for(LucTaskIndex = 0; LucTaskIndex < NUM_TASKS; LucTaskIndex++)
  {
    /* Get a pointer to the task information. */
    pTask = &Gaa_SchedulerTable[LucTaskIndex];

    /* Is this task active and, if so, is it time to call it's function? */
    if((pTask->blEnable) && 
       (LulNowTick - pTask->ulLastTick >= pTask->ulInterval))
    {
      /* Remember the timestamp at which we make the function call. */
      pTask->ulLastTick = LulNowTick;

      /* Call the task function, passing the provided parameter */
      pTask->pfnFunction();
    }
  }
}

//*****************************************************************************
//
//! Enables a task and allows the scheduler to call it periodically.
//!
//! \param ui32Index is the index of the task which is to be enabled in the
//! global \e Gaa_SchedulerTable array.
//! \param bRunNow is \b true if the task is to be run on the next call to
//! SchedulerPoll() or \b false if one whole period is to elapse before the task
//! is run.
//!
//! This function marks one of the configured tasks as enabled and causes
//! SchedulerPoll() to call that task periodically.  The caller may choose to
//! have the enabled task run for the first time on the next call to
//! SchedulerPoll() or to wait one full task period before making the first
//! call.
//!
//! \return None.
//
//*****************************************************************************
boolean SchedulerTaskEnable(uint8_t LucIndex, boolean blEnable)
{
    /* Is the task index passed valid? */
  if(LucIndex < NUM_TASKS)
  {
    /* Yes - mark the task as active. */
    Gaa_SchedulerTable[LucIndex].blEnable = TRUE;

    /* Set the last call time to ensure that the function is called either
     * next time the scheduler is run or after the desired number of ticks
     * depending upon the value of the bRunNow parameter. */
    
    if(blEnable == TRUE)
    {
      /* Cause the task to run on the next call to SchedulerPoll(). */
      Gaa_SchedulerTable[LucIndex].ulLastTick =
          (millis() - Gaa_SchedulerTable[LucIndex].ulInterval);
    }
    else
    {
      /* Cause the task to run after one full time period. */
      Gaa_SchedulerTable[LucIndex].ulLastTick = millis();
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
//! global \e Gaa_SchedulerTable array.
//!
//! This function marks one of the configured tasks as inactive and prevents
//! SchedulerPoll() from calling it.  The task may be reenabled by calling
//! SchedulerTaskEnable().
//!
//! \return None.
//
//*****************************************************************************
boolean SchedulerTaskDisable(uint8_t LucIndex)
{
  /* Is the task index passed valid? */
  if(LucIndex < NUM_TASKS)
  {
    /* Yes - mark the task as inactive. */
    Gaa_SchedulerTable[LucIndex].blEnable = FALSE;
    
    return E_OK;
  }
  else
  {
    return E_NOT_OK;
  }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
