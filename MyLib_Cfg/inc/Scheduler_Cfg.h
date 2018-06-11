/*******************************************************************************
 * @file     Scheduler_Cfg.h
 * @version  V1.00
 * $Revision: 1
 * $Date: 11/04/2018
 * @brief    Đây là file cấu hình cho Scheduler.
 *
 * @Encoding: UTF-8 without signature
 * @note
 *
 ******************************************************************************/
#ifndef __SCHEDULER_CFG_H__
#define __SCHEDULER_CFG_H__

#define MAX_TASKS   6U

/* Define Name's Tasks */
#define SCH_UpdateADC_Task            0U
#define SCH_Display_Task              1U
#define SCH_SendSetPoint_Task         2U
#define SCH_StoringWorkingTime_Task   3U
#define SCH_HeatingControl_Task       4U
#define SCH_HeaterDiagnostics_Task    5U

/* Scheduler call-back function prototypes */
extern void DisplayTask(void);
extern void Send_SetPoint_to_PC(void);
extern void UpdateADCValue(void);
extern void StoringWorkingTime(void);
/* Controlling temperture of heater. */
extern void HeatingControl_MainFunction(void);
/* Reads the heater diagnostic status periodically and sets product/development 
   accordingly. */
extern void Heater_MainFunctionDiagnostics(void);



#endif // __SCHEDULER_CFG_H__
