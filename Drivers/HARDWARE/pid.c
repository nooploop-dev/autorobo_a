#include "pid.h"

PID_LocTypeDef AOA_PID;                           //跟随角度PID结构体
PID_LocTypeDef LT_ANGLE_PID;                      //航向角角度PID结构体
PID_LocTypeDef LT_DISTANCE_PID;                   //LT距离PID结构体
PID_LocTypeDef LT_X_PID;                          //目标点追踪模式下最后收敛x轴PID结构体
PID_LocTypeDef LT_Y_PID;                          //目标点追踪模式下最后收敛y轴PID结构体

/************************************************
函数名称 ： PID_Init
函数功能 ： PID参数初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
void PID_Init(void)
{
	AOA_PID.Kp=2.5;                                 //2.0到3.0
	AOA_PID.Ki=0.0;                                 
	AOA_PID.Kd=-16.5;                               //-16    
	
	LT_ANGLE_PID.Kp=2.5;                                 
	LT_ANGLE_PID.Ki=0.0;                                 
	LT_ANGLE_PID.Kd=-16.5;   

	LT_DISTANCE_PID.Kp=20;                          //20       
	LT_DISTANCE_PID.Ki=0.0;                                 
	LT_DISTANCE_PID.Kd=0.0;   
	
	LT_X_PID.Kp=25;                                 //20       
	LT_X_PID.Ki=0.0;                                 
	LT_X_PID.Kd=0.0;  
	
	LT_Y_PID.Kp=25;                                 //20       
	LT_Y_PID.Ki=0.0;                                 
	LT_Y_PID.Kd=0.0;  
}


/************************************************
函数名称 ： PID_Loc
函数功能 ： PID位置(Location)计算
参    数 ： SetValue ------ 设置值(期望值)
            ActualValue --- 实际值(反馈值)
            PID ----------- PID数据结构
返 回 值 ： PIDLoc -------- PID位置
*************************************************/
float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID)
{
  float PIDLoc;                                   //位置

  PID->Ek = SetValue - ActualValue;               // 计算当前误差
  PID->LocSum += PID->Ek;                         //累计误差

  PIDLoc = PID->Kp * PID->Ek + (PID->Ki * PID->LocSum) + PID->Kd * (PID->Ek1 - PID->Ek);

  PID->Ek1 = PID->Ek;                             // 更新上次误差  
	
	return PIDLoc;
}


/************************************************
函数名称 ： PID_Inc
函数功能 ： PID增量(Increment)计算
参    数 ： SetValue ------ 设置值(期望值)
            ActualValue --- 实际值(反馈值)
            PID ----------- PID数据结构
返 回 值 ： PIDInc -------- 本次PID增量(+/-)
*************************************************/
float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID)
{
  float PIDInc;                                   //输出增量

  PID->Ek = SetValue - ActualValue;               // 计算当前误差
  PIDInc = (PID->Kp * PID->Ek) - (PID->Ki * PID->Ek1) + (PID->Kd * PID->Ek2);

  PID->Ek2 = PID->Ek1;                            // 更新上上次误差
  PID->Ek1 = PID->Ek;                             // 更新上次误差
	
	return PIDInc;
}




