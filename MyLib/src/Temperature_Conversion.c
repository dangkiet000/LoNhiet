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

#include "Temperature_Conversion.h"
#include "Dem.h"

/**
  * @brief  Convert adc value of LM35 to ambient temperature.
  * @param[in] adc The pointer of the specified ADC module.
  * @return ambient temperature.
  * @details  Convert adc value of LM35 to ambient temperature.
  TINH TOAN NHIET DO MOI TRUONG
    Temp_MT = Milivolt_LM35/10  
  
    Volt_LM35 = ADCValue*VREF_ADC_IN_VOLT/MAX_ADC_VALUE  
    Milivolt_LM35 = (ADCValue*VREF_ADC_IN_VOLT/MAX_ADC_VALUE)*1000
    => Temp_MT = (ADCValue*VREF_ADC_IN_VOLT/MAX_ADC_VALUE)*1000/10   
       Temp_MT = (ADCValue*VREF_ADC_IN_VOLT/MAX_ADC_VALUE)*100
    Ta co VREF_ADC_IN_VOLT*1000 = MAX_ADC_VALUE
       Temp_MT = (ADCValue*VREF_ADC_IN_VOLT/(VREF_ADC_IN_VOLT*1000))*100
  
    => Temp_MT = ADCValue/10  
  */
uint16 ThermoCouple_ADCToAmbientTemp(uint16 LusADCValue)
{
  uint16 LusADCLM35;
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
    Where:  E(mV), T(Celsius)
    
    Reference:
    http://srdata.nist.gov/its90/type_k/kcoefficients_inverse.html
  */
uint16 ThermoCouple_mV_To_Temp(float32 Lfl_EmV)
{
  uint16 LusTemp;
  float32 E;
  
  E = Lfl_EmV;
  
  if(E < 0)
  {
    return 0;
  }
  else if( E < 20.645)
  {
    LusTemp = (uint16) 
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
    LusTemp = (uint16) 
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
  * @Note
  CONG THUC BU NHIET THERMO-COUPLE 
    Temp_ThermoCouple_Real = Temp_ThermoCouple_Measure - Temp_Enviroment
    
    Where:
    Temp_Enviroment: nhiet do moi truong
    Temp_ThermoCouple_Measure: Nhiet do do duoc khi chua bu nhiet moi truong
  
  
  CONG THUC TINH DIEN AP CUA THERMO-COUPLE
    V_TC (V)= (ADCValue*Vref_ADC)/MAX_ADC_VALUE/AMP_FACTOR
    
    Trong do:
    V_TC(V): Dien ap cua Thermo-couple.
    ADCValue: Gia tri ADC cua Thermo-couple.
    Vref_ADC(V): Dien ap tahm chieu cua ADC.
    MAX_ADC_VALUE: 4096
    AMP_FACTOR: He so khuech dai cua Opamp.
    
    V_TC (mV)= (ADCValue*Vref_ADC*1000)/MAX_ADC_VALUE/AMP_FACTOR
    ma Vref_ADC*1000 = MAX_ADC_VALUE
    => V_TC (mV)= ADCValue/AMP_FACTOR
  */
uint16 ThermoCouple_ADCToTemp(uint16 ADC_ThermoCouple, \
                                   uint16 ADC_Enviroment)
{
  uint16 LusTemp_ThermoCouple; /* in Celsius */
  float32 LflTC_mV = 0; /* Voltage of Thermo-couple in milivolt */
  uint16 Lus_LM35_Temp = 0;
  uint16 Lus_TC_TempNotCom = 0;
  
  /* DET runtime error detect. */
  /* Checking if heater is not connect with Thermo-couple (ADC value = max)*/
  if(ADC_ThermoCouple > MAX_ADC_TYPE_K_NON_THERMO)
  {
    /* Report Det runtime error detect */
    LusTemp_ThermoCouple = TEMP_ERROR;
    
    Dem_SetEventStatus(ERROR_THERMO_NOT_CONNECTED, DEM_EVENT_STATUS_FAILED);
  }
  else if(ADC_ThermoCouple > MAX_ADC_TYPE_K)
  {
    LusTemp_ThermoCouple = TEMP_ERROR;
  }
  else
  {
  
    /* Convert ADC value to miliVoltage. */
    LflTC_mV = ((float32)(ADC_ThermoCouple))/ AMP_FACTOR;

    Lus_TC_TempNotCom = ThermoCouple_mV_To_Temp(LflTC_mV);

    Lus_LM35_Temp = ThermoCouple_ADCToAmbientTemp(ADC_Enviroment);
    
    if(Lus_TC_TempNotCom < Lus_LM35_Temp)
    {
      /* Report Det runtime error detect */
      LusTemp_ThermoCouple = TEMP_ERROR;
    }
    
    LusTemp_ThermoCouple = Lus_TC_TempNotCom - Lus_LM35_Temp;
    
    Dem_SetEventStatus(ERROR_THERMO_NOT_CONNECTED, DEM_EVENT_STATUS_PASSED);
  }
  return LusTemp_ThermoCouple;
}


/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/



