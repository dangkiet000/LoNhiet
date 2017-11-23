/**************************************************************************//**
 * @file     Temperature_Conversion.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 12/08/2016 3:58p $
 * @brief    Temperature library conversion.
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "NUC200Series.h"


/**
  * @brief  Convert adc value of LM35 to ambient temperature.
  * @param[in] adc The pointer of the specified ADC module.
  * @return ambient temperature.
  * @details  Convert adc value of LM35 to ambient temperature.
  TINH TOAN NHIET DO MOI TRUONG
    Temp_MT = Milivolt_LM35/10  
  
    Volt_LM35 = ADCValue*VREF_ADC/ADC_MAX_VALUE  
    Milivolt_LM35 = (ADCValue*VREF_ADC/ADC_MAX_VALUE)*1000
    => Temp_MT = (ADCValue*VREF_ADC/ADC_MAX_VALUE)*1000/10   
       Temp_MT = (ADCValue*VREF_ADC/ADC_MAX_VALUE)*100
    Ta co VREF_ADC*1000 = ADC_MAX_VALUE
       Temp_MT = (ADCValue*VREF_ADC/(VREF_ADC*1000))*100
  
    => Temp_MT = ADCValue/10  
  */
uint16_t Temp_LM35_Convert(uint16_t LusADCValue)
{
  uint16_t LusADCLM35;
  LusADCLM35 = LusADCValue;
  
  return (LusADCLM35/10);
}

/**
  * @brief  Convert voltage Thermo-couple to Thermo-couple temperature.
  * @param[in] E: Voltage measured at Thermo-couple (mV).
  * @return Thermo-couple temperature.
  * @details  Convert milivolt of Thermo-couple to Thermo-couple temperature.
  * The equation is of the form:
    T = d_0 + d_1*E + d_2*E^2 + ... + d_n*E^n
    Where:  E(mV), T(°C)
    
    Reference:
    http://srdata.nist.gov/its90/type_k/kcoefficients_inverse.html
  */
uint16_t Convert_mV_to_Temp(float Lfl_EmV)
{
  uint16_t LusTemp;
  float E;
  
  E = Lfl_EmV;
  
  if(E < 0)
  {
    return 0;
  }
  else if( E < 20.645)
  {
    LusTemp = (uint16_t) 
       (d1*E + 
        d2*E*E +
        d3*E*E*E +
        d4*E*E*E*E +
        d5*E*E*E*E*E +
        d6*E*E*E*E*E*E +
        d7*E*E*E*E*E*E*E +
        d8*E*E*E*E*E*E*E*E +
        d9*E*E*E*E*E*E*E*E*E
       );
  }
  
  else /* E(mV) >= 20.645 mV */
  {    
    LusTemp = (uint16_t) 
       (D0 +
        D1*E + 
        D2*E*E +
        D3*E*E*E +
        D4*E*E*E*E +
        D5*E*E*E*E*E +
        D6*E*E*E*E*E*E
       );
          
  }
  return  LusTemp;
  
}

/*******************************************************************************
**                      Low Level Function                                    **
*******************************************************************************/


/*******************************************************************************
**                      API Functions                                         **
*******************************************************************************/
/**
  * @brief  Convert voltage Thermo-couple to Thermo-couple temperature.
  * @param[in] ADC_ThermoCouple: ADC value of Thermo-couple channel.
  * @param[in] ADC_Enviroment: ADC value of temperature ambient sensor.
  * @return Thermo-couple temperature after compensating.
  * @details  Convert milivolt of Thermo-couple to Real Thermo-couple temperature.

  CONG THUC BU NHIET THERMO-COUPLE 
    Temp_ThermoCouple_Real = Temp_ThermoCouple_Measure - Temp_Enviroment
    
    Where:
    Temp_Enviroment: nhiet do moi truong
    Temp_ThermoCouple_Measure: Nhiet do do duoc khi chua bu nhiet moi truong
  
  
  CONG THUC TINH DIEN AP CUA THERMO-COUPLE
    V_TC (V)= (ADCValue*Vref_ADC)/ADC_MAX_VALUE/AMP_FACTOR
    
    Trong do:
    V_TC(V): Dien ap cua Thermo-couple.
    ADCValue: Gia tri ADC cua Thermo-couple.
    Vref_ADC(V): Dien ap tahm chieu cua ADC.
    ADC_MAX_VALUE: 4096
    AMP_FACTOR: He so khuech dai cua Opamp.
    
    V_TC (mV)= (ADCValue*Vref_ADC*1000)/ADC_MAX_VALUE/AMP_FACTOR
    ma Vref_ADC*1000 = ADC_MAX_VALUE
    => V_TC (mV)= ADCValue/AMP_FACTOR
  */
uint16_t Temp_ThermoCouple_Convert(uint16_t ADC_ThermoCouple, uint16_t ADC_Enviroment)
{
  uint16_t LusTemp_ThermoCouple; /* in °C */
  float LflTC_mV = 0; /* Voltage of Thermo-couple in milivolt */
  uint16_t Lus_LM35_Temp = 0;
  uint16_t Lus_TC_TempNotCom = 0;
  uint16_t LulADC_ThermoCouple = ADC_ThermoCouple;
  uint16_t LulADC_Enviroment = ADC_Enviroment;
  
  LflTC_mV = ((float)(LulADC_ThermoCouple))/ AMP_FACTOR;

  Lus_TC_TempNotCom = Convert_mV_to_Temp(LflTC_mV);

  Lus_LM35_Temp = Temp_LM35_Convert(LulADC_Enviroment);
  
  if(Lus_TC_TempNotCom < Lus_LM35_Temp)
  {
    return TEMP_ERROR;
  }
  
  LusTemp_ThermoCouple = Lus_TC_TempNotCom - Lus_LM35_Temp;
  return LusTemp_ThermoCouple;
}
/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/



