/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */
extern uint8_t usart1_tx_buf[usart1_tx_len];
extern uint8_t usart1_rx_buf[usart1_rx_len];
extern uint8_t i2c2_rx_buf[2];
extern uint32_t TIM17_mCounter;
extern uint32_t global_time_stamp;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */
	if(LL_DMA_IsActiveFlag_TC2(DMA1) != RESET)
  {
    LL_DMA_ClearFlag_TC2(DMA1);
  }
  /* USER CODE END DMA1_Channel2_3_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */

  /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 4, channel 5 and DMAMUX1 interrupts.
  */
void DMA1_Ch4_5_DMAMUX1_OVR_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Ch4_5_DMAMUX1_OVR_IRQn 0 */
	if(LL_DMA_IsActiveFlag_TC4(DMA1))
	{
		LL_DMA_ClearFlag_TC4(DMA1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
	}
  /* USER CODE END DMA1_Ch4_5_DMAMUX1_OVR_IRQn 0 */

  /* USER CODE BEGIN DMA1_Ch4_5_DMAMUX1_OVR_IRQn 1 */

  /* USER CODE END DMA1_Ch4_5_DMAMUX1_OVR_IRQn 1 */
}

/**
  * @brief This function handles ADC1 interrupt.
  */
void ADC1_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_IRQn 0 */

  /* USER CODE END ADC1_IRQn 0 */
  /* USER CODE BEGIN ADC1_IRQn 1 */

  /* USER CODE END ADC1_IRQn 1 */
}

/**
  * @brief This function handles TIM1 break, update, trigger and commutation interrupts.
  */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 0 */
	if(LL_TIM_IsActiveFlag_UPDATE(TIM1) == SET)
	{
		int t = TIM1->CNT;
		static uint32_t i = 0;
//			i++;
//		if(i>(40000-1))
//		{
//			i=0;
//			printf("%d 1s\r\n", i);
//		}
		global_time_stamp = TIM17->CNT;
		foc_controller();
		//foc_controller();	
		LL_TIM_ClearFlag_UPDATE(TIM1);
		//printf("tim1%d\r\n",TIM1->CNT);
	}
	
  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 0 */
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 1 */

  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 1 */
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */

  /* USER CODE END TIM1_CC_IRQn 0 */
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */

  /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
  * @brief This function handles TIM17 global interrupt.
  */
void TIM17_IRQHandler(void)
{
  /* USER CODE BEGIN TIM17_IRQn 0 */
	if(LL_TIM_IsActiveFlag_UPDATE(TIM17) == SET)
	{
		TIM17_mCounter++;
		LL_TIM_ClearFlag_UPDATE(TIM17);
	}
  /* USER CODE END TIM17_IRQn 0 */
  /* USER CODE BEGIN TIM17_IRQn 1 */

  /* USER CODE END TIM17_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
	uint8_t cnt = 0;
	if(LL_USART_IsActiveFlag_IDLE(USART1))
	{
		LL_USART_ClearFlag_IDLE(USART1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
		LL_DMA_ClearFlag_TC3(DMA1);
		cnt = LL_DMA_GetDataLength(DMA1,LL_DMA_CHANNEL_3);
		foc_communication(usart1_rx_buf,usart1_rx_len-cnt);
		//printf("data len is:%d\r\n",usart1_rx_len-cnt);
		//printf("%s\r\n", usart1_rx_buf);
		LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t)usart1_rx_buf);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3,usart1_rx_len);
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
	}
  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */
	if(LL_USART_IsActiveFlag_TC(USART1))
	{
		LL_USART_ClearFlag_TC(USART1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
	}
  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
