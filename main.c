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
  /* Unlock protected registers */
  SYS_UnlockReg();

  /* Init System, peripheral clock and multi-function I/O */
  SYS_Init();
  
  /* Init SystemTick timer */
  SYSTick_Init(ONE_MS_SYSTICK*1000);
  
  /* Lock protected registers */
  SYS_LockReg();

  /* Init GPIO */
  PORT_Init();
 
  /* Init to control 4 LED-7seg */
  LED_7Seg_Init();
  
  /* Init Button */
  Buttons_Init();
  
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
  
  Heater_ReadFlsData(&Heater, FLS_SETPOINT);
  Heater_ReadFlsData(&Heater, FLS_WORKINGTIME);
  
  
  Heater_ReadFlsData(&Heater, FLS_NGAY);
  Heater_ReadFlsData(&Heater, FLS_THANG);
  Heater_ReadFlsData(&Heater, FLS_NAM);

  #if (DEBUG_MODE == STD_ON)
  printf("Working Time: %d minutes\r\n", Heater.ulWorkingTime*30);
  printf("SetPoint: %d\r\n", Heater.usSetPoint);
  #endif
  
  
 
  #if (DEBUG_MODE == STD_ON)
  printf("Activation Lock Status: ");
  if(Heater.enActiLockStatus == LONHIET_LOCKED)
  {
    printf("LOCKED \r\n");
  }
  else if(Heater.enActiLockStatus == LONHIET_UNLOCKED)
  {
    printf("UNLOCKED \r\n");
  }
  else if(Heater.enActiLockStatus == LONHIET_TRIAL)
  {
    printf("TRIAL \r\n");
  }
  else
  {
    printf("ERROR \r\n");
  }
  #endif
  
  /* Display Set-Point at Startup time.*/
  LED7_DisplayNumber(Heater.usSetPoint);
  
  /* Blinking LED7_DisplayNumber to inform that STARTUP is finished! */
  BlinkingAllLED7_Synchronous(1500);

  /* Set haeter status as IDLE. */
  Heater.enOpStatus = HEATER_IDLE;
  
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

/*************    HardWare connection:    *************************************
* LED-7Seg:   a <-> PB0
*             f <-> PB1
*             b <-> PB2
*             g <-> PB3
*             c <-> PC0
*             dot <-> PC1
*             e <-> PC2
*             d <-> PC3
*          LED1 <-> PA8
*          LED2 <-> PA9
*          LED3 <-> PA10
*          LED4 <-> PA11
*
* Button:  Button+ <-> PA2
*          Button- <-> PA3
*          ButtonSET <-> PA4
* Triac: 
* Triac control  <-> PA12
* Triac feedback <-> PA5
*
* Speaker pin    <-> PB8
*
* ADC Thermo-Couple <-> PA0
* ADC LM35          <-> PA1
********************************************************************************
Do giam nhiet do: 19s/do C                                                    */
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
