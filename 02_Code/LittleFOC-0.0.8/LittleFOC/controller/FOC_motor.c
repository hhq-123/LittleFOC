#include "FOC_motor.h"

#include "LittleFOC.h"

#include "tim.h"
#include "usart.h"
#include "i2c.h"
#include "adc.h"

/**AS5600**/
PMSM_Motor *pmsm_motor;
float ia, ib, ic;

extern uint32_t time;

void FOC_Init(void)
{
	TIM17_Config();
	USART_DMA_Config();
	
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2);
	
	pmsm_motor = new_PMSM_Motor();
	pmsm_motor->init(pmsm_motor);
	pmsm_motor->start(pmsm_motor);
	
	printf("FOC initialization succeeded.\r\n");
}

void foc_MS_DMAstart(void)
{
	time = TIM17->CNT;
	pmsm_motor->MagSensor->startDMA(pmsm_motor->MagSensor);
}
	

void foc_controller(void)
{
	pmsm_motor->crtl(pmsm_motor);
}

void foc_communication(uint8_t* buffer, uint8_t length)
{
	if(buffer[0]==0x55)
		if(buffer[1]==FOC_ID)
			pmsm_motor->port(pmsm_motor, &buffer[3], buffer[2]-3);
}

void position_test(void)
{
	
	pmsm_motor->PIDCtrl_P->set(pmsm_motor->PIDCtrl_P, 0x0000);
	LL_mDelay(2000);
	pmsm_motor->PIDCtrl_P->set(pmsm_motor->PIDCtrl_P, 0x4000);
	LL_mDelay(2000);	
	pmsm_motor->PIDCtrl_P->set(pmsm_motor->PIDCtrl_P, 0x8000);
	LL_mDelay(2000);
	pmsm_motor->PIDCtrl_P->set(pmsm_motor->PIDCtrl_P, 0xC000);
	LL_mDelay(2000);
}