#include "svpwm.h"
#include "tim.h"

sv_mod_t sv = SVGEN_DEFAULTS;

void svpwm_calc(sv_mod_t* v){
	
	sv_mod_t *p = v;
	if(p == NULL){
		//todo error detect
	}
	p->tmp1= p->Ubeta;															
	//p->tmp2= _IQdiv2(p->Ubeta) + (_IQmpy(_IQ(0.866),p->Ualpha));
	p->tmp2= (p->Ubeta / 2) + 0.866*p->Ualpha;
  p->tmp3= p->tmp2 - p->tmp1;													
																				
	p->VecSector=3;																
	p->VecSector=(p->tmp2> 0)?( p->VecSector-1):p->VecSector;						
	p->VecSector=(p->tmp3> 0)?( p->VecSector-1):p->VecSector;						
	p->VecSector=(p->tmp1< 0)?(7-p->VecSector) :p->VecSector;						
																				
	if(p->VecSector==1 || p->VecSector==4){
		p->Ta= p->tmp2;												
    	p->Tb= p->tmp1-p->tmp3;
      	p->Tc=-p->tmp2;
	}
	else if(p->VecSector==2 || p->VecSector==5){
		p->Ta= p->tmp3+p->tmp2;
    	p->Tb= p->tmp1;
      	p->Tc=-p->tmp1;
	}else{
		p->Ta= p->tmp3;
			p->Tb=-p->tmp3;												
      	p->Tc=-(p->tmp1+p->tmp2);
	}
	
//	svpwm_overflow_check(p);
}

sv_regs_mod_t svpwm_get_regs_mod(float vdc,float pwm_t, sv_mod_t* v){
	
	/**		
	// Convert the unsigned GLOBAL_Q format (ranged (0,1)) -> signed GLOBAL_Q format (ranged (-1,1))
	v->Ta = _IQmpy(_IQ(2.0),(v->Ta-_IQ(0.5)));
	v->Tb = _IQmpy(_IQ(2.0),(v->Tb-_IQ(0.5)));
	v->Tc = _IQmpy(_IQ(2.0),(v->Tc-_IQ(0.5)));
	这三条语句实现了占空比由(0,1)到(-1，1)的转换
	*/	
	
	/**
	(-1，1)到(0,1)的转换
	*/
	sv_regs_mod_t sv_regs;
	float tmp = 0;
	tmp = v->Ta * pwm_t * vdc;
	//sv_regs.ccr1 = (uint16_t)((tmp >> 1) + (pwm_t>> 1));
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,(uint8_t)((tmp / 2) + (pwm_t / 2)));
	printf("ua,%f\n",tmp/180);
	tmp = v->Tb * pwm_t * vdc;
	//sv_regs.ccr2 = (uint16_t)((tmp >> 1) + (pwm_t>> 1));
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,(uint8_t)((tmp / 2) + (pwm_t / 2)));
	printf("ub,%f\n",tmp/180);
	tmp = v->Tc * pwm_t * vdc;
	//sv_regs.ccr3 = (uint16_t)((tmp >> +1) + (pwm_t>> 1));
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,(uint8_t)((tmp / 2) + (pwm_t / 2)));
	printf("uc,%f\n",tmp/180);
	return sv_regs;
}