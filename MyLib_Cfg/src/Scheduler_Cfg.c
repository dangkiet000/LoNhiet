/*******************************************************************************
 * @file     Scheduler_Cfg.c
 * @version  V1.00
 * $Revision: 1
 * $Date: 11/04/2018
 * @brief    Đây là file cấu hình cho Scheduler.
 *
 * @Encoding: UTF-8 without signature
 * @note
 *
 ******************************************************************************/

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
    1000,
    /* ulLastTick */
    0,
    /* enStatus */
    TASK_ENABLE
  },
  /* Index: 2 - SCH_SendSetPoint_Task */
  {
    /* pfnFunction */
    &Send_SetPoint_to_PC,
    /* ulInterval */
    20000,
    /* ulLastTick */
    0,
    /* enStatus */
    TASK_DISABLE
  },
  /* Index: 3 - SCH_StoringWorkingTime_Task */
  {
    /* pfnFunction */
    &StoringWorkingTime,
    /* ulInterval */
    30*60*1000, /* 30 minutes */
    /* ulLastTick */
    0,
    /* enStatus */
    TASK_ENABLE
  },
  /* Index: 4 - SCH_HeatingControl_Task */
  {
    /* pfnFunction */
    &HeatingControl_MainFunction,
    /* ulInterval */
    1,
    /* ulLastTick */
    0,
    /* enStatus */
    TASK_ENABLE
  },
  /* Index: 5 - SCH_HeaterDiagnostics_Task */
  {
    /* pfnFunction */
    &Heater_MainFunctionDiagnostics,
    /* ulInterval */
    3,
    /* ulLastTick */
    0,
    /* enStatus */
    TASK_ENABLE
  }
};

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
