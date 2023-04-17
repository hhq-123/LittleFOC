/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PID_CONTROLLER_H__
#define __PID_CONTROLLER_H__
/* =================================================================================
File name:       __PID_CONTROLLER_H__
改编自https://blog.csdn.net/wind4study/article/details/45116281
===================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

typedef struct PID_Controller_ PID_Controller;

typedef void (*PIDCfptrInit)(PID_Controller*, float, float, float, float, float);
typedef q15_t (*PIDCfptrCalc)(PID_Controller*, q15_t);
typedef void (*PIDCfptrSet)(PID_Controller*, q15_t);

struct PID_Controller_
{
	uint8_t ID[8];
	
	q17_15_t kp;
	q17_15_t ki;
	q17_15_t kd;
	
	q15_t limit1;
	q15_t limit2;
	
	q17_15_t kn;
	q17_15_t kn_1;
	q17_15_t kn_2;

	q15_t setValue;
	
	q15_t errorn_1;
	q15_t errorn_2;
	
	q15_t delta_Un;
	q15_t Un;
	q15_t output;
	
	//interface for function
	PIDCfptrInit init;
	PIDCfptrCalc calc;
	PIDCfptrSet set;
};
 
PID_Controller* new_PID_Controller(void);
void delete_PID_Controller(PID_Controller* const pPIDCObj);

void PID_Controller_init(PID_Controller* const pPIDCObj, float kp, float ki, float kd, float limit1, float limit2);
q15_t PID_Controller_calc(PID_Controller* const pPIDCObj, q15_t value);
void PID_Controller_set(PID_Controller* const pPIDCObj, q15_t value);

#endif