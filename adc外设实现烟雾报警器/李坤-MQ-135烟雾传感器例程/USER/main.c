#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "mq135.h"
#include "adcx.h"

/*****************辰哥单片机设计******************
											STM32
 * 项目			:	MQ-135空气质量传感器实验                     
 * 版本			: V1.0
 * 日期			: 2024.8.23
 * MCU			:	STM32F103C8T6
 * 接口			:	参看mq135.h							
 * BILIBILI	:	辰哥单片机设计
 * CSDN			:	辰哥单片机设计
 * 作者			:	辰哥 

**********************BEGIN***********************/

u16 value;
u8 buff[30];//参数显示缓存数组
float ppm;

int main(void)
{ 
	
  SystemInit();//配置系统时钟为72M	
	delay_init(72);
	LED_Init();
	LED_On();
	MQ135_Init();
	USART1_Config();//串口初始化
	
	OLED_Init();
	printf("Start \n");
	delay_ms(1000);
	
	OLED_Clear();
	//显示“空气质量:”
	OLED_ShowChinese(0,0,0,16,1);
	OLED_ShowChinese(16,0,1,16,1);
	OLED_ShowChinese(32,0,2,16,1);
	OLED_ShowChinese(48,0,3,16,1);
	OLED_ShowChar(64,0,':',16,1);
	
	

  while (1)
  {
		LED_Toggle();
		value = MQ135_GetData();  
		
		printf("空气质量: %d\r\n",value);
		OLED_ShowNum(80,0,value,4,16,1);
		
		ppm = MQ135_GetData_PPM();
		sprintf((char*)buff, "%.2fppm    ",ppm);
		OLED_ShowString(48,16,buff,16,1);
		
//		if(value)
//		{
//			OLED_ShowChinese(48,32,4,16,1);	//异
//			OLED_ShowChinese(64,32,6,16,1);	//常
//		}
//		else
//		{
//			OLED_ShowChinese(48,32,5,16,1);	//正
//			OLED_ShowChinese(64,32,6,16,1);	//常
//		}

		delay_ms(200);

  }
	
}


