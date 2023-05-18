/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LPF_CONTROLLER_H
#define __LPF_CONTROLLER_H
/* =================================================================================
File name:       __LPF_CONTROLLER_H
===================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

typedef struct LPF_Controller_ LPF_Controller;

typedef void (*LPFCfptrInit)(LPF_Controller*, q15_t);
typedef q15_t (*LPFCfptrCalc)(LPF_Controller*, q15_t);

struct LPF_Controller_
{
	q15_t tau;
	q15_t output;
	
	LPFCfptrInit init;
	LPFCfptrCalc calc;
};
 
LPF_Controller* new_LPF_Controller(q15_t tau);
void delete_LPF_Controller(LPF_Controller* const pLPFCObj);

void LPF_controller_init(LPF_Controller* const pLPFCObj, q15_t tau);
q15_t LPF_controller_calc(LPF_Controller* const pLPFCObj, q15_t value);

#endif