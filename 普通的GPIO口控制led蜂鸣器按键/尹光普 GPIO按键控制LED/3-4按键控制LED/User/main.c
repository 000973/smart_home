#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"

 int main(void)
{
	LED_Init();
	while(1)
	{
        LED1_ON();
		LED2_OFF();
		Delay_ms(500);
		LED1_OFF();
		LED2_ON();
		Delay_ms(500);
		
	}
}
