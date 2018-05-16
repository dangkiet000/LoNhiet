/**************************************************************************//**
 * @file     Get_ADC_Value.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 06/01/2016 01:10 $
 * @brief    Temperature library conversion.
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __GET_ADC_VALUE_H__
#define __GET_ADC_VALUE_H__

/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "NUC200Series.h"
#include "Std_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**                      Define macro                                          **
*******************************************************************************/
#define ADC_THC_CH        0U
#define ADC_LM35_CH       1U
  
#define PDMA_CHANNEL_ADC  0U   /* PDMA  channel0 use for ADC */
  
/******************************************************************************
  This is number of PDMA transfer data 
  ADC is trig by PDMA
  => This is number of ADC get sample when PDMA is trigged.
 *****************************************************************************/
#define PDMA_DATA_NUM     500U /* Max = 65535/2 */
  

#define THC_RATIO         3U
  
#define ADC_ERROR_VALUE   0U

/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/
/* Definition of event type */
typedef enum ETag_ADCStatusType
{
  ADC_IDLE = 0,
  ADC_BUSY,
  ADC_LM35_IS_CONVERTING
} ADCStatusType;

static uint32_t GulADC_THC_TB;  /* Gia tri ADC trung binh cua Thermo-couple */
static uint32_t GulADC_LM35_TB; /* Gia tri ADC trung binh cua LM35 */

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
void ADC_Init(void);
void PDMA_Init(void);
void Switch_PDMA_Channel(uint32_t LulChannel);

extern uint16_t ThermoCouple_ADCToTemp(uint16_t ADC_ThermoCouple, 
                                          uint16_t ADC_Enviroment);

/*******************************************************************************
**                      API Functions                                         **
*******************************************************************************/
void Get_ADC_Init(void);
Std_ReturnType ADC_StartConvert(void);
uint16_t Get_ADC_Value(boolean LblChannelType);
/* Temperature convert function */
uint16_t GetTemp_ThermoCouple(void);
ADCStatusType ADC_GetStatus(void);

#ifdef __cplusplus
}
#endif

#endif //__GET_ADC_VALUE_H__

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/

