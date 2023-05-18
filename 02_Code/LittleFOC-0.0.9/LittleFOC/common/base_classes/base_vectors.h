/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BASE_VECTORS_H
#define __BASE_VECTORS_H
/* =================================================================================
File name:       __BASE_VECTORS_H
===================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

typedef struct Base_Vectors_ Base_Vectors; 

typedef void (*fptrClarke)(Base_Vectors*);
typedef void (*fptrIClarke)(Base_Vectors*);
typedef void (*fptrPark)(Base_Vectors*);
typedef void (*fptrIPark)(Base_Vectors*);

struct Base_Vectors_
{
	float Vq;
	float Vd;
	float theta;
	
	float Valpha;
	float Vbeta;
	
	float Va;
	float Vb;
	float Vc;
	
	fptrClarke clarke;
	fptrIClarke iclarke;
	fptrPark park;
	fptrIPark ipark;
};

Base_Vectors* new_Base_Vectors(void);
void delete_Base_Vectors(Base_Vectors* const pBVObj);

void base_vectors_clarke(Base_Vectors* const pBVObj);
void base_vectors_iclarke(Base_Vectors* const pBVObj);
void base_vectors_park(Base_Vectors* const pBVObj);
void base_vectors_ipark(Base_Vectors* const pBVObj);

#endif
