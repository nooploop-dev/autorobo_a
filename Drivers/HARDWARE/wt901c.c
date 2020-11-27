#include "wt901c.h"

uint8_t u_rx_buf_4[50];                  //usart4接收缓存数组	
uint8_t WTIMU_unpack_correct_flag=0;     //外置9轴IMU一帧数据解包正确标志位	
uint8_t WTIMU_DMA_receive_flag=0;        //DMA接收一帧外置9轴IMU协议数据完成标志位
uint16_t WTIMU_data_length=0;            //DMA接收一帧外置9轴IMU协议数据长度
uint8_t loop_count=0;                    //循环计数变量
uint16_t loop_sum=0;                     //for循环中的求和变量
float WT_IMU_pitch=0.0;                  //外置9轴IMU输出的俯仰角x
float WT_IMU_roll=0.0;                   //外置9轴IMU输出的横滚角y
float WT_IMU_yaw=0.0;                    //外置9轴IMU输出的航向角z
float yaw_target_range=2.0;              //目标环范围，角度绝对值小于目标环时进入死区环判断
float yaw_dead_zone_range=9.0;           //死区环范围，角度绝对值小于死区环时输出为0
uint8_t yaw_dead_zone_range_flag=0;      //进入死区环标志位

/************************************************
函数名称 ： WTIMU_Application
函数功能 ： 接收和解码外置9轴IMU的数据
参    数 ： 无
返 回 值 ： 无
*************************************************/
void WTIMU_Application(void)
{
	for(loop_count=22;loop_count<32;loop_count++)
	{
		loop_sum+=u_rx_buf_4[loop_count];                                         //计算9轴IMU输出角度的校验和
	}
	if((uint8_t)loop_sum == u_rx_buf_4[32])                                     //如果算出的校验和的低字节等于协议中的校验和，说明解码正确
	{
		WT_IMU_yaw=(float)(((uint16_t)u_rx_buf_4[29]<<8)|(uint16_t)u_rx_buf_4[28])/32768*180; //取9轴IMU输出的航向角
	}
	loop_sum=0;                                                                 //清零求和变量
	if((WT_IMU_yaw>=0)&&(WT_IMU_yaw<90))                                        //坐标系夹角转换，转换后东为0度，北为90度，西为180（-180度）
	{
		WT_IMU_yaw+=90;
	}
	else
	{
		WT_IMU_yaw-=270;
	}
	if(anchor_to_east_angle>0)
	{
		if((WT_IMU_yaw<=anchor_to_east_angle-180)&&(WT_IMU_yaw>-180))             //坐标系夹角转换，转换后A0到A3方向为0度
		{
			WT_IMU_yaw+=360-anchor_to_east_angle;
		}
		else
		{
			WT_IMU_yaw-=anchor_to_east_angle;
		}
	}
	else if(anchor_to_east_angle<0)
	{
		if((WT_IMU_yaw>=180+anchor_to_east_angle)&&(WT_IMU_yaw<180))              //坐标系夹角转换，转换后A0到A3方向为0度
		{
			WT_IMU_yaw-=360+anchor_to_east_angle;
		}
		else
		{
			WT_IMU_yaw-=anchor_to_east_angle;
		}
	}
	
}
	