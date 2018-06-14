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

/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "Std_Types.h"


/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define  MAX_TEMP_TYPE_K       1350U

#define  MAX_ADC_VALUE         4096U
#define  VREF_ADC_IN_VOLT      4.096F /* 4.096V <=> 4096mV */
#define  VREF_ADC_IN_MILIVOLT  4096U
  

 
/* Opamp amplifier factor */
#define  AMP_FACTOR    (float32)63.29

#define  MAX_MILIVOLT_TYPE_K (float32)54.886


#define  MAX_ADC_TYPE_K        (uint16)(AMP_FACTOR*MAX_MILIVOLT_TYPE_K)

#define  MAX_ADC_TYPE_K_NON_THERMO   4000U 

#define  TEMP_ERROR            8888U


/* Dinh nghia cac he so cho cong thuc noi suy nhiet do cho Thermo-couple 
   type-K */
/* 0 - 500 Celsius */
#define d0      (float32)0
#define d1      (float32)2.5083550*10
#define d2      (float32)7.860106/100
#define d3      (float32)-2.503131/10
#define d4      (float32)8.315270/100
#define d5      (float32)-1.228034/100
#define d6      (float32)9.804036/10000
#define d7      (float32)-4.413030/100000
#define d8      (float32)1.057734/1000000
#define d9      (float32)-1.052755/100000000

/* 500 - 1372 Celsius */
#define D0      (float32)-1.318058*100
#define D1      (float32)4.830222*10
#define D2      (float32)-1.646031
#define D3      (float32)5.464731/100
#define D4      (float32)-9.650715/10000
#define D5      (float32)8.802193/1000000
#define D6      (float32)-3.110810/100000000
  
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
uint16 ThC_mV_To_Temp(float32 Lfl_EmV);
uint16 ThC_ADCToAmbientTemp(uint16 LusADCValue);
/*******************************************************************************
**                      API Functions                                         **
*******************************************************************************/
extern uint16 ThC_ADCToTemp(uint16 ADC_ThermoCouple, 
                                     uint16 ADC_Enviroment);


#endif //__TEMPERATURE_CONVERSION_H__

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/

