/* =================================================================================
File name:       __MAGNETIC_SENEOR_C
magnetic_sensor: MT6816
===================================================================================*/
#include "magnetic_sensor.h"

Magnetic_Sensor* new_Magnetic_Sensor(SPI_TypeDef *SPIx, 
	GPIO_TypeDef* CSN_GPIOx, uint32_t CSN_PinMask)
{
	Magnetic_Sensor* pObj = NULL;
	uint8_t i = 0;
	pObj = (Magnetic_Sensor*)malloc(sizeof(Magnetic_Sensor));
	if (pObj == NULL)
	{
		printf("WARN: Magnetic_Sensor initialization failed.\r\n");
		return NULL;
	}
	
	pObj->SPIx = SPIx;
	pObj->CSN_GPIOx = CSN_GPIOx;
	pObj->CSN_PinMask = CSN_PinMask;
	
	pObj->LPFCtrl_V = new_LPF_Controller(0x2000);
	pObj->LPFCtrl_P = new_LPF_Controller(0x2000);
	
	pObj->init = Magnetic_Sensor_init;
	pObj->start = Magnetic_Sensor_start;
	pObj->get = Magnetic_Sensor_get;
	pObj->getV = Magnetic_Sensor_get_V;
	
	LL_SPI_Enable(pObj->SPIx);
	
	printf("INFO: Magnetic_Sensor initialization succeeded.\r\n");
	return pObj;	
}

void delete_Magnetic_Sensor(Magnetic_Sensor* const pMSObj)
{
	free(pMSObj);
}

void Magnetic_Sensor_init(Magnetic_Sensor* const pCSObj, uint16_t origin)
{
	pCSObj->origin = origin;
}

void Magnetic_Sensor_start(Magnetic_Sensor* const pMSObj)
{
	uint8_t counter = 0;
	uint16_t temp = 0;
	
	LL_GPIO_ResetOutputPin(pMSObj->CSN_GPIOx, pMSObj->CSN_PinMask);
	pMSObj->Magnetic_Sensor_BUF[0] = SPIx_ReadData16(pMSObj->SPIx, 0x03, 0xffff)&0x00FF;
	LL_GPIO_SetOutputPin(pMSObj->CSN_GPIOx, pMSObj->CSN_PinMask);
	LL_GPIO_ResetOutputPin(pMSObj->CSN_GPIOx, pMSObj->CSN_PinMask);
	pMSObj->Magnetic_Sensor_BUF[1] = SPIx_ReadData16(pMSObj->SPIx, 0x04, 0xffff)&0x00FF;
	LL_GPIO_SetOutputPin(pMSObj->CSN_GPIOx, pMSObj->CSN_PinMask);
	
	temp = ((uint16_t)pMSObj->Magnetic_Sensor_BUF[0]<<8) + pMSObj->Magnetic_Sensor_BUF[1];
	
	for(uint8_t j=0; j<16; j++)
		if(temp & (0x01 << j))
			counter++;

	if(counter & 0x01)
		pMSObj->pc_flag = 0;
	else
	{
		pMSObj->pc_flag = 1;
	}
	
	if(pMSObj->pc_flag){
		pMSObj->Angle_BUF = temp & 0xfffffffc;
		pMSObj->no_mag_flag = (temp & (0x01 << 1));
	}
	return;
}



uint16_t Magnetic_Sensor_get(Magnetic_Sensor* const pMSObj)
{
	static uint8_t v_i = 0;
	
	
	pMSObj->start(pMSObj);
	pMSObj->angle = (pMSObj->Angle_BUF-pMSObj->origin);//arm_sin定义域为0-0x7FFF，映射到0-2pi
	pMSObj->angle_out = pMSObj->LPFCtrl_P->calc(pMSObj->LPFCtrl_P, pMSObj->angle);	
	return pMSObj->angle_out;
}


uint16_t Magnetic_Sensor_get_V(Magnetic_Sensor* const pMSObj)
{
	pMSObj->velocity = (pMSObj->angle - pMSObj->angle_n);
	pMSObj->velocity_out = pMSObj->LPFCtrl_V->calc(pMSObj->LPFCtrl_V, pMSObj->velocity);	
	pMSObj->angle_n = pMSObj->angle;//数据暂存
	return pMSObj->velocity_out;
}

