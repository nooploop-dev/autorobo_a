#ifndef __TOFSENSE_H
#define __TOFSENSE_H
#include "main.h"

#define AVOID_DANGER_SHIFT_SHORT_TIME 130     //较短的开环避障控制时间，单位ms
#define AVOID_DANGER_SHIFT_MEDIUM_TIME 300    //适中的开环避障控制时间，单位ms
#define AVOID_DANGER_SHIFT_LONG_TIME 500      //较长的开环避障控制时间，单位ms

#define TOF_M_POINT_GROUP_NUM 16              //TOF-M系列64点位分组左中右组每组的点位数量

void Avoid_Danger(void);                      //TOFSense避障函数
void TOF_Application(void);                   //TOFSense应用函数

extern uint8_t TOF_series_switch;             //当前使用的TOF激光传感器系列，0为TOFSense系列，1为TOFSense-F系列，2为TOFSense-M系列，根据协议功能码自动识别，F系列不支持级联，暂不支持

extern uint8_t u_rx_buf_3[1024];              //usart3接收缓存数组
extern uint8_t u_tx_buf_3[8];                 //usart3发送缓存数组，用于查询级联的TOF

extern uint8_t TOF_unpack_correct_flag;       //TOFSense一帧数据解包正确标志位
extern uint8_t TOF_DMA_receive_flag;          //DMA接收一帧TOFSense协议数据完成标志位
extern uint16_t TOF_data_length;              //DMA接收一帧TOFSense协议数据长度

extern uint8_t TOF_inquire_cycle_count;       //TOFSense模块串口查询模式查询周期计数变量（5ms查询一次）
extern uint8_t TOF_inquire_number;            //TOFSense模块串口查询模式当前查询的模块编号

extern float TOF_FR_dis0;                   	//0号（前右）TOFSense模块距离
extern float TOF_FM_dis1;                   	//1号（前中）TOFSense模块距离
extern float TOF_FL_dis2;                   	//2号（前左）TOFSense模块距离
extern float TOF_BL_dis3;                   	//3号（后左）TOFSense模块距离
extern float TOF_BM_dis4;                  	  //4号（后中）TOFSense模块距离
extern float TOF_BR_dis5;                  	  //5号（后右）TOFSense模块距离
extern uint8_t TOF_status0;                   //0号TOFSense模块距离状态指示
extern uint8_t TOF_status1;                   //1号TOFSense模块距离状态指示
extern uint8_t TOF_status2;                   //2号TOFSense模块距离状态指示
extern uint8_t TOF_status3;                   //3号TOFSense模块距离状态指示
extern uint8_t TOF_status4;                   //4号TOFSense模块距离状态指示
extern uint8_t TOF_status5;                   //5号TOFSense模块距离状态指示
extern uint16_t TOF_signal_strength0;         //0号TOFSense模块距离信号强度
extern uint16_t TOF_signal_strength1;         //1号TOFSense模块距离信号强度
extern uint16_t TOF_signal_strength2;         //2号TOFSense模块距离信号强度
extern uint16_t TOF_signal_strength3;         //3号TOFSense模块距离信号强度
extern uint16_t TOF_signal_strength4;         //4号TOFSense模块距离信号强度
extern uint16_t TOF_signal_strength5;         //5号TOFSense模块距离信号强度
extern uint16_t TOF_signal_strength;          //信号强度阈值，距离信号强度大于等于这个数值才执行避障操作
	
extern uint8_t avoid_danger_switch;           //避障模式开关，0关1开
extern uint8_t avoid_danger_status;           //避障模式状态，1表示处于避障算法控制中
extern float danger_distance;                 //避障距离阈值,单位：米
extern float slow_down_distance;            	//避障减速缓冲区距离阈值,单位：米
extern uint8_t avoid_danger_slow_down_speed;  //避障减速缓冲区减速速度，范围[0,127]
extern uint8_t avoid_danger_shift_speed;    	//避障开环控制横移速度，范围[0,127]
extern uint16_t avoid_danger_shift_time;      //避障开环控制横移时间，单位：ms
extern uint8_t avoid_danger_turn_flag;        //前置3TOF距离都小于避障阈值情况下开始转圈标志位
extern uint16_t avoid_danger_turn_count;      //前置3TOF距离都小于避障阈值情况下开始转圈计数变量


#endif



