/*******************************************************************************
 * @file     Dem_Cfg.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 10/06/2018 11:47p $
 * @brief    Diagnostic Event Manager configuration
 *
 * @note
 * Copyright (C) 2018 DangKiet Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __DEM_CFG_H__
#define __DEM_CFG_H__


/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "Std_Types.h"

/*******************************************************************************
 **                       Type definitions                                    **
 ******************************************************************************/
#define MAX_DEM_EVENT_ID                    (Dem_EventIdType)1U
#define ERROR_THERMO_NOT_CONNECTED          (Dem_EventIdType)0U

/*----------------------------------------------------------------------------*/
/*                          USER CONFIGURATION                                */
/*----------------------------------------------------------------------------*/
/* Scheduler call-back function prototypes */
extern void Dem_ErrThermoNotConnected_PassedEvent(void);
extern void Dem_ErrThermoNotConnected_FailedEvent(void);

#endif /* __DEM_CFG_H__ */

