#include "svpwm_driver.h"

SVPWM_Driver* new_SVPWM_Driver(q15_t Udc)
{
	SVPWM_Driver* pObj = NULL;
	uint8_t i = 0;
	pObj = (SVPWM_Driver*)malloc(sizeof(SVPWM_Driver));
	if (pObj == NULL)
	{
		printf("WARN: SVPWM_Driver initialization failed.\r\n");
		return NULL;
	}
	
	pObj->init = foc_svpwm_init;
	pObj->calc = foc_svpwm_calc;
	pObj->stop = foc_svpwm_stop;
	pObj->write_ = foc_svpwm_write_;
	
	pObj->init(pObj, Udc);
	
	TIM_PWM_Config();
	printf("INFO: SVPWM_Driver initialization succeeded.\r\n");
	return pObj;			
}

void delete_SVPWM_Driver(SVPWM_Driver* const pSDObj)
{
	free(pSDObj);
}

void foc_svpwm_init(SVPWM_Driver* const pSDObj, q15_t Udc)
{
	pSDObj->Udc = Udc;
}

void foc_svpwm_calc(SVPWM_Driver* const pSDObj ,q15_t Ud ,q15_t Uq ,uint16_t theta)
{
	pSDObj->theta = theta;
	pSDObj->Uq = Uq;
	pSDObj->Ud = Ud;
	q15_t Sin = arm_sin_q15((q15_t)(pSDObj->theta>>1));
	q15_t Cos = arm_cos_q15((q15_t)(pSDObj->theta>>1));

	pSDObj->Ualpha = my_mult_q15(pSDObj->Ud, Cos)- my_mult_q15(pSDObj->Uq, Sin);
	pSDObj->Ubeta  = my_mult_q15(pSDObj->Ud, Sin)+ my_mult_q15(pSDObj->Uq, Cos);

	pSDObj->tmp1= pSDObj->Ubeta;
	pSDObj->tmp2= my_mult_q15(pSDObj->Ubeta, 0x4000)+ my_mult_q15(pSDObj->Ualpha, 0x6ED9);
	pSDObj->tmp3= pSDObj->tmp2- pSDObj->tmp1;		

	pSDObj->VecSector=3;																
	pSDObj->VecSector=(pSDObj->tmp2> 0)?( pSDObj->VecSector-1):pSDObj->VecSector;						
	pSDObj->VecSector=(pSDObj->tmp3> 0)?( pSDObj->VecSector-1):pSDObj->VecSector;						
	pSDObj->VecSector=(pSDObj->tmp1< 0)?(7-pSDObj->VecSector) :pSDObj->VecSector;						
																				
	if(pSDObj->VecSector==1 || pSDObj->VecSector==4)
	{
		pSDObj->Ta = pSDObj->tmp2;												
   	pSDObj->Tb = pSDObj->tmp1-pSDObj->tmp3;
   	pSDObj->Tc = -pSDObj->tmp2;
	}
	else if(pSDObj->VecSector==2 || pSDObj->VecSector==5)
	{
		pSDObj->Ta = pSDObj->tmp3 + pSDObj->tmp2;
    pSDObj->Tb = pSDObj->tmp1;
    pSDObj->Tc = -pSDObj->tmp1;
	}else
	{
		pSDObj->Ta = pSDObj->tmp3;
		pSDObj->Tb = -pSDObj->tmp3;												
    pSDObj->Tc = -(pSDObj->tmp1+pSDObj->tmp2);
	}	
}

void foc_svpwm_stop(SVPWM_Driver* const pSDObj)
{
	LL_TIM_OC_SetCompareCH1(TIM3, 0);
	LL_TIM_OC_SetCompareCH2(TIM3, 0);
	LL_TIM_OC_SetCompareCH3(TIM3, 0);
}

void foc_svpwm_write_(SVPWM_Driver* const pSDObj, uint16_t pwm_t)
{
	short tmp = 0;
	//tmp = my_mult_q15(my_mult_q15((pSDObj->Ta), pSDObj->Udc), pwm_t) ;
	tmp = ((q31_t)my_mult_q15((pSDObj->Ta), pSDObj->Udc) * pwm_t)>>15 ;
	LL_TIM_OC_SetCompareCH1(TIM3, 199-((tmp + pwm_t)>>1));
	
	tmp = ((q31_t)my_mult_q15((pSDObj->Tb), pSDObj->Udc) * pwm_t)>>15 ;
	LL_TIM_OC_SetCompareCH2(TIM3, 199-((tmp + pwm_t)>>1));
	
	tmp = ((q31_t)my_mult_q15((pSDObj->Tc), pSDObj->Udc) * pwm_t)>>15 ;
	LL_TIM_OC_SetCompareCH3(TIM3, 199-((tmp + pwm_t)>>1));
}