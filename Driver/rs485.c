#include "sys.h"
#include "rs485.h"
#include "systick.h"
#include "usart.h"

#ifdef EN_UART3_RX
uint32_t RS485_RX_CNT = 0;

uint8_t RS485_RX_BUF[RS385_RX_BUF_LEN] 		__attribute__((at(0X20001000)));

void UART3_IRQHandler(void)
{
	u8 res;
	if (usart_interrupt_flag_get(UART3, USART_INT_FLAG_RBNE) != RESET)
	{

		res = usart_data_receive(UART3); 
		if (RS485_RX_CNT < RS385_RX_BUF_LEN)
		{
			RS485_RX_BUF[RS485_RX_CNT] = res; 
			RS485_RX_CNT++;					 
		}
	}
}
#endif

void RS485_Init(u32 bound)
{

    rcu_periph_clock_enable(RCU_GPIOC);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	RS485_TX_EN(0);     //defult Receive
	
	
	// Enable GPIO AF Clock
	rcu_periph_clock_enable( RCU_AF );
	// Enable UART3 Remap
	gpio_pin_remap1_config( GPIO_PCF5, GPIO_PCF5_UART3_REMAP, ENABLE );
	
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_UART3);
    
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_1);

	/* USART config */
	usart_deinit(UART3);
	usart_baudrate_set(UART3, bound);
	usart_word_length_set(UART3, USART_WL_8BIT);
	usart_stop_bit_set(UART3, USART_STB_1BIT);
	usart_parity_config(UART3, USART_PM_NONE);
	usart_hardware_flow_rts_config(UART3, USART_RTS_DISABLE);
	usart_hardware_flow_rts_config(UART3, USART_CTS_DISABLE);
	usart_receive_config(UART3, USART_RECEIVE_ENABLE);
	usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
	usart_enable(UART3);

	
	nvic_irq_enable(UART3_IRQn, 0, 0);
    /* Enable Receive Int, And Disable Send Int */
	usart_interrupt_disable( UART3, USART_INT_TBE );
	usart_interrupt_enable( UART3, USART_INT_RBNE);
}

void RS485_Receive_Data(uint8_t *buf)
{
	uint8_t rxlen = RS485_RX_CNT;
	uint8_t i = 0;						
	delay_ms(10);						
	if (rxlen == RS485_RX_CNT && rxlen) 
	{
		for (i = 0; i < rxlen; i++)
		{
			buf[i] = RS485_RX_BUF[i];
		}
		RS485_RX_CNT = 0;	 
	}
}
