/**************************************************************************//**
 * @file     Dem.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 08/06/2018 3:59p $
 * @brief    Dem event process.
 *
 * @note
*****************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Dem.h"

/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/


/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/


/*******************************************************************************
**                         Global Data                                        **
*******************************************************************************/

/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/


/*******************************************************************************
**                      Dem_SetEventStatus                                 **
*******************************************************************************/
/**
  * @brief Initialize dem.
  * @param[in] None.
  * @return  None.
  * @details None.
  * @note None.
  */
void Dem_Init(void)
{
  uint8 i;
  
  for(i=0; i<MAX_DEM_EVENT_ID; i++)
  {
    Dem_GaaConfig[i].enStatus = DEM_EVENT_STATUS_PASSED;
  }
}
/**
  * @brief Set dem event status.
  * @param[in] EventId: ID of Dem event.
  * @param[in] EventStatus: status of Dem event.
  * @return  None.
  * @details Set dem event status as DEM_EVENT_STATUS_PASSED or 
  *          DEM_EVENT_STATUS_FAILED.
  * @note None.
  */
void Dem_SetEventStatus(Dem_EventIdType EventId, \
                        Dem_EventStatusType EventStatus)
{
  if(EventStatus == DEM_EVENT_STATUS_PASSED)
  {
    if(Dem_GaaConfig[EventId].enStatus != DEM_EVENT_STATUS_PASSED)
    {
      Dem_GaaConfig[EventId].pPassedEvent();
    }
  }
  else
  {
    if(Dem_GaaConfig[EventId].enStatus != DEM_EVENT_STATUS_FAILED)
    {
      Dem_GaaConfig[EventId].pFailedEvent();
    }
  }
  Dem_GaaConfig[EventId].enStatus = EventStatus;
  
}

/**
  * @brief Get dem event status.
  * @param[in] EventId: ID of Dem event.
  * @return  Dem_EventStatusType :
  *          DEM_EVENT_STATUS_PASSED: if no error is detected.
  *          DEM_EVENT_STATUS_FAILED: if error is detected.
  * @details None.
  * @note None.
  */
Dem_EventStatusType Dem_GetEventStatus(Dem_EventIdType EventId)
{
  return Dem_GaaConfig[EventId].enStatus;
}

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
