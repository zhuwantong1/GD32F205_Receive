#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  

extern uint32_t RS485_RX_CNT;   			
#define RS385_RX_BUF_LEN 	1024*20

#define RS485_TX_EN(condition) ((condition) ? gpio_bit_set(GPIOC, GPIO_PIN_12) : gpio_bit_reset(GPIOC, GPIO_PIN_12))



#define EN_UART3_RX 	1			

void RS485_Init(u32 bound);
void RS485_Receive_Data(uint8_t *buf);


#endif	   
















