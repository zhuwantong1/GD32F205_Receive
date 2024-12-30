#ifndef __USART_H
#define __USART_H


#include "stdint.h"	
#include "stdio.h"
#define USART_REC_LEN  			200



extern uint8_t  USART_RX_BUF[USART_REC_LEN]; 
extern uint16_t USART_RX_STA;         		
extern uint32_t USART_RX_CNT;				

void usart_config(void);
#endif

