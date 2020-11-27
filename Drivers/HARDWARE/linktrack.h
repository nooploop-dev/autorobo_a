#ifndef __LINKTRACK_H
#define __LINKTRACK_H
#include "main.h"

void LT_Application(void);                    //LinkTrack定位应用函数

extern uint8_t u_rx_buf_1[256];               //usart1接收缓存数组	
extern uint8_t LT_unpack_correct_flag;        //LinkTrack一帧数据解包正确标志位	
extern uint8_t LT_DMA_receive_flag;           //DMA接收一帧LinkTrack协议（Node_Frame2）数据完成标志位
extern uint16_t LT_data_length;               //DMA接收一帧LinkTrack协议（Node_Frame2）数据长度	
	
extern uint8_t LT_tag_id;                     //标签的ID
extern uint32_t LT_local_time;                //LT标签的本地时间
extern uint32_t LT_temp_local_time;           //LT标签的本地时间临时储存变量，用于计算小车平均速度
extern float LT_pos_x;                      	//LinkTrack输出的x坐标
extern float LT_pos_y;                      	//LinkTrack输出的y坐标
extern float LT_pos_z;                      	//LinkTrack输出的z坐标
extern float LT_expect_pos_x;               	//期望的x坐标
extern float LT_expect_pos_y;               	//期望的y坐标
extern float anchor_to_east_angle;       			//基站坐标系与东方向夹角（小车车头朝向A0到A3连线方向测得）
extern float now_to_target_angle;           	//当前坐标点到目标坐标点的连线与x轴夹角
extern float forward_to_xaxis_angle;        	//当前小车轴线（前进方向）与x轴夹角
extern float forward_to_target_angle;       	//当前小车轴线（前进方向）与当前坐标到目标坐标连线的夹角	
extern uint8_t LT_start_flag;                 //设定期望坐标后的起步标志位
extern float yaw_expect;                    	//航向角期望值
extern float LT_dis_expect;                 	//与目标坐标点之间距离的期望值
extern float LT_dis_measure;                	//与目标坐标点之间距离的测量值，由坐标三角函数求得
extern float distance_target_range;         	//距离目标范围，单位：米	
extern uint8_t LT_DT_data_length;             //LinkTrack标签接收到的数传数据长度

extern float anchor_Vx;                       //沿基站坐标系x轴方向速度分量
extern float anchor_Vy;                       //沿基站坐标系y轴方向速度分量

#endif



