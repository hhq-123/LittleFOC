#include "pmsm_motor.h"
#include "LittleFOC.h"

uint32_t global_time_stamp = 0;

char uart_port_buf[256];
uint32_t time;

PMSM_Motor* new_PMSM_Motor(void)
{
	PMSM_Motor* pObj = NULL;
	uint8_t i = 0;
	pObj = (PMSM_Motor*)malloc(sizeof(PMSM_Motor));
	if (pObj == NULL)
	{
		printf("WARN: PMSM_Motor initialization failed.\r\n");
		return NULL;
	}
	
	pObj->MagSensor = new_Magnetic_Sensor(SPI1, GPIOA, LL_GPIO_PIN_0);
	pObj->CurSensor = new_Current_Sensor();
	pObj->SVPWM = new_SVPWM_Driver(0x7fff);
	pObj->PIDCtrl_I[0] = new_PID_Controller();
	pObj->PIDCtrl_I[1] = new_PID_Controller();
	
	pObj->PIDCtrl_V = new_PID_Controller();
	
	pObj->PIDCtrl_P_V = new_PID_Controller();
	pObj->PIDCtrl_P_I = new_PID_Controller();
	
	pObj->NICtrl_P = new_NI_Controller();
	
	pObj->init = pmsm_motor_init;
	pObj->start = pmsm_motor_start;
	pObj->crtl = pmsm_motor_ctrl;
	pObj->crtl_P = pmsm_motor_ctrl_position;
	pObj->sen = pmsm_motor_sensor_get;
	pObj->run = pmsm_motor_run;
	pObj->port = pmsm_motor_port;
	pObj->init(pObj);
	printf("INFO: PMSM_Motor initialization succeeded.\r\n");
	return pObj;				
}

void pmsm_motor_init(PMSM_Motor* const pPMObj)
{
	uint16_t theta;
	
	float ka_i, kb_i,ka_vi, ka_v,kb_vi, kb_v;

	pPMObj->SVPWM->stop(pPMObj->SVPWM);
	LL_mDelay(10);
	pPMObj->CurSensor->init(pPMObj->CurSensor, pPMObj->CurSensor->ADC_BUF[0], pPMObj->CurSensor->ADC_BUF[1]);
	//printf("ADC:%d %d;", pPMObj->CurSensor->ADC_BUF[0], pPMObj->CurSensor->ADC_BUF[1]);	
	
//	theta = 0;
//	pPMObj->SVPWM->calc(pPMObj->SVPWM, 0x7fff, 0x0000, theta);
//	pPMObj->SVPWM->write(pPMObj->SVPWM);
//	theta = pPMObj->MagSensor->get(pPMObj->MagSensor);
//	theta = pPMObj->MagSensor->get(pPMObj->MagSensor);
//	theta = pPMObj->MagSensor->get(pPMObj->MagSensor);
//	theta = pPMObj->MagSensor->get(pPMObj->MagSensor);
//	theta = pPMObj->MagSensor->get(pPMObj->MagSensor);
//	theta = pPMObj->MagSensor->get(pPMObj->MagSensor);
//	theta = pPMObj->MagSensor->get(pPMObj->MagSensor);
//	theta = pPMObj->MagSensor->get(pPMObj->MagSensor);
//	LL_mDelay(1);
//	pPMObj->SVPWM->stop(pPMObj->SVPWM);
//	printf("theta %d\r\n", theta);
	
	pPMObj->MagSensor->init(pPMObj->MagSensor, 16660);

	LL_mDelay(1);	
	theta = pPMObj->MagSensor->get(pPMObj->MagSensor);
	
  /*电流环初始化*/	
	ka_i = 0.25 * Motor_L * f_base * I_base / U_base;
	ka_vi = 0.25 * Motor_L * fv_base * I_base / U_base;
	kb_i = Motor_R * T_base / (2.0 * PI * Motor_L );
	kb_vi = Motor_R * Tv_base / (2.0 * PI * Motor_L );
	
	pPMObj->PIDCtrl_I[0]->init(pPMObj->PIDCtrl_I[0], ka_i, ka_i*kb_i, 0, -0.7, 0.7);
	pPMObj->PIDCtrl_I[1]->init(pPMObj->PIDCtrl_I[1], ka_i, ka_i*kb_i, 0, -0.7, 0.7);
	
	pPMObj->PIDCtrl_I[0]->set(pPMObj->PIDCtrl_I[0], my_float_to_q15(0.0));
	pPMObj->PIDCtrl_I[1]->set(pPMObj->PIDCtrl_I[1], my_float_to_q15(0.015));	
	/*速度环初始化*/	
	kb_v = ka_vi /(Motor_L * Motor_delta * Motor_delta);
	ka_v = Motor_delta * kb_vi /K_PL_J;
	
	pPMObj->PIDCtrl_V->init(pPMObj->PIDCtrl_V, ka_v, ka_v*kb_v, 0 , -0.5, 0.5);
	//pPMObj->PIDCtrl_V->init(pPMObj->PIDCtrl_V, 0.025, 0.2, 0, -0.5, 0.5);
	pPMObj->PIDCtrl_V->set(pPMObj->PIDCtrl_V, my_float_to_q15(-0.01));

	/*位置环初始化*/

	//pPMObj->PIDCtrl_P_V->init(pPMObj->PIDCtrl_P_V, 1, 0.001 , 20, -0.4, 0.4);
	pPMObj->PIDCtrl_P_V->init(pPMObj->PIDCtrl_P_V, 0.5, 0.005 , 20.0, -0.05, 0.05);
	pPMObj->PIDCtrl_P_V->set(pPMObj->PIDCtrl_P_V, 0x0000);
	
//	pPMObj->PIDCtrl_P_I->init(pPMObj->PIDCtrl_P_I, 50.0, 0.0 , 0.0, -0.5, 0.5);
//	pPMObj->PIDCtrl_P_I->set(pPMObj->PIDCtrl_P_I, 0x0000);	
	//pPMObj->PIDCtrl_P_V->set(pPMObj->PIDCtrl_P_V, theta);
	
	pPMObj->NICtrl_P->init(pPMObj->NICtrl_P);
	
	pPMObj->NICtrl_P->calc(pPMObj->NICtrl_P, theta, 1, pPMObj->state&0x01);
	
	pPMObj->state = CMD_NICrtl_P_CW;
	
}


void pmsm_motor_start(PMSM_Motor* const pPMObj)
{
	LL_TIM_EnableIT_UPDATE(TIM1);
}

void delete_PMSM_Motor(PMSM_Motor* const pPMObj)
{
	delete_SVPWM_Driver(pPMObj->SVPWM);
	delete_Magnetic_Sensor(pPMObj->MagSensor);
	free(pPMObj);
}

void pmsm_motor_port(PMSM_Motor* const pPMObj, uint8_t* data, uint8_t length)//通讯控制接口
{
	uint32_t value[4];
	q15_t t = 0;
	pPMObj->state = data[0];
	switch(pPMObj->state)
	{
		case 0x01: //停机
			
		break;
		case CMD_NICrtl_P_CW: //牛插位置控制器
		case CMD_NICrtl_P_CCW:
			
			sscanf((char*)&data[1], "%x %x", &value[0], &value[1]);
			pPMObj->NICtrl_P->calc(pPMObj->NICtrl_P, value[0], value[1], pPMObj->state&0x01);
		break;
		default:
		break;
	}
}

void pmsm_motor_ctrl(PMSM_Motor* const pPMObj)
{
	switch(pPMObj->state)
	{
		case CMD_NICrtl_P_CW:
		case CMD_NICrtl_P_CCW:
			//pPMObj->crtl_P(pPMObj,pPMObj->NICtrl_P->get(pPMObj->NICtrl_P));
		pPMObj->crtl_P(pPMObj,pPMObj->Position);
		break;
	}
}

void pmsm_motor_ctrl_position(PMSM_Motor* const pPMObj, q15_t position)
{
	static uint8_t v_counter, p_v_counter, p_i_counter;
	q15_t temp;
//	time = TIM17->CNT;
	pPMObj->sen(pPMObj);
	pPMObj->PIDCtrl_P_V->set(pPMObj->PIDCtrl_P_V, position);
	temp = 	position - pPMObj->MagSensor->angle;

		if(p_v_counter++>3)
		{
			p_v_counter=0;
			/*位置环计算*/
			if(temp>0x1000)
				pPMObj->Velocity = 0x0100;
			else if(temp<-0x1000)
				pPMObj->Velocity = -0x0100;
			else
				pPMObj->Velocity = pPMObj->PIDCtrl_P_V->calc(pPMObj->PIDCtrl_P_V, pPMObj->MagSensor->angle);
				pPMObj->PIDCtrl_V->set(pPMObj->PIDCtrl_V, pPMObj->Velocity);
		}

		if(v_counter++>3)
		{
			//
			v_counter=0;
			/*速度环计算*/

			pPMObj->Iq = pPMObj->PIDCtrl_V->calc(pPMObj->PIDCtrl_V, pPMObj->MagSensor->getV(pPMObj->MagSensor));
			pPMObj->PIDCtrl_I[1]->set(pPMObj->PIDCtrl_I[1], pPMObj->Iq);
		}

	
	//pPMObj->Iq = -pPMObj->PIDCtrl_P_V->calc(pPMObj->PIDCtrl_P_V, pPMObj->MagSensor->angle);
	
	
	
	
		
	pPMObj->run(pPMObj);

}

void pmsm_motor_sensor_get(PMSM_Motor* const pPMObj)
{
	/*传感器数据采集*/
	pPMObj->angle_rotor = pPMObj->MagSensor->get(pPMObj->MagSensor) * 7;
	pPMObj->angle_stator = pPMObj->angle_rotor;
	pPMObj->CurSensor->get(pPMObj->CurSensor, pPMObj->angle_stator);
}

void pmsm_motor_run(PMSM_Motor* const pPMObj)
{
	/*电流环计算*/
	pPMObj->Ud = pPMObj->PIDCtrl_I[0]->calc(pPMObj->PIDCtrl_I[0], pPMObj->CurSensor->Id_out);
	pPMObj->Uq = pPMObj->PIDCtrl_I[1]->calc(pPMObj->PIDCtrl_I[1], pPMObj->CurSensor->Iq_out);
	
	pPMObj->SVPWM->calc(pPMObj->SVPWM, 
											pPMObj->Ud, 
											pPMObj->Uq,
											pPMObj->angle_stator);

	//pPMObj->angle_stator+=10;
	pPMObj->SVPWM->write_(pPMObj->SVPWM, 195);
	
	uint8_t len = 0;
//	time = TIM17->CNT - time;
	//printf("%dus\r\n", time);
	//len = sprintf(uart_port_buf, "$%+.2f %+.2f;", my_q15_to_float(pPMObj->Uq), my_q15_to_float(pPMObj->Ud));
//	len = sprintf(uart_port_buf, "$%+.4f %+.4f;", my_q15_to_float(pPMObj->CurSensor->Ia), my_q15_to_float(pPMObj->CurSensor->Ic));
	//len = sprintf(uart_port_buf, "$%+.2f %+.2f;",my_q15_to_float(pPMObj->CurSensor->Id_out), my_q15_to_float(pPMObj->CurSensor->Iq_out));
//	//len = sprintf(uart_port_buf, "$%+.2f %+.2f %+.2f %d;", my_q15_to_float(pPMObj->Position), my_q15_to_float(pPMObj->MagSensor->angle), my_q15_to_float(pPMObj->Velocity), pPMObj->PIDCtrl_P_V->delta_Un);
	//len = sprintf(uart_port_buf, "$%+.2f %+.2f;", my_q15_to_float(pPMObj->CurSensor->Ia), my_q15_to_float(pPMObj->CurSensor->Ic));
	//len = sprintf(uart_port_buf, "$%+.2f %+.2f;", my_q15_to_float(pPMObj->CurSensor->Iq_out), my_q15_to_float(pPMObj->CurSensor->Id_out));
	len = sprintf(uart_port_buf, "$%.2f %.2f %.5f;", my_q15_to_float(pPMObj->CurSensor->Iq_out), my_q15_to_float(pPMObj->MagSensor->angle), my_q15_to_float(pPMObj->MagSensor->velocity_out));
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)uart_port_buf);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, len);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
}

void pmsm_motor_sensor_state(PMSM_Motor* const pPMObj)
{
	uint8_t len = 0;
	
	len = sprintf(uart_port_buf, "$%+.2f %+.2f;", my_q15_to_float(pPMObj->CurSensor->Ia), my_q15_to_float(pPMObj->CurSensor->Ic));
	//len = sprintf(uart_port_buf, "$%+.2f %+.2f;", my_q15_to_float(pPMObj->CurSensor->Iq), my_q15_to_float(pPMObj->CurSensor->Id));
	//len = sprintf(uart_port_buf, "$%.2f %.2f %.5f;", my_q15_to_float(pPMObj->CurSensor->Iq_out), my_q15_to_float(pPMObj->MagSensor->angle), my_q15_to_float(pPMObj->MagSensor->velocity_out));
	//len = sprintf(uart_port_buf, "$%+.2f %+.2f;", my_q15_to_float(pPMObj->CurSensor->Iq_out), my_q15_to_float(pPMObj->CurSensor->Id_out));
	//len = sprintf(uart_port_buf, "$%+.2f %+.2f;", my_q15_to_float(Ud), my_q15_to_float(pPMObj->CurSensor->Id_out));
	//len = sprintf(uart_port_buf, "$%+.2f;", my_q15_to_float(pPMObj->PIDCtrl_I[0]->setValue));
	//len = sprintf(uart_port_buf, "$%d %d %.2f;", pPMObj->MagSensor->angle, V, my_q15_to_float(pPMObj->MagSensor->velocity_out*100));
	//len = sprintf(uart_port_buf, "$%+.2f %+.2f %+.2f %+.2f;", my_q15_to_float(Uq), my_q15_to_float(Ud), my_q15_to_float(pPMObj->CurSensor->Id_out), my_q15_to_float(pPMObj->CurSensor->Iq_out));
	//len = sprintf(uart_port_buf, "$%d %+.2f %+.2f;", pPMObj->PIDCtrl_I[1]->delta_Un, my_q15_to_float(Uq), my_q15_to_float(pPMObj->CurSensor->Iq_out));
	//len = sprintf(uart_port_buf, "$%d %d %d %d;", pPMObj->PIDCtrl_I[1]->kn, pPMObj->PIDCtrl_I[1]->errorn_1, pPMObj->PIDCtrl_I[1]->delta_Un, Uq);
	//len = sprintf(uart_port_buf, "$%d %d;", Uq, pPMObj->CurSensor->Iq_out);
	//len = sprintf(uart_port_buf, "$%.3f %.3f %.3f %.f;", my_q15_to_float(pPMObj->CurSensor->Id_out), my_q15_to_float(pPMObj->CurSensor->Iq_out), my_q15_to_float(Ud), my_q15_to_float(Uq));
	//len = sprintf(uart_port_buf, "$%d %d %d %d;", pPMObj->PIDCtrl_P_V->errorn_1, pPMObj->PIDCtrl_P_V->errorn_2, pPMObj->PIDCtrl_P_V->delta_Un, Motor_V);
	//len = sprintf(uart_port_buf, "$%.5f;", my_q15_to_float(pPMObj->MagSensor->velocity_out*100));
	//len = sprintf(uart_port_buf, "$%d %d %d;", pPMObj->MagSensor->angle, V, pPMObj->PIDCtrl_P_V->setValue);
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)uart_port_buf);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, len);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
}