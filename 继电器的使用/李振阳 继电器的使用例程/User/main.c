#include "stm32f10x.h"
#include "relay.h"
#include "delay.h"

int main(void) {
    // 初始化：继电器+延时
    Relay_Init();
    
    while (1) {
        // 场景1：模拟"开灯"（继电器吸合）
        Relay_SetState(RELAY1_PIN, RELAY_ON);
        Delay_ms(3000);  // 保持开启3秒
        
        // 场景2：模拟"关灯"（继电器断开）
        Relay_SetState(RELAY1_PIN, RELAY_OFF);
        Delay_ms(3000);  // 保持关闭3秒
        
       
    }
}