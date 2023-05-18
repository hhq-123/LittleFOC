/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PMSM_MOTOR_H
#define __PMSM_MOTOR_H
/* =================================================================================
File name:       __PMSM_MOTOR_H
===================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "driver/svpwm_driver.h"
#include "sensor/magnetic_sensor.h"
#include "sensor/current_sensor.h"
#include "common/math_tools/PID_controller.h"
#include "common/math_tools/NI_controller.h"


typedef struct PMSM_Motor_ PMSM_Motor;

typedef void (*fptrPMInit)(PMSM_Motor*);
typedef void (*fptrPMStart)(PMSM_Motor*);
typedef void (*fptrPMCtrl)(PMSM_Motor*);
typedef void (*fptrPMCtrl_P)(PMSM_Motor*, q15_t);
typedef void (*fptrPMSensor)(PMSM_Motor*);
typedef void (*fptrPMRun)(PMSM_Motor*);
typedef void (*fptrPMPort)(PMSM_Motor*, uint8_t*, uint8_t);


struct PMSM_Motor_
{
	uint8_t ID;
	
	SVPWM_Driver *SVPWM;
	Magnetic_Sensor *MagSensor;
	Current_Sensor *CurSensor;
	
	
	PID_Controller *PIDCtrl_I[2];
	PID_Controller *PIDCtrl_V;
	PID_Controller *PIDCtrl_P_V;
	PID_Controller *PIDCtrl_P_I;
	NI_Controller *NICtrl_P;
	
	uint8_t state;
	
	q15_t angle_stator;
	q15_t angle_cstator;
	q15_t angle_rotor;
	
	q15_t Position;
	q15_t Velocity;
	q15_t Uq;
	q15_t Ud;
	q15_t Iq;
	q15_t Id;
	
	fptrPMInit init;
	fptrPMStart start;
	fptrPMSensor sen;
	fptrPMCtrl crtl;
	fptrPMCtrl_P crtl_P;
	fptrPMRun run;
	fptrPMPort port;
};

PMSM_Motor* new_PMSM_Motor(void);
void delete_PMSM_Motor(PMSM_Motor* const pPMObj);
void pmsm_motor_init(PMSM_Motor* const pPMObj);
void pmsm_motor_start(PMSM_Motor* const pPMObj);
void pmsm_motor_sensor_get(PMSM_Motor* const pPMObj);
void pmsm_motor_ctrl(PMSM_Motor* const pPMObj);
void pmsm_motor_ctrl_position(PMSM_Motor* const pPMObj, q15_t position);
void pmsm_motor_run(PMSM_Motor* const pPMObj);
void pmsm_motor_port(PMSM_Motor* const pPMObj, uint8_t* data, uint8_t length);

#endif
