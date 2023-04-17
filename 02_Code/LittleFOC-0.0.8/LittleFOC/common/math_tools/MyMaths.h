/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MYMATHS_H
#define __MYMATHS_H

#ifdef __cplusplus
extern "C" {
#endif

#define M_2_PI 		6.28318530718
#define M_PI 		3.14159265359
#define M_PI_2 	1.57079632679
#define M_PI_4	0.785398163397

/* Includes ------------------------------------------------------------------*/

#include "stm32g0xx.h"
#include "arm_math.h"

typedef int16_t q6_10_t;
typedef int32_t q22_10_t;
typedef int32_t q17_15_t;
typedef int16_t q14_t;

q22_10_t my_mult_q22_10_and_q6_10(q22_10_t SrcA, q6_10_t SrcB);
q17_15_t my_mult_q17_15_t_and_q15(q17_15_t SrcA, q15_t SrcB);
q14_t my_mult_q14(q14_t SrcA, q14_t SrcB);
q15_t my_mult_q15(q15_t SrcA, q15_t SrcB);
q22_10_t my_float_to_q22_10_t(float Src);
q17_15_t my_float_to_q17_15_t(float Src);
q15_t my_float_to_q15(float Src);
float my_q15_to_float(q15_t Src);


#endif