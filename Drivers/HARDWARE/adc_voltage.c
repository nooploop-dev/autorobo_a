#include "adc_voltage.h"

__IO uint16_t ADC_converted_value=0;               //ADC转换的电池电压16位数值
float battery_voltage=0.0;                         //电池电压
uint16_t ADC_converted_count=0;                    //AD转换计时变量
uint8_t battery_low_voltage_count=0;               //电池低电压计数变量
uint8_t battery_low_voltage_flag=0;                //电池低电压标志位


/************************************************
函数名称 ： Battery_Low_Voltage_Warning
函数功能 ： 电池低电压控制蜂鸣器报警防止过放
参    数 ： 无
返 回 值 ： 无
*************************************************/	
void Battery_Low_Voltage_Warning(void)
{
	if(battery_low_voltage_flag == 1)                //如果电池低电压标志位为1
	{
		BEE(BEE_ON);                                   //开启蜂鸣器
	}
}




