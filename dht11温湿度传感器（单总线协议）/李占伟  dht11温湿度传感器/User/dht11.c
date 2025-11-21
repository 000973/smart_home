#include "dht11.h"
#include "delay.h"  // 需自行实现微秒级延时（HAL 库默认无 us 延时）

// 微秒级延时函数（STM32F103 72MHz 主频，循环次数需根据实际主频调整）
void delay_us(uint32_t us) {
    uint32_t delay = (SystemCoreClock / 1000000) * us / 5;  // 简化计算，实际需校准
    while(delay--);
}

// 1. DHT11 初始化：配置引脚 + 发送起始信号检测响应
uint8_t DHT11_Init(void) {
    DHT11_GPIO_CLK_EN;  // 使能 GPIO 时钟
    
    // 发送起始信号
    DHT11_MODE_OUT();
    DHT11_SET_LOW();
    delay_ms(20);       // 拉低 ≥18ms（此处取 20ms，留冗余）
    DHT11_SET_HIGH();
    delay_us(30);       // 拉高 20~40μs（此处取 30μs）
    
    // 切换为输入模式，等待 DHT11 响应
    DHT11_MODE_IN();
    delay_us(10);       // 等待响应信号稳定
    
    // 检测响应信号（DHT11 应拉低 80μs）
    if(DHT11_READ_PIN() == GPIO_PIN_RESET) {
        // 等待响应低电平结束（80μs）
        while(DHT11_READ_PIN() == GPIO_PIN_RESET);
        // 等待响应高电平结束（80μs）
        while(DHT11_READ_PIN() == GPIO_PIN_SET);
        return 0;  // 初始化成功
    } else {
        return 1;  // 初始化失败（无响应，检查硬件连接）
    }
}

// 2. 读取 1 个字节数据（DHT11 高位在前）
static uint8_t DHT11_Read_Byte(void) {
    uint8_t byte = 0;
    
    for(uint8_t i = 0; i < 8; i++) {
        // 等待数据位起始低电平（50μs）
        while(DHT11_READ_PIN() == GPIO_PIN_RESET);
        // 延时 30μs：若此时为低，说明是 0；若为高，说明是 1
        delay_us(30);
        byte <<= 1;  // 左移一位，准备接收下一位
        if(DHT11_READ_PIN() == GPIO_PIN_SET) {
            byte |= 0x01;  // 高电平持续 ≥70μs，为 1
            while(DHT11_READ_PIN() == GPIO_PIN_SET);  // 等待高电平结束
        }
    }
    return byte;
}

// 3. 读取完整温湿度数据（40bit = 5 字节）
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi) {
    uint8_t buf[5] = {0};  // 存储 5 字节数据：humi_int, humi_dec, temp_int, temp_dec, checksum
    
    // 发送起始信号（与初始化一致，每次读取前需重新发起）
    DHT11_MODE_OUT();
    DHT11_SET_LOW();
    delay_ms(20);
    DHT11_SET_HIGH();
    delay_us(30);
    DHT11_MODE_IN();
    delay_us(10);
    
    // 检测响应信号
    if(DHT11_READ_PIN() == GPIO_PIN_RESET) {
        // 等待响应低/高电平结束
        while(DHT11_READ_PIN() == GPIO_PIN_RESET);
        while(DHT11_READ_PIN() == GPIO_PIN_SET);
        
        // 读取 5 字节数据
        for(uint8_t i = 0; i < 5; i++) {
            buf[i] = DHT11_Read_Byte();
        }
        
        // 校验和验证（前 4 字节之和的低 8 位应等于第 5 字节）
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4]) {
            *humi = buf[0];  // 湿度整数（DHT11 湿度小数位固定为 0，可忽略 buf[1]）
            *temp = buf[2];  // 温度整数（DHT11 温度小数位固定为 0，可忽略 buf[3]）
            return 0;  // 读取成功
        } else {
            return 2;  // 校验失败（数据错误）
        }
    } else {
        return 1;  // 无响应（硬件问题或通信干扰）
    }
}