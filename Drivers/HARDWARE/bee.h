#ifndef __BEE_H
#define __BEE_H
#include "stm32h7xx.h"

/** 控制蜂鸣器响的宏，
	* 蜂鸣器高电平响，设置ON=1，OFF=0
	* 若蜂鸣器低电平响，把宏设置成ON=0 ，OFF=1 即可
	*/
#define BEE_ON   GPIO_PIN_SET
#define BEE_OFF  GPIO_PIN_RESET

#define BEE(a)	HAL_GPIO_WritePin(BEE_GPIO_Port,BEE_Pin,a)
#define BEE_TOGGLE	HAL_GPIO_TogglePin(BEE_GPIO_Port, BEE_Pin)




#endif