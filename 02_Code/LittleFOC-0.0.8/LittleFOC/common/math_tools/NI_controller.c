/* =================================================================================
File name:       __NI_CONTROLLER_C__
===================================================================================*/
#include "NI_controller.h"

NI_Controller* new_NI_Controller(void)
{
	NI_Controller* pObj = NULL;
	uint8_t i = 0;
	pObj = (NI_Controller*)malloc(sizeof(NI_Controller));
	if (pObj == NULL)
	{
		printf("WARN: NI_Controller initialization failed.\r\n");
		return NULL;
	}
	
	pObj->init = NI_Controller_init;
	pObj->calc = NI_Controller_calc;
	pObj->get_ = NI_Controller_get_;
	pObj->get = NI_Controller_get;
	
	pObj->init(pObj);
	printf("INFO: NI_Controller initialization succeeded.\r\n");
	return pObj;
}

void delete_NI_Controller(NI_Controller* const pNICObj)
{
	free(pNICObj);
}

void NI_Controller_init(NI_Controller* const pNICObj)
{
	
	pNICObj->channel = 0;
	pNICObj->channel_ = 1;
	
	pNICObj->length[0][0] = 1;
	pNICObj->length[0][1] = 1;
	
	pNICObj->length[1][0] = 1;
	pNICObj->length[1][1] = 1;
	
}

void NI_Controller_calc(NI_Controller* const pNICObj, q15_t value, q15_t len, uint8_t direction)
{
	
	
	pNICObj->F_buf[0] = pNICObj->F_buf[1];
	pNICObj->F_buf[1] = (q17_15_t)((q15_t)(pNICObj->F_buf[2]>>15))<<15;

	pNICObj->F_buf[2] = (q17_15_t)value<<15;
	
	if((pNICObj->F_buf[2] > pNICObj->F_buf[1])&&(direction==1))//数据上正时钟但方向负时钟
		pNICObj->F_buf[2] -= 0x80000000;
	else if((pNICObj->F_buf[2] < pNICObj->F_buf[1])&&(direction==0))//数据上负时钟但方向正时钟
		pNICObj->F_buf[2] += 0x80000000;

	pNICObj->length[pNICObj->channel_][0] = pNICObj->length[pNICObj->channel][1];
	pNICObj->length[pNICObj->channel_][1] = len;
	
	pNICObj->NI_D_buf[pNICObj->channel_][0] = pNICObj->F_buf[0];
	pNICObj->NI_D_buf[pNICObj->channel_][1] = (pNICObj->F_buf[1]-pNICObj->F_buf[0])/pNICObj->length[pNICObj->channel_][0];
	pNICObj->NI_D_buf[pNICObj->channel_][2] = ((pNICObj->F_buf[2]-pNICObj->F_buf[1])/pNICObj->length[pNICObj->channel_][1]-pNICObj->NI_D_buf[pNICObj->channel_][1])
																		/(pNICObj->length[pNICObj->channel_][0]+pNICObj->length[pNICObj->channel_][1]);
	pNICObj->state |= 0x03;//内容更新 运行
	//printf("F_buf %x %x %x %x\r\n", pNICObj->F_buf[0], pNICObj->F_buf[1], pNICObj->F_buf[2], pNICObj->state);
	//printf("NI_D_buf %x %x %x\r\n", pNICObj->NI_D_buf[pNICObj->channel_][0], pNICObj->NI_D_buf[pNICObj->channel_][1], pNICObj->NI_D_buf[pNICObj->channel_][2]);
}

q15_t NI_Controller_get_(NI_Controller* const pNICObj, uint8_t channel, uint16_t count)
{	
	q17_15_t temp = 0;
	
	temp = ((pNICObj->NI_D_buf[channel][0]
			 + pNICObj->NI_D_buf[channel][1] * (count + pNICObj->length[pNICObj->channel][0])
			 + pNICObj->NI_D_buf[channel][2] * (count + pNICObj->length[pNICObj->channel][0]) * count))>>15;
	return temp;
}

q15_t NI_Controller_get(NI_Controller* const pNICObj)
{
	q15_t temp = 0;
	if((pNICObj->state&0x02) == 0x02)
	{
		temp = pNICObj->get_(	pNICObj,
												pNICObj->channel,
												pNICObj->cnt++);
		if(pNICObj->cnt>=pNICObj->length[pNICObj->channel][1])
		{
			if((pNICObj->state&0x01) == 0x01)//数据更新
			{
				pNICObj->cnt = 0;
				pNICObj->channel_ = pNICObj->channel;
				pNICObj->channel = (pNICObj->channel+1)&0x01;
				//printf("channel change %x %x\r\n", pNICObj->channel, pNICObj->channel_);
				
				pNICObj->state &= 0xfe;//等待下一次数据更新
			}
			else
			{
				pNICObj->F_buf[0] = pNICObj->F_buf[1];
				pNICObj->F_buf[1] = pNICObj->F_buf[2];
				pNICObj->state &= 0xfd;//功能暂停 仅返回当前值
				//printf("temp:%x\r\n", temp);
			}
		}
	}
	else
		temp = pNICObj->F_buf[2]>>15;
	return temp;
}