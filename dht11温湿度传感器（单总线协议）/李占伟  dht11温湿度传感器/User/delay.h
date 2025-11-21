#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f1xx_hal.h"

void delay_us(uint32_t us);  // 微秒延时
void delay_ms(uint32_t ms);  // 毫秒延时（复用 HAL_Delay，可选）

#endif