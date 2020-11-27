#ifndef __PID_H
#define __PID_H
#include "motor.h"

typedef struct
{
  float Kp;                       //比例系数Proportional
  float Ki;                       //积分系数Integral
  float Kd;                       //微分系数Derivative

  float Ek;                       //当前误差
  float Ek1;                      //前一次误差 e(k-1)
  float Ek2;                      //再前一次误差 e(k-2)
  float LocSum;                   //累计积分位置
}PID_LocTypeDef;                  //位置式PID结构体

typedef struct
{
  float Kp;                       //比例系数Proportional
  float Ki;                       //积分系数Integral
  float Kd;                       //微分系数Derivative

  float Ek;                       //当前误差
  float Ek1;                      //前一次误差 e(k-1)
  float Ek2;                      //再前一次误差 e(k-2)
}PID_IncTypeDef;                  //增量式PID结构体

extern PID_LocTypeDef AOA_PID;                       //跟随角度PID结构体
extern PID_LocTypeDef LT_ANGLE_PID;                  //航向角角度PID结构体
extern PID_LocTypeDef LT_DISTANCE_PID;               //LT距离PID结构体
extern PID_LocTypeDef LT_X_PID;                      //目标点追踪模式下最后收敛x轴PID结构体
extern PID_LocTypeDef LT_Y_PID;                      //目标点追踪模式下最后收敛y轴PID结构体

float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID); //位置式PID函数
float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID); //增量式PID函数

void PID_Init(void);              //PID初始化函数

#endif


