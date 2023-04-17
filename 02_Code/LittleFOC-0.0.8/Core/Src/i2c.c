/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include "tim.h"

uint8_t i2c2_rx_buf[i2c2_rx_len];

extern unsigned long long TIM17_mCounter;
/* USER CODE END 0 */

/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  /**I2C2 GPIO Configuration
  PA11 [PA9]   ------> I2C2_SCL
  PA12 [PA10]   ------> I2C2_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);

  /* I2C2 DMA Init */

  /* I2C2_RX Init */
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMAMUX_REQ_I2C2_RX);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_BYTE);

  /* I2C2 interrupt Init */
  NVIC_SetPriority(I2C2_IRQn, 1);
  NVIC_EnableIRQ(I2C2_IRQn);

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */

  /** I2C Initialization
  */
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x00301183;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C2, &I2C_InitStruct);
  LL_I2C_EnableAutoEndMode(I2C2);
  LL_I2C_SetOwnAddress2(I2C2, 0, LL_I2C_OWNADDRESS2_NOMASK);
  LL_I2C_DisableOwnAddress2(I2C2);
  LL_I2C_DisableGeneralCall(I2C2);
  LL_I2C_EnableClockStretching(I2C2);

  /** I2C Fast mode Plus enable
  */
  LL_SYSCFG_EnableFastModePlus(LL_SYSCFG_I2C_FASTMODEPLUS_I2C2);
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/* USER CODE BEGIN 1 */

void I2C2_DMA_Config_(uint8_t *pData, uint16_t Size)
{
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_1, LL_I2C_DMA_GetRegAddr(I2C2, LL_I2C_DMA_REG_DATA_RECEIVE));
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMAMUX_REQ_I2C2_RX);
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)pData);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, Size);
	
	LL_I2C_EnableDMAReq_RX(I2C2);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
	
	//LL_I2C_EnableIT_STOP(I2C2);
}

void I2Cx_Mem_Read_DMA(I2C_TypeDef *I2Cx, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint16_t Size, uint32_t Timeout)
{
	uint32_t i2c2_mtime_start = TIM17_mCounter;
	uint32_t i2c2_utime_start = TIM17->CNT;
	uint32_t i2c2_mtime = 0;
	uint32_t i2c2_utime = 0;
	while(!Loop_Is_Timeout_Xms(!LL_I2C_IsActiveFlag_BUSY(I2C2), Timeout))
		;
  LL_I2C_HandleTransfer(I2Cx, DevAddress, MemAddSize, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

	while(!Loop_Is_Timeout_Xms(LL_I2C_IsActiveFlag_TXE(I2C2), Timeout))
		;
  LL_I2C_TransmitData8(I2Cx, MemAddress);

	while(!Loop_Is_Timeout_Xms(LL_I2C_IsActiveFlag_TC(I2C2), Timeout))
		;
	LL_I2C_HandleTransfer(I2Cx, DevAddress, MemAddSize, Size, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
	LL_I2C_ClearFlag_TXE(I2Cx);
	i2c2_mtime = TIM17_mCounter - i2c2_mtime_start;
	i2c2_utime = TIM17->CNT - i2c2_utime_start;
	//printf("I2C_Mem_Read_DMA:%dms %dus \r\n", i2c2_mtime, i2c2_utime);
	return;
}

//unsigned char I2C_Read(I2C_TypeDef *I2Cx, unsigned char slave_addr, unsigned char reg_addr)//I2C_Read(I2C2, 0x36<<1, 0x0E);
//{
//	unsigned char read_byte = 0;
//	unsigned char data_size = 1;
//	uint8_t i = 0;
//  /* Master Generate Start condition for a write request :              */
//  /*    - to the Slave with a 7-Bit SLAVE_OWN_ADDRESS                   */
//  /*    - with a auto stop condition generation when transmit all bytes */
//  LL_I2C_HandleTransfer(I2Cx, slave_addr, LL_I2C_ADDRSLAVE_7BIT, data_size, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

//  /* Loop until STOP flag is raised  */
//	/* This loop is dangerous when power support is terrrible. */
//  while(!LL_I2C_IsActiveFlag_TC(I2Cx))
//	{
//    /* Check TXIS flag value in ISR register */
//    if(LL_I2C_IsActiveFlag_TXE(I2Cx) && data_size == 1)
//    {
//      /* Write data in Transmit Data register.
//      TXIS flag is cleared by writing data in TXDR register */
//      LL_I2C_TransmitData8(I2Cx, reg_addr);
//			data_size--;
//    }
//		if(Loop_Is_Timeout_Xms(10))
//			break;
//  }
//	LL_I2C_ClearFlag_TXE(I2Cx);

//	data_size = 1;
//	LL_I2C_HandleTransfer(I2Cx, slave_addr, LL_I2C_ADDRSLAVE_7BIT, data_size, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
//  /* Loop until STOP flag is raised  */
//	while(!LL_I2C_IsActiveFlag_STOP(I2Cx))
//	{
//		if(Loop_Is_Timeout_Xms(10))
//			break;
//	}
//	if(LL_I2C_IsActiveFlag_RXNE(I2Cx))
//			read_byte = LL_I2C_ReceiveData8(I2Cx);
//  /* (3) Clear pending flags, Data consistency are checking into Slave process */
//  LL_I2C_ClearFlag_STOP(I2Cx);

//	return read_byte;
//}


/* USER CODE END 1 */
