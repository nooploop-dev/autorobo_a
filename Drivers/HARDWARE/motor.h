#ifndef __MOTOR_H
#define __MOTOR_H
#include "main.h"

#define MOTOR1_F HAL_GPIO_WritePin(Motor1__Direction_GPIO_Port,Motor1__Direction_Pin,GPIO_PIN_RESET)      // PD7  1号电机正转
#define MOTOR1_B HAL_GPIO_WritePin(Motor1__Direction_GPIO_Port,Motor1__Direction_Pin,GPIO_PIN_SET)    		// PD7  1号电机反转
#define MOTOR2_F HAL_GPIO_WritePin(Motor2__Direction_GPIO_Port,Motor2__Direction_Pin,GPIO_PIN_RESET)      // PB3  2号电机正转
#define MOTOR2_B HAL_GPIO_WritePin(Motor2__Direction_GPIO_Port,Motor2__Direction_Pin,GPIO_PIN_SET)    		// PB3  2号电机反转
#define MOTOR3_F HAL_GPIO_WritePin(Motor3__Direction_GPIO_Port,Motor3__Direction_Pin,GPIO_PIN_RESET)      // PB4  3号电机正转
#define MOTOR3_B HAL_GPIO_WritePin(Motor3__Direction_GPIO_Port,Motor3__Direction_Pin,GPIO_PIN_SET)    		// PB4  3号电机反转
#define MOTOR4_F HAL_GPIO_WritePin(Motor4__Direction_GPIO_Port,Motor4__Direction_Pin,GPIO_PIN_RESET)      // PB5  4号电机正转
#define MOTOR4_B HAL_GPIO_WritePin(Motor4__Direction_GPIO_Port,Motor4__Direction_Pin,GPIO_PIN_SET)   			// PB5  4号电机反转

#define LIMIT(x, min, max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )                       //占空比限幅

extern int x1_output;                                //左右平移输出量，范围[0，255]
extern int x1_increase;                              //左右平移输出增量，范围[0，127]，决定左右平移的速度
extern int y1_output;                                //前进后退输出量，范围[0，255]
extern int y1_increase;                              //前进后退输出增量，范围[0，127]，决定前进后退的速度
extern int z1_output;                                //旋转输出量，范围[0，255]
extern int z1_increase;                              //旋转输出增量，范围[0，127]，决定旋转的速度

void Moto1(int L, int R);
void Moto2(int L, int R);
void Move_Control(uint16_t x, uint16_t y , uint16_t z);
void Reinit_XYZ_Output(void);

#endif

