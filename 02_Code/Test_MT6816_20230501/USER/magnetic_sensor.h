/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAGNETIC_SENEOR_H
#define __MAGNETIC_SENEOR_H
/* =================================================================================
File name:       __MAGNETIC_SENEOR_H
magnetic_sensor: MT6816
===================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
//
//#include "common/math_tools/LPF_controller.h"

typedef struct Magnetic_Sensor_ Magnetic_Sensor; 

typedef void (*MSfptrInit)(Magnetic_Sensor*, uint16_t);
typedef void (*MSfptrStart)(Magnetic_Sensor*);
typedef uint16_t (*MSfptrGet)(Magnetic_Sensor*);

struct Magnetic_Sensor_
{
	SPI_TypeDef *SPIx;
	GPIO_TypeDef* CSN_GPIOx;
	uint32_t CSN_PinMask;
	
	//LPF_Controller *LPFCtrl;
	
	uint8_t Magnetic_Sensor_BUF[2];
	uint16_t Angle_BUF;
	uint8_t pc_flag;
	uint8_t no_mag_flag;
	
	q15_t origin;

	q15_t angle;
	q15_t angle_n;
	q15_t velocity;
	q15_t velocity_out;
	
	//interface for function
	MSfptrInit init;
	MSfptrStart start;
	MSfptrGet get;
};

//extern Magnetic_Sensor* ;

Magnetic_Sensor* new_Magnetic_Sensor(SPI_TypeDef *SPIx, GPIO_TypeDef* CSN_GPIOx, uint32_t CSN_PinMask);

void delete_Magnetic_Sensor(Magnetic_Sensor* const pMSObj);

void Magnetic_Sensor_init(Magnetic_Sensor* const pCSObj, uint16_t origin);
void Magnetic_Sensor_start(Magnetic_Sensor* const pMSObj);
uint16_t Magnetic_Sensor_get(Magnetic_Sensor* const pMSObj);

#endif