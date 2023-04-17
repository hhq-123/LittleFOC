/* =================================================================================
File name:       __MAGNETIC_SENEOR_H
===================================================================================*/
#include "magnetic_sensor.h"

Magnetic_Sensor* new_Magnetic_Sensor__(I2C_TypeDef *I2Cx, 
	uint16_t DevAddress, uint16_t MemAddress)
{
	Magnetic_Sensor* pObj = NULL;
	uint8_t i = 0;
	pObj = (Magnetic_Sensor*)malloc(sizeof(Magnetic_Sensor));
	if (pObj == NULL)
	{
		printf("WARN: Magnetic_Sensor initialization failed.\r\n");
		return NULL;
	}
	
	pObj->I2Cx = I2Cx;
	pObj->DevAddress = DevAddress;
	pObj->MemAddress = MemAddress;
	
	pObj->LPFCtrl = new_LPF_Controller(0x6000);
	
	pObj->init = Magnetic_Sensor_init;
	pObj->startDMA = Magnetic_Sensor_start_DMA;
	pObj->get = Magnetic_Sensor_get;
	
	I2C2_DMA_Config_(pObj->Magnetic_Sensor_BUF, 2);
	
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

void Magnetic_Sensor_start_DMA(Magnetic_Sensor* const pMSObj)
{
	I2Cx_Mem_Read_DMA(pMSObj->I2Cx, pMSObj->DevAddress, pMSObj->MemAddress, LL_I2C_ADDRSLAVE_7BIT, 2, 10);
}

uint16_t Magnetic_Sensor_get(Magnetic_Sensor* const pMSObj)
{
	pMSObj->angle_n = pMSObj->angle;//数据暂存
	pMSObj->Angle_BUF = ((uint16_t)pMSObj->Magnetic_Sensor_BUF[0]<<8) + pMSObj->Magnetic_Sensor_BUF[1];
	pMSObj->angle = ((pMSObj->Angle_BUF-pMSObj->origin)<<4);//arm_sin定义域为0-0x7FFF，映射到0-2pi
	pMSObj->velocity = (pMSObj->angle - pMSObj->angle_n);
	pMSObj->velocity_out = pMSObj->LPFCtrl->calc(pMSObj->LPFCtrl, pMSObj->velocity);
	return pMSObj->angle;
}

