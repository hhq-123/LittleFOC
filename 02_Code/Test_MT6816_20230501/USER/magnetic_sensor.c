/* =================================================================================
File name:       __MAGNETIC_SENEOR_C
magnetic_sensor: MT6816
===================================================================================*/
#include "magnetic_sensor.h"
#include "spi.h"
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
	
	//pObj->LPFCtrl = new_LPF_Controller(0x6000);
	
	pObj->init = Magnetic_Sensor_init;
	pObj->start = Magnetic_Sensor_start;
	pObj->get = Magnetic_Sensor_get;
	
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
	uint32_t time = 0;
	time = TIM17->CNT;
	pMSObj->angle_n = pMSObj->angle;//数据暂存
	pMSObj->start(pMSObj);
	pMSObj->angle = (pMSObj->Angle_BUF-pMSObj->origin);//arm_sin定义域为0-0x7FFF，映射到0-2pi
	pMSObj->velocity = (pMSObj->angle - pMSObj->angle_n);
	//pMSObj->velocity_out = pMSObj->LPFCtrl->calc(pMSObj->LPFCtrl, pMSObj->velocity);
	time = TIM17->CNT - time;
	printf("%dus\r\n", time);
	return pMSObj->angle;
}

