/**************************************************************************//**
 * @file     Get_ADC_Value.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 06/01/2016 01:10 $
 * @brief    Get ADC Value library conversion.
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "Get_ADC_Value.h"

/*******************************************************************************
**                      Global Data Types                                    **
*******************************************************************************/
uint16_t LusADC_Val[PDMA_DATA_NUM] = {0}; /* Mang Gia tri ADC */
static uint32_t GulADC_THC_TB;  /* Gia tri ADC trung binh cua Thermo-couple */
static uint32_t LulADC_THC_TB;  /* Gia tri ADC trung binh cua Thermo-couple */
static uint32_t GulADC_LM35_TB; /* Gia tri ADC trung binh cua LM35 */
static uint8_t LucCnt;
/* Status of ADC */
volatile ADCStatusType LenConvertStatus = ADC_IDLE;

/*******************************************************************************
**                      Internal Functions                                    **
*******************************************************************************/
/*
Ngat nay xay ra khi DMA van chuyen tat ca gia tri ADC va luu tai mang LusADC_Val[]
-> Moi lan ngat la 300 lan (PDMA_DATA_NUM) lay mau ADC

Cu 4 xay ra ngat convert ThermoCouple thi co 1 lan convert LM35
4 :ThermoCouple
1 :LM35
*/
void PDMA_IRQHandler(void)
{
  if((PDMA_GET_CH_INT_STS(PDMA_CHANNEL_ADC) & PDMA_ISR_BLKD_IF_Msk))
  {
    /* Now, Data is loaded in LusADC_Val[] array. */
    /* ADC_Sum: Tong cac gia tri ADC trong mang LusADC_Val[] 
       cua channel hien tai */
    static uint32_t ADC_Sum = 0;
    volatile uint16_t n = 0;
    
    /* Clear PDMA Block transfer done interrupt flag */
    PDMA_CLR_CH_INT_FLAG(PDMA_CHANNEL_ADC, PDMA_ISR_BLKD_IF_Msk);
    
    /* Calculate the average of ADC value */
    for(n = 0; n < PDMA_DATA_NUM; n++)
    {
      ADC_Sum += LusADC_Val[n];  
      LusADC_Val[n] = 0;     
    }
      
    /* Current channel is Thermo-couple */
    if(LenConvertStatus == ADC_BUSY) 
    {    
      LulADC_THC_TB += (ADC_Sum / PDMA_DATA_NUM);
      LucCnt++;
      if(LucCnt > 1)
      {
        GulADC_THC_TB = LulADC_THC_TB/LucCnt;
        LucCnt = 0;
        LulADC_THC_TB = 0;
        /* Switch ADC channel to ADC_LM35_CH */
        Switch_PDMA_Channel(ADC_LM35_CH);
        
        /* Change status */
        LenConvertStatus = ADC_LM35_IS_CONVERTING;
      }
      
      /* Reset value */
      ADC_Sum = 0;
   
    }
    /* Current channel is LM35 */
    else if (LenConvertStatus == ADC_LM35_IS_CONVERTING)
    {
      GulADC_LM35_TB = ADC_Sum / PDMA_DATA_NUM;
      
      /* To Start convert ADC_THC_CH */
      Switch_PDMA_Channel(ADC_THC_CH);
      
      /* Change status */
      LenConvertStatus = ADC_IDLE;
      
      /* Reset value */
      ADC_Sum = 0;
    }
    else 
    { /* Nothing to do */ }

    if(LenConvertStatus != ADC_IDLE)
    {
      /* Trigger PDMA */
      PDMA_Trigger(PDMA_CHANNEL_ADC);
    }
    else 
    { /* Nothing to do */ }
  }
}

/**
  * @brief  Initialize ADC channel to get ADC values.
  * @param[in] None.
  * @return None.
  * @details  Initialize ADC channel to get ADC values.
  *           ADC use for temperature sensor and thermo-couple.
 **/
void ADC_Init(void)
{
  /* 1. Enable ADC module clock */
  CLK_EnableModuleClock(ADC_MODULE);

  /* 2. ADC clock source is 50MHz, set divider to 16, ADC clock is 3.125MHz*/
  CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_PLL, CLK_CLKDIV_ADC(16));

  /* 5. Set the ADC operation mode as continuous scan, input mode as single-end and
     enable the analog input channel 0 and 1 */
  ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_CONTINUOUS, (1 << ADC_LM35_CH));

  /* 6. ADC convert complete interrupt: Disable */
  ADC_DisableInt(ADC, ADC_ADF_INT);

  /* 7. Power on ADC module */
  ADC_POWER_ON(ADC);

  /* 8. Clear the A/D interrupt flag for safe */
  ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

}

/**
  * @brief  Initialize PDMA to transfer ADC values to array.
  * @param[in] None.
  * @return None.
  * @details  Initialize PDMA to transfer ADC values to array.
  *           PDMA use for tranfering data ADC.
 **/
void PDMA_Init(void)
{
  /* 1. Enable PDMA clock source */
  CLK_EnableModuleClock(PDMA_MODULE);

  /* 2. Reset PDMA module */
  SYS_ResetModule(PDMA_RST);

  /* 3. Enable PDMA channel 0 clock */
  PDMA_Open(1 << PDMA_CHANNEL_ADC);

  /* 4. ADC PDMA channel configuration */
  /* 4.1 PDMA channel enable */
  PDMA_CH_Enable(PDMA_CHANNEL_ADC);

  /* 4.2 Transfer width 16 bits and Transfer count */
  PDMA_SetTransferCnt(PDMA_CHANNEL_ADC, PDMA_WIDTH_16, PDMA_DATA_NUM);

  /* 4.3 Set PDMA Transfer Address: Fixed source address and
         Increment destination address */
  PDMA_SetTransferAddr(PDMA_CHANNEL_ADC, \
                       (uint32_t) &ADC->ADDR[ADC_LM35_CH], PDMA_SAR_FIX, \
                       (uint32_t) &LusADC_Val, PDMA_DAR_INC);

  /* 4.4 Set mode: Peripheral-to-Memory and PDMA channel is connected on-chip \
         peripheral ADC */
  PDMA_SetTransferMode(PDMA_CHANNEL_ADC, PDMA_ADC, FALSE, 0);

  /* 5. Enable the channel interrupt when PDMA transfer is done */
  PDMA_EnableInt(PDMA_CHANNEL_ADC, PDMA_IER_BLKD_IE_Msk);

  /* 6. Enable Interrupt PDMA in NVIC */
  NVIC_EnableIRQ(PDMA_IRQn);
  NVIC_SetPriority(PDMA_IRQn, 0);
  
  /* 7. Enable PDMA transfer */
  ADC_ENABLE_PDMA(ADC);

  /* 8. Enable ADC interrupt in NVIC */
  ADC_DisableInt(ADC, ADC_ADF_INT);
  NVIC_DisableIRQ(ADC_IRQn);

  /* 9. Trigger the selected channel */
  //PDMA_Trigger(PDMA_CHANNEL_ADC);

  /* 10. Enable PDMA transfer */
  ADC_ENABLE_PDMA(ADC);

  /* 11. Start A/D conversion */
  ADC_START_CONV(ADC);

}

/**
  * @brief  Switch PDMA to transfer ADC LM35 channel or 
  *         ADC Thermo-couple channel to array.
  * @param[in] None.
  * @return None.
  * @details  Switch PDMA to transfer ADC LM35 channel or 
  *         ADC Thermo-couple channel to array.         
 **/
void Switch_PDMA_Channel(uint32_t LulChannel)
{
  /* Set ADC input channel. */
  ADC_SET_INPUT_CHANNEL(ADC, (1 << LulChannel));

  /* Set the selected channel source address. */
  PDMA_SET_SRC_ADDR(PDMA_CHANNEL_ADC, (uint32_t) &ADC->ADDR[LulChannel]);
}

void Get_ADC_Init(void)
{
  ADC_Init();
  PDMA_Init();
  
  LenConvertStatus = ADC_IDLE;
  GulADC_THC_TB = 0;
  GulADC_LM35_TB = 0;
  LulADC_THC_TB = 0;
  LucCnt = 0;
}

/**
  * @brief  Switch PDMA to transfer ADC LM35 channel or 
  *         ADC Thermo-couple channel to array.
  * @param[in] None.
  * @return None.
  * @details  Switch PDMA to transfer ADC LM35 channel or 
  *           ADC Thermo-couple channel to array.         
 **/
Std_ReturnType ADC_StartConvert(void)
{
  /* To check PDMA channel is connverting or not */
  if(LenConvertStatus != ADC_IDLE)
  {
    return E_NOT_OK;
  }
  else
  {
    LenConvertStatus = ADC_BUSY;
    
    /* Trigger PDMA */
    PDMA_Trigger(PDMA_CHANNEL_ADC);
    
    return E_OK;
  }
  
}
/**
  * @brief  To get ADC value.
  * @param[in] LblChannelType    ADC_THC_CH
                                 ADC_LM35_CH
  * @return value of ADC.
  * @details  To get ADC value of channel ADC_THC_CH or ADC_LM35_CH.         
 **/
uint16_t Get_ADC_Value(boolean LblChannelType)
{
  if(LenConvertStatus == ADC_IDLE)
  {
    if(LblChannelType == ADC_THC_CH)
    {
      return (uint16_t)GulADC_THC_TB;
    }
    else
    {
      return (uint16_t)GulADC_LM35_TB;
    }
  }
  else
  {
    return ADC_ERROR_VALUE;
  } 
}

/* Temperature convert function */
uint16_t GetTemp_ThermoCouple(void)
{
  return ThermoCouple_ADCToTemp(GulADC_THC_TB, GulADC_LM35_TB);
}

ADCStatusType ADC_GetStatus(void)
{
  return LenConvertStatus;
}
/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/



