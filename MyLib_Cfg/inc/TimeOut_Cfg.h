/*******************************************************************************
 * @file     TimeOut_Cfg.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 08/05/2018
 * @brief    TimeOut configuration.
 *
 * @note
*******************************************************************************/
#ifndef __TIMEOUT_CFG_H__
#define __TIMEOUT_CFG_H__

/*******************************************************************************
**                      Include                                               **
*******************************************************************************/

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define MAX_TO_CHANNELS              3U


#define TO_UpdateSetPoint_Channel    0U
#define TO_SetDateTime_Channel       1U
#define TO_EnterPassword_Channel     2U



extern void TO_UpdateSetPoint(void);
extern void TO_SetDateTime(void);
extern void TO_EnterPassword(void);


#define TIMEOUT_CONFIG               (&TO_GaaChannels[0])

#endif //__TIMEOUT_CFG_H__


