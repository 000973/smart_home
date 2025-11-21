#include "delay.h"

// 微秒延时函数（STM32F103 72MHz 主频校准，直接用）
void delay_us(uint32_t us) {
    uint32_t ticks = (SystemCoreClock / 1000000) * us;  // 72MHz 时，1us = 72 个时钟周期
    uint32_t start = SysTick->VAL;                      // 读取 SysTick 当前值（24 位计数器）
    
    while ((SysTick->VAL - start + (1 << 24)) % (1 << 24) < ticks);  // 循环等待
}

// 毫秒延时（可选，直接复用 HAL_Delay，避免重复定义）
void delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}