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

#define MAX_TASKS   5U

/* Define Name's Tasks */
#define SCH_UpdateADC_Task            0U
#define SCH_Display_Task              1U
#define SCH_BlinkingLED_Task          2U
#define SCH_SendSetPoint_Task         3U
#define SCH_StoringWorkingTime_Task   4U

/* Scheduler call-back function prototypes */
extern void DisplayTask(void);
extern void BlinkingLed(void);
extern void Send_SetPoint_to_PC(void);
extern void UpdateADCValue(void);
extern void StoringWorkingTime(void);

#endif // __SCHEDULER_CFG_H__
