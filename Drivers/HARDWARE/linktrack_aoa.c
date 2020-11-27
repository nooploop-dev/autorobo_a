#include "linktrack_aoa.h"

uint8_t u_rx_buf_2[256];                 //usart2接收缓存数组
uint8_t AOA_unpack_correct_flag=0;       //AOA一帧数据解包正确标志位
uint8_t AOA_DMA_receive_flag=0;          //DMA接收一帧AOA协议数据完成标志位
uint16_t AOA_data_length=0;              //DMA接收一帧AOA协议数据长度
uint8_t AOA_valid_node_count=0;          //AOA可用节点数，用于判断标签突然断电情况
float angle_target_range=2.0;            //双环电机消抖中目标环范围，角度绝对值小于目标环时进入死区环判断
float angle_dead_zone_range=9.0;         //双环电机消抖中死区环范围，角度绝对值小于死区环时输出为0
uint8_t angle_dead_zone_range_flag=0;    //进入死区环标志位

float angle_measure=0.0;                 //角度测量值
float angle_expect=0.0;                  //零偏值，角度跟随期望值

float dis_measure=0.0;                   //距离测量值
float dis_expect=0.7;                    //区间距离跟随暂时不采用PID，该值为区间中心，单位：m
float dis_range=0.4;                     //区间距离跟随的区间大小，单位：m

uint8_t follow_me_speed=90;              //初始跟随速度，范围[0,127]

/************************************************
函数名称 ： AOA_Application
函数功能 ： AOA跟随应用函数
参    数 ： 无
返 回 值 ： 无
*************************************************/
void AOA_Application(void)
{
	AOA_unpack_correct_flag=g_nltaoa_nodeframe0.UnpackData(u_rx_buf_2,AOA_data_length); //调用函数指针运行解码函数并返回解码正确/失败值
	if((g_nltaoa_nodeframe0.result.nodes[0] != 0)&&(AOA_unpack_correct_flag == 1)) //如果nodes[0]数据有效且解码正确
	{
		angle_measure=g_nltaoa_nodeframe0.result.nodes[0]->angle;                 //取AOA测量角度值
		dis_measure=g_nltaoa_nodeframe0.result.nodes[0]->dis;                     //取AOA测量距离值
		AOA_valid_node_count=g_nltaoa_nodeframe0.result.valid_node_count;         //取AOA可用节点数
	}					
	if((mode_selection == 1)||(mode_selection == 2))                            //如果处于角度跟随或角度距离全跟随模式
	{
		/*跟随模式处理代码 开始*/
		z1_increase = (int)PID_Loc(angle_expect, angle_measure, &AOA_PID);        //PID算出输出量
		if(fabsf(angle_measure)<angle_target_range)                               //判断角度测量值是否达到了目标环内
		{
			angle_dead_zone_range_flag=1;                                           //如果达到目标环，进入死区环判断并置位标志位
		}
		if(angle_dead_zone_range_flag == 1)                                       //如果死区环标志位为1
		{
			if(fabsf(angle_measure)<angle_dead_zone_range)                          //如果角度波动范围小于死区环范围
			{
				z1_increase=0;                                                        //输出改为0
			}
			else
			{
				angle_dead_zone_range_flag=0;                                         //如果角度波动范围大于死区环范围，标志位清零
			}
		}
		if(z1_increase>127)                                                       //输出限幅
		{
			z1_increase=127;
		}
		else if(z1_increase<-127)
		{
			z1_increase=-127;
		}
		z1_output = z1_increase+127;                                              //调整输出量范围
				
		if(AOA_valid_node_count>0)                                                //如果可用节点数大于0再控制，防止标签突然断电小车失控
		{
			if(mode_selection == 1)                                                 //如果处于角度跟随模式
			{
				Move_Control( 127, 127, z1_output);                                   //角度跟随模式下根据PID输出值进行转向运动控制
			}
			else if(mode_selection == 2)                                            //如果处于角度距离全跟随模式
			{
				y1_increase=follow_me_speed;                                          //初始跟随速度
				if(avoid_danger_switch == 1)                                          //如果开启了避障模式
				{
					if((dis_measure>=(dis_expect+dis_range))||(dis_measure<=(dis_expect-dis_range))) //在距离跟随的时候才进行避障
					{ 
						if(((TOF_FR_dis0<danger_distance)&&(TOF_signal_strength0>TOF_signal_strength))||
								((TOF_FM_dis1<danger_distance)&&(TOF_signal_strength1>TOF_signal_strength))||
								((TOF_FL_dis2<danger_distance)&&(TOF_signal_strength2>TOF_signal_strength)))
						{
							avoid_danger_status=1;                                          //如果符合避障的条件，避障控制标志位置1
						}
					}
							
					if(avoid_danger_status == 1)                                        //如果避障控制标志位为1，开始避障控制
					{
						Avoid_Danger();                                                   //调用避障函数
					}
					else                                                                //如果避障控制标志位为0，开始常规跟随控制
					{
						if(((TOF_FR_dis0<slow_down_distance)&&(TOF_signal_strength0>TOF_signal_strength))||
								((TOF_FM_dis1<slow_down_distance)&&(TOF_signal_strength1>TOF_signal_strength))||
								((TOF_FL_dis2<slow_down_distance)&&(TOF_signal_strength2>TOF_signal_strength))) 
						{
							y1_increase=avoid_danger_slow_down_speed;                       //如果有一个传感器距离小于减速缓冲区阈值且相应的信号强度大于阈值，减速
						}
						else
						{
							y1_increase=follow_me_speed;                                    //恢复初始跟随速度
						}
						if(dis_measure>=(dis_expect+dis_range)) 													//如果测量距离值大于等于范围值上限
						{
							Move_Control( 127, y1_output-y1_increase, z1_output);           //角度距离全跟随模式下运动控制
						}
						else if(dis_measure<=(dis_expect-dis_range)) 										  //如果测量距离值小于等于范围值下限
						{
							Move_Control( 127, y1_output+y1_increase, z1_output);           //角度距离全跟随模式下运动控制
						}
						else
						{
							Move_Control( 127, y1_output, z1_output);                       //距离跟随范围内只进行角度跟随
						}
					}
				}
				else                                                                  //如果关闭了避障模式
				{
					if(dis_measure>=(dis_expect+dis_range)) 														//如果测量距离值大于等于范围值上限
					{
						Move_Control( 127, y1_output-y1_increase, z1_output);             //角度距离全跟随模式下运动控制
					}
					else if(dis_measure<=(dis_expect-dis_range)) 												//如果测量距离值小于等于范围值下限
					{
						Move_Control( 127, y1_output+y1_increase, z1_output);             //角度距离全跟随模式下运动控制
					}
					else
					{
						Move_Control( 127, y1_output, z1_output);                         //距离跟随范围内只进行角度跟随
					}
				}
						
			}
		}
		else
		{
			Move_Control( 127, 127, 127);                        							      //如果可用节点数=0，小车保持不动
		}
				
		/*跟随模式处理代码 结束*/
	}
}
