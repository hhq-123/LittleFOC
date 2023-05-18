/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  /**ADC1 GPIO Configuration
  PA4   ------> ADC1_IN4
  PA5   ------> ADC1_IN5
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* ADC1 DMA Init */

  /* ADC1 Init */
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMAMUX_REQ_ADC1);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PDATAALIGN_HALFWORD);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MDATAALIGN_HALFWORD);

  /* ADC1 interrupt Init */
  NVIC_SetPriority(ADC1_IRQn, 0);
  NVIC_EnableIRQ(ADC1_IRQn);

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */

   #define ADC_CHANNEL_CONF_RDY_TIMEOUT_MS ( 1U)
   #if (USE_TIMEOUT == 1)
   uint32_t Timeout ; /* Variable used for Timeout management */
   #endif /* USE_TIMEOUT */

  ADC_InitStruct.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_LEFT;
  ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  LL_ADC_REG_SetSequencerConfigurable(ADC1, LL_ADC_REG_SEQ_FIXED);
  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_EXT_TIM3_TRGO;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
  ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
  LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
  LL_ADC_REG_SetSequencerScanDirection(ADC1, LL_ADC_REG_SEQ_SCAN_DIR_FORWARD);
  LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);
  LL_ADC_SetTriggerFrequencyMode(ADC1, LL_ADC_CLOCK_FREQ_MODE_HIGH);
  LL_ADC_REG_SetSequencerChAdd(ADC1, LL_ADC_CHANNEL_4|LL_ADC_CHANNEL_5);

   /* Poll for ADC channel configuration ready */
   #if (USE_TIMEOUT == 1)
   Timeout = ADC_CHANNEL_CONF_RDY_TIMEOUT_MS;
   #endif /* USE_TIMEOUT */
   while (LL_ADC_IsActiveFlag_CCRDY(ADC1) == 0)
     {
   #if (USE_TIMEOUT == 1)
   /* Check Systick counter flag to decrement the time-out value */
   if (LL_SYSTICK_IsActiveCounterFlag())
     {
   if(Timeout-- == 0)
         {
   Error_Handler();
         }
     }
   #endif /* USE_TIMEOUT */
     }
   /* Clear flag ADC channel configuration ready */
   LL_ADC_ClearFlag_CCRDY(ADC1);
  LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_FALLING);
  LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_COMMON_1, LL_ADC_SAMPLINGTIME_3CYCLES_5);
  LL_ADC_DisableIT_EOC(ADC1);
  LL_ADC_DisableIT_EOS(ADC1);

   /* Enable ADC internal voltage regulator */
   LL_ADC_EnableInternalRegulator(ADC1);
   /* Delay for ADC internal voltage regulator stabilization. */
   /* Compute number of CPU cycles to wait for, from delay in us. */
   /* Note: Variable divided by 2 to compensate partially */
   /* CPU processing cycles (depends on compilation optimization). */
   /* Note: If system core clock frequency is below 200kHz, wait time */
   /* is only a few CPU processing cycles. */
   uint32_t wait_loop_index;
   wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
   while(wait_loop_index != 0)
     {
   wait_loop_index--;
     }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/* USER CODE BEGIN 1 */
void ADC1_DMA_Config_(uint16_t *pData, uint16_t Size)
{
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_2, LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA));
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMAMUX_REQ_ADC1);
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_2,(uint32_t)pData);
	LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_2, Size);
	
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_2);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
	
	//LL_ADC_StartCalibration(ADC1);
	while(LL_ADC_IsCalibrationOnGoing(ADC1));
	LL_mDelay(2);
	LL_ADC_Enable(ADC1);
  LL_ADC_REG_StartConversion(ADC1);        //对于F0系列的芯片需要加上这句代码
}
/* USER CODE END 1 */
