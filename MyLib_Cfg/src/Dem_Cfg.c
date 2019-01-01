/*******************************************************************************
 * @file     Dem_Cfg.c
 * @version  V1.00
 * $Revision: 1
 * $Date: 10/06/2018
 * @brief    Diagnostic Event Manager configuration in application.
 *
 * @Encoding: UTF-8 without signature
 * @note
 *
 ******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Dem.h"

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
Dem_ConfigType Dem_GaaConfig[] =
{
  /* Index: 0 - ERROR_THERMO_NOT_CONNECTED */
  {
    /* enStatus */
    DEM_EVENT_STATUS_PASSED,
    /* pFunction */
    &Dem_ErrThermoNotConnected_PassedEvent,
    /* pFunction */
    &Dem_ErrThermoNotConnected_FailedEvent,
  },
  /* Index: 1 - ERROR_LM35_NOT_WORKING */
  {
    /* enStatus */
    DEM_EVENT_STATUS_PASSED,
    /* pFunction */
    &Dem_ErrLM35NotWorking_PassedEvent,
    /* pFunction */
    &Dem_ErrLM35NotWorking_FailedEvent,
  }
};

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
