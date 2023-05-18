/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIME_UTILS_H
#define __TIME_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

uint32_t Get_sys_time_ms(void);
uint32_t Get_sys_time_us(void);

#endif