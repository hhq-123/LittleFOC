#include "current_sensor.h"

Current_Sensor* new_Current_Sensor(void)
{
	Current_Sensor* pObj = NULL;
	uint8_t i = 0;
	pObj = (Current_Sensor*)malloc(sizeof(Current_Sensor));
	if (pObj == NULL)
	{
		printf("WARN: Current_Sensor initialization failed.\r\n");
		return NULL;
	}
	
	pObj->LPFCtrl[0] = new_LPF_Controller(0x400);
	pObj->LPFCtrl[1] = new_LPF_Controller(0x400);
	
	pObj->init = Current_Sensor_init;
	pObj->startDMA = Current_Sensor_start_DMA;
	pObj->get = Current_Sensor_get;
	
	ADC1_DMA_Config_(pObj->ADC_BUF, 2);
	
	printf("INFO: Current_Sensor initialization succeeded.\r\n");	
	return pObj;	
}

void delete_CS(Current_Sensor* const pCSObj)
{
	free(pCSObj);
}


void Current_Sensor_init(Current_Sensor* const pCSObj, uint16_t Ia, uint16_t Ic)
{
	pCSObj->origin[0] = Ia;
	pCSObj->origin[1] = Ic;
}

void Current_Sensor_start_DMA(Current_Sensor* const pCSObj)
{
	LL_ADC_REG_StartConversion(ADC1); 
}

void Current_Sensor_get(Current_Sensor* const pCSObj, uint16_t theta)
{
	q15_t Sin = arm_sin_q15((q15_t)(theta>>1));
	q15_t Cos = arm_cos_q15((q15_t)(theta>>1));
	
	pCSObj->Ia = -(pCSObj->ADC_BUF[0] - pCSObj->origin[0]);
	pCSObj->Ic = -(pCSObj->ADC_BUF[1] - pCSObj->origin[1]);
	pCSObj->Ib = -pCSObj->Ia - pCSObj->Ic;
	
	pCSObj->Ialpha = pCSObj->Ia;
	pCSObj->Ibeta = my_mult_q15(-pCSObj->Ia - pCSObj->Ic - pCSObj->Ic, 0x49E6);
	
	pCSObj->Id = my_mult_q15(pCSObj->Ialpha, Cos) + my_mult_q15(pCSObj->Ibeta, Sin);
	pCSObj->Iq = my_mult_q15(pCSObj->Ibeta, Cos) - my_mult_q15(pCSObj->Ialpha, Sin);
	
	pCSObj->Id_out = pCSObj->LPFCtrl[0]->calc(pCSObj->LPFCtrl[0], pCSObj->Id);
	pCSObj->Iq_out = pCSObj->LPFCtrl[1]->calc(pCSObj->LPFCtrl[1], pCSObj->Iq);
}

