/**************************************************************************//**
 * @file     UserInput_Validation.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 26/05/2018 3:59p $
 * @brief    DateTime Validation application source file.
 *
 * @note
*****************************************************************************/
#include "UserInput_Validation.h"
#include "LED7Segment.h"

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
const uint8 GaaMapPlaceToMaxTempDigit[4] = {
  MAX_THOUSANDS_TEMPERATURE, 
  MAX_HUNDREDS_TEMPERATURE,
  MAX_TENS_TEMPERATURE,
  MAX_ONES_TEMPERATURE
};


/*******************************************************************************
**                      Function                                              **
*******************************************************************************/
/**
  * @brief  Get multiplication factor of number having 4 digits.
  * @param[in] Place: place of digit which is setting.
  * @param[out] None.
  * @return  Multiplication factor.
  *            Place: ONES      => return 1; 
  *            Place: TENS      => return 10;
  *            Place: HUNDREDS  => return 100;
  *            Place: THOUSANDS => return 1000;
  * @details  None.
  */
STATIC uint16 GetMulFactor(uint8 Place)
{
  uint16 MulFactor;
  
  if(Place == THOUSANDS)
  {
    MulFactor = 1000;
  }
  else if(Place == HUNDREDS)
  {
    MulFactor = 100;
  }
  else if(Place == TENS)
  {
    MulFactor = 10;
  }
  else if(Place == ONES)
  {
    MulFactor = 1;
  }
  
  return MulFactor;
}
/**
  * @brief  Get max day of month.
  * @param[in] *DateTime: pointer point to DateTime of Heater.
  * @param[out] *DateTime.
  * @return  max day of month.
  * @details  None.
  */
STATIC uint8 Heater_GetMaxDay(Heater_DateTimeType DateTime)
{
  uint8 LucMaxDay;
  
  switch (DateTime.ucMonth)
  {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
    {
      LucMaxDay = 31;
      break;
    }
    case 2:
    {
      /* Checking leap year. */
      if (DateTime_CheckLeapYear(DateTime.usYear) == TRUE)
          LucMaxDay = 29;
      else 
          LucMaxDay = 28;
      break;
    }
    case 4:
    case 6:
    case 9:
    case 11:
    {
      LucMaxDay = 30;
      break;
    }
    default: break;
  }
  
  return LucMaxDay;
}

/**
  * @brief  Correct Month when user increase or decrease value.
  * @param[in] *DateTime: pointer point to DateTime of Heater.
  * @param[out] *DateTime.
  * @return None.
  * @details  None.
  */
STATIC boolean DateTime_CheckLeapYear(uint16 year)
{
  boolean LblRetVal;
  
  /* Checking leap year. */
  if ((year%4==0 && year%100!=0) || (year%400==0))
  {
    LblRetVal = TRUE;
  }
  else 
  {
    LblRetVal = FALSE;
  }
  
  return LblRetVal;
}



/**
  * @brief  Correct Month when user decrease value.
  * @param[in] *DateTime: pointer point to DateTime of Heater.
  * @param[in] Place: place of digit which is setting.
  * @param[out] *DateTime.
  * @return None.
  * @details  None.
  */
void Heater_MonMinus(Heater_DateTimeType *DateTime, uint8 Place)
{
  uint8 LucMaxValue;
  uint8 LucChuc;
  uint8 LucDonvi;
  
  LucChuc = DateTime->ucMonth/10;
  LucDonvi = DateTime->ucMonth%10;
  
  if(Place == TENS)
  {
    LucMaxValue = 1;
    
    /* User is configuring HÀNG CHỤC. */
    if (LucChuc == MIN_MON_TENS)
    {
      /* reset hàng chục về MAX. */
      LucChuc = LucMaxValue;
    }
    else
    {
      /* Giảm hàng chục 1. */
      LucChuc--;
    }
  }
  else if(Place == ONES)
  {
    /* User is configuring HÀNG ĐƠN VỊ. */

    switch(LucChuc)
    {
      case 0: {  LucMaxValue = 9; break;  }
      case 1: {  LucMaxValue = 2; break;  }
      default: break;
    }
    
    if (LucDonvi == 0)
    {
      /* reset hàng đơn vị về MAX */
      LucDonvi = LucMaxValue;
    }
    else if((LucChuc == MIN_MON_TENS) && (LucDonvi == MIN_MON_ONES))
    {
      /* reset hàng đơn vị về MAX */
      LucDonvi = LucMaxValue;
    }
    else
    {
      /* Giảm hàng đơn vị 1. */
      LucDonvi--;
    }
  }
  
  DateTime->ucMonth = (LucChuc*10) + LucDonvi;
}

/**
  * @brief  Correct Month when user increase value.
  * @param[in] *DateTime: pointer point to DateTime of Heater.
  * @param[in] Place: place of digit which is setting.
  * @param[out] *DateTime.
  * @return None.
  * @details  None.
  */
void Heater_MonPlus(Heater_DateTimeType *DateTime, uint8 Place)
{
  uint8 LucMaxValue;
  uint8 LucChuc;
  uint8 LucDonvi;
  
  LucChuc = DateTime->ucMonth/10;
  LucDonvi = DateTime->ucMonth%10;
  
  if(Place == TENS)
  {
    LucMaxValue = 1;
    
    /* User is configuring HÀNG CHỤC. */
    if (LucChuc >= LucMaxValue)
    {
      /* reset hàng chục về 0 */
      LucChuc = MIN_MON_TENS;
    }
    else
    {
      /* Tăng hàng chục 1. */
      LucChuc++;
    }
  }
  else if(Place == ONES)
  {
    /* User is configuring HÀNG ĐƠN VỊ. */
    switch(LucChuc)
    {
      case 0: {  LucMaxValue = 9; break;  }
      case 1: {  LucMaxValue = 2; break;  }
      default: break;
    }
    
    if (LucDonvi >= LucMaxValue)
    {
      if(LucChuc == MIN_MON_TENS)
      {
        /* reset hàng đơn vị về MIN_MON_ONES */
        LucDonvi = MIN_MON_ONES;
      }
      else
      {
        /* reset hàng đơn vị về 0 */
        LucDonvi = 0;
      }
    }
    else
    {
      /* Tăng hàng đơn vị 1. */
      LucDonvi++;
    }
  }
  
  DateTime->ucMonth = (LucChuc*10) + LucDonvi;
}


/**
  * @brief  Correct Day when user increase value.
  * @param[in] *DateTime: pointer point to DateTime of Heater.
  * @param[in] Place: place of digit which is setting.
  * @param[out] *DateTime.
  * @return None.
  * @details  None.
  */
void Heater_DayPlus(Heater_DateTimeType *DateTime, uint8 Place)
{
  uint8 LucMaxDay;
  uint8 LucMaxValue;
  uint8 LucChuc;
  uint8 LucDonvi;
  
  LucMaxDay = Heater_GetMaxDay(*DateTime);
  
  LucChuc = DateTime->ucDay/10;
  LucDonvi = DateTime->ucDay%10;
  
  if(Place == TENS)
  {
    /* User is configuring TENS. */
    LucMaxValue = LucMaxDay/10;

    if (LucChuc >= LucMaxValue)
    {
      /* reset hàng chục về 0 */
      LucChuc = 0;
    }
    else
    {
      /* Tăng hàng chục 1. */
      LucChuc++;
    }
  }
  /* User is configuring ONES */
  else if(Place == ONES)
  {
    switch(LucChuc)
    {
      case 0: {  LucMaxValue = 9; break;  }
      case 1: {  LucMaxValue = 9; break;  }
      case 2: 
      {  
        if(DateTime->ucMonth == 2)
        {
          LucMaxValue = LucMaxDay%10;  
        }
        else
        {
          LucMaxValue = 9;
        }
        break;
      }
      case 3: {  LucMaxValue = LucMaxDay%10; break;  }
      default: break;
    }
    
    if(LucMaxValue == 0)
    {
      LucDonvi = 0;
    }
    else if (LucDonvi >= LucMaxValue)
    {
      if(LucChuc == MIN_DAY_TENS)
      {
        /* reset hàng đơn vị về MIN_DAY_ONES */
        LucDonvi = MIN_DAY_ONES;
      }
      else
      {
        /* reset hàng đơn vị về 0 */
        LucDonvi = 0;
      }
    }
    else
    {
      /* Tăng hàng đơn vị 1. */
      LucDonvi++;
    }
  }
  
  DateTime->ucDay = (LucChuc*10) + LucDonvi;
}

/**
  * @brief  Correct Day when user decrease value.
  * @param[in] *DateTime: pointer point to DateTime of Heater.
  * @param[in] Place: place of digit which is setting.
  * @param[out] *DateTime.
  * @return None.
  * @details  None.
  */
void Heater_DayMinus(Heater_DateTimeType *DateTime, uint8 Place)
{
  uint8 LucMaxDay;
  uint8 LucMaxValue;
  uint8 LucChuc;
  uint8 LucDonvi;
  
  LucMaxDay = Heater_GetMaxDay(*DateTime);
  
  LucChuc = DateTime->ucDay/10;
  LucDonvi = DateTime->ucDay%10;
  
  if(Place == TENS)
  {
    /* User is configuring TENS. */
    LucMaxValue = LucMaxDay/10;

    if (LucChuc == 0)
    {
      /* Reset hàng chục về MAX */
      LucChuc = LucMaxValue;
    }
    else
    {
      /* Giảm hàng chục 1. */
      LucChuc--;
    }
  }
  /* User is configuring ONES */
  else if(Place == ONES)
  {
    switch(LucChuc)
    {
      case 0: {  LucMaxValue = 9; break;  }
      case 1: {  LucMaxValue = 9; break;  }
      case 2: {  LucMaxValue = LucMaxDay%10; break;  }
      case 3: {  LucMaxValue = LucMaxDay%10; break;  }
      default: break;
    }
    
    if(LucMaxValue == 0)
    {
      LucDonvi = 0;
    }
    else if((LucChuc == MIN_DAY_TENS) && (LucDonvi == MIN_DAY_ONES))
    {
      /* reset hàng đơn vị về 0 */
      LucDonvi = LucMaxValue;
    }
    else if (LucDonvi == 0)
    {
      /* reset hàng đơn vị về 0 */
      LucDonvi = LucMaxValue;
    }
    else
    {
      /* Giảm hàng đơn vị 1. */
      LucDonvi--;
    }
  }
  
  DateTime->ucDay = (LucChuc*10) + LucDonvi;
}


/**
  * @brief  Correct temperature when user increase value in setting mode.
  * @param[in] SetPoint.
  * @param[in] Place: place of digit which is setting.
  * @return None.
  * @details  None.
  */
uint16 Heater_TempPlus(uint16 SetPoint, uint8 Place)
{
  uint8  LaaSetPoint[4];
  uint16 MulFactor;
  
  MulFactor = GetMulFactor(Place);
  
  Int_to_Array(SetPoint, LaaSetPoint, LED7_DISPLAY_NUMBER_LEADING_ZEROS);
  
  SetPoint += MulFactor;

  if(SetPoint > MAX_HEATER_TEMPERATURE)
  {
    /* Set this digit value to 0. */
    LaaSetPoint[Place] = 0;
    SetPoint = Array_To_Int(LaaSetPoint);
  }
  else
  {
    if(LaaSetPoint[Place] >= 9)
    {
      /* Set this digit value to 0. */
      LaaSetPoint[Place] = 0;
      SetPoint = Array_To_Int(LaaSetPoint);
    }
  }
  return SetPoint;
}

/**
  * @brief  Correct temperature when user decrease value in setting mode.
  * @param[in] SetPoint.
  * @param[in] Place: place of digit which is setting. Range: 0..3
  * @return None.
  * @details  None.
  */
uint16 Heater_TempMinus(uint16 SetPoint, uint8 Place)
{
  uint8  LaaSetPoint[4];
  uint16 MulFactor;
  
  MulFactor = GetMulFactor(Place);
  
  Int_to_Array(SetPoint, LaaSetPoint, LED7_DISPLAY_NUMBER_LEADING_ZEROS);
  
  if(SetPoint != 0)
  {
    SetPoint -= MulFactor;
    
    if(LaaSetPoint[Place] == 0)
    {
      /* Set this digit value to max. */
      LaaSetPoint[Place] = 9;
      SetPoint = Array_To_Int(LaaSetPoint);
      if(SetPoint > MAX_HEATER_TEMPERATURE)
      {
        /* Set this digit value to max. */
        LaaSetPoint[Place] = GaaMapPlaceToMaxTempDigit[Place];
        SetPoint = Array_To_Int(LaaSetPoint);
      }
    }
  }
  else /* Nếu setpoint là 0 mà trừ thì sẽ bị overflow */
  {
    if((Place > 0) && (GaaMapPlaceToMaxTempDigit[Place-1] > 0))
    {
      /* Set this digit value to max number. */
      LaaSetPoint[Place] = 9;
      SetPoint = Array_To_Int(LaaSetPoint);
    }
    else
    {
      /* Set this digit value to max temperature digit. */
      LaaSetPoint[Place] = GaaMapPlaceToMaxTempDigit[Place];
      SetPoint = Array_To_Int(LaaSetPoint);
    }
  }
  

  
  return SetPoint;
}


/*@}*/ /* None */

/*@}*/ /* None */

/*@}*/ /* None */

