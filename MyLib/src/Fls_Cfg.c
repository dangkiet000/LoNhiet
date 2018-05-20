/*******************************************************************************
 * @file     Fls_Cfg.c
 * @version  V1.00
 * $Revision: 1
 * $Date: 19/05/2018
 * @brief    Đây là file cấu hình cho Flash memory in application.
 *
 * @Encoding: UTF-8 without signature
 * @note
 *
 ******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Fls_Cfg.h"

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
const Fls_DataConfigType Fls_GaaConfig[] =
{
  /* Index: 0 - FLS_SETPOINT */
  {
    /* ddLen */
    1,
    /* ddAddr */
    1,
    /* enPage */
    DATA_FLS_PAGE_ONE
  },
  /* Index: 1 - FLS_WORKINGTIME */
  {
    /* ddLen */
    1,
    /* ddAddr */
    2,
    /* enPage */
    DATA_FLS_PAGE_ONE
  },
  /* Index: 2 - FLS_ACTILOCKSTATUS */
  {
    /* ddLen */
    1,
    /* ddAddr */
    3,
    /* enPage */
    DATA_FLS_PAGE_ONE
  },
  /* Index: 3 - FLS_NGAY */
  {
    /* ddLen */
    1,
    /* ddAddr */
    4,
    /* enPage */
    DATA_FLS_PAGE_ONE
  },
  /* Index: 4 - FLS_THANG */
  {
    /* ddLen */
    1,
    /* ddAddr */
    5,
    /* enPage */
    DATA_FLS_PAGE_ONE
  },
  /* Index: 5 - FLS_NAM */
  {
    /* ddLen */
    1,
    /* ddAddr */
    6,
    /* enPage */
    DATA_FLS_PAGE_ONE
  }
};

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
