#include "base_vectors.h"

Base_Vectors* new_Base_Vectors(void)
{
	Base_Vectors* pObj = NULL;
	uint8_t i = 0;
	pObj = (Base_Vectors*)malloc(sizeof(Base_Vectors));
	if (pObj == NULL)
	{
		return NULL;
	}
	pObj->clarke = base_vectors_clarke;
	pObj->iclarke = base_vectors_iclarke;
	pObj->park = base_vectors_park;
	pObj->ipark = base_vectors_ipark;
	
	return pObj;		
}

void delete_Base_Vectors(Base_Vectors* const pBVObj)
{
	free(pBVObj);
}

void base_vectors_clarke(Base_Vectors* const pBVObj)
{
	pBVObj->Valpha = pBVObj->Va;
	pBVObj->Vbeta = (pBVObj->Va + 2*pBVObj->Vb)*0.57735026919;//0.5773 5026 9189 63
}

void base_vectors_iclarke(Base_Vectors* const pBVObj)
{
	pBVObj->Va = pBVObj->Valpha;
	pBVObj->Vb = (pBVObj->Vbeta * 1.73205080757 - pBVObj->Valpha)/2;//1.73205080757
	pBVObj->Vc = -pBVObj->Va-pBVObj->Vb;
}

void base_vectors_park(Base_Vectors* const pBVObj)
{
	float Sin = sin(pBVObj->theta);
	float Cos = cos(pBVObj->theta);
	
  pBVObj->Vd =  pBVObj->Valpha * Cos + pBVObj->Vbeta  * Sin;
  pBVObj->Vq = -pBVObj->Vbeta  * Sin + pBVObj->Valpha * Cos;
}

void base_vectors_ipark(Base_Vectors* const pBVObj)
{
	float Sin = sin(pBVObj->theta);
	float Cos = cos(pBVObj->theta);
	//printf("theta: %f\r\n", pBVObj->theta);
  pBVObj->Valpha = pBVObj->Vd * Cos - pBVObj->Vq * Sin;
  pBVObj->Vbeta  = pBVObj->Vd * Sin + pBVObj->Vq * Cos;	
}
