/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FOC_H
#define __FOC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "spi.h"
#include "adc.h"
#include "tim.h"


void FOC_Init(void);
void foc_controller(void);
void foc_communication(uint8_t* buffer, uint8_t length);
void position_test(void);

#endif