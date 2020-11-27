#include "linktrack.h"

uint8_t u_rx_buf_1[256];                 //usart1接收缓存数组
uint8_t LT_unpack_correct_flag=0;        //LinkTrack一帧数据解包正确标志位
uint8_t LT_DMA_receive_flag=0;           //DMA接收一帧LinkTrack协议（Node_Frame2）数据完成标志位
uint16_t LT_data_length=0;               //DMA接收一帧LinkTrack协议（Node_Frame2）数据长度
	
uint8_t LT_tag_id=0;                     //标签的ID
uint32_t LT_local_time=0;                //LT标签的本地时间
uint32_t LT_temp_local_time=0;           //LT标签的本地时间临时储存变量，用于计算小车平均速度
float LT_pos_x=0.0;                      //LinkTrack输出的x坐标
float LT_pos_y=0.0;                      //LinkTrack输出的y坐标
float LT_pos_z=0.0;                      //LinkTrack输出的z坐标
float LT_expect_pos_x=0.0;               //期望的x坐标
float LT_expect_pos_y=0.0;               //期望的y坐标
float anchor_to_east_angle=0.0;       //基站坐标系与东方向夹角（小车车头朝向A0到A3连线方向测得）   注：改变基站位置后需重新测量夹角
float now_to_target_angle=0.0;           //当前坐标点到目标坐标点的连线与x轴夹角
float forward_to_xaxis_angle=0.0;        //当前小车轴线（前进方向）与x轴夹角
float forward_to_target_angle=0.0;       //当前小车轴线（前进方向）与当前坐标到目标坐标连线的夹角
uint8_t LT_start_flag=0;                 //设定期望坐标后的起步标志位
float yaw_expect=0.0;                    //航向角期望值
float LT_dis_expect=0.0;                 //与目标坐标点之间距离的期望值
float LT_dis_measure=0.0;                //与目标坐标点之间距离的测量值，由坐标三角函数求得
float distance_target_range=0.5;         //距离目标范围，单位：米
uint8_t LT_DT_data_length=0;             //LinkTrack标签接收到的数传数据长度

float anchor_Vx=0.0;                     //沿基站坐标系x轴方向速度分量
float anchor_Vy=0.0;                     //沿基站坐标系y轴方向速度分量

/************************************************
函数名称 ： LT_Application
函数功能 ： LinkTrack定位应用函数
参    数 ： 无
返 回 值 ： 无
*************************************************/
void LT_Application(void)
{
	if((u_rx_buf_1[0] == 0x55)&&(u_rx_buf_1[1] == 0x04))                        //如果接收到的是Node_Frame2的定位帧
	{
		LT_unpack_correct_flag=g_nlt_nodeframe2.UnpackData(u_rx_buf_1,LT_data_length); //调用函数指针运行解码函数并返回解码正确/失败值
		if(LT_unpack_correct_flag == 1)                                           //如果解码正确
		{
			LT_tag_id=g_nlt_nodeframe2.result.id;                                   //取当前连接的标签ID
			LT_local_time=g_nlt_nodeframe2.result.local_time;                       //取当前连接的标签的本地时间
			LT_pos_x=g_nlt_nodeframe2.result.pos_3d[0];                             //取LinkTrack输出的x坐标
			LT_pos_y=g_nlt_nodeframe2.result.pos_3d[1];                             //取LinkTrack输出的y坐标
		}
	}
	else if((u_rx_buf_1[0] == 0x55)&&(u_rx_buf_1[1] == 0x02))                   //如果接收到的是Node_Frame0的数传帧
	{
		LT_unpack_correct_flag=g_nlt_nodeframe0.UnpackData(u_rx_buf_1,LT_data_length); //调用函数指针运行解码函数并返回解码正确/失败值
		if((LT_unpack_correct_flag == 1)&&(g_nlt_nodeframe0.result.valid_node_count>0))//如果解码正确且数传可用节点大于0
		{
			LT_DT_data_length=g_nlt_nodeframe0.result.nodes[0]->data_length;        //取LinkTrack标签接收到的数传数据长度
			if((g_nlt_nodeframe0.result.nodes[0]->data[2] == '.')&&
					(g_nlt_nodeframe0.result.nodes[0]->data[4] == ' ')&&
					 (g_nlt_nodeframe0.result.nodes[0]->data[7] == '.'))                  //判断接收到的数据是否符合格式
			{
				LT_expect_pos_x=(float)(((uint8_t)(g_nlt_nodeframe0.result.nodes[0]->data[0])-48)*100+
																((uint8_t)(g_nlt_nodeframe0.result.nodes[0]->data[1])-48)*10+
																((uint8_t)(g_nlt_nodeframe0.result.nodes[0]->data[3])-48))/10;  //计算期望x坐标
				LT_expect_pos_y=(float)(((uint8_t)(g_nlt_nodeframe0.result.nodes[0]->data[5])-48)*100+
																((uint8_t)(g_nlt_nodeframe0.result.nodes[0]->data[6])-48)*10+
																((uint8_t)(g_nlt_nodeframe0.result.nodes[0]->data[8])-48))/10;  //计算期望y坐标
				LT_start_flag=1;                                                      //如果接收到了期望坐标，起步标志位置1
			}

		}				
	}
			
	if(mode_selection == 3)                                                     //如果处于定位目标点移动模式
	{
		if((LT_start_flag == 1)||(LT_start_flag == 2))                            //如果起步标志位为1或2（阶段1：只转向  阶段2：边转向边前进）
		{
			forward_to_xaxis_angle=WT_IMU_yaw;                                      //当前小车轴线（前进方向）与x轴夹角
			now_to_target_angle=(float)(atan2(((double)(LT_expect_pos_y-LT_pos_y)),((double)(LT_expect_pos_x-LT_pos_x)))*180/3.14); //反三角函数计算当前坐标点到目标坐标点的连线与x轴夹角
			forward_to_target_angle=now_to_target_angle-forward_to_xaxis_angle;     //计算此时小车轴线（前进方向）与当前坐标到目标坐标连线的夹角
			yaw_expect=now_to_target_angle;                                         //计算期望航向角
															
			if(((yaw_expect-WT_IMU_yaw)>180)||((yaw_expect-WT_IMU_yaw)<-180))         //以与期望值小于180度的方向进行转向
			{
				z1_increase = (int)PID_Loc(yaw_expect, WT_IMU_yaw, &LT_ANGLE_PID);      //PID算出输出量
			}
			else
			{
				z1_increase = -(int)PID_Loc(yaw_expect, WT_IMU_yaw, &LT_ANGLE_PID);     //PID算出输出量
			}
			if(fabsf(yaw_expect-WT_IMU_yaw)<yaw_target_range)                         //判断航向角测量值是否达到了目标环内
			{
				yaw_dead_zone_range_flag=1;                                             //如果达到目标环，进入死区环判断并置位标志位
				LT_start_flag=2;                                                        //起步标志位置2，在角度PID控制的同时进行距离PID控制
			}
			if(yaw_dead_zone_range_flag == 1)                                         //如果死区环标志位为1
			{
				if(fabsf(yaw_expect-WT_IMU_yaw)<yaw_dead_zone_range)                    //如果角度波动范围小于死区环范围
				{
					z1_increase=0;                                                        //输出改为0
				}
				else
				{
					yaw_dead_zone_range_flag=0;                                           //如果角度波动范围大于死区环范围，标志位清零
				}
			}
			if(z1_increase>0)                                                         //输出值加大一点，防止小角度时因地面摩擦力等因素导致小车转不动
			{
				z1_increase+=20;
			}
			else if(z1_increase<0)
			{
				z1_increase-=20;
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
													
			LT_dis_measure=(float)sqrt(pow((double)(LT_expect_pos_x-LT_pos_x),2)+pow((double)(LT_expect_pos_y-LT_pos_y),2)); //计算此时与期望坐标点之间距离
			y1_increase = (int)PID_Loc( LT_dis_expect, LT_dis_measure, &LT_DISTANCE_PID); //PID算出输出量
			if(y1_increase>0)                                                       //输出值加大一点，防止输出较小时因地面摩擦力等因素导致小车走不动
			{
				y1_increase+=20;
			}
			else if(y1_increase<0)
			{
				y1_increase-=20;
			}
			if(y1_increase>127)                                                     //输出限幅
			{
				y1_increase=127;
			}
			else if(y1_increase<-127)
			{
				y1_increase=-127;
			}
			y1_output = y1_increase+127;                                            //调整输出量范围
			if(fabsf(LT_dis_expect-LT_dis_measure)<distance_target_range)           //判断距离测量值是否达到了目标范围内，如果到达，停止控制
			{
				Move_Control( 127, 127, 127);                                         //停止
				LT_start_flag=3;                                                      //起步标志位置3，进行下一步
			}
			else
			{
				if(LT_start_flag == 1)
				{
					Move_Control( 127, 127, z1_output);                                 //根据PID输出值进行角度运动控制
				}
				else if(LT_start_flag == 2)
				{
					Move_Control( 127, y1_output, z1_output);                           //根据PID输出值进行前进和角度运动控制
				}

			}

		}
		else if(LT_start_flag == 3)                                               //如果起步标志位为3，进行下一步（阶段3：利用麦轮特性平移直至测量坐标收敛接近目标点）
		{					
			anchor_Vx = PID_Loc( LT_expect_pos_x, LT_pos_x, &LT_X_PID);             //PID算出输出量
			anchor_Vy = PID_Loc( LT_expect_pos_y, LT_pos_y, &LT_Y_PID);             //PID算出输出量
			x1_increase=(int)(anchor_Vx*sin((double)WT_IMU_yaw*3.14/180)-anchor_Vy*cos((double)WT_IMU_yaw*3.14/180)); //计算载体坐标系下x轴速度输出增量（即左右平移输出增量）
			y1_increase=-(int)(anchor_Vx*cos((double)WT_IMU_yaw*3.14/180)+anchor_Vy*sin((double)WT_IMU_yaw*3.14/180)); //计算载体坐标系下y轴速度输出增量（即前进后退输出增量）

			if(x1_increase>0)                                                       //输出值加大一点，防止输出较小时因地面摩擦力等因素导致小车走不动
			{
				x1_increase+=20;
			}
			else if(x1_increase<0)
			{
				x1_increase-=20;
			}
			if(x1_increase>127)                                                     //输出限幅
			{
				x1_increase=127;
			}
			else if(x1_increase<-127)
			{
				x1_increase=-127;
			}					
			if(y1_increase>0)                                                       //输出值加大一点，防止输出较小时因地面摩擦力等因素导致小车走不动
			{
				y1_increase+=20;
			}
			else if(y1_increase<0)
			{
				y1_increase-=20;
			}
			if(y1_increase>127)                                                     //输出限幅
			{
				y1_increase=127;
			}
			else if(y1_increase<-127)
			{
				y1_increase=-127;
			}
			x1_output = x1_increase+127;                                            //调整输出量范围
			y1_output = y1_increase+127;                                            //调整输出量范围
					
			if((fabsf(LT_expect_pos_x-LT_pos_x)<0.03)&&(fabsf(LT_expect_pos_y-LT_pos_y)<0.03)) //判断距离测量值是否达到了目标范围内，如果到达，停止控制
			{
				Move_Control( 127, 127, 127);                                         //停止
				LT_start_flag=4;                                                      //起步标志位置4，进行下一步
			}
			else
			{
				Move_Control( x1_output, y1_output, 127);                             //根据PID输出值进行前进运动控制
			}
					
		}
		else
		{
			Move_Control( 127, 127, 127);                                           //如果起步标志位为其他值，小车停止，防止模式切换后失控
		}
		
	}
	
}


