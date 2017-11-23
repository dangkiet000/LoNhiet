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
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include <stdio.h>
#include "NUC200Series.h"
#include "Get_ADC_Value.h"
#include "PID_C.h"
#include "Scheduler.h"
#include "LED7Segment.h"
#include "Button_Processing.h"
#include "DataFlash.h"

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define CHU_KY_PID 15000





#define DEBUG_MODE                STD_ON
#define WRITE_DATABASE_MODE       STD_OFF
#define TEST_MODE                 STD_OFF

/* System define */
#define PLLCON_SETTING            CLK_PLLCON_50MHz_HXT
#define PLL_CLOCK                 50000000
#define SYSTEMTICK_INTERRUPTVALUE 6000000UL


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

#define MAXDUTY                   50000U /* 2s */
#define PWM_FREQUENCE             25000U /* 1s */
/* DUTY50MS = (25000U/1000)*50 or PWM_FREQUENCE/20 */
#define DUTY50MS                  1250U 


/* Speaker control define */
#define SPEAKER_ON                0U
#define SPEAKER_OFF               1U
#define SPEAKER_PIN               PB8



/* Data flash define */
#define DATA_FLS_LEN              1U
#define DATA_FLS_SETPOINT_IDX     0U

/* Scheduler define */
#define PID_AUTOTURN_IDX          0U
#define UPDATE_ADC_IDX            1U
#define PID_OUT_IDX               2U
#define DISPLAY_IDX               3U
#define BLINKING_LED_IDX          4U
#define SEND_SETPOINT_IDX         5U


#define NUM_OF_BLINK              10U
#define BLINK_TIME                400U

/* Lo Nhiet status */
#define READING_INFO_SYSTEM       0U
#define JUST_START                1U
#define HEATING_FIRST             2U
#define STABLE                    3U



/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
void SYS_Init(void);
void GPIO_Init(void);
void Buttons_Init(void);
void Timer0_Init(void);
void PWM_Init(void);

/* Scheduler function prototypes */
void Scheduler_Init(void);
void DisplayTask(void);
void BlinkingLed(void);
void Send_SetPoint_to_PC(void);
void PIDOut_Task(void);
void UpdateADCValue(void);
void PIDAutoTurn_Task(void);

/* Button call-back function prototypes */
void BSET_HoldToThres(void);
void BSET_Release(void);
void BCONG_Release(void);
void BTRU_Release(void);

/* PID function prototypes */
void PIDTemp_Init(void);
void PwmTriac(int32_t LslDuty);

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

#if (WRITE_DATABASE_MODE == STD_ON)
void SaveDataBase_to_Flash(void);
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
d
/* LED control variables */
uint8_t GucBlinkTimes;

/* PID variables */
int32_t GslSetPoint = 60; 
uint16_t GusStopTemp = 100;
int32_t Kd, KpStable, GslPID_Output;
int32_t *GpKp, *GpKi;
uint16_t GusLastTempTHC;
uint8_t GucLoNhietStatus;
uint16_t GusOverShoot, GusLastOverShoot;

/*******************************************************************************
**                      Interrupt Service Routine                             **
*******************************************************************************/
/* Scheduler Events */
void PIDAutoTurn_Task(void)
{
  /* Start turning */
  if(GucLoNhietStatus == HEATING_FIRST)
  {
    //printf("HEATING_FIRST\n");
    if(GusTempTHC + GusStopTemp < GslSetPoint)
    {
      PwmTriac(MAXDUTY);
    }
    else
    {
      PwmTriac(0);
      /* State change to STABLE */
      GucLoNhietStatus = STABLE;  
      Gaa_SchedulerTable[PID_AUTOTURN_IDX].ulInterval = 6000;
      Gaa_SchedulerTable[PID_OUT_IDX].blEnable = TRUE;       
    }   
  }
  else if(GucLoNhietStatus == STABLE)
  {
    printf("Kp %d\n", PID_GetKp());
    
    GslPID_Output = PID_Compute((int32_t)GusTempTHC);
    
    printf("PIDOut %d\n", GslPID_Output);
    
    PwmTriac(GslPID_Output);
  }
  else if(GucLoNhietStatus == JUST_START)
  {
    //printf("JUST_START\n");
    if(GusTempTHC < GslSetPoint)
    {
      /* State change to HEATING_FIRST */
      GucLoNhietStatus = HEATING_FIRST;
      Gaa_SchedulerTable[PID_AUTOTURN_IDX].ulInterval = 4000;
      Gaa_SchedulerTable[PID_OUT_IDX].blEnable = TRUE;
    }
    else
    {
      /* State change to STABLE */
      GucLoNhietStatus = STABLE;
      Gaa_SchedulerTable[PID_AUTOTURN_IDX].ulInterval = 8000;
      Gaa_SchedulerTable[PID_OUT_IDX].blEnable = TRUE;   
    }
  }
  
  /* Update last temperature */
  GusLastTempTHC = GusTempTHC;
  
  printf("STA %d\n", GucLoNhietStatus);
}
void DisplayTask(void)
{
  LED_TEST ^= 1;
  
  if(Gaa_SchedulerTable[BLINKING_LED_IDX].blEnable == FALSE)
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
    Gaa_SchedulerTable[BLINKING_LED_IDX].blEnable = FALSE;
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
void PIDOut_Task(void)
{

}
void UpdateADCValue(void)
{
  if(ADC_IDLE == ADC_GetStatus())
  {
    GusTempTHC = GetTemp_ThermoCouple();
  }
  ADC_StartConvert();
}
/* Button processing events */
void BCONG_Release(void)
{
  GslSetPoint++;
  GslSetPoint = ((GslSetPoint > MAX_TEMP_TYPE_K) ? MAX_TEMP_TYPE_K : GslSetPoint);
  GucBlinkTimes = NUM_OF_BLINK;
  Gaa_SchedulerTable[BLINKING_LED_IDX].blEnable = TRUE;
  Gaa_SchedulerTable[BLINKING_LED_IDX].ulInterval = BLINK_TIME;
  LED7Seg_Show(GslSetPoint);
}

void BTRU_Release(void)
{
  if(GslSetPoint > 0)
  {
    GslSetPoint--;
    GucBlinkTimes = NUM_OF_BLINK;
    Gaa_SchedulerTable[BLINKING_LED_IDX].blEnable = TRUE;
    Gaa_SchedulerTable[BLINKING_LED_IDX].ulInterval = BLINK_TIME;
  }
  LED7Seg_Show(GslSetPoint);
}

void BSET_HoldToThres(void)
{
  
}
void BSET_Release(void)
{
  GaaStoreData[DATA_FLS_SETPOINT_IDX] = (uint32_t) GslSetPoint;
  DataFlash_Write(DATA_FLS_PAGE_ONE, GaaStoreData, DATA_FLS_LEN);
  GucBlinkTimes = 8;
  Gaa_SchedulerTable[BLINKING_LED_IDX].blEnable = TRUE;
  Gaa_SchedulerTable[BLINKING_LED_IDX].ulInterval = 200;
  LED7Seg_Show(GslSetPoint);
  
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
  SYSTick_Init(SYSTEMTICK_INTERRUPTVALUE);
  
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
  
  /* Init PWM0 use for on/off TRIAC */
  PWM_Init();
  
#if (DEBUG_MODE == STD_ON)
  /* Check the system reset source and report */
  SYS_CheckResetSrc();
#endif
  
  /* Read data from flash memory to get setpoint */
  DataFlash_Read(DATA_FLS_PAGE_ONE, GaaStoreData, DATA_FLS_LEN);
  GslSetPoint = (uint16_t) GaaStoreData[DATA_FLS_SETPOINT_IDX];
  
  PIDTemp_Init();
  
  Scheduler_Init();
  
  GucLoNhietStatus = READING_INFO_SYSTEM;
  printf("STA %d\n", GucLoNhietStatus);
  
  /*---------------------------------------------------------------------------- 
    Waiting ADC get first value. TimeOut = 3s 
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
  Gaa_SchedulerTable[BLINKING_LED_IDX].blEnable = TRUE;
  /*---------------------------------------------------------------------------- 
    Waiting LED blink done. TimeOut = 10s
  ----------------------------------------------------------------------------*/
  while(Gaa_SchedulerTable[BLINKING_LED_IDX].blEnable == TRUE)   
  {
    SchedulerPoll(); 
    if(millis() > 5000)
    {
      /* Report BLINKING LED ERROR */
      printf("\nBLINKING_LED_ERROR\n");
      break;
    }
  }
  
  GucLoNhietStatus = JUST_START;
  Gaa_SchedulerTable[PID_AUTOTURN_IDX].blEnable = TRUE;
  Gaa_SchedulerTable[UPDATE_ADC_IDX].blEnable = TRUE;
  Gaa_SchedulerTable[SEND_SETPOINT_IDX].blEnable = TRUE;
      
  /* Cai dat Kp: 0 - 50000 */
  *GpKp = 2500;
  //*GpKi = DUTY50MS;
  
  GucLoNhietStatus = STABLE;
  while(1)
  { 
    SchedulerPoll(); 
    ButtonsPoll();
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
}


void Buttons_Init(void)
{
  Gaa_NotificationTable[BTRU_INDEX].enEventType = RELEASE;
  Gaa_NotificationTable[BTRU_INDEX].pfnFunction = &BTRU_Release;
  
  Gaa_NotificationTable[BCONG_INDEX].enEventType = RELEASE;
  Gaa_NotificationTable[BCONG_INDEX].pfnFunction = &BCONG_Release;
  
  Gaa_NotificationTable[BSET_INDEX].enEventType = HOLD;
  Gaa_NotificationTable[BSET_INDEX].usHoldThresTime = 2000;
  Gaa_NotificationTable[BSET_INDEX].pfnFunction = &BSET_HoldToThres;
  Gaa_NotificationTable[BSET_INDEX].pfnHoldFunction = &BSET_Release;
  
  ButtonsProcessing_Init();
}
/*****************************************************************************
 * In this project, we use PWM0 (PA.12) to generate PWM 
 * => Timer0 will be disable!                            
 * PWMA group is used.
******************************************************************************/
void PWM_Init(void)
{
   /* Enable PWM clock source */
  CLK_EnableModuleClock(PWM01_MODULE);
  
  /* Select PWMA clock source: HCLK 50 MHz */
  CLK_SetModuleClock(PWM01_MODULE, CLK_CLKSEL_PWM01_HCLK, 0);
  
  /* Reset PWMA: channel0~channel3 */
  SYS_ResetModule(PWM03_RST);
  
  /************************ PWM-Timer Start Produce *************************/
  
  /* 1. Setup clock selector (CSR): bo chia tan dau tien */
  /* Set the divider = PWM_CLK/16 of the PWMA channel0 */
  PWM_SET_DIVIDER(PWMA, PWM_CH0, PWM_CLK_DIV_16);
  
  /* 2. Setup prescaler (PPR)
  * Set the prescaler = PWM_CLK/(124+1) of the PWMA channel0 */
  PWM_WaitBusyTimeOK(PWMA, PWM_CH0);
  PWM_SET_PRESCALER(PWMA, PWM_CH0, 124);

  /* 3. Enable Auto-Reload mode for Channel0 */
  PWM_WaitBusyTimeOK(PWMA, PWM_CH0);
  PWMA->PCR |= PWM_PCR_CH0MOD_Msk; 
  
  /* 4. Because MOC3020 control signal is active low => invert output */
  PWM_ENABLE_OUTPUT_INVERTER(PWMA, (1 << PWM_CH0));
  PWMA->PCR |= PWM_PCR_CH0PINV_Msk;
  
  /* 5. Setup comparator register (CMR) for setting PWM duty */
  PWM_WaitBusyTimeOK(PWMA, PWM_CH0);
  PWM_SET_CMR(PWMA, PWM_CH0, MAXDUTY);
  
  /* 6. Setup PWM down-counter register (CNR) for setting PWM period: 0.5Hz */
  PWM_WaitBusyTimeOK(PWMA, PWM_CH0);
  PWM_SET_CNR(PWMA, PWM_CH0, MAXDUTY);

  /* 6. Setup corresponding GPIO pin as PWM function (enable POE and disable
   *   CAPENR) for corresponding PWM channel.
   * Set PA.12 multi-function pins for PWMA Channel0 */
  SYS->GPA_MFP |= SYS_GPA_MFP_PA12_PWM0;
  
  /* 8. Enable PWM timer start running (Set CHxEN = 1 in PCR)
   *  Enable PWM Output path for PWMA channel0 <=> BIT0 */
  PWM_EnableOutput(PWMA, (1 << PWM_CH0));
  
  /* 9. Start PWMA Channel0 <=> BIT0 */
  PWM_Start(PWMA, (1 << PWM_CH0));
  
  PwmTriac(0);
  /*****************************************************************************
  *                         Tinh toan tan so PWM: 0.5Hz (T(s): 2)
  * PWM Clock Source: 50 MHz
  * PWM divider: 16
  * PRESCALER: 1-256
  * Counter register: 0-65535
  * FREQ_PWM =  50 MHz / (16 * PRESCALER)
  * FREQ_PWM =  3125000 Hz / PRESCALER
  * Chon PRESCALER = 125
  * => FREQ_PWM = 25000
  *****************************************************************************/
}


void Scheduler_Init(void)
{
  Gaa_SchedulerTable[PID_AUTOTURN_IDX].pfnFunction = &PIDAutoTurn_Task;
  Gaa_SchedulerTable[PID_AUTOTURN_IDX].ulInterval = CHU_KY_PID;
  Gaa_SchedulerTable[PID_AUTOTURN_IDX].ulLastTick = 0;
  Gaa_SchedulerTable[PID_AUTOTURN_IDX].blEnable = FALSE;
  
  Gaa_SchedulerTable[UPDATE_ADC_IDX].pfnFunction = &UpdateADCValue;
  Gaa_SchedulerTable[UPDATE_ADC_IDX].ulInterval = 250;
  Gaa_SchedulerTable[UPDATE_ADC_IDX].ulLastTick = 0;
  Gaa_SchedulerTable[UPDATE_ADC_IDX].blEnable = TRUE;
  
  Gaa_SchedulerTable[DISPLAY_IDX].pfnFunction = &DisplayTask;
  Gaa_SchedulerTable[DISPLAY_IDX].ulInterval = 500;
  Gaa_SchedulerTable[DISPLAY_IDX].ulLastTick = 0;
  Gaa_SchedulerTable[DISPLAY_IDX].blEnable = TRUE;
  
  Gaa_SchedulerTable[PID_OUT_IDX].pfnFunction = &PIDOut_Task;
  Gaa_SchedulerTable[PID_OUT_IDX].ulInterval = 5000;
  Gaa_SchedulerTable[PID_OUT_IDX].ulLastTick = 0;
  Gaa_SchedulerTable[PID_OUT_IDX].blEnable = FALSE;
  
  Gaa_SchedulerTable[BLINKING_LED_IDX].pfnFunction = &BlinkingLed;
  Gaa_SchedulerTable[BLINKING_LED_IDX].ulInterval = BLINK_TIME;
  Gaa_SchedulerTable[BLINKING_LED_IDX].ulLastTick = 0;
  Gaa_SchedulerTable[BLINKING_LED_IDX].blEnable = FALSE;
  
  Gaa_SchedulerTable[SEND_SETPOINT_IDX].pfnFunction = &Send_SetPoint_to_PC;
  Gaa_SchedulerTable[SEND_SETPOINT_IDX].ulInterval = 20000;
  Gaa_SchedulerTable[SEND_SETPOINT_IDX].ulLastTick = 0;
  Gaa_SchedulerTable[SEND_SETPOINT_IDX].blEnable = FALSE;

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

void PIDTemp_Init(void)
{
  /* If user congifure 1000 C 
  => OUTPUT = MAXDUTY = 50000 = 1000*Kp 
  => Kp = 50
  */
  GpKp = &GslKp;
  *GpKp = 0;
  GpKi = &GslKi;
  *GpKi = 0;
  Kd = 0;
  PID_Init(&GslSetPoint, GpKp, GpKi, &Kd, PID_DIRECT);
  PID_SetOutputLimits(0, MAXDUTY);

}
/**
 * @brief The function output pwm 
 * @param[in] LusDuty : 0 - 50000(MAXDUTY)
 * @retval    None
*/
void PwmTriac(int32_t LslDuty)
{
  /* Limit output to avoid overflow */
  if(LslDuty > MAXDUTY)
    
  {
    LslDuty = MAXDUTY;
  }
  else if(LslDuty < 0)
  {
    LslDuty = 0;
  }
  PWM_SET_CMR(PWMA, PWM_CH0, MAXDUTY - LslDuty); \
}
  /*****************************************************************************
  *                         PWM-Timer Stop Produce 
  * 1. Method 1:(Recommended)
  * Set 16-bit down counter (CNR) as 0, and monitor PDR (current value of 16-bit 
  * down counter). When PDR reaches to 0, disable PWM-Timer(CHxEN in PCR).
  *
  * 2. Method 2:(Recommended)
  * Set 16-bit down counter (CNR) as 0> When interrupt request happens, disable 
  * PWM-Timer (CHxEN in PCR).
  * Ex: PWM_Stop(PWMA, (1 << PWM_CHx));
  * 
  * 3. Method 2:(Not recommended)
  * Disble PWM-Timer directly (CHxEN in PCR).
  * => khong nen dung ham PWM_ForceStop()
  *****************************************************************************/
#if (WRITE_DATABASE_MODE == STD_ON)
void SaveDataBase_to_Flash(void)
{
  const uint32_t LaaKp_DataBase[128];
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
