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
#include <stdio.h>
#include "SystemTick_App.h"

#include "Temperature_Conversion.h"
#include "Get_ADC_Value.h"
#include "Scheduler.h"
#include "LED7Segment.h"
#include "Button_Processing.h"
#include "Fls.h"
#include "TimeOut.h"

#include "Heater_Types.h"
#include "LoNhiet_UserCfg.h"
#include "LoNhiet_DevCfg.h"
/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/


#define NUM_OF_BLINK              10U
#define BLINK_TIME                400U

#define LIMIT_MAX_SETPOINT(SPvar)  ((SPvar > MAX_TEMP_TYPE_K) ? MAX_TEMP_TYPE_K : SPvar)
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
void SYS_Init(void);
void PORT_Init(void);
void Buttons_Init(void);
void Timer0_Init(void);

/* Read all data which store in data flash memory. */
void Heater_ReadAllFlsData(HeaterType *pHeater);

/* Blinking LED7-Seg in milisecond synchronously. */
void BlinkingLED7_Synchronous(uint32 duration);


/* Button call-back function prototypes */
void BSET_HoldToThres_Event(void);
void BSET_BCONG_HoldToThres_Event(void);
void BCONG_Release_Event(void);
void BTRU_Release_Event(void);
void BSET_Release_Event(void);


#if (DEBUG_MODE == STD_ON)
Std_ReturnType SYS_CheckResetSrc(void);
void UART1_Init(void);

#if (DATA_LOGGING == STD_ON)
void Send_ADCtoPC(uint16_t *Data, uint16_t Len);
void Send_TemptoPC(void);
#endif

#endif


#if (TEST_MODE == STD_ON)
void SpeakerTest(void);
void TriacTest(void);
void Led7segTest(void);
#endif

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/* Temperature variables */
uint8_t  Temp_MT = 29;   /* Nhiet do Moi Truong    */

/* Data flash variables */
/* This array contains all data stored in flash memory */
uint32_t GaaStoreData[DATA_FLS_LEN];
uint32_t GaaReadStoreData[DATA_FLS_LEN];

/* LED7 control variables */
uint8_t GucBlinkTimes;

uint8_t GucLEDIndex;

uint16_t GusLastTempTHC;

/* Lo Nhiet Global structure */
HeaterType Heater;

/*******************************************************************************
**                      Interrupt Service Routine                             **
*******************************************************************************/
/*------------------------- TimeOut Events -----------------------------------*/
void TO_UpdateSetPoint(void)
{
  printf("TimeOut Event = %d", millis());
}
void TO_SetDateTime(void)
{

}
void TO_EnterPassword(void)
{

}

/*------------------------- Scheduler Events ---------------------------------*/

void DisplayTask(void)
{
  LED_TEST ^= 1;
  
  if(Sch_GetStatus(SCH_BlinkingLED_Task) == TASK_DISABLE)
  {
    LED7Seg_Show(Heater.usTempTHC);
    
    #if (DATA_LOGGING == STD_ON)
    Send_TemptoPC();
    #endif
  }
}
void BlinkingLed(void)
{
  if(GucBlinkTimes == 0)
  {

    Sch_TaskDisable(SCH_BlinkingLED_Task);
  }
  else
  {

    GucBlinkTimes--;
  }
}


void Send_SetPoint_to_PC(void)
{
  #if (DATA_LOGGING == STD_ON)
  printf("SP %d\n", Heater.usSetPoint);
  #endif
}


void UpdateADCValue(void)
{
  if(ADC_IDLE == ADC_GetStatus())
  {
    Heater.usTempTHC = GetTemp_ThermoCouple();
  }
  ADC_StartConvert();
}

/* Storing working-time every 30 minutes to data flash memory */
void StoringWorkingTime(void)
{
  /* To check if working-time is over Trial time */
  if(Heater.ulWorkingTime > TRIAL_TIME_IN_MIN)
  {
    /* Yes. Disable LoNhiet */
  
  }
  else /* No */
  {
    /* Accumulate working time */
    Heater.ulWorkingTime += 30;
    
    /* To Store working-time in flash memory */
    Fls_Write(FLS_PAGE_ONE, DATA_FLS_WORKINGTIME_ADDR, &Heater.ulWorkingTime, 1);
  }
  
}



/*------------------------- Button processing events -------------------------*/
void BCONG_Release_Event(void)
{
  LED7_DisableBlinking();
  /*
  TO_Clear(TO_UpdateSetPoint_Channel);
  printf("TimeOut Clear Event = %d", millis());
  */
  Heater.usSetPoint++;
  Heater.usSetPoint = LIMIT_MAX_SETPOINT(Heater.usSetPoint);

  GucBlinkTimes = NUM_OF_BLINK;
  Sch_TaskEnable(SCH_BlinkingLED_Task, SCH_RUN_LATER);
  Sch_SetInterval(SCH_BlinkingLED_Task, BLINK_TIME);
  LED7Seg_Show(Heater.usSetPoint);
}

void BTRU_Release_Event(void)
{
  if(Heater.usSetPoint > 0)
  {
    Heater.usSetPoint--;
    GucBlinkTimes = NUM_OF_BLINK;
    Sch_TaskEnable(SCH_BlinkingLED_Task, SCH_RUN_LATER);
    Sch_SetInterval(SCH_BlinkingLED_Task, BLINK_TIME);
  }
  LED7Seg_Show(Heater.usSetPoint);
}


void BSET_Release_Event(void)
{
  if(Heater.enOpStatus == HEATER_UPDATE_SETPOINT)
  {
    /* Blink next LED7seg. */
    Heater.ucBlinkLED7Idx++;
    LED7_EnableBlinking(NUMBER_TO_LEDID(GucLEDIndex), 300); 
    LED7Seg_Show(Heater.usSetPoint);

    if(GucLEDIndex > 3)
    {
      /* Store Set-point to memory. */
      
      /* Exit HEATER_UPDATE_SETPOINT mode */
      Sch_TaskEnable(SCH_Display_Task, SCH_RUN_LATER);
      GucLEDIndex = 0;
      LED7_DisableBlinking();
      Heater.enOpStatus = HEATER_IDLE;
    }
    else
    {
    
    }
  }
}
void BSET_HoldToThres_Event(void)
{
  if((Heater.enOpStatus != HEATER_UPDATE_SETPOINT) && \
     (Heater.enOpStatus != HEATER_ENTER_PASSWORD))
  {
    /* Enter HEATER_UPDATE_SETPOINT mode. */
    Sch_TaskDisable(SCH_Display_Task);
    LED7Seg_Show(Heater.usSetPoint);
    
    Heater.ucBlinkLED7Idx = 0;
    LED7_EnableBlinking(NUMBER_TO_LEDID(GucLEDIndex), 300); 

    Heater.enOpStatus = HEATER_UPDATE_SETPOINT;
  }
  else
  {
  
  }
}
void BSET_BCONG_HoldToThres_Event(void)
{
  if((Heater.enOpStatus != HEATER_UPDATE_SETPOINT) && \
     (Heater.enOpStatus != HEATER_ENTER_PASSWORD))
  {
    Heater.enOpStatus = HEATER_ENTER_PASSWORD;
  }
  else
  {
  
  }
}
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
  
  Heater_ReadAllFlsData(&Heater);
  
  
  
  /* To check if working-time is over Trial time */
  if(Heater.ulWorkingTime > TRIAL_TIME_IN_MIN)
  {
    /* Yes. Disable LoNhiet */
    Heater.enActiLockStatus = LONHIET_LOCKED;
  }
  else /* No. Do nothing */
  {
  
  }
 
  LED7Seg_Show(Heater.usSetPoint);
  
  /* Blinking LED7Seg_Show to inform that STARTUP is finished! */
  BlinkingLED7_Synchronous(2000);

  /* Set haeter status as IDLE. */
  Heater.enOpStatus = HEATER_IDLE;
  
  Sch_TaskEnable(SCH_UpdateADC_Task, SCH_RUN_LATER);
  Sch_TaskEnable(SCH_SendSetPoint_Task, SCH_RUN_LATER);
  Sch_TaskEnable(SCH_StoringWorkingTime_Task, SCH_RUN_LATER);
  Sch_TaskDisable(SCH_Display_Task);
  
  /* Start convert ADC to get temperature. */
  ADC_StartConvert();
  
  
  
  //LED7_EnableBlinking(NUMBER_TO_LEDID(3), 300);
  LED7Seg_Show(1234);
  while(1)
  { 
    TO_MainFunction(); 
    Sch_MainFunction(); 
    Btn_MainFunction();
  }
}

/*******************************************************************************
**                      Function                                              **
*******************************************************************************/
/* Read all data which store in data flash memory. */
void Heater_ReadAllFlsData(HeaterType *pHeater)
{
  /* Read Working-time. */
  Fls_Read(FLS_PAGE_ONE, DATA_FLS_ACTILOCKSTATUS_ADDR, \
                        (Fls_DataType *)&pHeater->enActiLockStatus, 1);
                        
  /* Read Activation Lock Status. */
  Fls_Read(FLS_PAGE_ONE, DATA_FLS_WORKINGTIME_ADDR, &pHeater->ulWorkingTime, 1);
  
  /* Read Setpoint. */
  Fls_Read(FLS_PAGE_ONE, DATA_FLS_SETPOINT_ADDR, \
                        (Fls_DataType *)&pHeater->usSetPoint, 1);
  pHeater->usSetPoint = 1111;
}
/* Blinking LED7-Seg in milisecond synchronously. */
void BlinkingLED7_Synchronous(uint32 duration)
{
  LED7_EnableBlinking(LED7_ALL, 300);
  DelaySystemTick_ms(duration);
  LED7_DisableBlinking();
}

/* Init System, peripheral clock and multi-function I/O. */
void SYS_Init(void)
{
  /*--------------------------------------------------------------------------*/
  /* Init System Clock                                                        */
  /*--------------------------------------------------------------------------*/

  /* Enable Internal RC 22.1184MHz clock */
  CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

  /* Waiting for Internal RC clock ready */
  CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

  /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
  CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

  /* Enable external XTAL 12MHz clock */
  CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

  /* Waiting for external XTAL clock ready */
  CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

  /* Set core clock as PLL_CLOCK from PLL */
  CLK_SetCoreClock(PLL_CLOCK);

  /* Disable clock Watchdog */
  CLK_DisableModuleClock(WDT_MODULE);
  
}




/* Configure port pin as alternative function. */
void PORT_Init(void)
{
  /* Cau hinh chan DIEU KHIEN LED_TEST */
  GPIO_SetMode(LED_TEST_PORT, LED_TEST_BIT, GPIO_PMD_OUTPUT);
  LED_TEST = 0;
  
  /* Cau hinh chan DIEU KHIEN TRIAC */
  GPIO_SetMode(TRIAC_PORT, TRIAC_BIT, GPIO_PMD_OUTPUT);
  TRIAC_PIN = TRIAC_OFF;
  
  /* Set GPB multi-function pins for UART0 RXD and TXD */
  SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB4_Msk | SYS_GPB_MFP_PB5_Msk);
  SYS->GPB_MFP |= SYS_GPB_MFP_PB4_UART1_RXD | SYS_GPB_MFP_PB5_UART1_TXD;
  
  /* Disable the GPA0 - GPA1 digital input path to avoid the leakage current. */
  GPIO_DISABLE_DIGITAL_PATH(PA, (1 << ADC_LM35_CH)|(1 << ADC_THC_CH));

  /* Configure the GPA0 - GPA1 ADC analog input pins */
  SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA0_Msk | SYS_GPA_MFP_PA1_Msk);
  SYS->GPA_MFP |= SYS_GPA_MFP_PA0_ADC0 | SYS_GPA_MFP_PA1_ADC1;
  SYS->ALT_MFP1 &= ~(SYS_ALT_MFP1_PA0_Msk| SYS_ALT_MFP1_PA1_Msk);
}
/* Initialize button processing events. */
void Buttons_Init(void)
{
  Btn_ConfigSet[BTRU_ID].enEventType = BTN_RELEASED_EVENT;
  Btn_ConfigSet[BTRU_ID].pfnFunction = &BTRU_Release_Event;
  
  Btn_ConfigSet[BCONG_ID].enEventType = BTN_RELEASED_EVENT;
  Btn_ConfigSet[BCONG_ID].pfnFunction = &BCONG_Release_Event;
  Btn_ConfigSet[BCONG_ID].usHoldThresTime = 3000;
  
  Btn_ConfigSet[BSET_ID].enEventType = BTN_HOLD_EVENT;
  Btn_ConfigSet[BSET_ID].usHoldThresTime = 3000;
  Btn_ConfigSet[BSET_ID].pfnFunction = &BSET_Release_Event;
  Btn_ConfigSet[BSET_ID].pfnHoldEvent1 = &BSET_HoldToThres_Event;
  Btn_ConfigSet[BSET_ID].pfnHoldEvent2 = &BSET_BCONG_HoldToThres_Event;
  
  Btn_Init();
}

#if (DEBUG_MODE == STD_ON)
/* This function check the system reset source and report */
Std_ReturnType SYS_CheckResetSrc(void)
{
  uint32_t LulResetValue = 0;
  Std_ReturnType LddReturnValue = E_OK;
  
  /* Get reset source from last operation */
  LulResetValue = SYS_GetResetSrc();

  if(0 != (LulResetValue & SYS_RSTSRC_RSTS_BOD_Msk))
  {
    printf("Brown-out detector reset 0x%x\n", LulResetValue);
  }
  else if(0 != (LulResetValue & SYS_RSTSRC_RSTS_CPU_Msk))
  {
    printf("CPU reset 0x%x\n", LulResetValue);
  }
  else if(0 != (LulResetValue & SYS_RSTSRC_RSTS_LVR_Msk))
  {
    printf("Low-Voltage-Reset 0x%x\n", LulResetValue);
  }
  else if(0 != (LulResetValue & SYS_RSTSRC_RSTS_POR_Msk))
  {
    printf("Power-on Reset 0x%x\n", LulResetValue);
  }
  else if(0 != (LulResetValue & SYS_RSTSRC_RSTS_RESET_Msk))
  {
    printf("Reset pin reset 0x%x\n", LulResetValue);
  }
  else if(0 != (LulResetValue & SYS_RSTSRC_RSTS_SYS_Msk))
  {
    printf("System reset 0x%x\n", LulResetValue);
  }
  else if(0 != (LulResetValue & SYS_RSTSRC_RSTS_WDT_Msk))
  {
    printf("Window watch dog reset 0x%x\n", LulResetValue);
  }
  else
  {
    LddReturnValue = E_NOT_OK;
    return LddReturnValue;
  }
  
  /* Clear reset source */
  SYS_ClearResetSrc(LulResetValue);
  
  return LddReturnValue;
}

/* UART0 use for debug and testing */
void UART1_Init(void)
{
  /* Enable UART module clock */
  CLK_EnableModuleClock(UART1_MODULE);

  /* Select UART module clock source */
  CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

  /*------------------------------------------------------------------------*/
  /* Init UART                                                              */
  /*------------------------------------------------------------------------*/
  /* Reset UART0 */
  SYS_ResetModule(UART1_RST);

  UART_DisableInt(UART1, UART_IER_RDA_IEN_Msk);

  /* Configure UART0 and set UART0 Baudrate */
  UART_Open(UART1, 115200);
}

void Send_ADCtoPC(uint16_t *Data, uint16_t Len)
{
  uint16_t i=0;
  for(i=0; i<Len; i++)
  {
    printf("TC %d\n", Data[i]);
  }
}

void Send_TemptoPC(void)
{
  printf("TC %d\n", Heater.usTempTHC);
}


#endif

#if (TEST_MODE == STD_ON)
void SpeakerTest()
{
  uint8_t LucCount;
  for(LucCount = 0; LucCount< 15; LucCount++)
  {
    SPEAKER_PIN = SPEAKER_OFF;
    DelaySystemTick_ms(10);
    SPEAKER_PIN = SPEAKER_ON;
    DelaySystemTick_ms(10);
  }
}
void TriacTest(void)
{

}
void Led7segTest(void)
{
  uint8_t LucCount;
  for(LucCount = 0; LucCount< 10; LucCount++)
  {
    LED7Seg_Show(LucCount++);
    DelaySystemTick_ms(400);
  }
}
#endif


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
