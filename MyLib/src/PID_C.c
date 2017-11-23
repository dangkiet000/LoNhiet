/*******************************************************************************
 * Port Arduino PID Library - Version 1.1.1
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 *
 * This Library is licensed under a GPLv3 License
 ******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "PID_C.h"
#include "Std_Common.h"

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
  
int32_t GslKp;                  // * (P)roportional Tuning Parameter
int32_t GslKi;                  // * (I)ntegral Tuning Parameter
int32_t GslKd;                  // * (D)erivative Tuning Parameter
/* Pointers to the Input, Output, and Setpoint variables
 * This creates a hard link between the variables and the PID, freeing the user 
 * from having to constantly tell us what these values are with pointers we'll
 * just know.
 */           
int32_t *GpSetpoint;  
int32_t ITerm, GslLastInput;
boolean GblPIDDirection;
boolean GblPIDMode;
int32_t GslMinOutputValue, GslMaxOutputValue;

/*Constructor (...)*********************************************************
 *    The parameters specified here are those for for which we can't set up 
 *    reliable defaults, so we need to have the user set them.
 ***************************************************************************/
void PID_Init(int32_t *LpSetpoint,
     int32_t *LslKp, int32_t *LslKi, int32_t *LslKd, boolean LblPIDDirection)
{
  GpSetpoint = LpSetpoint;
  
  /* Default PID mode: AUTOMATIC */
	GblPIDMode = PID_AUTOMATIC;
	
  /* Default output limit 0 - 255 */
	PID_SetOutputLimits(0, 255);	
  
  /* Default output limit 0 - 255 */
  GblPIDDirection = LblPIDDirection;
  PID_SetTunings(LslKp, LslKi, LslKd);			
}

/**
  * @brief  Allows the controller Mode to be set to manual (0) or 
  *         Automatic (non-zero).
  * @param[in] Mode: PID_AUTOMATIC or PID_MANUAL.
  * @return None.
  * @details  Allows the controller Mode to be set to manual (0) or 
  *           Automatic (non-zero) when the transition from manual to auto
  *           occurs, the controller is automatically initialized.
  */
void PID_SetMode(boolean LblNewMode)
{
  
  GblPIDMode = LblNewMode;
}
 
/* Compute() **********************************************************************
 *     This, as they say, is where the magic happens.  this function should be called
 *   every time "void loop()" executes.  the function will decide for itself whether a new
 *   pid Output needs to be computed.  returns TRUE when the output is computed,
 *   FALSE when nothing has been done.
 * PID equation:
 * Output = Kp*Err + Ki*ErrSum + Kd*dErr
 * dErr/dt = - dInput/dt
 * Output = Kp*Err + Ki*ErrSum - Kd*dInput
 * Where: 
 *        Err = Setpoint - Input;
 **********************************************************************************/ 
int32_t PID_Compute(int32_t LslInput)
{
  int32_t LslError;
  int32_t LslDInput = 0;
  int32_t LslOutput = 0;
  
  if(GblPIDMode == PID_MANUAL)
  {
    return FALSE;
  }
    
  /* Calculate Proportional Error for Kp */
  LslError = *GpSetpoint - LslInput;
  
  /* Nhiet do thay doi rat cham, quan tinh lon 
  => ngay tu luc ERROR=0 , t se bom truoc */
  if(LslError == 0)
  {
    LslError = 1;
  }
  /* Calculate Integral Error for Ki */
  ITerm += (GslKi*LslError);
  if(ITerm > GslMaxOutputValue) ITerm = GslMaxOutputValue;
  else if(ITerm < GslMinOutputValue) ITerm = GslMinOutputValue;
  
  /* Calculate Derivative Error for Kd */
  LslDInput = (LslInput - GslLastInput);

  /* Compute PID Output */
  LslOutput = GslKp*LslError + ITerm - GslKd*LslDInput;

  /* Set Output limit value */
  if(LslOutput > GslMaxOutputValue)
  {    
    LslOutput = GslMaxOutputValue;
  }
  else if(LslOutput < GslMinOutputValue) 
  {
    LslOutput = GslMinOutputValue;
  }
  /* Remember some variables for next time */
  GslLastInput = LslInput;
  
  return LslOutput;
}

/* SetOutputLimits(...)****************************************************
 *     This function will be used far more often than SetInputLimits.  while
 *  the input to the controller will generally be in the 0-1023 range (which is
 *  the default already,)  the output will be a little different.  maybe they'll
 *  be doing a time window and will need 0-8000 or something.  or maybe they'll
 *  want to clamp it from 0-125.  who knows.  at any rate, that can all be done
 *  here.
 **************************************************************************/
boolean PID_SetOutputLimits(int32_t LslMinOutValue, int32_t LslMaxOutValue)
{
  if(LslMinOutValue >= LslMaxOutValue)
  {
    return E_NOT_OK;
  }

  GslMinOutputValue = LslMinOutValue;
  GslMaxOutputValue = LslMaxOutValue;

  if(GblPIDMode == PID_AUTOMATIC)
  {
   if(ITerm > GslMaxOutputValue)
   {
     ITerm = GslMaxOutputValue;
   }
   else if(ITerm < GslMinOutputValue)
   {       
     ITerm = GslMinOutputValue;
   }
  }
  return E_OK;
}

/* SetTunings(...)*************************************************************
 * This function allows the controller's dynamic performance to be adjusted. 
 * it's called automatically from the constructor, but tunings can also
 * be adjusted on the fly during normal operation
 ******************************************************************************/ 
boolean PID_SetTunings(int32_t *LslKp, int32_t *LslKi, int32_t *LslKd)
{
  if (*LslKp<0 || *LslKi<0 || *LslKd<0)
  {
    return E_NOT_OK;
  }

  /* We have to convert PID_SAMPLETIME (ms) to seconds */
  if(*LslKp > GslMaxOutputValue)
  {
    GslKp = GslMaxOutputValue;
    *LslKp = GslMaxOutputValue;
  }
  else
  {
    GslKp = *LslKp;
  }
  
  if(*LslKi > GslMaxOutputValue)
  {
    GslKi = GslMaxOutputValue;
    *LslKi = GslMaxOutputValue;
  }
  else
  {
    GslKi = ((*LslKi)*PID_SAMPLETIME)/1000;
  }
  
  if(*LslKd > GslMaxOutputValue)
  {
    GslKd = GslMaxOutputValue;
    *LslKd = GslMaxOutputValue;
  }
  else
  {
    GslKd = ((*LslKd)*1000) / PID_SAMPLETIME; 
  }

  if(GblPIDDirection == PID_REVERSE)
  {
    GslKp = (0 - GslKp);
    GslKi = (0 - GslKi);
    GslKd = (0 - GslKd);
  }
  
  return E_OK;
}
  

/* SetControllerDirection(...)*************************************************
 * The PID will either be connected to a PID_DIRECT acting process (+Output  
 * leads to +Input) or a PID_REVERSE acting process(+Output leads to -Input).
 * We need to know which one, because otherwise we may increase the output 
 * when we should be decreasing.
 * This is called from the constructor.
 ******************************************************************************/
void PID_SetControllerDirection(boolean LblDirection)
{
  if(GblPIDMode && LblDirection != GblPIDDirection)
  {
    GslKp = (0 - GslKp);
    GslKi = (0 - GslKi);
    GslKp = (0 - GslKp);
  }
  
  GblPIDDirection = LblDirection;
}

/* SetSampleTime(...) *********************************************************
 * sets the period, in Milliseconds, at which the calculation is performed
 *
 * PID equation:
 * Output = Kp*Err + Ki*ErrSum + Kd*dErr;
 * Where: 
 * Err = Setpoint - Input;
 * ErrSum += (Err*PID_SAMPLETIME);
 * dErr = (Err - lastErr) / PID_SAMPLETIME;
 * If PID_SAMPLETIME is divide 10 times (PID_SAMPLETIME/10)
 * => Ki_new = Ki/10;
 *    Kp_new = Kp*10;
 * => PID_SAMPLETIME use for change Ki and Kp
 ******************************************************************************/
void PID_SetSampleTime(uint16_t LusSampleTime)
{ 
  float ratio  = (float)LusSampleTime / (float)PID_SAMPLETIME;
  GslKi *= ratio;
  GslKd /= ratio;
}
 
/* Status Funcions*************************************************************
 * Just because you set the Kp=-1 doesn't mean it actually happened.  these
 * functions query the internal state of the PID.  they're here for display 
 * purposes.  this are the functions the PID Front-end uses for example
 ******************************************************************************/
int32_t PID_GetKp(void)
{ 
  return  GslKp;
}
int32_t PID_GetKi(void)
{ 
  return  GslKi*1000/PID_SAMPLETIME;
}
int32_t PID_GetKd(void)
{ 
  return  GslKd*PID_SAMPLETIME/1000;
}
int32_t PID_GetSetPoint(void)
{ 
  return  *GpSetpoint;
}
boolean PID_GetMode(void)
{ 
  return  GblPIDMode ? PID_AUTOMATIC : PID_MANUAL;
}
boolean PID_GetDirection(void)
{ 
  return GblPIDDirection;
}
