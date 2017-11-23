/**********************************************************************************************
 * Port Arduino PID Library - Version 1.1.1
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 *
 * This Library is licensed under a GPLv3 License
 **********************************************************************************************/

#ifndef PID_v1_h
#define PID_v1_h
#include <stdint.h>
#include "Std_Common.h"

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define LIBRARY_VERSION	1.1.1

/* Definition PID mode */
#define PID_AUTOMATIC	1
#define PID_MANUAL	0

/* Define direction Output */
#define PID_DIRECT  0
#define PID_REVERSE  1

/* Default Controller Sample Time is 1000 (ms) */
#define PID_SAMPLETIME 1000

extern int32_t GslKp;
extern int32_t GslKi;
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
/**********************    Commonly used functions ****************************/
void PID_Init(int32_t *LpSetpoint,
     int32_t *LslKp, int32_t *LslKi, int32_t *LslKd, boolean LblPIDDirection);

/* Sets PID to either Manual (0) or Auto (non-0) */
void PID_SetMode(boolean LblNewMode);
int32_t PID_Compute(int32_t LslInput);
boolean PID_SetOutputLimits(int32_t LslMinOutValue, int32_t LslMaxOutValue);

/**********************   No Commonly used functions **************************/
boolean PID_SetTunings(int32_t *LslKp, int32_t *LslKi, int32_t *LslKd);
void PID_SetControllerDirection(boolean Direction);
void PID_SetSampleTime(uint16_t LusSampleTime);
int32_t PID_GetKp(void);
int32_t PID_GetKi(void);
int32_t PID_GetKd(void);
int32_t PID_GetSetPoint(void);
boolean PID_GetMode(void);
boolean PID_GetDirection(void);
void PID_Initialize(void);
#endif

