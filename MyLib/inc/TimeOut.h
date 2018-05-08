/*******************************************************************************
 * @file     TimeOut.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 07/05/2018
 * @brief    TimeOut library.
 *
 * @note
*******************************************************************************/
#ifndef __TIMEOUT_H__
#define __TIMEOUT_H__

/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "SystemTick_App.h"
#include "TimeOut_Cfg.h"

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/


/*******************************************************************************
**                      Type Definition                                       **
*******************************************************************************/
/* Definition of TO channel status type */
typedef enum STag_TO_ChannelStatusType
{
  TO_RUNNING = 1,
  TO_IDLE
} TO_ChannelStatusType;

typedef struct STag_TO_ChannelType
{
  /* Timeout of channel in miliseconds. */
  const uint32 ulTimeOut;
  
  /* ulStartTime of timeout channel */
  uint32 ulStartTime;
  
  /* TO channel status. */
  TO_ChannelStatusType enStatus;
  
  /* A function pointer to the user function which is to be called periodically
   * by the scheduler.
   * This is const pointer to function
   */
  void (*const pNotification)(void);
}TO_ChannelType;
 

/*******************************************************************************
**                      Global data                                           **
*******************************************************************************/
extern TO_ChannelType *TO_GpConfig;
extern TO_ChannelType TO_GaaChannels[];

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
Std_ReturnType TO_Trigger(uint8 Channel);
Std_ReturnType TO_Clear(uint8 Channel);
Std_ReturnType TO_Init(TO_ChannelType *ConfigPtr);
void TO_DeInit(void);
void TO_MainFunction(void);

#endif //__TIMEOUT_H__


