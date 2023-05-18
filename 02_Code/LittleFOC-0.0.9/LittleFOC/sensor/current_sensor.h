/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CURRENT_SENEOR_H
#define __CURRENT_SENEOR_H
/* =================================================================================
File name:       __CURRENT_SENEOR_H
===================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "common/math_tools/LPF_controller.h"

#define current_sampling_times 2
#define current_tau 1
#define current_T	 	2

typedef struct Current_Sensor_ Current_Sensor; 
typedef void (*CSfptrInit)(Current_Sensor*, uint16_t, uint16_t);
typedef void (*CSfptrStart)(Current_Sensor*);
typedef void (*CSfptrStartDMA)(Current_Sensor*);
typedef void (*CSfptrGet)(Current_Sensor*, uint16_t);

struct Current_Sensor_
{
	LPF_Controller *LPFCtrl[2];
	
	uint16_t ADC_BUF[2]; 
	uint16_t origin[2];
	
	q15_t Ia;
	q15_t Ib;
	q15_t Ic;
	
	q15_t Ialpha;
	q15_t Ibeta;
	
	q15_t Id;
	q15_t Iq;
	
	q15_t Id_out;
	q15_t Iq_out;
	
	//interface for function
	CSfptrInit init;
	CSfptrStart start;
	CSfptrStartDMA startDMA;
	CSfptrGet get;
};

extern Current_Sensor* foc_current;

Current_Sensor* new_Current_Sensor(void);

void delete_CS(Current_Sensor* const pCSObj);
void Current_Sensor_init(Current_Sensor* const pCSObj, uint16_t Ia, uint16_t Ic);
void Current_Sensor_start(Current_Sensor* const pCSObj);
void Current_Sensor_start_DMA(Current_Sensor* const pCSObj);
void Current_Sensor_get(Current_Sensor* const pCSObj, uint16_t theta);


#endif