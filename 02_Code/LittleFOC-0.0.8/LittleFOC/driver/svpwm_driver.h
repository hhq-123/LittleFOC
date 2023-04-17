/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SVPWM_DRIVER_H
#define __SVPWM_DRIVER_H
/* =================================================================================
File name:       __SVPWM_DRIVER_H
===================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"


typedef struct SVPWM_Driver_ SVPWM_Driver;

typedef void (*SDfptrInit)(SVPWM_Driver*, q15_t);
typedef void (*SDfptrCalc)(SVPWM_Driver* ,q15_t ,q15_t ,uint16_t);
typedef void (*SDfptrStop)(SVPWM_Driver*);
typedef void (*SDfptrWrite_)(SVPWM_Driver*, uint16_t);

struct SVPWM_Driver_
{
	q15_t Udc;
	
	uint16_t theta;
	
	q15_t Uq;
	q15_t Ud;
	
	q15_t Ualpha;
	q15_t Ubeta;
	
	q15_t  Ta;				// Output: reference phase-a switching function		
	q15_t  Tb;				// Output: reference phase-b switching function 
	q15_t  Tc;				// Output: reference phase-c switching function
	
	q15_t  tmp1;				// Variable: temp variable
	q15_t  tmp2;				// Variable: temp variable
	q15_t  tmp3;				// Variable: temp variable	
	
	uint8_t VecSector;		// Space vector sector
	
	SDfptrInit init;
	SDfptrCalc	calc;
	SDfptrStop	stop;
	SDfptrWrite_	write_;
	#define write(a) write_(a, 200)
};

SVPWM_Driver* new_SVPWM_Driver(q15_t Udc);
void delete_SVPWM_Driver(SVPWM_Driver* const pSDObj);

void foc_svpwm_init(SVPWM_Driver* const pSDObj, q15_t Udc);
void foc_svpwm_calc(SVPWM_Driver* const pSDObj ,q15_t Ud ,q15_t Uq ,uint16_t theta);
void foc_svpwm_stop(SVPWM_Driver* const pSDObj);
void foc_svpwm_write_(SVPWM_Driver* const pSDObj, uint16_t pwm_t);


#define foc_svpwm_write(a) foc_svpwm_write_(a, 190)

#endif
