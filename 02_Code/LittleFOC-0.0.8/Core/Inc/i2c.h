/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define i2c2_rx_len 2
extern uint8_t i2c2_rx_buf[i2c2_rx_len];
/* USER CODE END Private defines */

void MX_I2C2_Init(void);

/* USER CODE BEGIN Prototypes */

void I2C2_DMA_Config_(uint8_t *pData, uint16_t Size);
void I2Cx_Mem_Read_DMA(I2C_TypeDef *I2Cx, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint16_t Size, uint32_t Timeout);
unsigned char I2C_Read(I2C_TypeDef *I2Cx, unsigned char slave_addr, unsigned char reg_addr);

#define I2C2_DMA_Config(void) I2C2_DMA_Config_(i2c2_rx_buf, i2c2_rx_len)

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

