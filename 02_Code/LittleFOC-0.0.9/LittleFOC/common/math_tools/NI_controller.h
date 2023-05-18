/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NI_CONTROLLER_H__
#define __NI_CONTROLLER_H__
/* =================================================================================
File name:       __NI_CONTROLLER_H__
===================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef struct NI_Controller_ NI_Controller; 

typedef void (*NICfptrInit)(NI_Controller*);
typedef void (*NICfptrcalc)(NI_Controller*, q15_t, q15_t, uint8_t);
typedef q15_t (*NICfptrget_)(NI_Controller*, uint8_t, uint16_t);
typedef q15_t (*NICfptrget)(NI_Controller*);

struct NI_Controller_
{  
	//q15_t NI_buffer[2][1024];
	q17_15_t F_buf[3];
	q17_15_t NI_D_buf[2][3];
	uint16_t length[2][2];
	uint8_t state;// 第8位标志 0 未更新下一点信息 1 已更新下一点信息；第7位标志 0 停止运行 1 运行；
	uint8_t channel;
	uint8_t channel_;
	uint16_t cnt;
	
	//interface for function
	NICfptrInit init;
	NICfptrcalc calc;
	NICfptrget_ get_;
	NICfptrget get;
	
};

NI_Controller* new_NI_Controller(void);

void delete_NI_Controller(NI_Controller* const pNICObj);

void NI_Controller_init(NI_Controller* const pNICObj);
void NI_Controller_calc(NI_Controller* const pNICObj, q15_t value, q15_t len, uint8_t direction);
q15_t NI_Controller_get_(NI_Controller* const pNICObj, uint8_t channel, uint16_t count);
q15_t NI_Controller_get(NI_Controller* const pNICObj);


#ifdef __cplusplus
}
#endif


#endif /* __NEWTON_INTERPOLATION_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
