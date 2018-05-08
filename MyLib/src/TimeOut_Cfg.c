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
  /* Index: 0 - TO_UpdateSetPoint */
  {
    /* ulTimeOut */
    4000,
    
    /* ulStartTime */
    0,
    
    /* enStatus */
    TO_IDLE,
    
    /* pNotification */
    NULL_PTR
  }
};

