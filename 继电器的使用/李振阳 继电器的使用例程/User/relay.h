#ifndef __RELAY_H
#define __RELAY_H

#include "stm32f10x.h"

// 继电器控制引脚定义
#define RELAY1_PIN    GPIO_Pin_0
#define RELAY1_PORT   GPIOA
#define RELAY1_CLK    RCC_APB2Periph_GPIOA

// 继电器状态定义
typedef enum {
    RELAY_OFF = 0,  // 断开（对应设备关闭）
    RELAY_ON = 1    // 吸合（对应设备开启）
} Relay_StateTypeDef;

// 函数声明
void Relay_Init(void);                          // 所有继电器初始化
void Relay_SetState(uint16_t RelayPin, Relay_StateTypeDef State);  // 设置单个继电器状态
Relay_StateTypeDef Relay_GetState(uint16_t RelayPin);               // 获取单个继电器状态

#endif