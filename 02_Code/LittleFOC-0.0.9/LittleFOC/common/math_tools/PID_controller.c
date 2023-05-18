/* =================================================================================
File name:       __PID_CONTROLLER_C__
===================================================================================*/
#include "PID_controller.h"

PID_Controller* new_PID_Controller(void)
{
	PID_Controller* pObj = NULL;
	pObj = (PID_Controller*)malloc(sizeof(PID_Controller));
	if (pObj == NULL)
	{
		printf("WARN: PID_Controller initialization failed.\r\n");
		return NULL;
	}
	
	pObj->init = PID_Controller_init;
	pObj->calc = PID_Controller_calc;
	pObj->set = PID_Controller_set;
	
	pObj->Un = 0;
	printf("INFO: PID_Controller initialization succeeded.\r\n");
	return pObj;			
}

void delete_PID_Controller(PID_Controller* const pPIDCObj)
{
	free(pPIDCObj);
}

void PID_Controller_init(PID_Controller* const pPIDCObj, float kp, float ki, float kd, float limit1, float limit2)
{
	pPIDCObj->setValue = 0;
	
	pPIDCObj->kp = my_float_to_q17_15_t(kp);
	pPIDCObj->ki = my_float_to_q17_15_t(ki);
	pPIDCObj->kd = my_float_to_q17_15_t(kd);
	
	pPIDCObj->kn = pPIDCObj->kp + pPIDCObj->ki + pPIDCObj->kd;
	pPIDCObj->kn_1 = -pPIDCObj->kp - 2 * pPIDCObj->kd;
	pPIDCObj->kn_2 = pPIDCObj->kd;

	pPIDCObj->limit2 = my_float_to_q15(limit2);
//	if(pPIDCObj->limit2 > 0x3fff)
//	{
//		pPIDCObj->limit2 = 0x3fff;
//		printf("Warning: The value of limit2 is too large!\r\n");
//	}
	
	pPIDCObj->limit1 = my_float_to_q15(limit1);
//	if(pPIDCObj->limit1 < -0x3fff)
//	{
//		pPIDCObj->limit1 = -0x3fff;
//		printf("Warning: The value of limit1 is too large!\r\n");
//	}
	
	pPIDCObj->delta_Un = 0;
	pPIDCObj->errorn_1 = 0;
	pPIDCObj->errorn_2 = 0;
}
 
#define limit 0x800

q15_t PID_Controller_calc(PID_Controller* const pPIDCObj, q15_t value)
{
	q15_t errorn =0;
	q17_15_t delta_Un_;
 
	errorn = (pPIDCObj->setValue-value);
	
	delta_Un_	= my_mult_q17_15_t_and_q15(pPIDCObj->kn, errorn)
						+ my_mult_q17_15_t_and_q15(pPIDCObj->kn_1, pPIDCObj->errorn_1)
						+ my_mult_q17_15_t_and_q15(pPIDCObj->kn_2, pPIDCObj->errorn_2);
	
	pPIDCObj->errorn_2 = pPIDCObj->errorn_1;
	pPIDCObj->errorn_1 = errorn;
	
//	if(delta_Un_ > limit)
//		delta_Un_ = limit;
//	else if(delta_Un_ < -limit)
//		delta_Un_ = -limit;

	pPIDCObj->delta_Un = delta_Un_;
	//printf("%d %d %d\r\n", delta_Un_, pPIDCObj->errorn_1, pPIDCObj->errorn_2);
	pPIDCObj->Un += pPIDCObj->delta_Un; 
	
	pPIDCObj->output = pPIDCObj->Un;
	
	if(pPIDCObj->output > pPIDCObj->limit2)
		pPIDCObj->output = pPIDCObj->limit2;
	else if(pPIDCObj->output < pPIDCObj->limit1)
		pPIDCObj->output = pPIDCObj->limit1;
	
	return pPIDCObj->output;
}

void PID_Controller_set(PID_Controller* const pPIDCObj, q15_t value)
{
	pPIDCObj->setValue = value;
}
