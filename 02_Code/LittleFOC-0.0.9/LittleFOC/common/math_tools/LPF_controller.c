/* =================================================================================
File name:       __LPF_CONTROLLER_C
===================================================================================*/
#include "LPF_controller.h"

LPF_Controller* new_LPF_Controller(q15_t tau)
{
	LPF_Controller* pObj = NULL;
	pObj = (LPF_Controller*)malloc(sizeof(LPF_Controller));
	if (pObj == NULL)
	{
		printf("WARN: LPF_Controller initialization failed.\r\n");
		return NULL;
	}
	
	pObj->init = LPF_controller_init;
	pObj->calc = LPF_controller_calc;
	
	pObj->output = 0;
	pObj->init(pObj, tau);
	printf("INFO: LPF_Controller initialization succeeded.\r\n");
	return pObj;			
}

void delete_LPF_Controller(LPF_Controller* const pLPFCObj)
{
	free(pLPFCObj);
}

void LPF_controller_init(LPF_Controller* const pLPFCObj, q15_t tau)
{
	pLPFCObj->tau = tau;
}

q15_t LPF_controller_calc(LPF_Controller* const pLPFCObj, q15_t value)
{
	pLPFCObj->output = my_mult_q15(value, pLPFCObj->tau) + my_mult_q15(pLPFCObj->output, 0x7fff - pLPFCObj->tau);
	return pLPFCObj->output;
}
