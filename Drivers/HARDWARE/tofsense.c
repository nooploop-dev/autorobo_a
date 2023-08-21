#include "tofsense.h"

uint8_t TOF_series_switch=0;             //当前使用的TOF激光传感器系列，0为TOFSense系列，1为TOFSense-F系列，2为TOFSense-M系列，根据协议功能码自动识别，F系列不支持级联，暂不支持

uint8_t u_rx_buf_3[1024];                //usart3接收缓存数组
uint8_t u_tx_buf_3[8]={0x57,0x10,0xff,0xff,0x00,0xff,0xff,0x63}; //usart3发送缓存数组，用于查询级联的TOF

uint8_t TOF_unpack_correct_flag=0;       //TOFSense一帧数据解包正确标志位
uint8_t TOF_DMA_receive_flag=0;          //DMA接收一帧TOFSense协议数据完成标志位
uint16_t TOF_data_length=0;              //DMA接收一帧TOFSense协议数据长度

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

uint8_t TOF_M_left_point_group[TOF_M_POINT_GROUP_NUM]={1,0,9,8,17,16,25,24,33,32,41,40,49,48,57,56};//TOFSense-M系列64点位分组，用于计算等效左中右的距离值
uint8_t TOF_M_middle_point_group[TOF_M_POINT_GROUP_NUM]={4,3,12,11,20,19,28,27,36,35,44,43,52,51,60,59};//TOFSense-M系列64点位分组，用于计算等效左中右的距离值
uint8_t TOF_M_right_point_group[TOF_M_POINT_GROUP_NUM]={7,6,15,14,23,22,31,30,39,38,47,46,55,54,63,62};//TOFSense-M系列64点位分组，用于计算等效左中右的距离值

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
	uint16_t count_i=0;                                                           //循环计数变量
	float TOF_M_left_dis_temp=999999.0;                                           //TOFSense-M的左侧距离值临时值
	float TOF_M_middle_dis_temp=999999.0;                                         //TOFSense-M的中间距离值临时值
	float TOF_M_right_dis_temp=999999.0;                                          //TOFSense-M的右侧距离值临时值
	uint8_t TOF_M_left_dis_status_temp=0;                                         //TOFSense-M的左侧距离状态指示临时值
	uint8_t TOF_M_middle_dis_status_temp=0;                                       //TOFSense-M的中间距离状态指示临时值
	uint8_t TOF_M_right_dis_status_temp=0;                                        //TOFSense-M的右侧距离状态指示临时值
	uint8_t TOF_M_left_dis_signal_strength_temp=0;                                //TOFSense-M的左侧距离信号强度临时值
	uint8_t TOF_M_middle_dis_signal_strength_temp=0;                              //TOFSense-M的中间距离信号强度临时值
	uint8_t TOF_M_right_dis_signal_strength_temp=0;                               //TOFSense-M的右侧距离信号强度临时值


	if((u_rx_buf_3[0] == 0x57)&&(u_rx_buf_3[1] == 0x00))                          //如果接收到的是TOFSense的协议帧
	{
		TOF_unpack_correct_flag=g_nts_frame0.UnpackData(u_rx_buf_3,TOF_data_length);//调用函数指针运行解码函数并返回解码正确/失败值
		if(TOF_unpack_correct_flag == 1)                                            //如果解码正确
		{
			TOF_series_switch=0;                                                      //当前连接的是TOFSense系列激光测距传感器

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
	else if((u_rx_buf_3[0] == 0x57)&&(u_rx_buf_3[1] == 0x01))                     //如果接收到的是TOFSense-M的协议帧
	{
		TOF_unpack_correct_flag=g_ntsm_frame0.UnpackData(u_rx_buf_3,TOF_data_length);//调用函数指针运行解码函数并返回解码正确/失败值
		if(TOF_unpack_correct_flag == 1)                                            //如果解码正确
		{
			TOF_series_switch=2;                                                      //当前连接的是TOFSense-M系列激光测距传感器

			if(g_ntsm_frame0.id == 0)                                                 //如果该帧是编号0的TOFSense-M模块数据
			{
      //右侧组别
				for(count_i=0;count_i<TOF_M_POINT_GROUP_NUM;count_i++)//筛选TOFSense-M左侧组别中最小的且有效的距离作为左侧的距离值
				{
					if(g_ntsm_frame0.pixels[TOF_M_right_point_group[count_i]].dis_status == 0)//遍历判断组别中每个点的距离状态指示筛掉不合格的数据
					{
						if(g_ntsm_frame0.pixels[TOF_M_right_point_group[count_i]].dis<TOF_M_right_dis_temp)//如果当前遍历的点的距离小于距离临时变量则把当前点的距离赋值给临时变量，最终取最小值
						{
							TOF_M_right_dis_temp=g_ntsm_frame0.pixels[TOF_M_right_point_group[count_i]].dis;
							TOF_M_right_dis_status_temp=g_ntsm_frame0.pixels[TOF_M_right_point_group[count_i]].dis_status;
							TOF_M_right_dis_signal_strength_temp=g_ntsm_frame0.pixels[TOF_M_right_point_group[count_i]].signal_strength;
						}

					}
				}
				TOF_signal_strength0=TOF_M_right_dis_signal_strength_temp;              //取TOFSense距离信号强度
				TOF_status0=TOF_M_right_dis_status_temp;                                //取TOFSense距离状态值
				TOF_FR_dis0=TOF_M_right_dis_temp;                                       //取TOFSense测量距离值
      //中间组别
				for(count_i=0;count_i<TOF_M_POINT_GROUP_NUM;count_i++)//筛选TOFSense-M中间组别中最小的且有效的距离作为中间的距离值
				{
					if(g_ntsm_frame0.pixels[TOF_M_middle_point_group[count_i]].dis_status == 0)//遍历判断组别中每个点的距离状态指示筛掉不合格的数据
					{
						if(g_ntsm_frame0.pixels[TOF_M_middle_point_group[count_i]].dis<TOF_M_middle_dis_temp)//如果当前遍历的点的距离小于距离临时变量则把当前点的距离赋值给临时变量，最终取最小值
						{
							TOF_M_middle_dis_temp=g_ntsm_frame0.pixels[TOF_M_middle_point_group[count_i]].dis;
							TOF_M_middle_dis_status_temp=g_ntsm_frame0.pixels[TOF_M_middle_point_group[count_i]].dis_status;
							TOF_M_middle_dis_signal_strength_temp=g_ntsm_frame0.pixels[TOF_M_middle_point_group[count_i]].signal_strength;
						}

					}
				}
				TOF_signal_strength1=TOF_M_middle_dis_signal_strength_temp;             //取TOFSense距离信号强度
				TOF_status1=TOF_M_middle_dis_status_temp;                               //取TOFSense距离状态值
				TOF_FM_dis1=TOF_M_middle_dis_temp;                                  	  //取TOFSense测量距离值
      //左边组别
				for(count_i=0;count_i<TOF_M_POINT_GROUP_NUM;count_i++)//筛选TOFSense-M左侧组别中最小的且有效的距离作为左侧的距离值
				{
					if(g_ntsm_frame0.pixels[TOF_M_left_point_group[count_i]].dis_status == 0)//遍历判断组别中每个点的距离状态指示筛掉不合格的数据
					{
						if(g_ntsm_frame0.pixels[TOF_M_left_point_group[count_i]].dis<TOF_M_left_dis_temp)//如果当前遍历的点的距离小于距离临时变量则把当前点的距离赋值给临时变量，最终取最小值
						{
							TOF_M_left_dis_temp=g_ntsm_frame0.pixels[TOF_M_left_point_group[count_i]].dis;
							TOF_M_left_dis_status_temp=g_ntsm_frame0.pixels[TOF_M_left_point_group[count_i]].dis_status;
							TOF_M_left_dis_signal_strength_temp=g_ntsm_frame0.pixels[TOF_M_left_point_group[count_i]].signal_strength;
						}

					}
				}
				TOF_signal_strength2=TOF_M_left_dis_signal_strength_temp;               //取TOFSense距离信号强度
				TOF_status2=TOF_M_left_dis_status_temp;                                 //取TOFSense距离状态值
				TOF_FL_dis2=TOF_M_left_dis_temp;                                        //取TOFSense测量距离值

/*
从小车上往小车前方看，TOFSense-M系列的点位空间分布如下：
（要求TOFSense-M接口朝上安装，TOFSense-M S接线朝上安装）

        00 01 02 03 04 05 06 07
        08 09 10 11 12 13 14 15
        16 17 18 19 20 21 22 23
        24 25 26 27 28 29 30 31
        32 33 34 35 36 37 38 39
        40 41 42 43 44 45 46 47
        48 49 50 51 52 53 54 55
        56 57 58 59 60 61 62 63

               左侧两列去除状态无效值然后取最小值作为左侧TOF的避障等效距离
        00 01
        08 09
        16 17
        24 25
        32 33
        40 41
        48 49
        56 57
               中间两列去除状态无效值然后取最小值作为中间TOF的避障等效距离
                 03 04
                 11 12
                 19 20
                 27 28
                 35 36
                 43 44
                 51 52
                 59 60
               右边两列去除状态无效值然后取最小值作为中间TOF的避障等效距离
                          06 07
                          14 15
                          22 23
                          30 31
                          38 39
                          46 47
                          54 55
                          62 63

 */
			}

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


