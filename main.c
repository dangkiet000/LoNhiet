/*******************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 1
 * $Date: 14/11/27
 * @brief    Initilize version
 * @Encoding: UTF-8 without signature.
 * @note
 *  1. Hàm printf: khi print \n sẽ tự động print thêm \n
 *     => Edit hàm SendChar_ToUART trong retarget.c
 *
 ******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Heater_App.h"

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
void Error_Delay(uint32 LulTimeOutCnt);

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/


/*******************************************************************************
**                      Interrupt Service Routine                             **
*******************************************************************************/
/*******************************************************************************
**                     MAIN FUNCTION                                          **
*******************************************************************************/
int main()
{
  Dem_Init();
  
  /* Init GPIO */
  PORT_Init();
  
  
  
  /* Unlock protected registers */
  SYS_UnlockReg();

  /* Init System, peripheral clock and multi-function I/O */
  SYS_Init();

  /* If External crystal is not working, blink LED green. */
  if(DEM_EVENT_STATUS_FAILED == Dem_GetEventStatus(ERROR_CRYSTAL_NOT_WORKING))
  {
    LED_STATUS = LED_STATUS_ON();
    Error_Delay(20000000);
    LED_STATUS = LED_STATUS_OFF();
    Error_Delay(20000000);
    LED_STATUS = LED_STATUS_ON();
    Error_Delay(20000000);
    LED_STATUS = LED_STATUS_OFF();
  }
  
  /* Init SystemTick timer */
  SYSTick_Init(ONE_MS_SYSTICK*1000);

  /* Lock protected registers */
  SYS_LockReg();

  
  
  /* Init to control 4 LED-7seg */
  LED_7Seg_Init();

  /* Initialize button processing events. */
  Btn_Init();

  /* Init Flash module */
  Fls_Init(FLS_CONFIG);

  /* Init TimeOut module */
  TO_Init(TIMEOUT_CONFIG);
  
  #if (DEBUG_MODE == STD_ON)
  /* Init UART1 use for debug and testing */
  UART1_Init();
  #endif

  /* Init ADC use for temperature sensor and thermo-couple
     and PDMA use for tranfering ADC data */
  Get_ADC_Init();


  #if (DEBUG_MODE == STD_ON)
  /* Check the system reset source and report */
  SYS_CheckResetSrc();
  #endif
    
  /* Checking startup date is configured or not. */
  Heater_CheckFlashData();

  Heater_Startup();


  #if (DATA_LOGGING == STD_ON)
  Sch_TaskEnable(SCH_SendSetPoint_Task, SCH_RUN_LATER);
  #endif

  /* Start convert ADC to get temperature. */
  ADC_StartConvert();

  while(1)
  {
    TO_MainFunction();
    Sch_MainFunction();
    Btn_MainFunction();
  }
}

void Error_Delay(uint32 LulTimeOutCnt)
{
  while(LulTimeOutCnt != 0)
  {
     LulTimeOutCnt--;
  }
}

/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
