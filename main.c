/*******************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 1
 * $Date: 14/11/27
 * @brief    Initilize version
 *
 * @note
 *  1. Hàm printf: khi print \n se tu dong print them \n
 *     => Edit hàm SendChar_ToUART trong retarget.c
 *
 ******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include <stdio.h>
#include "SystemTick_App.h"
#include "LoNhiet_Cfg.h"
#include "Temperature_Conversion.h"
#include "Get_ADC_Value.h"
#include "Scheduler.h"
#include "LED7Segment.h"
#include "Button_Processing.h"
#include "Fls.h"

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/

/* System define */
#define PLLCON_SETTING            CLK_PLLCON_50MHz_HXT
#define PLL_CLOCK                 50000000


/* Led test define */
#define LED_TEST                  PA13
#define LED_TEST_PORT             PA
#define LED_TEST_BIT              BIT13

/* Triac control define */
#define TRIAC_PORT                PA
#define TRIAC_PIN                 PA12
#define TRIAC_BIT                 BIT12
#define TRIAC_ON                  0U
#define TRIAC_OFF                 1U


/* Speaker control define */
#define SPEAKER_ON                0U
#define SPEAKER_OFF               1U
#define SPEAKER_PIN               PB8



/* Data flash define */
#define DATA_FLS_LEN              10U
#define DATA_FLS_SETPOINT_ADDR     0U
#define DATA_FLS_WORKINGTIME_ADDR  1U




#define NUM_OF_BLINK              10U
#define BLINK_TIME                400U

/* Lo Nhiet status */
typedef enum ETag_LoNhietStatusType
{
  READING_INFO_SYSTEM = 0,
  LONHIET_IDLE,
  STABLE,
  /* State which user is setting setpoint */
  SETUP_SETPOINT,
  /* State which user is setting date-time */
  SETUP_DATETIME,
  /* State which user is typing password */
  ENTER_PASSWORD
}LoNhietStatusType;



/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
void SYS_Init(void);
void GPIO_Init(void);
void Buttons_Init(void);
void Timer0_Init(void);




/* Button call-back function prototypes */
void BSET_HoldToThres(void);
void BSET_BCONG_HoldToThres(void);
void BCONG_Release(void);
void BTRU_Release(void);


#if (DEBUG_MODE == STD_ON)
Std_ReturnType SYS_CheckResetSrc(void);
void UART1_Init(void);
void Send_ADCtoPC(uint16_t *Data, uint16_t Len);
void Send_TemptoPC(void);
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
uint16_t GusTempTHC = 30;  /* Nhiet do Thermo-couple */

/* Data flash variables */
/* This array contains all data stored in flash memory */
uint32_t GaaStoreData[DATA_FLS_LEN];
uint32_t GaaReadStoreData[DATA_FLS_LEN];

/* LED control variables */
uint8_t GucBlinkTimes;

/* PID variables */
int32_t GslSetPoint = 60; 
uint16_t GusLastTempTHC;
LoNhietStatusType GenLoNhietStatus;

/* This variable store working-time (Unit: minute) */
uint32_t GulWorkingTime;


/*******************************************************************************
**                      Interrupt Service Routine                             **
*******************************************************************************/
/* Scheduler Events */

void DisplayTask(void)
{
  LED_TEST ^= 1;
  
  if(Sch_GetStatus(SCH_BlinkingLED_Task) == TASK_DISABLE)
  {
    LED7Seg_Show(GusTempTHC);
    Send_TemptoPC();
  }
}
void BlinkingLed(void)
{
  if(GucBlinkTimes == 0)
  {
    BlinkingLED(STD_OFF);
    Sch_TaskDisable(SCH_BlinkingLED_Task);
  }
  else
  {
    BlinkingLED(STD_ON);
    GucBlinkTimes--;
  }
}
void Send_SetPoint_to_PC(void)
{
  printf("SP %d\n", GslSetPoint);
}

void UpdateADCValue(void)
{
  if(ADC_IDLE == ADC_GetStatus())
  {
    GusTempTHC = GetTemp_ThermoCouple();
  }
  ADC_StartConvert();
}

/* Storing working-time every 30 minutes to data flash memory */
void StoringWorkingTime(void)
{
  /* To check if working-time is over Trial time */
  if(GulWorkingTime > TRIAL_TIME_IN_MIN)
  {
    /* Yes. Disable LoNhiet */
  
  }
  else /* No */
  {
    /* Accumulate working time */
    GulWorkingTime += 30;
    
    /* To Store working-time in flash memory */
    Fls_Write(FLS_PAGE_ONE, DATA_FLS_WORKINGTIME_ADDR, &GulWorkingTime, 1);
  }
  
}






/* Button processing events */
void BCONG_Release(void)
{
  GslSetPoint++;
  GslSetPoint = ((GslSetPoint > MAX_TEMP_TYPE_K) ? MAX_TEMP_TYPE_K : GslSetPoint);
  GucBlinkTimes = NUM_OF_BLINK;
  Sch_TaskEnable(SCH_BlinkingLED_Task, SCH_RUN_LATER);
  Sch_SetInterval(SCH_BlinkingLED_Task, BLINK_TIME);
  LED7Seg_Show(GslSetPoint);
}

void BTRU_Release(void)
{
  if(GslSetPoint > 0)
  {
    GslSetPoint--;
    GucBlinkTimes = NUM_OF_BLINK;
    Sch_TaskEnable(SCH_BlinkingLED_Task, SCH_RUN_LATER);
    Sch_SetInterval(SCH_BlinkingLED_Task, BLINK_TIME);
  }
  LED7Seg_Show(GslSetPoint);
}

void BSET_HoldToThres(void)
{
  GenLoNhietStatus = SETUP_SETPOINT;
}
void BSET_BCONG_HoldToThres(void)
{
  /*
  GaaStoreData[DATA_FLS_SETPOINT_ADDR] = (uint32_t) GslSetPoint;
  //Fls_Write(DATA_FLS_PAGE_ONE, GaaStoreData, DATA_FLS_LEN);
  GucBlinkTimes = 8;
  Sch_TaskEnable(SCH_BlinkingLED_Task, SCH_RUN_LATER);
  Sch_SetInterval(SCH_BlinkingLED_Task, 200);
  LED7Seg_Show(GslSetPoint);
  */
  GenLoNhietStatus = ENTER_PASSWORD;
  
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
  GPIO_Init();
 
  /* Init to control 4 LED-7seg */
  LED_7Seg_Init();
  
  /* Init Button */
  Buttons_Init();

  #if (DEBUG_MODE == STD_ON)
  /* Init UART1 use for debug and testing */
  UART1_Init();
  #endif
  
  /* Init ADC use for temperature sensor and thermo-couple 
     and PDMA use for tranfering ADC data */
  Get_ADC_Init();
  ADC_StartConvert();
  
  #if (DEBUG_MODE == STD_ON)
  /* Check the system reset source and report */
  SYS_CheckResetSrc();
  #endif
  
  /* Read data from flash memory to get working-time of LoNhiet */
  Fls_Read(FLS_PAGE_ONE, DATA_FLS_WORKINGTIME_ADDR, &GulWorkingTime, 1);
  
  /* To check if working-time is over Trial time */
  if(GulWorkingTime > TRIAL_TIME_IN_MIN)
  {
    /* Yes. Disable LoNhiet */
  
  }
  else /* No. Do nothing */
  {
  
  }
  
  /* Read data from flash memory to get setpoint */
  Fls_Read(FLS_PAGE_ONE, DATA_FLS_SETPOINT_ADDR, &GaaStoreData[0], 1);
  GslSetPoint = (sint16)GaaStoreData[0];
  
  GenLoNhietStatus = READING_INFO_SYSTEM;
  printf("STA %d\n", GenLoNhietStatus);

  /*---------------------------------------------------------------------------- 
    Waiting ADC get first value. TimeOut = 1s 
  ----------------------------------------------------------------------------*/
  while(ADC_GetStatus() != ADC_IDLE)
  {
    if(millis() > 1000)
    {
      /* Report ADC ERROR */
      printf("\nADC_ERROR\n");
      break;
    }
  } 
  GusLastTempTHC = GetTemp_ThermoCouple();
  LED7Seg_Show(GslSetPoint);
  
  /* Blinking LED to display */
  GucBlinkTimes = 6;
  Sch_TaskEnable(SCH_BlinkingLED_Task, SCH_RUN_LATER);
  /*---------------------------------------------------------------------------- 
    Waiting LED blink done. TimeOut = 5s
  ----------------------------------------------------------------------------*/
  while(Sch_GetStatus(SCH_BlinkingLED_Task) == TASK_ENABLE)   
  {
    Sch_MainFunction(); 
    if(millis() > 5000)
    {
      /* Report BLINKING LED ERROR */
      printf("\nBLINKING_LED_ERROR\n");
      break;
    }
  }
  
  GenLoNhietStatus = LONHIET_IDLE;
  Sch_TaskEnable(SCH_UpdateADC_Task, SCH_RUN_LATER);
  Sch_TaskEnable(SCH_SendSetPoint_Task, SCH_RUN_LATER);
  Sch_TaskEnable(SCH_StoringWorkingTime_Task, SCH_RUN_LATER);
  
  
  
  
  
  
  
  
  
  
  

  while(1)
  { 
    //Sch_MainFunction(); 
    //Btn_MainFunction();
  }
}

/*******************************************************************************
**                      Function                                              **
*******************************************************************************/
/* Init System, peripheral clock and multi-function I/O */
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


void GPIO_Init(void)
{
  /* Cau hinh chan DIEU KHIEN LED_TEST */
  GPIO_SetMode(LED_TEST_PORT, LED_TEST_BIT, GPIO_PMD_OUTPUT);
  LED_TEST = 0;
  
  /* Cau hinh chan DIEU KHIEN TRIAC */
  GPIO_SetMode(TRIAC_PORT, TRIAC_BIT, GPIO_PMD_OUTPUT);
  TRIAC_PIN = TRIAC_OFF;
}


void Buttons_Init(void)
{
  Btn_ConfigSet[BTRU_ID].enEventType = BTN_RELEASED_EVENT;
  Btn_ConfigSet[BTRU_ID].pfnFunction = &BTRU_Release;
  
  Btn_ConfigSet[BCONG_ID].enEventType = BTN_RELEASED_EVENT;
  Btn_ConfigSet[BCONG_ID].pfnFunction = &BCONG_Release;
  Btn_ConfigSet[BCONG_ID].usHoldThresTime = 3000;
  
  Btn_ConfigSet[BSET_ID].enEventType = BTN_HOLD_EVENT;
  Btn_ConfigSet[BSET_ID].usHoldThresTime = 3000;
  Btn_ConfigSet[BSET_ID].pfnFunction = &BSET_HoldToThres;
  Btn_ConfigSet[BSET_ID].pfnHoldFunction2 = &BSET_BCONG_HoldToThres;
  
  Btn_Init();
}
/*****************************************************************************
 * In this project, we use PWM0 (PA.12) to generate PWM 
 * => Timer0 will be disable!                            
 * PWMA group is used.
******************************************************************************/

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

  /* Set GPB multi-function pins for UART0 RXD and TXD */
  SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB4_Msk | SYS_GPB_MFP_PB5_Msk);
  SYS->GPB_MFP |= SYS_GPB_MFP_PB4_UART1_RXD | SYS_GPB_MFP_PB5_UART1_TXD;

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
  printf("TC %d\n", GusTempTHC);
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
