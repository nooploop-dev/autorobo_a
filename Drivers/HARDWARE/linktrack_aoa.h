#ifndef __LINKTRACK_AOA_H
#define __LINKTRACK_AOA_H
#include "main.h"

extern uint8_t u_rx_buf_2[256];               //usart2接收缓存数组
extern uint8_t AOA_unpack_correct_flag;       //AOA一帧数据解包正确标志位
extern uint8_t AOA_DMA_receive_flag;          //DMA接收一帧AOA协议数据完成标志位
extern uint16_t AOA_data_length;              //DMA接收一帧AOA协议数据长度
extern uint8_t AOA_valid_node_count;          //AOA可用节点数，用于判断标签突然断电情况
extern float angle_target_range;            	//双环电机消抖中目标环范围，角度绝对值小于目标环时进入死区环判断
extern float angle_dead_zone_range;         	//双环电机消抖中死区环范围，角度绝对值小于死区环时输出为0
extern uint8_t angle_dead_zone_range_flag;    //进入死区环标志位

extern float angle_measure;                   //角度测量值
extern float angle_expect;                    //零偏值，角度跟随期望值

extern float dis_measure;                     //距离测量值
extern float dis_expect;                      //区间距离跟随暂时不采用PID，该值为区间中心，单位：m
extern float dis_range;                       //区间距离跟随的区间大小，单位：m

void AOA_Application(void);                   //AOA跟随应用函数

#endif


