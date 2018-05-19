/*******************************************************************************
 * @file     TimeOut.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 07/05/2018
 * @brief    TimeOut library.
 *
 * @note
*******************************************************************************/
#include "TimeOut.h"


/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
TO_ChannelType *TO_GpConfig;

/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/
/**
  * @brief  This function start count timeout.
  * @param[in]  TO_Channel: Channel id.
  * @return  E_OK.
  *          E_NOT_OK.
  * @details 
  */
Std_ReturnType TO_Trigger(uint8 Channel)
{
  TO_ChannelType *pTOChannel;
  
  if (Channel >= MAX_TO_CHANNELS)
  {
    return E_NOT_OK;
  }
  else
  {
    /* Get a pointer to the task information. */
    pTOChannel = &TO_GaaChannels[Channel];
    
    if(pTOChannel->enStatus == TO_IDLE)
    {
      /* Get start time of channel. */
      pTOChannel->ulStartTime = millis();
      
      /* Change status to running */
      pTOChannel->enStatus = TO_RUNNING;
    }
    else
    {
      return E_NOT_OK;
    }
    return E_OK;
  } 
}

/**
  * @brief  Clear timeout counter .
  * @param[in]  TO_Channel: Channel id.
  * @return  E_OK.
  *          E_NOT_OK.
  * @details 
  */
Std_ReturnType TO_Clear(uint8 Channel)
{
  TO_ChannelType *pTOChannel;
  
  if (Channel >= MAX_TO_CHANNELS)
  {
    return E_NOT_OK;
  }
  else
  {
    /* Get a pointer to the task information. */
    pTOChannel = &TO_GaaChannels[Channel];
    
    if(pTOChannel->enStatus == TO_RUNNING)
    {
      /* Reset status and start time of channel. */
      pTOChannel->enStatus = TO_IDLE;
      pTOChannel->ulStartTime = 0;
    }

    return E_OK;
  } 
}

/**
  * @brief  Polling all channel to check timeout events.
  * @param[in]  None.
  * @return  None.
  * @details 
  */
void TO_MainFunction(void)
{
  uint8 LucCnt;
  uint32 LulNowTick;
  TO_ChannelType *pTOChannel;
  
  /* Get a pointer to the task information. */
  pTOChannel = &TO_GaaChannels[0];
  
  /* Get current time */
  LulNowTick = millis();
  
  /* Loop through each task in the task table. */
  for(LucCnt = 0; LucCnt < MAX_TO_CHANNELS; LucCnt++)
  {
    /* To check channel is active or not */
    if(pTOChannel->enStatus == TO_RUNNING)
    {
      if((LulNowTick - pTOChannel->ulStartTime) > pTOChannel->ulTimeOut)
      {
        /* Call notification function. */
        pTOChannel->pNotification();
        
        /* Reset status and start time of channel. */
        pTOChannel->ulStartTime = 0;
        pTOChannel->enStatus = TO_IDLE;
      }
      else
      {
        /* Do nothing. */
      }
    }
    else
    {
      /* Do nothing. */
    }
    
    pTOChannel++;
  }
}

/**
  * @brief  Initialization of TimeOut channels.
  * @param[in]  *ConfigPtr: pointer point to TimeOut configuration.
  * @return  E_OK.
  *          E_NOT_OK.
  * @details 
  */
Std_ReturnType TO_Init(TO_ChannelType *ConfigPtr)
{
  uint8 LucCnt;
  TO_ChannelType *pTOChannel;
  
  if(ConfigPtr == NULL_PTR)
  {
    return E_NOT_OK;
  }

  /* Load ConfigPtr to Global pointer variable */
  TO_GpConfig = ConfigPtr;
  
  /* Loop through each task in the task table. */
  for(LucCnt = 0; LucCnt < MAX_TO_CHANNELS; LucCnt++)
  {
    pTOChannel = TO_GpConfig + LucCnt;

    /* Reset status and start time of channel. */
    pTOChannel->ulStartTime = 0;
    pTOChannel->enStatus = TO_IDLE;
  }
  
  return E_OK;
}

/**
  * @brief  De-Initialization of TimeOut channels.
  * @param[in]  None.
  * @return  None.
  * @details 
  */
void TO_DeInit(void)
{
  TO_GpConfig = NULL_PTR;
}

/**
  * @brief  Reload timeout counter .
  * @param[in]  TO_Channel: Channel id.
  * @return  E_OK.
  *          E_NOT_OK.
  * @details 
  */
Std_ReturnType TO_Reload(uint8 Channel)
{
  TO_ChannelType *pTOChannel;
  
  if (Channel >= MAX_TO_CHANNELS)
  {
    return E_NOT_OK;
  }
  else
  {
    /* Get a pointer to the task information. */
    pTOChannel = &TO_GaaChannels[Channel];
    
    if(pTOChannel->enStatus == TO_RUNNING)
    {
      /* Reload counter. */
      pTOChannel->ulStartTime = millis();
    }

    return E_OK;
  } 
}
/*******************************************************************************
**                      Low Level Function                                    **
*******************************************************************************/


/*******************************************************************************
**                      API Functions                                         **
*******************************************************************************/




