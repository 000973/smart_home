#include "stm32f10x.h" // Device header
#include "usart2.h"
#include "Delay.h"

void System_Init(void)
{
    USART2_Init(115200);
}

int main(void)
{
    System_Init();
    
    u2_printf("UART Basic Demo Ready!\r\n");
    u2_printf("Send any text and it will be echoed back.\r\n");
    
    while(1) {
			//如果有数据接收完成
        if(USART2_RX_STA & 0x8000) {
            u16 len = USART2_RX_STA & 0x7FFF;
            USART2_RX_BUF[len] = '\0';  //添加结束符
           
					//回显接收到的数据
            u2_printf("Echo: %s\r\n", USART2_RX_BUF);
            //重置接收状态
            USART2_RX_STA = 0;
        }
        
        Delay_ms(10);
    }
}

