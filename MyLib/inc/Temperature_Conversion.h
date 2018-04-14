/**************************************************************************//**
 * @file     Temperature_Conversion.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 12/08/2016 3:58p $
 * @brief    Temperature library conversion.
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __TEMPERATURE_CONVERSION_H__
#define __TEMPERATURE_CONVERSION_H__


#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define  MAX_TEMP_TYPE_K       1350U

#define  ADC_MAX_VALUE         4096U
#define  VREF_ADC              4.096F
#define  VREF_ADC_X1000        4096U
  
#define  MAX_TEMP_TYPE_K 1350U
  
/* Opamp amplifier factor */
#define  AMP_FACTOR (float) 63.29
  
#define  TEMP_ERROR 8888U
/* Dinh nghia cac he so cho cong thuc noi suy nhiet do cho Thermo-couple 
   type-K */
/* 0 - 500 °C */
#define d0      (float)0
#define d1      (float)2.5083550*10
#define d2      (float)7.860106/100
#define d3      (float)-2.503131/10
#define d4      (float)8.315270/100
#define d5      (float)-1.228034/100
#define d6      (float)9.804036/10000
#define d7      (float)-4.413030/100000
#define d8      (float)1.057734/1000000
#define d9      (float)-1.052755/100000000

/* 500 - 1372 °C */
#define D0      (float)-1.318058*100
#define D1      (float)4.830222*10
#define D2      (float)-1.646031
#define D3      (float)5.464731/100
#define D4      (float)-9.650715/10000
#define D5      (float)8.802193/1000000
#define D6      (float)-3.110810/100000000
  
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
uint16_t Convert_mV_to_Temp(float Lfl_EmV);

/*******************************************************************************
**                      API Functions                                         **
*******************************************************************************/
extern uint16_t Temp_ThermoCouple_Convert(uint16_t ADC_ThermoCouple, 
                                          uint16_t ADC_Enviroment);
uint16_t Temp_LM35_Convert(uint16_t LusADCValue);

#ifdef __cplusplus
}
#endif

#endif //__TEMPERATURE_CONVERSION_H__

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/

