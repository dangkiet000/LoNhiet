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


const uint8 GaaMapNumberToPlace[4] = {
  THOUSANDS, 
  HUNDREDS,
  TENS,
  ONES
};
/*******************************************************************************
**                      Interrupt Service Routine                             **
*******************************************************************************/
/*------------------------- TimeOut Events -----------------------------------*/
void TO_UpdateSetPoint(void)
{
  /* This is the last LED7 digit(LED7_3). */
  if(Heater.ucBlinkLED7Idx == 3)
  {
    /* Exit HEATER_UPDATE_SETPOINT mode */
    Exit_HEATER_UPDATE_SETPOINT_mode(HEATER_TIMEOUT_FALSE);
  }
  else
  {
    /* Exit HEATER_UPDATE_SETPOINT mode */
    Exit_HEATER_UPDATE_SETPOINT_mode(HEATER_TIMEOUT_TRUE);
  }
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
  LED_STATUS ^= 1;

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
    case HEATER_WAIITING_USER_SETUP_DATETIME:
    {
      LED7_DisplayNumber(Heater.usTempTHC);
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
    Heater.ulWorkingTime += STORE_WORKING_TIME_INTERVAL_IN_MIN;

    /* To Store working-time in flash memory */
    Heater_StoreFlsData(&Heater, FLS_WORKINGTIME);
  }

}



/*------------------------- Button processing events -------------------------*/
/* This event occurs if user release CONG button. */
void BCONG_Release_Event(void)
{
  if(Heater.enOpStatus == HEATER_UPDATE_SETPOINT)
  {
    /* Reload timeout counter. */
    TO_Reload(TO_UpdateSetPoint_Channel);
    
    /* Increase LED7 value. */
    Heater.usSetPoint = Heater_TempPlus(Heater.usSetPoint, \
                           GaaMapNumberToPlace[Heater.ucBlinkLED7Idx]);
    
    LED7_DisplayLeadingZeros(Heater.usSetPoint);   
  }
  else if(Heater.enOpStatus == HEATER_ENTER_PASSWORD)
  {
    /* Increase LED7 value. */
    LED7_IncreaseLED7(Heater.ucBlinkLED7Idx, &Heater.usUserPassword);
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
  else if(Heater.enOpStatus == HEATER_SETUP_DAY)
  {
    /* Increase day value. */
    switch(Heater.ucBlinkLED7Idx)
    {
      case 2: {  Heater_DayPlus(&Heater.MDate, TENS); break;  }
      case 3: {  Heater_DayPlus(&Heater.MDate, ONES); break;  }
      default: break;
    }

    LED7_DisplayMon(Heater.MDate.ucDay);

    /* Reload timeout counter. */
    TO_Reload(TO_SetDateTime_Channel);
  }
  else
  {
    /* Do Nothing. */
  }
}
/* This event occurs if user release TRU button. */
void BTRU_Release_Event(void)
{
  if(Heater.enOpStatus == HEATER_UPDATE_SETPOINT)
  {
        /* Reload timeout counter. */
    TO_Reload(TO_UpdateSetPoint_Channel);
    
    /* Increase LED7 value. */
    Heater.usSetPoint = Heater_TempMinus(Heater.usSetPoint, \
                           GaaMapNumberToPlace[Heater.ucBlinkLED7Idx]);
    
    LED7_DisplayLeadingZeros(Heater.usSetPoint);  
  }
  else if(Heater.enOpStatus == HEATER_ENTER_PASSWORD)
  {
    /* Decrease LED7 value. */
    LED7_DecreaseLED7(Heater.ucBlinkLED7Idx, &Heater.usUserPassword);

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
  else if(Heater.enOpStatus == HEATER_SETUP_DAY)
  {
    /* Decrease Day value. */
    switch(Heater.ucBlinkLED7Idx)
    {
      case 2: {  Heater_DayMinus(&Heater.MDate, TENS); break;  }
      case 3: {  Heater_DayMinus(&Heater.MDate, ONES); break;  }
      default: break;
    }

    LED7_DisplayMon(Heater.MDate.ucDay);

    /* Reload timeout counter. */
    TO_Reload(TO_SetDateTime_Channel);
  }
  else
  {

  }
}

/* This event occurs if user release SET button. */
void BSET_Release_Event(void)
{
  if(Heater.enOpStatus == HEATER_UPDATE_SETPOINT)
  {
    /* Reload timeout counter. */
    TO_Reload(TO_UpdateSetPoint_Channel);

    /* Blink next LED7seg. */
    Heater.ucBlinkLED7Idx++;
    LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                        SETTING_MODE_BLINK_TIME);
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
    LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                        SETTING_MODE_BLINK_TIME);
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
    LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                        SETTING_MODE_BLINK_TIME);

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
  else if(Heater.enOpStatus == HEATER_SETUP_DAY)
  {
    /* Reload timeout counter. */
    TO_Reload(TO_SetDateTime_Channel);

    /* Blink next LED7seg. */
    Heater.ucBlinkLED7Idx++;
    LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                        SETTING_MODE_BLINK_TIME);

    LED7_DisplayDay(Heater.MDate.ucDay);

    if(Heater.ucBlinkLED7Idx > 3)
    {
      /* Exit HEATER_SETUP_DAY mode */
      Exit_HEATER_SETUP_DAY_mode(HEATER_TIMEOUT_FALSE);
    }
    else
    {

    }
  }
  else
  {

  }

}

/* This event occurs if user press and hold SET button longer 3 seconds. */
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
/* This event occurs if user hold CONG and SET button longer 3 seconds. */
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
/* This event occurs if user hold TRU and SET button longer 3 seconds. */
void BSET_BTRU_HoldToThres_Event(void)
{
  if(HEATER_IS_NOT_SETTING_MODE(Heater.enOpStatus) == TRUE)
  {
    /* Set Heater status as HEATER_SETUP_DATETIME. */
    Enter_HEATER_SETUP_MON_mode();
  }
}

/* This event occurs if user press and hold TRU button longer 6 seconds. */
void BTRU_HoldToThres_Event(void)
{
  if(HEATER_IS_NOT_SETTING_MODE(Heater.enOpStatus))
  {
    Heater.enOpStatus = HEATER_WORINGTIME_DISPLAY;
    
    Heater_DisplayWorkingTime();
    
    Heater.enOpStatus = HEATER_IDLE;
  }
}
/*******************************************************************************
**                      Function                                              **

*******************************************************************************/
/**
  * @brief Get temperature of heater.
  * @param[in] None.
  * @return  uint16_t: temperature of heater in degree celsius.
  * @details None.
  * @note None.
  */
uint16_t GetTemp_ThermoCouple(void)
{
  return ThermoCouple_ADCToTemp(GulADC_THC_TB, GulADC_LM35_TB);
}
/**
  * @brief Display heater working time in day.
  * @param[in] None.
  * @return  None.
  * @details Display working time of heater.
  *          Unit: day.
  * @note None.
  */
void Heater_DisplayWorkingTime(void)
{
  uint16 Lusdays;
  
  /* Heater.ulWorkingTime| unit: minutes 
  => hour = Heater.ulWorkingTime/60
  because heater work 8h/day.
  => days = hour/8.
  */
  Lusdays = (uint16)(Heater.ulWorkingTime/(NUM_MIN_PER_HOUR*NUM_HOUR_PER_DAY));
  
  LED7_DisplayNumber(Lusdays);
  DelaySystemTick_ms(HEATER_WORKING_DISPLAY_TIME);
  BlinkingAllLED7_Synchronous(800);

}
/**
  * @brief Display heater information at startup time.
  * @param[in] None.
  * @return  None.
  * @details 1. Display day in 1s.
             2. Display month in 1s.
             3. Display year in 1s.
             4. Display serial number in 1s.
             5. Display set point in 1s.
  * @note None.
  */
void Heater_DisplayInfoAtStartup(void)
{
  LED7_DisplayDay(Heater.MDate.ucDay);
  DelaySystemTick_ms(1000);
  BlinkingAllLED7_Synchronous(800);
  
  LED7_DisplayMon(Heater.MDate.ucMonth);
  DelaySystemTick_ms(1000);
  BlinkingAllLED7_Synchronous(800);
  
  LED7_DisplayLeadingZeros(Heater.MDate.usYear);
  DelaySystemTick_ms(1000);
  BlinkingAllLED7_Synchronous(800);
  
  LED7_DisplayLeadingZeros(SERIAL_NUMBER);
  DelaySystemTick_ms(1000);
  BlinkingAllLED7_Synchronous(800);
  
  LED7_DisplayNumber(Heater.usSetPoint);
  DelaySystemTick_ms(1000);
  BlinkingAllLED7_Synchronous(800);
}
/**
  * @brief Checking Activation Lock status and return heater is disable or
           enable.
  * @param[in] None.
  * @return  HEATER_TRIAC_ENABLE: Heater is enabled triac controlling.
             HEATER_TRIAC_DISABLE: Heater is disabled triac controlling.
  * @details Checking Activation Lock status and return heater is disable or
           enable.
  * @note None.
  */
Heater_TriacStatusType Heater_CheckActivationLock(void)
{
  Heater_TriacStatusType LddRetVal;

  Heater_ReadFlsData(&Heater, FLS_ACTILOCKSTATUS);

  if(Heater.enActiLockStatus == LONHIET_LOCKED)
  {
    LddRetVal = HEATER_TRIAC_DISABLE;
  }
  else if(Heater.enActiLockStatus == LONHIET_UNLOCKED)
  {
    LddRetVal = HEATER_TRIAC_ENABLE;
  }
  else if(Heater.enActiLockStatus == LONHIET_TRIAL)
  {
    /* To check if working-time is over Trial time */
    if(Heater.ulWorkingTime > TRIAL_TIME_IN_MIN)
    {
      /* Yes. Disable LoNhiet */
      Heater.enActiLockStatus = LONHIET_LOCKED;
      Heater_StoreFlsData(&Heater, FLS_ACTILOCKSTATUS);

      LddRetVal = HEATER_TRIAC_DISABLE;
    }
    else /* No. */
    {
      LddRetVal = HEATER_TRIAC_ENABLE;
    }
  }
  else
  {
    /* Do Nothing */
  }

  return LddRetVal;
}
/**
  * @brief Checking SERIAL_NUMBER is equal serial number in the flash memory.
  * @param[in] None.
  * @return  None.
  * @details Checking SERIAL_NUMBER is equal serial number in the flash memory.
             Yes: Nothing to do.
             No: write essential data(startup data) to flash memory.
  * @note Heater need some mandatory data to startup correctly.
  */
void Heater_CheckFlashData(void)
{
  Fls_DataType LddFlsData;

  Fls_Read(FLS_SERIAL_NUMBER, &LddFlsData);

  /* Checking SERIAL_NUMBER is equal serial number in the flash memory. */
  if(LddFlsData == SERIAL_NUMBER)
  {
    /* Startup data is configured. */
  }
  else
  {
    /* Write Startup data to memory. */
    /* 1. Flash SERIAL_NUMBER. */
    Heater_StoreFlsData(&Heater, FLS_SERIAL_NUMBER);

    /* 2. Flash date product status. */
    LddFlsData = HEATER_DATEPRODUCT_IS_NOT_CONFIGURED;
    Fls_Write(FLS_DATEPRODUCTSTATUS, &LddFlsData);

    /* 3. Flash activation lock status. */
    Heater.enActiLockStatus = LONHIET_TRIAL;
    Heater_StoreFlsData(&Heater, FLS_ACTILOCKSTATUS);

    /* 4. Flash working time as 0. */
    Heater.ulWorkingTime = 0;
    Heater_StoreFlsData(&Heater, FLS_WORKINGTIME);

    /* 5. Flash year of heater. */
    Heater.MDate.usYear = MANUFACTURE_YEAR;
    Heater_StoreFlsData(&Heater, FLS_NAM);

    /* 6. Flash default setpoint as 70. */
    Heater.usSetPoint = 70;
    Heater_StoreFlsData(&Heater, FLS_SETPOINT);

    /* Reset MCU. */
  }

}
/**
  * @brief Startup heater.
  * @param[in] None.
  * @return  None.
  * @details None.
  * @note None.
  */
void Heater_Startup(void)
{
  /* To check that heater is configured datetime or not. */
  if(FALSE == Heater_DateProductIsConfigured())
  {
    Heater.enTriacStatus = HEATER_TRIAC_DISABLE;
    Heater.enOpStatus = HEATER_WAIITING_USER_SETUP_DATETIME;
  }
  else /* DateTime is configured. */
  {
    /* Checking Activation Lock status. */
    Heater.enTriacStatus = Heater_CheckActivationLock();

    /* Read all information. */
    Heater_ReadFlsData(&Heater, FLS_SETPOINT);
    Heater_ReadFlsData(&Heater, FLS_WORKINGTIME);

    Heater_ReadFlsData(&Heater, FLS_NGAY);
    Heater_ReadFlsData(&Heater, FLS_THANG);
    Heater_ReadFlsData(&Heater, FLS_NAM);
    
    Heater_DisplayInfoAtStartup();
    
    #if (DEBUG_MODE == STD_ON)
    printf("Ngay: %d\r\n", Heater.MDate.ucDay);
    printf("Thang: %d\r\n", Heater.MDate.ucMonth);
    printf("Nam: %d\r\n", Heater.MDate.usYear);
    printf("Working Time: %d minutes\r\n", Heater.ulWorkingTime);
    printf("SetPoint: %d\r\n", Heater.usSetPoint);
    printf("Activation Lock Status: ");
    switch(Heater.enActiLockStatus)
    {
      case LONHIET_LOCKED: { printf("LOCKED \r\n"); break;  }
      case LONHIET_UNLOCKED: { printf("UNLOCKED \r\n"); break;  }
      case LONHIET_TRIAL: {  printf("TRIAL \r\n"); break;  }
      default: break;
    }
    #endif
  
  }

}
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
  LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                      SETTING_MODE_BLINK_TIME);

  /* Start timeout count-down. */
  TO_Trigger(TO_SetDateTime_Channel);
}
void Exit_HEATER_SETUP_DAY_mode(boolean IsTimeOut)
{
  /* Clear timeout counter. */
  TO_Clear(TO_SetDateTime_Channel);

  if(IsTimeOut == HEATER_TIMEOUT_TRUE)
  {
    /* Don't apply new datetime. */
    LED7_DisplayResult(LED7_FAIL);
  }
  else
  {
    /* Store DateTime to memory. */
    Heater_StoreFlsData(&Heater, FLS_NGAY);
    Heater_StoreFlsData(&Heater, FLS_THANG);
    Heater.MDate.usYear = MANUFACTURE_YEAR;
    Heater_StoreFlsData(&Heater, FLS_NAM);

    /* Mark that user is configured datetime completely. */
    Heater_StoreFlsData(&Heater, FLS_DATEPRODUCTSTATUS);
  }
  /* Reset varibales. */
  Heater.ucBlinkLED7Idx = 0;

  /* Blinking all LED7 to inform that Setup-SetPoint is finished. */
  BlinkingAllLED7_Synchronous(1500);

  /* Set heater status. */
  if(IsTimeOut == HEATER_TIMEOUT_TRUE)
  {
    Heater.enOpStatus = HEATER_WAIITING_USER_SETUP_DATETIME;
  }
  else
  {
    Heater.enOpStatus = HEATER_IDLE;
  }
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
  LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                      SETTING_MODE_BLINK_TIME);

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

  }
  /* Reset varibales. */
  Heater.ucBlinkLED7Idx = 0;


  /* Blinking all LED7 to inform that Setup-SetPoint is finished. */
  BlinkingAllLED7_Synchronous(1500);

  if(IsTimeOut == HEATER_TIMEOUT_TRUE)
  {
    /* Set heater status. */
    Heater.enOpStatus = HEATER_WAIITING_USER_SETUP_DATETIME;
  }
  else
  {
    Enter_HEATER_SETUP_DAY_mode();
  }
}

void Enter_HEATER_UPDATE_SETPOINT_mode(void)
{
  /* Set Heater status as HEATER_UPDATE_SETPOINT. */
  Heater.enOpStatus = HEATER_UPDATE_SETPOINT;

  /* Blinking All LED7 to inform that Heater is in Update setpoint mode. */
  BlinkingAllLED7_Synchronous(1500);

  /* Blinking LED7_0 first. */
  Heater.ucBlinkLED7Idx = 0;
  
  if(MAX_THOUSANDS_TEMPERATURE == 0)
  {
    Heater.ucBlinkLED7Idx++;
  }
  if(MAX_HUNDREDS_TEMPERATURE == 0)
  {
    Heater.ucBlinkLED7Idx++;
  }

  /* Enable blinking LED7. */
  LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                      SETTING_MODE_BLINK_TIME);

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
  LED7_EnableBlinking(NUMBER_TO_LEDID(Heater.ucBlinkLED7Idx), \
                      SETTING_MODE_BLINK_TIME);

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
      pHeater->MDate.usYear = (uint16)Temp;
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
      Temp = (Fls_DataType)pHeater->MDate.usYear;
      Fls_Write(FLS_NAM, &Temp);
      break;
    }
    case FLS_DATEPRODUCTSTATUS: /* Read DateTime product status. */
    {
      Temp = (Fls_DataType)HEATER_DATEPRODUCT_IS_CONFIGURED;
      Fls_Write(FLS_DATEPRODUCTSTATUS, &Temp);
      break;
    }
    case FLS_SERIAL_NUMBER: /* Read DateTime product status. */
    {
      Temp = (Fls_DataType)SERIAL_NUMBER;
      Fls_Write(FLS_SERIAL_NUMBER, &Temp);
      break;
    }
    default: break;
  }
}

/* Blinking LED7-Seg in milisecond synchronously. */
void BlinkingAllLED7_Synchronous(uint32 duration)
{
  LED7_EnableBlinking(LED7_ALL, END_EVENT_BLINK_TIME);
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
  /* Cau hinh chan DIEU KHIEN LED_STATUS */
  GPIO_SetMode(LED_STATUS_PORT, LED_STATUS_BIT, GPIO_PMD_OUTPUT);
  LED_STATUS = 0;

  /* Cau hinh chan DIEU KHIEN TRIAC */
  GPIO_SetMode(TRIAC_PORT, TRIAC_BIT, GPIO_PMD_OUTPUT);
  TRIAC_OFF();

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


/**
  * @brief Controlling temperture of heater.
  * @param[in] None.
  * @return  None.
  * @details None.
  * @note None.
  */
void HeatingControl_MainFunction(void)
{
  if((Heater.enTriacStatus == HEATER_TRIAC_ENABLE) && \
     (Heater.enOpStatus == HEATER_IDLE))
  {
    if(Heater.usTempTHC < Heater.usSetPoint)
    {
      TRIAC_ON();
    }
    else
    {
      TRIAC_OFF();
    }
  }
  else
  {
  
  }
}

/**
  * @brief Reads the heater diagnostic status periodically and sets 
  *        product/development accordingly.
  * @param[in] None.
  * @return  None.
  * @details None.
  * @note None.
  */
void Heater_MainFunctionDiagnostics(void)
{
  if(DEM_EVENT_STATUS_PASSED == Dem_GetEventStatus(ERROR_THERMO_NOT_CONNECTED))
  {
    Heater.enTriacStatus = HEATER_TRIAC_ENABLE;
  }
  else
  {
    Heater.enTriacStatus = HEATER_TRIAC_DISABLE;
  }
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
