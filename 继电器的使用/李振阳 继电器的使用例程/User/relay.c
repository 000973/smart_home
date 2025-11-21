#include "relay.h"

// 继电器GPIO初始化
void Relay_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能继电器控制引脚时钟
    RCC_APB2PeriphClockCmd(RELAY1_CLK, ENABLE);
    
    // 配置RELAY1引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = RELAY1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // 推挽输出，驱动能力强
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // 输出速率50MHz
    GPIO_Init(RELAY1_PORT, &GPIO_InitStructure);
    
    // 初始状态：继电器断开
    Relay_SetState(RELAY1_PIN, RELAY_OFF);
}

// 设置继电器状态
void Relay_SetState(uint16_t RelayPin, Relay_StateTypeDef State) {
    if (State == RELAY_ON) {
        GPIO_SetBits(RELAY1_PORT, RelayPin);  // 高电平吸合（若继电器是低电平吸合，改为GPIO_ResetBits）
    } else {
        GPIO_ResetBits(RELAY1_PORT, RelayPin); // 低电平断开（对应修改）
    }
}

// 获取继电器当前状态
Relay_StateTypeDef Relay_GetState(uint16_t RelayPin) {
    if (GPIO_ReadOutputDataBit(RELAY1_PORT, RelayPin) == 1) {
        return RELAY_ON;
    } else {
        return RELAY_OFF;
    }
}