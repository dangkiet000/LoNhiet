/**************************************************************************//**
 * @file     Heater_App.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 26/05/2018 3:59p $
 * @brief    Heater application source file.
 *
 * @note
*****************************************************************************/
#include "Heater_App.h"

/** @addtogroup None
  @{
*/

/** @addtogroup None
  @{
*/

/** @addtogroup None
  @{
*/
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/* Lo Nhiet Global structure */
HeaterType Heater;

/*******************************************************************************
**                      Interrupt Service Routine                             **
*******************************************************************************/
/*------------------------- TimeOut Events -----------------------------------*/
void TO_UpdateSetPoint(void)
{
  /* Exit HEATER_UPDATE_SETPOINT mode */
  Exit_HEATER_UPDATE_SETPOINT_mode(HEATER_TIMEOUT_TRUE);
}
void TO_SetDateTime(void)
{
  if(Heater.enOpStatus == HEATER_SETUP_MON)
  {
    /* Exit HEATER_SETUP_MON mode */
    Exit_HEATER_SETUP_MON_mode(HEATER_TIMEOUT_TRUE);
  }
  else if(Heater.enOpStatus == HEATER_SETUP_DAY)
  {
    /* Exit HEATER_SETUP_DAY mode */
    Exit_HEATER_SETUP_DAY_mode(HEATER_TIMEOUT_TRUE);
  }
}
void TO_EnterPassword(void)
{
  /* Exit HEATER_ENTER_PASSWORD mode */
  Exit_HEATER_ENTER_PASSWORD_mode(HEATER_TIMEOUT_TRUE);
}

/*------------------------- Scheduler Events ---------------------------------*/
/* Display LED7segment every 1000ms. */
void DisplayTask(void)
{
  LED_TEST ^= 1;
  
  switch(Heater.enOpStatus)
  {
    case HEATER_UPDATE_SETPOINT:
    {
      LED7_DisplayLeadingZeros(Heater.usSetPoint);
      break;
    }
    case HEATER_BUSY:
    {
      LED7_DisplayNumber(Heater.usTempTHC);
      #if (DATA_LOGGING == STD_ON)
      Send_TemptoPC();
      #endif
      break;
    }
    case HEATER_IDLE:
    {
      LED7_DisplayNumber(Heater.usTempTHC);
      break;
    }
    case HEATER_ENTER_PASSWORD:
    {
      LED7_DisplayLeadingZeros(Heater.usUserPassword);
      break;
    }
    case HEATER_SETUP_DAY:
    {
      LED7_DisplayDay(Heater.MDate.ucDay);
      break;
    }
    case HEATER_SETUP_MON:
    {
      LED7_DisplayDay(Heater.MDate.ucMonth);
      break;
    }
    default: break;
  }
}


void Send_SetPoint_to_PC(void)
{
  #if (DATA_LOGGING == STD_ON)
  printf("SP %d\n", Heater.usSetPoint);
  #endif
}


/* Get temperature of Thermo-Couple every 250ms.*/
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
    Heater_StoreFlsData(&Heater, FLS_WORKINGTIME);
  }
  
}



/*------------------------- Button processing events -------------------------*/
void BCONG_Release_Event(void)
{
  if(Heater.enOpStatus == HEATER_UPDATE_SETPOINT)
  {
    /* Increase LED7 value. */
    LED7_IncreaseLED7(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                      &Heater.usSetPoint);
    LED7_DisplayLeadingZeros(Heater.usSetPoint);
    
    /* Reload timeout counter. */
    TO_Reload(TO_UpdateSetPoint_Channel);
  }
  else if(Heater.enOpStatus == HEATER_ENTER_PASSWORD)
  {
    /* Increase LED7 value. */
    LED7_IncreaseLED7(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                      &Heater.usUserPassword);
    LED7_DisplayLeadingZeros(Heater.usUserPassword);
    
    /* Reload timeout counter. */
    TO_Reload(TO_EnterPassword_Channel);
  }
  else if(Heater.enOpStatus == HEATER_SETUP_MON)
  {
    /* Increase Month value. */
    switch(Heater.ucBlinkLED7Idx)
    {
      case 2: {  Heater_MonPlus(&Heater.MDate, TENS); break;  }
      case 3: {  Heater_MonPlus(&Heater.MDate, ONES); break;  }
      default: break;
    }
    
    LED7_DisplayMon(Heater.MDate.ucMonth);
    
    /* Reload timeout counter. */
    TO_Reload(TO_SetDateTime_Channel);
  }
  else
  {

  }
}

void BTRU_Release_Event(void)
{
  if(Heater.enOpStatus == HEATER_UPDATE_SETPOINT)
  {
    /* Decrease LED7 value. */
    LED7_DecreaseLED7(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                      &Heater.usSetPoint);
    LED7_DisplayLeadingZeros(Heater.usSetPoint);
    
    /* Reload timeout counter. */
    TO_Reload(TO_UpdateSetPoint_Channel);
  }
  else if(Heater.enOpStatus == HEATER_ENTER_PASSWORD)
  {
    /* Decrease LED7 value. */
    LED7_DecreaseLED7(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                      &Heater.usUserPassword);
 
    LED7_DisplayLeadingZeros(Heater.usUserPassword);
    
    /* Reload timeout counter. */
    TO_Reload(TO_EnterPassword_Channel);
  }
  else if(Heater.enOpStatus == HEATER_SETUP_MON)
  {
    /* Decrease Month value. */
    switch(Heater.ucBlinkLED7Idx)
    {
      case 2: {  Heater_MonMinus(&Heater.MDate, TENS); break;  }
      case 3: {  Heater_MonMinus(&Heater.MDate, ONES); break;  }
      default: break;
    }
    
    LED7_DisplayMon(Heater.MDate.ucMonth);
    
    /* Reload timeout counter. */
    TO_Reload(TO_SetDateTime_Channel);
  }
  else
  {

  }
}


void BSET_Release_Event(void)
{
  if(Heater.enOpStatus == HEATER_UPDATE_SETPOINT)
  {
    /* Reload timeout counter. */
    TO_Reload(TO_UpdateSetPoint_Channel);

    /* Blink next LED7seg. */
    Heater.ucBlinkLED7Idx++;
    LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), 300); 
    LED7_DisplayLeadingZeros(Heater.usSetPoint);

    if(Heater.ucBlinkLED7Idx > 3)
    {
      /* Exit HEATER_UPDATE_SETPOINT mode */
      Exit_HEATER_UPDATE_SETPOINT_mode(HEATER_TIMEOUT_FALSE);      
    }
    else
    {
    
    }
  }
  else if(Heater.enOpStatus == HEATER_ENTER_PASSWORD)
  {
    /* Reload timeout counter. */
    TO_Reload(TO_EnterPassword_Channel);
    
    /* Blink next LED7seg. */
    Heater.ucBlinkLED7Idx++;
    LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), 300); 
    LED7_DisplayLeadingZeros(Heater.usUserPassword);

    if(Heater.ucBlinkLED7Idx > 3)
    {
      /* Exit HEATER_ENTER_PASSWORD mode */
      Exit_HEATER_ENTER_PASSWORD_mode(HEATER_TIMEOUT_FALSE);
    }
    else
    {
    
    }
  }
  else if(Heater.enOpStatus == HEATER_SETUP_MON)
  {
    /* Reload timeout counter. */
    TO_Reload(TO_SetDateTime_Channel);
    
    /* Blink next LED7seg. */
    Heater.ucBlinkLED7Idx++;
    LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), 300); 
    
    LED7_DisplayMon(Heater.MDate.ucMonth);

    if(Heater.ucBlinkLED7Idx > 3)
    {
      /* Exit HEATER_SETUP_MON mode */
      Exit_HEATER_SETUP_MON_mode(HEATER_TIMEOUT_FALSE);
    }
    else
    {
    
    }
  }
  else
  {
  
  }
    
}
void BSET_HoldToThres_Event(void)
{
  if(HEATER_IS_NOT_SETTING_MODE(Heater.enActiLockStatus))
  {
    /* Enter HEATER_UPDATE_SETPOINT mode. */
    Enter_HEATER_UPDATE_SETPOINT_mode();
  }
  else
  {
  
  }
}
void BSET_BCONG_HoldToThres_Event(void)
{
  if(HEATER_IS_NOT_SETTING_MODE(Heater.enOpStatus))
  {
    /* Set Heater status as HEATER_ENTER_PASSWORD. */
    Heater.enOpStatus = HEATER_ENTER_PASSWORD;
    Enter_HEATER_ENTER_PASSWORD_mode();
  }
  else
  {
  
  }
}

void BSET_BTRU_HoldToThres_Event(void)
{
  if(HEATER_IS_NOT_SETTING_MODE(Heater.enOpStatus) == TRUE)
  {
    /* Set Heater status as HEATER_SETUP_DATETIME. */
    Enter_HEATER_SETUP_MON_mode();
  }
}
/*******************************************************************************
**                      Function                                              **
*******************************************************************************/
/**
  * @brief Check date of product is configured or not.
  * @param[in] None.
  * @return  TRUE: Date product of heater is congigured. Otherwise, return 
  *          is FALSE.
  * @details None.
  * @note None.
  */
boolean Heater_DateProductIsConfigured(void)
{
  Fls_DataType LddDPStatus;
  boolean Lblretval;
  
  Fls_Read(FLS_DATEPRODUCTSTATUS, &LddDPStatus);
  
  if(LddDPStatus == HEATER_DATEPRODUCT_IS_CONFIGURED)
  {
    /* Date product of heater is congigured. */
    Lblretval = TRUE;
  }
  else
  {
    /* Date product of heater has not congigured yet. */
    Lblretval = FALSE;
  }
  
  return Lblretval;
}

void Enter_HEATER_SETUP_DAY_mode(void)
{
    /* Set Heater status as HEATER_SETUP_DATETIME. */
  Heater.enOpStatus = HEATER_SETUP_DAY;
  
  Heater.MDate.ucDay = 1;
  
  /* Blinking All LED7 to inform that Heater is in setup datetime mode. */
  BlinkingAllLED7_Synchronous(1500);
  
  /* Blinking LED7_2 first. */
  Heater.ucBlinkLED7Idx = 2;
  
  /* Enable blinking LED7. */
  LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), 300); 

  /* Start timeout count-down. */
  TO_Trigger(TO_SetDateTime_Channel);
}
void Exit_HEATER_SETUP_DAY_mode(boolean IsTimeOut)
{
  /* Clear timeout counter. */
  TO_Clear(TO_SetDateTime_Channel);
  
  if(IsTimeOut == HEATER_TIMEOUT_TRUE)
  {
    /* Don't apply new set-point and read set-point in flash memory to restore 
     set point. */
    LED7_DisplayResult(LED7_FAIL);  
  }
  else
  {
    /* Store DateTime to memory. */
    Heater_StoreFlsData(&Heater, FLS_NGAY);
    Heater_StoreFlsData(&Heater, FLS_THANG);
  }
  /* Reset varibales. */
  Heater.ucBlinkLED7Idx = 0;
  
  /* Blinking all LED7 to inform that Setup-SetPoint is finished. */
  BlinkingAllLED7_Synchronous(1500);
  
  Heater.enOpStatus = HEATER_IDLE;
}

void Enter_HEATER_SETUP_MON_mode(void)
{
  /* Set Heater status as HEATER_SETUP_DATETIME. */
  Heater.enOpStatus = HEATER_SETUP_MON;
  
  Heater.MDate.ucMonth = 1;
  
  /* Blinking All LED7 to inform that Heater is in setup datetime mode. */
  BlinkingAllLED7_Synchronous(1500);
  
  /* Blinking LED7_2 first. */
  Heater.ucBlinkLED7Idx = 2;
  
  /* Enable blinking LED7. */
  LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), 300); 

  /* Start timeout count-down. */
  TO_Trigger(TO_SetDateTime_Channel);
}
void Exit_HEATER_SETUP_MON_mode(boolean IsTimeOut)
{
  /* Clear timeout counter. */
  TO_Clear(TO_SetDateTime_Channel);
  
  if(IsTimeOut == HEATER_TIMEOUT_TRUE)
  {
    /* Don't apply new set-point and read set-point in flash memory to restore 
     set point. */
    LED7_DisplayResult(LED7_FAIL);

    Heater.enOpStatus = HEATER_IDLE;
  }
  else
  {
    Enter_HEATER_SETUP_DAY_mode();
  }
  /* Reset varibales. */
  Heater.ucBlinkLED7Idx = 0;
  
  /* Blinking all LED7 to inform that Setup-SetPoint is finished. */
  BlinkingAllLED7_Synchronous(1500);
}

void Enter_HEATER_UPDATE_SETPOINT_mode(void)
{
  /* Set Heater status as HEATER_UPDATE_SETPOINT. */
  Heater.enOpStatus = HEATER_UPDATE_SETPOINT;
  
  /* Blinking All LED7 to inform that Heater is in Update setpoint mode. */
  BlinkingAllLED7_Synchronous(1500);
  
  /* Blinking LED7_0 first. */
  Heater.ucBlinkLED7Idx = 0;
  
  /* Enable blinking LED7. */
  LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), 300); 

  /* Start timeout count-down. */
  TO_Trigger(TO_UpdateSetPoint_Channel);
}
void Exit_HEATER_UPDATE_SETPOINT_mode(boolean IsTimeOut)
{
  /* Clear timeout counter. */
  TO_Clear(TO_UpdateSetPoint_Channel);
  
  if(IsTimeOut == HEATER_TIMEOUT_TRUE)
  {
    /* Don't apply new set-point and read set-point in flash memory to restore set
     point. */
    Heater_ReadFlsData(&Heater, FLS_SETPOINT);
    LED7_DisplayLeadingZeros(Heater.usSetPoint);
  }
  else
  {
    /* To check temperature is greater than MAX temperture of Thermo-Couple type
       K. */
    if(Heater.usSetPoint > MAX_TEMP_TYPE_K)
    {
      LED7_DisplayError(LED7_ERR1);
    }
    else
    {
      /* To Store set-point in flash memory */
      Heater_StoreFlsData(&Heater, FLS_SETPOINT);
    }
  }
  /* Reset varibales. */
  Heater.ucBlinkLED7Idx = 0;
  
  /* Blinking all LED7 to inform that Setup-SetPoint is finished. */
  BlinkingAllLED7_Synchronous(1500);
  Heater.enOpStatus = HEATER_IDLE;
}
void Enter_HEATER_ENTER_PASSWORD_mode(void)
{
  /* Reset user password. */
  Heater.usUserPassword = 0;
  
  /* Display [0000] first. */
  LED7_DisplayLeadingZeros(Heater.usUserPassword);
  
  /* Blinking All LED7 to inform that Heater is in Enter password mode. */
  BlinkingAllLED7_Synchronous(1500);
  
  /* Blinking LED7_0 first. */
  Heater.ucBlinkLED7Idx = 0;
  
  /* Enable blinking LED7. */
  LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), 300); 

  /* Start timeout count-down. */
  TO_Trigger(TO_EnterPassword_Channel);
}

void Exit_HEATER_ENTER_PASSWORD_mode(boolean IsTimeOut)
{
  /* Clear timeout counter. */
  TO_Clear(TO_EnterPassword_Channel);
  
  if(IsTimeOut == HEATER_TIMEOUT_TRUE)
  {
    LED7_DisplayResult(LED7_FAIL);
  }
  else
  {
    /* Validate user password. */
    if(Heater.usUserPassword == HEATER_PASSWORD)
    {
      Heater.enActiLockStatus = LONHIET_UNLOCKED;
      
      Heater_StoreFlsData(&Heater, FLS_ACTILOCKSTATUS);
      
      LED7_DisplayResult(LED7_PASS);
    }
    else
    {
      LED7_DisplayResult(LED7_FAIL);
    }
  }
  
  /* Blinking all LED7 to inform that Enter-password is finished. */
  BlinkingAllLED7_Synchronous(2500);

  /* Reset variables. */
  Heater.ucBlinkLED7Idx = 0;
  Heater.usUserPassword = 0;
  
  Heater.enOpStatus = HEATER_IDLE;
}

/* Read Heater data which store in data flash memory. */
void Heater_ReadFlsData(HeaterType *pHeater, Fls_DataIdType FlsId)
{  
  Fls_DataType Temp;
  
  /* Reset value as default value of flash memory. */
  Temp = 0xFFFFFFFF;

  switch(FlsId)
  {
    case FLS_SETPOINT: /* Read Setpoint. */
    {
      Fls_Read(FLS_SETPOINT, &Temp);
      pHeater->usSetPoint = (uint16)Temp;
      break;
    }
    case FLS_WORKINGTIME: /* Read Working-time. */
    {
      Fls_Read(FLS_WORKINGTIME, &Temp);
      pHeater->ulWorkingTime = (uint32)Temp; 
      break;
    }
    case FLS_ACTILOCKSTATUS: /* Read Activation Lock Status. */ 
    {
      Fls_Read(FLS_ACTILOCKSTATUS, &Temp);
      pHeater->enActiLockStatus = (Heater_ActiLockStatusType)Temp; 
      break;
    }
    case FLS_NGAY: /* Read Date product. */ 
    {
      Fls_Read(FLS_NGAY, &Temp);
      pHeater->MDate.ucDay = (uint8)Temp; 
      break;
    }
    case FLS_THANG: /* Read Month product. */ 
    {
      Fls_Read(FLS_THANG, &Temp);
      pHeater->MDate.ucMonth = (uint8)Temp; 
      break;
    }
    case FLS_NAM: /* Read Year product. */ 
    {
      Fls_Read(FLS_NAM, &Temp);
      pHeater->MDate.ucYear = (uint8)Temp;  
      break;
    }
    default: break;
  } 
}

/* Store Heater data to flash memory. */
void Heater_StoreFlsData(HeaterType *pHeater, Fls_DataIdType FlsId)
{  
  Fls_DataType Temp;
  
  switch(FlsId)
  {
    case FLS_SETPOINT: /* Store Setpoint. */
    {
      Temp = (Fls_DataType)pHeater->usSetPoint;
      Fls_Write(FLS_SETPOINT, &Temp);
      break;
    }
    case FLS_WORKINGTIME: /* Store Working-time. */
    {
      Temp = (Fls_DataType)pHeater->ulWorkingTime;
      Fls_Write(FLS_WORKINGTIME, &Temp);
      break;
    }
    case FLS_ACTILOCKSTATUS: /* Store Activation Lock Status. */ 
    {
      Temp = (Fls_DataType)pHeater->enActiLockStatus;
      Fls_Write(FLS_ACTILOCKSTATUS, &Temp);
      break;
    }
    case FLS_NGAY: /* Read Date product. */ 
    {
      Temp = (Fls_DataType)pHeater->MDate.ucDay;
      Fls_Write(FLS_NGAY, &Temp);
      break;
    }
    case FLS_THANG: /* Read Month product. */ 
    {
      Temp = (Fls_DataType)pHeater->MDate.ucMonth;
      Fls_Write(FLS_THANG, &Temp);
      break;
    }
    case FLS_NAM: /* Read Year product. */ 
    {
      Temp = (Fls_DataType)pHeater->MDate.ucYear;
      Fls_Write(FLS_NAM, &Temp);  
      break;
    }
    default: break;
  } 
}

/* Blinking LED7-Seg in milisecond synchronously. */
void BlinkingAllLED7_Synchronous(uint32 duration)
{
  LED7_EnableBlinking(LED7_ALL, 150);
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
  Btn_ConfigSet[BTRU_ID].pfnHoldEvent1 = &BSET_BTRU_HoldToThres_Event;
  Btn_ConfigSet[BTRU_ID].usHoldThresTime = 3000;
  
  Btn_ConfigSet[BCONG_ID].enEventType = BTN_RELEASED_EVENT;
  Btn_ConfigSet[BCONG_ID].pfnFunction = &BCONG_Release_Event;
  Btn_ConfigSet[BCONG_ID].pfnHoldEvent1 = &BSET_BCONG_HoldToThres_Event;
  Btn_ConfigSet[BCONG_ID].usHoldThresTime = 3000;
  
  Btn_ConfigSet[BSET_ID].enEventType = BTN_HOLD_EVENT;
  Btn_ConfigSet[BSET_ID].usHoldThresTime = 3000;
  Btn_ConfigSet[BSET_ID].pfnFunction = &BSET_Release_Event;
  Btn_ConfigSet[BSET_ID].pfnHoldEvent1 = &BSET_HoldToThres_Event;
  
  Btn_Init();
}
/*******************************************************************************
**                      Testing Functions                                     **
*******************************************************************************/
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

#if (DATA_LOGGING == STD_ON)
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
    LED7_DisplayNumber(LucCount++);
    DelaySystemTick_ms(400);
  }
}
#endif




/*@}*/ /* None */

/*@}*/ /* None */

/*@}*/ /* None */

