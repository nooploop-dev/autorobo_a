#ifndef __WS2812B_H
#define __WS2812B_H
#include "stm32h7xx_hal.h"
#include "spi.h"

#define PIXEL_MAX 10                     //最大像素点数，表示最多控制LED的个数

void Send_8_Bits(uint8_t data);
void Send_24_Bits(uint8_t RData,uint8_t GData,uint8_t BData);
void SetAllPixelColor(uint8_t R, uint8_t G, uint8_t B);
void setPixelColor(uint16_t X, uint8_t R, uint8_t G, uint8_t B);
void SetPixelColor(uint16_t X, uint32_t D);
void Pixel_Update(void);
uint32_t Wheel(uint8_t Wheel_Pos);
uint32_t Color(uint8_t R, uint8_t G, uint8_t B);
void Rainbow(uint16_t time);
void RainbowCycle(uint16_t time);
void ColorWipe(uint32_t D, uint16_t time);

void Ws2812b_Init(void);
void Ws2812b_Test(void);
void RGB_Show(uint8_t mode);

extern uint16_t RGB_delay;               //RGB灯条定时器延时时间
extern uint16_t RGB_delay_count;         //RGB灯条定时器延时时间计数变量，表示经过了多少个延时周期

#endif




