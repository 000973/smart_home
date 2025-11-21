#include "LCD1602.h" 

GPIO_InitTypeDef GPIO_InitStructure;
#define DELAY_2N     0
/*--------------驱动LCD1602模块----------------------

注意：	此版本为较少IO版本，只接LCD1602数据口高四位。
		PB3、PB4特殊功能已经禁用JTAG调试。
接口：		*D4---->PB6
			*D5---->PB7
			*D6---->PB8
			*D7---->PB9
			*RS---->PB3
			*RW---->PB11
			*EN---->PB10
调用方法：
#include "LCD1602.h"

float flo = 99.99;			//定义float型的数（少用）
lcd_str[]="hello!"			//测试使用，可删除
char_num='a';				//测试使用，可删除

	LCD_init();						//初始化LCD
	
		LCD1602_shownum(1,1,num,5);			//显示数字
		LCD1602_showchar(1,1,char_num);		//显示字符
		LCD1602_showstring(1,1,lcd_str);		//显示字符串
		LCD1602_showstring(1,1,"hello!");		//显示字符串		
		LCD1602_showflo(0,2,flo,2,2);		//显示小数，第一个2是整数长度（两位），第二个2是小数长度（2位）
			
			
附：	RW引脚可以接GND，再少用一个引脚，但是此情况只能写不能读。
			仿真无需修改直接可以用。
*/
void LCD_init(void)
{
    /*********************液晶使用的I/O口初始化**************************/ 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 |GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_11  ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    CLR_RW();			//读写位直接低电平，只写不读

    /*********************液晶初始化**************************/        
    delay_lcd1602 (15000);
	

   
    CLR_RS();
    LCD_Write_half_byte(0x3);                 
    delay_lcd1602 (15000);
    LCD_Write_half_byte(0x3);
    delay_lcd1602 (15000);
    LCD_Write_half_byte(0x3);
    LCD_Write_half_byte(0x2);
   
    
    LCD_write_cmd(0x28);          // 4bit显示模式,2行,5x7字体
    delay_lcd1602 (20000);  
    LCD_write_cmd(0x08);         // 显示关闭 
    delay_lcd1602 (20000); 
    LCD_write_cmd(0x01);         // 显示清屏 
    delay_lcd1602 (20000); 
    LCD_write_cmd(0x06);         // 显示光标移动设置 
    delay_lcd1602 (20000);
    LCD_write_cmd(0x0C);         //显示开,光标关，
    //LCD_write_cmd(0x0F);         // 显示开，光标开，光标闪烁
    delay_lcd1602 (20000);
	LCD_write_cmd(0x01);         //清屏
}
/*--------------------------------------------------
函数说明：写命令到液晶

---------------------------------------------------*/
void LCD_write_cmd(unsigned char cmd)
{
    CLR_RS();
    LCD_Write_half_byte(cmd >> 4);
    LCD_Write_half_byte(cmd);
    delay_lcd1602 (10000);
}
/*--------------------------------------------------
函数说明：写数据到液晶


---------------------------------------------------*/
void LCD_write_data(unsigned char w_data)
{
    SET_RS();
    LCD_Write_half_byte(w_data >> 4);
    LCD_Write_half_byte(w_data);
    delay_lcd1602 (10000);
}
/*--------------------------------------------------
函数说明：写4bit到液晶
--------------------------------------------------*/
void LCD_Write_half_byte(unsigned char half_byte)
{  

	if (half_byte&0x01)
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
	else
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);

	if (half_byte&0x02)
			GPIO_SetBits(GPIOB,GPIO_Pin_7);
	else
			GPIO_ResetBits(GPIOB,GPIO_Pin_7);

	if (half_byte&0x04)
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
	else
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);

	if (half_byte&0x08)
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
	else
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);

    SET_EN();
    delay_lcd1602(2000);
    CLR_EN(); 
    delay_lcd1602(2000);
}

/*----------------------------------------------------
LCD_set_xy        : 设置LCD显示的起始位置
输入参数：x、y    : 显示字符串的位置，X:1-16，Y:1-2                
-----------------------------------------------------*/
void LCD_set_xy( unsigned char x, unsigned char y )
{
    unsigned char address;
    if (y==0) 
    {
        address=0x80+x;
    }
    else 
    {
        address=0xc0+x;
    }
    LCD_write_cmd(address);
}
/*---------------------------------------------------
LCD_write_string  : 英文字符串显示函数
输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置                
---------------------------------------------------*/
void LCD1602_showstring(unsigned char X,unsigned char Y,unsigned char *s)
{
    LCD_set_xy( X, Y );   
    while (*s) 
    {
        LCD_write_data(*s);
        s++;
    }
}


void LCD1602_showchar(unsigned char X,unsigned char Y,unsigned char s)
{
    LCD_set_xy( X, Y );   
    LCD_write_data(s);

}

int LCD1602_POW(int x,int y)  		//x的y次方
{
	unsigned char i;
	int Result=1;
	for(i=0;i<y;i++)
		Result*=x;
	return Result;
}
/********************************************************************
* 名称 : LCD1602_ShowNum(unsigned char Line,unsigned char Colum, unsigned int Number,unsigned char Length )
* 功能 : hang：1或2		lie：1-16			Number：写入的数字   0~65536			Length：数字长度
* 输入 : 行，列，需要输入1602的数据，数字长度
* 输出 : 无
***********************************************************************/
void LCD1602_shownum(unsigned char hang,unsigned char lie, unsigned int Number,unsigned char Length )
{
	unsigned char i;
	LCD_set_xy(hang,lie);
	for(i=Length;i>0;i--)
	{
		LCD_write_data(0x30+Number/LCD1602_POW(10,i-1)%10);  	//从数字转Asiic表  0x30或'0'就是标的字符0开始这样加上多少就是显示多少数字
	}
 
}



//*************************************************************************************
//写入数字float
//Number是要显示的小数，z_Length为整数长度，f_Length为小数长度
//*************************************************************************************

void LCD1602_showflo(unsigned char X, unsigned char Y, double  number, unsigned char z_Length, unsigned char f_Length )
{
	unsigned char NUM_CHAR[11] = {0};
	int i = 0;
	int x = 0;
	
	if(z_Length < 1)
	{
		z_Length = 1;
	}
    for (i = 1; i < z_Length; i++)
    {
        number /= 10;
    }
    for (i = 0; i < z_Length; i++)
    {
		NUM_CHAR[x++] = ((int)number)%10 + '0';
		number *= 10;
    }
	
	NUM_CHAR[x++] =  '.';
	
    for (i = 0; i < f_Length; i++)
    {
		NUM_CHAR[x++] = ((int)number)%10 + '0';
		number *= 10;
    }
	
 	LCD1602_showstring(X,Y,NUM_CHAR);
}

//=======================================================
/*
void Move(unsigned int step,unsigned int dirction,unsigned int time)
{
          unsigned int i;
         for(i=0;i<step-1;i++)
         {
                   LCD_write_byte(1,dirction);      //字符移动方向                                    
         Delay_nms(time);                //控制移动时间
         }
}
*/
//=========================================================
/*
void Flash_lcd(unsigned int delay_t, unsigned int times)
{
           unsigned int j;
         for(j=0;j<times;j++)
         {
                  LCD_write_byte(1,0x08);
                Delay_nms(delay_t);
                LCD_write_byte(1,0x0c);
                Delay_nms(delay_t);
         }
}
*/
//========================================================
void delay_lcd1602(vu32 cnt)
{
  cnt <<= DELAY_2N;

  while (cnt--);
}
//========================================================

