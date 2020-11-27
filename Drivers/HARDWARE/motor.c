#include "motor.h"

int v1,v2,v3,v4 = 0;                              //4个电机的占空比
uint8_t motor_dead_width1=90;                     //1号电机死区
uint8_t motor_dead_width2=90;                     //2号电机死区
uint8_t motor_dead_width3=90;                     //3号电机死区
uint8_t motor_dead_width4=90;                     //4号电机死区

int x1_output=127;                                //左右平移输出量，范围[0，255]
int x1_increase=0;                                //左右平移输出增量，范围[0，127]，决定左右平移的速度
int y1_output=127;                                //前进后退输出量，范围[0，255]
int y1_increase=0;                                //前进后退输出增量，范围[0，127]，决定前进后退的速度
int z1_output=127;                                //旋转输出量，范围[0，255]
int z1_increase=0;                                //旋转输出增量，范围[0，127]，决定旋转的速度

/************************************************
函数名称 ： Reinit_XYZ_Output
函数功能 ： 复位前进后退、左右平移、旋转输出（增）量，防止模式切换后小车失控
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Reinit_XYZ_Output(void)
{
	x1_output=127;                                //重置左右平移输出量
	x1_increase=0;                                //重置左右平移输出增量
	y1_output=127;                                //重置前进后退输出量
	y1_increase=0;                                //重置前进后退输出增量
	z1_output=127;                                //重置旋转输出量
	z1_increase=0;                                //重置旋转输出增量
}

/************************************************
函数名称 ： Moto1
函数功能 ： 控制1，2号电机转速
参    数 ： L ------ 1号电机占空比
						R ------ 2号电机占空比
返 回 值 ： 无
*************************************************/
void Moto1(int L, int R)
{
	if (L > 0)
	{
		TIM4->CCR1 = LIMIT(L, 0, 999);
	}
	else
	{
		TIM4->CCR1 = LIMIT(-L, 0, 999);
	}

	if (R > 0)
	{
		TIM4->CCR2 = LIMIT(R, 0, 999);
	}
	else
	{
		TIM4->CCR2 = LIMIT(-R, 0, 999);
	}
}

/************************************************
函数名称 ： Moto2
函数功能 ： 控制3，4号电机转速
参    数 ： L ------ 3号电机占空比
						R ------ 4号电机占空比
返 回 值 ： 无
*************************************************/
void Moto2(int L, int R)
{
	if (L > 0)
	{
		TIM4->CCR3 = LIMIT(L, 0, 999);
	}
	else
	{
		TIM4->CCR3 = LIMIT(-L, 0, 999);
	}

	if (R > 0)
	{
		TIM4->CCR4 = LIMIT(R, 0, 999);
	}
	else
	{
		TIM4->CCR4 = LIMIT(-R, 0, 999);
	}
}

/************************************************
函数名称 ： Move_Control
函数功能 ： 麦轮运动合成控制
参    数 ： x ------ 平移输出量
            y ------ 前进后退输出量
            z ------ 转向输出量
返 回 值 ： 无
*************************************************/
void Move_Control(uint16_t x, uint16_t y ,uint16_t z) //运动控制
{
	if( (abs(x-128)>3) || abs(y-128)>3)             //大于遥控的摇杆死区再进行控制
	{
		v1= abs(y+x -256)*3  - z+128;                 //麦轮运动合成，得到四个电机的转速
		v2= abs(y-x )*3      + z-128;
		v3= abs(y+x -256)*3  + z-128;
		v4= abs(y-x )*3      - z+128;	
			
		v1=v1*1.25+motor_dead_width1;                 //适当增大占空比，并加上电机死区
		v2=v2*1.25+motor_dead_width2;
		v3=v3*1.25+motor_dead_width3;
		v4=v4*1.25+motor_dead_width4;
		
		Moto1(v1,v2);                                 //控制电机转速
		Moto2(v3,v4);	                                //控制电机转速
			
	  if(256-y-x<0 )                                //确定电机转向
    { 
			MOTOR1_F;		
      MOTOR3_B;
		} 
    if(256-y-x>0 )
    { 
	    MOTOR1_B;		
      MOTOR3_F;
    }
	  if(y - x>0)
    {
    	MOTOR2_B;		
			MOTOR4_F;
    }
    if(y - x<0)
    { 
    	MOTOR2_F;		
			MOTOR4_B;
    }	
	}
	else if( abs(z-128)>3)                          //大于遥控的摇杆死区再进行控制
	{
		if(z<128)                                     //转向运动控制
		{
			v1=v2=v3=v4=abs(z-128)*2.5;
			MOTOR1_B;
			MOTOR2_B;
			MOTOR3_B;
			MOTOR4_B;	
			v1=v1*1.25+motor_dead_width1;
			v2=v2*1.25+motor_dead_width2;
			v3=v3*1.25+motor_dead_width3;
			v4=v4*1.25+motor_dead_width4;
			Moto1(v1,v2);
			Moto2(v3,v4);			
		}
		else
		{
			v1=v2=v3=v4=abs(z-128)*2.5;
			MOTOR1_F;
			MOTOR2_F;
			MOTOR3_F;
			MOTOR4_F;
			v1=v1*1.25+motor_dead_width1;
			v2=v2*1.25+motor_dead_width2;
			v3=v3*1.25+motor_dead_width3;
			v4=v4*1.25+motor_dead_width4;
			Moto1(v1,v2);
			Moto2(v3,v4);
		}
	}
	else
	{
		Moto1(0,0);
		Moto2(0,0);
	}
}


