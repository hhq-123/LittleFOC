/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LITTLEFOC_H
#define __LITTLEFOC_H
/* =================================================================================
File name:       __LITTLEFOC_H
===================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

#include "pmsm_motor.h"
#include "controller/FOC_motor.h"

#include "common/time_utils.h"

#include "common/math_tools/MyMaths.h"
#include "common/math_tools/PID_controller.h"
#include "common/math_tools/LPF_controller.h"
#include "common/math_tools/NI_controller.h"

#include "driver/svpwm_driver.h"
#include "sensor/magnetic_sensor.h"
#include "sensor/current_sensor.h"

#define FOC_ID 0x30

#define CMD_NICrtl_P_CW 0x02
#define CMD_NICrtl_P_CCW 0x03

#define U_base 12.0
#define I_base 1.65
#define f_base 5000.0
#define R_base U_base / I_base
#define w_base 2.0*PI*f_base
#define phi_base = U_base / w_base
#define L_base U_base / (w_base * I_base)
#define T_base 1.0 / f_base

#define Motor_J 0.02 * 0.015 * 0.015 /2
#define Motor_Plambda 0.03/0.1
#define K_PL_J Motor_Plambda/Motor_J
#define Motor_delta 5.0

#define Motor_L 0.001
#define Motor_R 5.0

#endif
