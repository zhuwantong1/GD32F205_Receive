#include "project.h"
#include "string.h"
/**
 * @brief   main function
 *
 * @note    This function is the entry of the program. It configures the system,
 *          initializes the peripherals and then enters the super loop.
 *
 * @param   None
 *
 * @retval  None
 */
void RS485_Jump(void);
static uint32_t AppCodeLength = 0; // 接收到的app代码长度
static uint8_t AppRunFlag = 0; // 应用程序运行标志,APP已经写入到flash，等待跳转
extern uint8_t RS485_RX_BUF[RS385_RX_BUF_LEN] 		__attribute__((at(0X20001000)));
int main(void)
{
	
	nvic_priority_group_set( NVIC_PRIGROUP_PRE4_SUB0 );
	systick_config();
	usart_config();
	led_gpio_init();
    RS485_Init(115200);
    
	while (1)
	{
        led_toggle();	
		if(AppCodeLength == RS485_RX_CNT)
		{
			RS485_Jump();
		}
		else
		{
			AppCodeLength = RS485_RX_CNT;
		}
		
		
//		RS485_Receive_Data(RS485_RX_BUF);
//		for(int i=0;i<20;i++)
//		{
//			printf("USART_RX_BUF_485[%d] = %d\r\n",i,USART_RX_BUF_485[i]);
//		}
//		printf("----------------received finish ----------------\r\n");
//		memset(USART_RX_BUF_485,0,20);
	}			
}

void  RS485_Jump( void )
{
	/* judge the command */
		
		if(RS485_RX_BUF[0] == 0xbb&&RS485_RX_BUF[1] == 0xbb&&RS485_RX_BUF[2] == 0xbb
			&&RS485_RX_BUF[3] == 0xbb&&RS485_RX_BUF[4] == 0x11&&RS485_RX_CNT == 5)
		{
			/* compare received command */
			printf("received jump command ! \r\n");
			memset(RS485_RX_BUF,0,RS485_RX_CNT);
			RS485_RX_CNT = 0;
		}
		else if(RS485_RX_CNT>10)
		{
			/* copy RS485_RX_BUF to sram and jump to app */
			iap_write_appbin(FLASH_APP1_ADDR, RS485_RX_BUF, AppCodeLength);
			AppRunFlag = 1;
		}
		
		if (AppRunFlag) // App运行标志置位标志写入完成，开始执行APP的代码
		{
			printf("Start running app code!\r\n");
			delay_ms(10);
			if (((*(volatile uint32_t *)(FLASH_APP1_ADDR + 4)) & 0xFF000000) == 0x08000000) // 判断代码合法性
			{
				AppRunFlag = 0;
				iap_load_app(FLASH_APP1_ADDR); // 执行FLASH APP代码
			}
			else
			{
				printf("App code is illegal!\r\n");
			}
			RS485_RX_CNT = 0;
		}
//		else if(sizeof(RS485_RX_BUF)>20)
//		{
//			
//		}
}
