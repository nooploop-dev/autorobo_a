#ifndef __WT901C_H
#define __WT901C_H
#include "main.h"

void WTIMU_Application(void);                 //外置9轴IMU应用函数

extern uint8_t u_rx_buf_4[50];                //usart4接收缓存数组	
extern uint8_t WTIMU_unpack_correct_flag;     //外置9轴IMU一帧数据解包正确标志位	
extern uint8_t WTIMU_DMA_receive_flag;        //DMA接收一帧外置9轴IMU协议数据完成标志位
extern uint16_t WTIMU_data_length;            //DMA接收一帧外置9轴IMU协议数据长度
extern uint8_t loop_count;                    //循环计数变量
extern uint16_t loop_sum;                     //for循环中的求和变量
extern float WT_IMU_pitch;                  	//外置9轴IMU输出的俯仰角x
extern float WT_IMU_roll;                   	//外置9轴IMU输出的横滚角y
extern float WT_IMU_yaw;                    	//外置9轴IMU输出的航向角z
extern float yaw_target_range;              	//目标环范围，角度绝对值小于目标环时进入死区环判断
extern float yaw_dead_zone_range;           	//死区环范围，角度绝对值小于死区环时输出为0
extern uint8_t yaw_dead_zone_range_flag;      //进入死区环标志位


#endif


