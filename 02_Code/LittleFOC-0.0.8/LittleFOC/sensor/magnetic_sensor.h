/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAGNETIC_SENEOR_H
#define __MAGNETIC_SENEOR_H
/* =================================================================================
File name:       __MAGNETIC_SENEOR_H
===================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "AS5600.h"
#include "common/math_tools/LPF_controller.h"

typedef struct Magnetic_Sensor_ Magnetic_Sensor; 

typedef void (*MSfptrInit)(Magnetic_Sensor*, uint16_t);
typedef void (*MSfptrStart)(Magnetic_Sensor*);
typedef void (*MSfptrStartDMA)(Magnetic_Sensor*);
typedef uint16_t (*MSfptrGet)(Magnetic_Sensor*);

struct Magnetic_Sensor_
{
	I2C_TypeDef *I2Cx;
	uint16_t DevAddress;
	uint16_t MemAddress;
	
	LPF_Controller *LPFCtrl;
	
	uint8_t Magnetic_Sensor_BUF[2];
	uint16_t Angle_BUF;
	
	q15_t origin;

	q15_t angle;
	q15_t angle_n;
	q15_t velocity;
	q15_t velocity_out;
	
	//interface for function
	MSfptrInit init;
	MSfptrStart start;
	MSfptrStartDMA startDMA;
	MSfptrGet get;
};

//extern Magnetic_Sensor* ;

Magnetic_Sensor* new_Magnetic_Sensor__(I2C_TypeDef *I2Cx, 
	uint16_t DevAddress, uint16_t MemAddress);

#define new_Magnetic_Sensor(void) new_Magnetic_Sensor__(I2C2, AS5600_ADDRESS<<1, AS5600_ANGLE)
#define new_Magnetic_Sensor_(I2Cx) new_Magnetic_Sensor__(I2Cx, AS5600_ADDRESS<<1, AS5600_ANGLE)

void delete_Magnetic_Sensor(Magnetic_Sensor* const pMSObj);

void Magnetic_Sensor_init(Magnetic_Sensor* const pCSObj, uint16_t origin);
void Magnetic_Sensor_start(Magnetic_Sensor* const pMSObj);
void Magnetic_Sensor_start_DMA(Magnetic_Sensor* const pMSObj);
void Magnetic_Sensor_ISR_DMA(Magnetic_Sensor* const pMSObj);
uint16_t Magnetic_Sensor_get(Magnetic_Sensor* const pMSObj);

#endif