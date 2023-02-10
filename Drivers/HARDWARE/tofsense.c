#include "tofsense.h"

uint8_t u_rx_buf_3[16];                  //usart3接收缓存数组
uint8_t TOF_unpack_correct_flag=0;       //TOFSense一帧数据解包正确标志位
uint8_t TOF_DMA_receive_flag=0;          //DMA接收一帧TOFSense协议数据完成标志位
uint8_t TOF_data_length=0;               //DMA接收一帧TOFSense协议数据长度
uint8_t u_tx_buf_3[8]={0x57,0x10,0xff,0xff,0x00,0xff,0xff,0x63}; //usart3发送缓存数组，用于查询级联的TOF
uint8_t TOF_inquire_cycle_count=0;       //TOFSense模块串口查询模式查询周期计数变量（5ms查询一次）
uint8_t TOF_inquire_number=0;            //TOFSense模块串口查询模式当前查询的模块编号
float TOF_FR_dis0=0.0;                   //0号（前右）TOFSense模块距离
float TOF_FM_dis1=0.0;                   //1号（前中）TOFSense模块距离
float TOF_FL_dis2=0.0;                   //2号（前左）TOFSense模块距离
float TOF_BL_dis3=0.0;                   //3号（后左）TOFSense模块距离
float TOF_BM_dis4=0.0;                   //4号（后中）TOFSense模块距离
float TOF_BR_dis5=0.0;                   //5号（后右）TOFSense模块距离
uint8_t TOF_status0=0;                   //0号TOFSense模块距离状态指示
uint8_t TOF_status1=0;                   //1号TOFSense模块距离状态指示
uint8_t TOF_status2=0;                   //2号TOFSense模块距离状态指示
uint8_t TOF_status3=0;                   //3号TOFSense模块距离状态指示
uint8_t TOF_status4=0;                   //4号TOFSense模块距离状态指示
uint8_t TOF_status5=0;                   //5号TOFSense模块距离状态指示
uint16_t TOF_signal_strength0=0;         //0号TOFSense模块距离信号强度
uint16_t TOF_signal_strength1=0;         //1号TOFSense模块距离信号强度
uint16_t TOF_signal_strength2=0;         //2号TOFSense模块距离信号强度
uint16_t TOF_signal_strength3=0;         //3号TOFSense模块距离信号强度
uint16_t TOF_signal_strength4=0;         //4号TOFSense模块距离信号强度
uint16_t TOF_signal_strength5=0;         //5号TOFSense模块距离信号强度
uint16_t TOF_signal_strength=2;          //信号强度阈值，距离信号强度大于等于这个数值才执行避障操作
	
uint8_t avoid_danger_switch=1;           //避障模式开关，0关1开
uint8_t avoid_danger_status=0;           //避障模式状态，1表示处于避障算法控制中
float danger_distance=0.37;              //避障距离阈值,单位：米
float slow_down_distance=1.0;            //避障减速缓冲区距离阈值,单位：米
uint8_t avoid_danger_slow_down_speed=35; //避障减速缓冲区减速速度，范围[0,127]
uint8_t avoid_danger_shift_speed=100;    //避障开环控制横移速度，范围[0,127]
uint16_t avoid_danger_shift_time=0;      //避障开环控制横移时间，单位：ms
uint8_t avoid_danger_turn_flag=0;        //前置3TOF距离都小于避障阈值情况下开始转圈标志位
uint16_t avoid_danger_turn_count=0;      //前置3TOF距离都小于避障阈值情况下开始转圈计数变量

/************************************************
函数名称 ： TOF_Application
函数功能 ： TOFSense应用函数
参    数 ： 无
返 回 值 ： 无
*************************************************/
void TOF_Application(void)
{
	TOF_unpack_correct_flag=g_nts_frame0.UnpackData(u_rx_buf_3,TOF_data_length);//调用函数指针运行解码函数并返回解码正确/失败值
	if(TOF_unpack_correct_flag == 1)                                            //如果解码正确
	{
		if(g_nts_frame0.result.id == 0)                                           //如果该帧是编号0的TOFSense模块数据
		{		
			TOF_signal_strength0=g_nts_frame0.result.signal_strength;               //取TOFSense距离信号强度
			TOF_status0=g_nts_frame0.result.dis_status;                             //取TOFSense距离状态值
			TOF_FR_dis0=g_nts_frame0.result.dis;                                    //取TOFSense测量距离值
		}
		else if(g_nts_frame0.result.id == 1)                                      //如果该帧是编号1的TOFSense模块数据
		{				
			TOF_signal_strength1=g_nts_frame0.result.signal_strength;               //取TOFSense距离信号强度
			TOF_status1=g_nts_frame0.result.dis_status;                             //取TOFSense距离状态值
			TOF_FM_dis1=g_nts_frame0.result.dis;                                  	//取TOFSense测量距离值
		}
		else if(g_nts_frame0.result.id == 2)                                      //如果该帧是编号2的TOFSense模块数据
		{	
			TOF_signal_strength2=g_nts_frame0.result.signal_strength;               //取TOFSense距离信号强度
			TOF_status2=g_nts_frame0.result.dis_status;                             //取TOFSense距离状态值
			TOF_FL_dis2=g_nts_frame0.result.dis;                                   	//取TOFSense测量距离值
		}
		else if(g_nts_frame0.result.id == 3)                                      //如果该帧是编号3的TOFSense模块数据
		{		
			TOF_signal_strength3=g_nts_frame0.result.signal_strength;               //取TOFSense距离信号强度
			TOF_status3=g_nts_frame0.result.dis_status;                             //取TOFSense距离状态值
			TOF_BL_dis3=g_nts_frame0.result.dis;                                 		//取TOFSense测量距离值
		}		
		else if(g_nts_frame0.result.id == 4)                                      //如果该帧是编号4的TOFSense模块数据
		{	
			TOF_signal_strength4=g_nts_frame0.result.signal_strength;               //取TOFSense距离信号强度
			TOF_status4=g_nts_frame0.result.dis_status;                             //取TOFSense距离状态值
			TOF_BM_dis4=g_nts_frame0.result.dis;                                 		//取TOFSense测量距离值
		}
		else if(g_nts_frame0.result.id == 5)                                      //如果该帧是编号5的TOFSense模块数据
		{				
			TOF_signal_strength5=g_nts_frame0.result.signal_strength;               //取TOFSense距离信号强度
			TOF_status5=g_nts_frame0.result.dis_status;                             //取TOFSense距离状态值
			TOF_BR_dis5=g_nts_frame0.result.dis;                                    //取TOFSense测量距离值
		}				
	}
		
}

/************************************************
函数名称 ： Avoid_Danger
函数功能 ： TOFSense避障函数
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Avoid_Danger(void)
{
	if((TOF_FR_dis0<danger_distance)&&(TOF_FM_dis1<danger_distance)&&(TOF_FL_dis2<danger_distance)
			&&(TOF_signal_strength0>TOF_signal_strength)&&(TOF_signal_strength1>TOF_signal_strength)
			&&(TOF_signal_strength2>TOF_signal_strength)&&(TOF_status0 != 255)&&(TOF_status1 != 255)&&(TOF_status2 != 255))//（V1.0.1增加距离状态指示判断）如果前右、前中、前左TOF传感器距离小于避障阈值且对应信号强度大于阈值
	{
		if(avoid_danger_turn_flag == 0)                                                  //如果第一次出现3TOF距离小于阈值情况
		{
			Move_Control( 127, 127, 127-avoid_danger_shift_speed); 											   //避障运动控制（逆时针转）
			avoid_danger_turn_flag=1;                                                      //标志位置1，在滴答定时器内延时xms
		}
		else if(avoid_danger_turn_flag == 2)                                             //如果滴答定时器定时xms结束3个TOF还是小于阈值（说明这个方向也是墙）
		{
			Move_Control( 127, 127, 127+avoid_danger_shift_speed); 											   //避障运动控制（顺时针转）
		}
	}
	else if((TOF_FR_dis0<danger_distance)&&(TOF_FM_dis1<danger_distance)&&
			(TOF_signal_strength0>TOF_signal_strength)&&(TOF_signal_strength1>TOF_signal_strength)&&
			(TOF_status0 != 255)&&(TOF_status1 != 255)) //如果前右、前中TOF传感器距离小于避障阈值且对应信号强度大于阈值
	{
		Move_Control( 127-avoid_danger_shift_speed, 127, 127);                           //避障运动控制
	}
	else if((TOF_FM_dis1<danger_distance)&&(TOF_FL_dis2<danger_distance)&&
			(TOF_signal_strength1>TOF_signal_strength)&&(TOF_signal_strength2>TOF_signal_strength)&&
			(TOF_status1 != 255)&&(TOF_status2 != 255)) //如果前中、前左TOF传感器距离小于避障阈值且对应信号强度大于阈值
	{
		Move_Control( 127+avoid_danger_shift_speed, 127, 127);                           //避障运动控制
	}
	else if((TOF_FR_dis0<danger_distance)&&(TOF_FL_dis2<danger_distance)&&
			(TOF_signal_strength0>TOF_signal_strength)&&(TOF_signal_strength2>TOF_signal_strength)&&
			(TOF_status0 != 255)&&(TOF_status2 != 255)) //如果前右、前左TOF传感器距离小于避障阈值且对应信号强度大于阈值
	{
		Move_Control( 127-avoid_danger_shift_speed, 127, 127); 												   //避障运动控制
	}
	else if((TOF_FR_dis0<danger_distance)&&(TOF_signal_strength0>TOF_signal_strength)&&(TOF_status0 != 255)) //如果前右TOF传感器距离小于避障阈值且对应信号强度大于阈值
	{
		Move_Control( 127-avoid_danger_shift_speed, 127, 127);                           //避障运动控制
		avoid_danger_shift_time=AVOID_DANGER_SHIFT_SHORT_TIME;                           //横向开环控制一段较短时间
		if(avoid_danger_turn_flag == 2)                                                  //如果3TOF距离都小于阈值，开始顺时针旋转后发现一侧距离大于阈值
		{
			avoid_danger_turn_flag=0;                                                      //清零转向标志位，按照横移避障逻辑避障
		}
		else if(avoid_danger_turn_flag == 1)                                             //如果3TOF距离都小于阈值，开始逆时针旋转后发现一侧距离大于阈值
		{
			avoid_danger_turn_flag=0;                                                      //清零转向标志位，按照横移避障逻辑避障
			avoid_danger_turn_count=0;                                                     //清零转向计数变量，按照横移避障逻辑避障
		}
	}
	else if((TOF_FM_dis1<danger_distance)&&(TOF_signal_strength1>TOF_signal_strength)&&(TOF_status1 != 255)) //如果前中TOF传感器距离小于避障阈值且对应信号强度大于阈值
	{
		Move_Control( 127-avoid_danger_shift_speed, 127, 127); 												   //避障运动控制
	}
	else if((TOF_FL_dis2<danger_distance)&&(TOF_signal_strength2>TOF_signal_strength)&&(TOF_status2 != 255)) //如果前左TOF传感器距离小于避障阈值且对应信号强度大于阈值
	{
		Move_Control( 127+avoid_danger_shift_speed, 127, 127);                           //避障运动控制
		avoid_danger_shift_time=AVOID_DANGER_SHIFT_SHORT_TIME;                           //横向开环控制一段较短时间
		if(avoid_danger_turn_flag == 2)                                                  //如果3TOF距离都小于阈值，开始顺时针旋转后发现一侧距离大于阈值
		{
			avoid_danger_turn_flag=0;                                                      //清零转向标志位，按照横移避障逻辑避障
		}
		else if(avoid_danger_turn_flag == 1)                                             //如果3TOF距离都小于阈值，开始逆时针旋转后发现一侧距离大于阈值
		{
			avoid_danger_turn_flag=0;                                                      //清零转向标志位，按照横移避障逻辑避障
			avoid_danger_turn_count=0;                                                     //清零转向计数变量，按照横移避障逻辑避障
		}
	}										
	
}


