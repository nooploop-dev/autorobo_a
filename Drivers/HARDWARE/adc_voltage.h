#ifndef __ADC_VOLTAGE_H
#define __ADC_VOLTAGE_H
#include "main.h"

#define LOW_VOLTAGE 7.4

extern __IO uint16_t ADC_converted_value;               //ADC转换的电池电压16位数值
extern float battery_voltage;                           //电池电压
extern uint16_t ADC_converted_count;                    //AD转换计时变量
extern uint8_t battery_low_voltage_count;               //电池低电压计数变量
extern uint8_t battery_low_voltage_flag;                //电池低电压标志位

void Battery_Low_Voltage_Warning(void);                 //电池低电压控制蜂鸣器报警防止过放函数

#endif

