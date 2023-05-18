#include "time_utils.h"

#define HCLK_Frequency 64

uint32_t Get_sys_time_ms(void)
{
	return LL_Get();
}

uint32_t Get_sys_time_us(void)
{
  uint32_t sys_us = 1000*HAL_GetTick()+(SysTick->LOAD - SysTick->VAL) / HCLK_Frequency;
  return sys_us;
}