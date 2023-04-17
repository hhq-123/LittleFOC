#ifndef __SVPWM_H
#define __SVPWM_H

/* =================================================================================
File name:       __SVPWM_H.H  
===================================================================================*/

#include "main.h"
#include "MyMaths.h"

struct sv_mod{ 
	float  Ualpha; 			// Input: reference alpha-axis phase voltage 
	float  Ubeta;				// Input: reference beta-axis phase voltage 
	float  Ta;				// Output: reference phase-a switching function		
	float  Tb;				// Output: reference phase-b switching function 
	float  Tc;				// Output: reference phase-c switching function
	float  tmp1;				// Variable: temp variable
	float  tmp2;				// Variable: temp variable
	float  tmp3;				// Variable: temp variable
	uint16_t VecSector;		// Space vector sector
};
typedef struct 	sv_mod sv_mod_t;

struct sv_regs_mod{
	uint8_t ccr1;				// Output: pwm compare register 1
	uint8_t ccr2;				// Output: pwm compare register 2
	uint8_t ccr3;				// Output: pwm compare register 3
};
typedef	struct sv_regs_mod	sv_regs_mod_t;

#define SVGEN_DEFAULTS { 0,0,0,0,0 }
extern sv_mod_t sv;
/*------------------------------------------------------------------------------
	Space Vector  Generator (SVGEN) Macro Definition
------------------------------------------------------------------------------*/

void svpwm_calc(sv_mod_t* v);
sv_regs_mod_t svpwm_get_regs_mod(float vdc,float pwm_t, sv_mod_t* v);

//end of SVGENDQ_MACRO												   		
																				

//end of SVGENDQ_MACRO



#endif
