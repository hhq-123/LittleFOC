/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stdlib.h"

typedef struct NI_Controller_ NI_Controller; 

typedef short q15_t;
typedef int q17_15_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

typedef void (*NICfptrInit)(NI_Controller*);
typedef void (*NICfptrcalc)(NI_Controller*, q15_t, q15_t);
typedef q15_t (*NICfptrget_)(NI_Controller*, uint8_t, uint16_t);
typedef q15_t (*NICfptrget)(NI_Controller*);

struct NI_Controller_
{  
	//q15_t NI_buffer[2][1024];
	q17_15_t F_buf[3];
	q17_15_t NI_D_buf[2][3];
	uint16_t length[2];
	uint8_t BUSY_flag;
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

void delete_NI_Controller_(NI_Controller* const pNICObj);

void NI_Controller_init(NI_Controller* const pNICObj);
void NI_Controller_calc(NI_Controller* const pNICObj, q15_t value, q15_t len);
q15_t NI_Controller_get_(NI_Controller* const pNICObj, uint8_t channel, uint16_t count);
q15_t NI_Controller_get(NI_Controller* const pNICObj);


#include <stdio.h>

int main()
{
	uint8_t i=0;
	NI_Controller* NIC;
	q15_t Test_buf[4][2] = {
	{0,100},
	{200,20},
	{100,80},
	{50,200}
	};
	
	NIC = new_NI_Controller();
	
  while (1)
  {
    /* USER CODE END WHILE */
		
		if(NIC->cnt == 0)
		{
			NIC->calc(NIC, Test_buf[i][0], Test_buf[i][1]);
			i++;
			if(i>=4)
				break;
		}
		
		printf("%d: %d\r\n", NIC->cnt, NIC->get(NIC));
		
    /* USER CODE BEGIN 3 */
		//LL_ADC_REG_StartConversion(ADC1);
		//position_test();
		//printf("%d %d\r\n", pmsm_motor->CurSensor->ADC_BUF[0], pmsm_motor->CurSensor->ADC_BUF[1]);
  }
	
}

NI_Controller* new_NI_Controller(void)
{
	NI_Controller* pObj = NULL;
	uint8_t i = 0;
	pObj = (NI_Controller*)malloc(sizeof(NI_Controller));
	if (pObj == NULL)
	{
		return NULL;
	}
	
	pObj->init = NI_Controller_init;
	pObj->calc = NI_Controller_calc;
	pObj->get_ = NI_Controller_get_;
	pObj->get = NI_Controller_get;
	
	pObj->init(pObj);
	return pObj;
}

void delete_NI(NI_Controller* const pNICObj)
{
	free(pNICObj);
}

void NI_Controller_init(NI_Controller* const pNICObj)
{

}

void NI_Controller_calc(NI_Controller* const pNICObj, q15_t value, q15_t len)
{
	pNICObj->F_buf[0] = pNICObj->F_buf[1];
	pNICObj->F_buf[1] = pNICObj->F_buf[2];
	pNICObj->F_buf[2] = (q17_15_t)value<<15;
	
	pNICObj->length[pNICObj->channel_] = len;
	
	pNICObj->NI_D_buf[pNICObj->channel_][0] = (pNICObj->F_buf[1]-pNICObj->F_buf[0])/pNICObj->length[pNICObj->channel];
	pNICObj->NI_D_buf[pNICObj->channel_][1] = (pNICObj->F_buf[2]-pNICObj->F_buf[1])/pNICObj->length[pNICObj->channel_];
	pNICObj->NI_D_buf[pNICObj->channel_][2] = (pNICObj->NI_D_buf[pNICObj->channel_][1]-pNICObj->NI_D_buf[pNICObj->channel_][0])
																		/(pNICObj->length[pNICObj->channel]+pNICObj->length[pNICObj->channel_]);
}

q15_t NI_Controller_get_(NI_Controller* const pNICObj, uint8_t channel, uint16_t count)
{	
	q15_t temp = 0;
	
	temp = (pNICObj->NI_D_buf[channel][0]
			 + pNICObj->NI_D_buf[channel][1] * (count + pNICObj->length[pNICObj->channel_])
			 + pNICObj->NI_D_buf[channel][2] * (count + pNICObj->length[pNICObj->channel_]) * count)>>15;

	return temp;
}

q15_t NI_Controller_get(NI_Controller* const pNICObj)
{
	if(pNICObj->cnt>=pNICObj->length[pNICObj->channel])
	{
		pNICObj->cnt = 0;
		pNICObj->channel_ = pNICObj->channel;
		pNICObj->channel = (pNICObj->channel+1)&0x01;
	}
	return pNICObj->get_(	pNICObj,
												pNICObj->channel,
												pNICObj->cnt++);
}

