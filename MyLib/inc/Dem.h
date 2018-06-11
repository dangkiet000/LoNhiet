/**************************************************************************//**
 * @file     Dem.h
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
#include "Std_Types.h"
#include "Dem_Cfg.h"

#ifndef __DEM_H__
#define __DEM_H__
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/


/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

typedef enum {
 DEM_EVENT_STATUS_PASSED,
 DEM_EVENT_STATUS_FAILED
} Dem_EventStatusType;

typedef uint16 Dem_EventIdType;

/* Definition of Dem configuration type. */
typedef struct STag_Dem_ConfigType
{
  /* Dem Event Status. */
  Dem_EventStatusType enStatus;
}Dem_ConfigType;

/* Global pointer point to constant flash configuration. */
extern Dem_ConfigType Dem_GaaConfig[];

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
void Dem_SetEventStatus(Dem_EventIdType EventId, \
                        Dem_EventStatusType EventStatus);
Dem_EventStatusType Dem_GetEventStatus (Dem_EventIdType EventId);

#endif /* __DEM_H__ */

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
