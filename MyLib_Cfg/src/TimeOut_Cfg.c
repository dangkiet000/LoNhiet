/*******************************************************************************
 * @file     TimeOut_Cfg.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 08/05/2018
 * @brief    TimeOut configuration.
 *
 * @note
*******************************************************************************/
#include "TimeOut.h"

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

TO_ChannelType TO_GaaChannels[] =
{
  /* Index: 0 - TO_UpdateSetPoint_Channel */
  {
    /* ulTimeOut: miliseconds */
    6000,
    
    /* ulStartTime */
    0,
    
    /* enStatus */
    TO_IDLE,
    
    /* pNotification */
    &TO_UpdateSetPoint
  },
  /* Index: 1 - TO_SetDateTime_Channel */
  {
    /* ulTimeOut: miliseconds */
    6000,
    
    /* ulStartTime */
    0,
    
    /* enStatus */
    TO_IDLE,
    
    /* pNotification */
    &TO_SetDateTime
  },
  /* Index: 2 - TO_EnterPassword_Channel */
  {
    /* ulTimeOut: miliseconds */
    10000,
    
    /* ulStartTime */
    0,
    
    /* enStatus */
    TO_IDLE,
    
    /* pNotification */
    &TO_EnterPassword
  }
};

