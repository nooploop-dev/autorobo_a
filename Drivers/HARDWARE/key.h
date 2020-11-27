#ifndef __KEY_H
#define __KEY_H
#include "stm32h7xx.h"

#define READ_KEY1	HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)

extern uint8_t key_delay_count;               //按键软件防抖延时计数变量
extern uint8_t mode_selection;                //模式切换变量 0：遥控模式  1：角度跟随模式  2：角度距离全跟随模式  3：定位目标点移动模式


#endif

