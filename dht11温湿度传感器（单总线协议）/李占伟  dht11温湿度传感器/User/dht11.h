#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f1xx_hal.h"

// 自定义 DHT11 数据线引脚（可修改为其他 GPIO）
#define DHT11_GPIO_PORT    GPIOB
#define DHT11_GPIO_PIN     GPIO_PIN_0
#define DHT11_GPIO_CLK_EN  __HAL_RCC_GPIOB_CLK_ENABLE()  // PB 时钟使能

// GPIO 模式切换宏定义（简化代码）
#define DHT11_MODE_OUT()  do { \
                            GPIO_InitTypeDef gpio_init = {0}; \
                            gpio_init.Pin = DHT11_GPIO_PIN; \
                            gpio_init.Mode = GPIO_MODE_OUTPUT_PP; \
                            gpio_init.Speed = GPIO_SPEED_FREQ_LOW; \
                            HAL_GPIO_Init(DHT11_GPIO_PORT, &gpio_init); \
                          } while(0)

#define DHT11_MODE_IN()   do { \
                            GPIO_InitTypeDef gpio_init = {0}; \
                            gpio_init.Pin = DHT11_GPIO_PIN; \
                            gpio_init.Mode = GPIO_MODE_INPUT_PULLUP; \
                            HAL_GPIO_Init(DHT11_GPIO_PORT, &gpio_init); \
                          } while(0)

// 数据位操作
#define DHT11_SET_HIGH()  HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET)
#define DHT11_SET_LOW()   HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET)
#define DHT11_READ_PIN()  HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

// 函数声明
uint8_t DHT11_Init(void);                // 初始化 DHT11（引脚 + 通信检测）
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi);  // 读取温湿度数据

#endif