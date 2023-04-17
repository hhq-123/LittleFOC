/* =================================================================================
File name:       __NI_CONTROLLER_C__
===================================================================================*/
#include "NI_controller.h"

NI_Controller* new_NI(void)
{
	NI_Controller* pObj = NULL;
	uint8_t i = 0;
	pObj = (NI_Controller*)malloc(sizeof(NI_Controller));
	if (pObj == NULL)
	{
		return NULL;
	}
	
	pObj->init = NI_Controller_init;
	pObj->calc = NI_Controller_calc;
	pObj->get_ = NI_Controller_get_;
	pObj->get = NI_Controller_get;
	
	pObj->init(pObj);
	return pObj;
}

void delete_NI(NI_Controller* const pNICObj)
{
	free(pNICObj);
}

void NI_Controller_calc(NI_Controller* const pNICObj, q15_t value, q15_t len)
{
	pNICObj->F_buf[0] = pNICObj->F_buf[1];
	pNICObj->F_buf[1] = pNICObj->F_buf[2];
	pNICObj->F_buf[2] = (q17_15_t)value<<15;
	
	pNICObj->length[pNICObj->channel_] = len;
	
	pNICObj->NI_D_buf[pNICObj->channel_][0] = (pNICObj->F_buf[1]-pNICObj->F_buf[0])/pNICObj->length[pNICObj->channel];
	pNICObj->NI_D_buf[pNICObj->channel_][1] = (pNICObj->F_buf[2]-pNICObj->F_buf[1])/pNICObj->length[pNICObj->channel_];
	pNICObj->NI_D_buf[pNICObj->channel_][2] = (pNICObj->NI_D_buf[pNICObj->channel_][1]-pNICObj->NI_D_buf[pNICObj->channel_][0])
																		/(pNICObj->length[pNICObj->channel]+pNICObj->length[pNICObj->channel_]);
}

q15_t NI_Controller_get_(NI_Controller* const pNICObj, uint8_t channel, uint16_t count)
{	
	q15_t temp = 0;
	
	temp = (pNICObj->NI_D_buf[channel][0]
			 + pNICObj->NI_D_buf[channel][1] * (count + pNICObj->length[pNICObj->channel_])
			 + pNICObj->NI_D_buf[channel][2] * (count + pNICObj->length[pNICObj->channel_]) * count)>>15;

	return temp;
}

q15_t NI_Controller_get(NI_Controller* const pNICObj)
{
	if(pNICObj->cnt>=pNICObj->length[pNICObj->channel])
	{
		pNICObj->cnt = 0;
		pNICObj->channel_ = pNICObj->channel;
		pNICObj->channel = (pNICObj->channel+1)&0x01;
	}
	return pNICObj->get_(	pNICObj,
												pNICObj->channel,
												pNICObj->cnt++);
}
