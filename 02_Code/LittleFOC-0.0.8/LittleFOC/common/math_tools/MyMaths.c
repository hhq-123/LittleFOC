#include "MyMaths.h"

/* includes ------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/

/* 取0~90度的结果，其他角度可以通过对称性和周期性推算出 */
#define DBLTOFIX(fix_t, fracbits, x) \
    fix_t(((x) * (double)((fix_t)(1) << (fracbits)))))
q22_10_t my_mult_q22_10_and_q6_10(q22_10_t SrcA, q6_10_t SrcB)
{
	return (SrcA * SrcB)>>10;
}

q17_15_t my_mult_q17_15_t_and_q15(q17_15_t SrcA, q15_t SrcB)
{
	return ((int64_t)SrcA * SrcB)>>15;
}

q14_t my_mult_q14(q14_t SrcA, q14_t SrcB)
{
	return ((int32_t)SrcA * SrcB)>>14;
}

q15_t my_mult_q15(q15_t SrcA, q15_t SrcB)
{
	return ((q31_t)SrcA * SrcB)>>15;
}

q22_10_t my_float_to_q22_10_t(float Src)
{
	q22_10_t temp;
	uint32_t* ptr = (uint32_t*)&Src;

	int32_t sign_bit = ((*ptr & 0x80000000) >> 31);
	int32_t exponent = ((*ptr & 0x7f800000)>>23) - 127 - 23 + 10;//自行选择对齐的小数点位置
	uint32_t mantissa = (*ptr & 0x007fffff)+0x00800000;
	
	temp = (exponent>0)?(mantissa<<exponent):(mantissa>>-exponent);
	temp = (sign_bit)?-temp:temp;

	return temp ;
}

q17_15_t my_float_to_q17_15_t(float Src)
{
	q22_10_t temp;
	uint32_t* ptr = (uint32_t*)&Src;

	int32_t sign_bit = ((*ptr & 0x80000000) >> 31);
	int32_t exponent = ((*ptr & 0x7f800000)>>23) - 127 - 23 + 15;//自行选择对齐的小数点位置
	uint32_t mantissa = (*ptr & 0x007fffff)+0x00800000;
	
	temp = (exponent>0)?(mantissa<<exponent):(mantissa>>-exponent);
	temp = (sign_bit)?-temp:temp;

	return temp ;
}

q15_t my_float_to_q15(float Src)
{
	q15_t temp;
	arm_float_to_q15(&Src, &temp, 1);
	return temp;
}

float my_q15_to_float(q15_t Src)
{
	float temp;
	arm_q15_to_float(&Src, &temp, 1);
	return temp;
}
