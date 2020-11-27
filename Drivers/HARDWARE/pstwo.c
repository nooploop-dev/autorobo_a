#include "pstwo.h"

uint8_t PS2_delay_count1=0;              //PS2读取延时计数变量1（10ms）
uint8_t PS2_delay_count2=0;              //PS2读取延时计数变量2（50ms）
uint8_t PS2_delay_flag1=0;               //PS2读取延时计数结束变量1（10ms）
uint8_t PS2_delay_flag2=0;               //PS2读取延时计数结束变量2（50ms）
uint8_t PS2_DataKey_flag=0;              //key1=PS2_DataKey()语句执行标志变量
uint8_t key1=0;                          //PS2键值变量
int x_data,y_data,z_data=0;              //PS2左右摇杆模拟量变量，xy为左摇杆左右、上下方向，z为右摇杆左右方向

uint16_t handkey;                        //键值暂存变量
uint8_t comd[2]={0x01,0x42};	           //开始命令。请求数据
uint8_t data_ps2[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
uint16_t mask_ps2[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	                                   //按键值与按键名

/************************************************
函数名称 ： PS2_Application
函数功能 ： 通过PS2手柄控制小车
参    数 ： 无
返 回 值 ： 无
*************************************************/	
void PS2_Application(void)
{
	if(PS2_DataKey_flag == 0)
	{
		key1=PS2_DataKey();
		PS2_DataKey_flag=1;
		if(PS2_delay_count1 == 0)
		{
			PS2_delay_count1 = 1;              //开始PS2读取10ms延时计时
		}
	}		

	if((PS2_delay_flag1 == 1)||(PS2_delay_flag2 == 1))
	{
		PS2_DataKey_flag=0;
		PS2_delay_flag1=0;
		if(PS2_delay_count2 == 0)
		{
			if(PS2_delay_flag2 == 1)           //50ms延时结束
			{
				PS2_delay_flag2=0;
				LED1(ON);
				PS2_DataKey();	                 //手柄按键捕获处理
				x_data = PS2_AnologData(PSS_LX); //左摇杆左右方向摇杆值读取
				y_data = PS2_AnologData(PSS_LY); //左摇杆上下方向摇杆值读取
				z_data = PS2_AnologData(PSS_RX); //右摇杆左右方向摇杆值读取
				if(mode_selection == 0)          //如果是在遥控模式下
				{
					Move_Control(x_data, y_data, z_data);//根据摇杆值进行运动控制
						
					if(key1 == 10)								 //如果按键10被按下		
					{
						Moto1(360, 360);
						Moto2(360, 360);
						MOTOR1_F;
						MOTOR2_F;
						MOTOR3_F;
						MOTOR4_F;
					}
					else if(key1 == 9)             //如果按键9被按下
					{
						Moto1(360, 360);
						Moto2(360, 360);
						MOTOR1_B;	
						MOTOR2_B;
						MOTOR3_B;
						MOTOR4_B;
					}
				}
					
			}														
			else if(!PS2_RedLight())           //如果处于红灯模式
			{
				if(PS2_delay_count2 == 0)
				{
					PS2_delay_count2 = 1;          //开始PS2读取50ms延时计时
				}
				PS2_DataKey_flag=1;				 	 
			}
			else
			{		
				LED1(OFF);
				if(mode_selection == 0)          //如果处于遥控模式
				{
					Moto1(0, 0);
					Moto2(0, 0);
				}
			}				
		}						
	}
}

/************************************************
函数名称 ： Delay_us
函数功能 ： us级延时函数，最大延时1000us
参    数 ： udelay ------ 延时时间
返 回 值 ： 无
*************************************************/
void Delay_us(uint32_t udelay)             
{
  uint32_t start_val,tickx,delayx,waits;
 
  start_val = SysTick->VAL;
  tickx = HAL_GetTick();
  //sysc = 480000;  //SystemCoreClock / (1000U / uwTickFreq);
  delayx =udelay * 480; //sysc / 1000 * udelay;
  if(delayx > start_val)
    {
      while(HAL_GetTick() == tickx)
        {
 
        }
      waits = 480000 + start_val - delayx;
      while(waits < SysTick->VAL)
        {
 
        }
    }
  else
    {
      waits = start_val - delayx;
      while(waits < SysTick->VAL && HAL_GetTick() == tickx)
        {
 
        }
    }
}	

/************************************************
函数名称 ： PS2_Cmd
函数功能 ： 向手柄发送命令
参    数 ： CMD ------ 需要发送的命令
返 回 值 ： 无
*************************************************/
void PS2_Cmd(uint8_t CMD)
{
	volatile uint16_t ref=0x01;
	data_ps2[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                              //输出一位控制位
		}
		else DO_L;

		CLK_H;                               //时钟信号拉高
		Delay_us(10);
		CLK_L;
		Delay_us(10);
		CLK_H;
		if(DI)
			data_ps2[1] = ref|data_ps2[1];
	}
}

/************************************************
函数名称 ： PS2_RedLight
函数功能 ： 判断是否为红灯模式
参    数 ： 无
返 回 值 ： 0 ------ 红灯模式
            其他值 - 其他模式
*************************************************/
uint8_t PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(comd[0]);                      //开始命令
	PS2_Cmd(comd[1]);                      //请求数据
	CS_H;
	if( data_ps2[1] == 0X73)
	{
		return 0 ;
	}
	else
	{
		return 1;
	}
		
}

/************************************************
函数名称 ： PS2_ReadData
函数功能 ： 读取手柄数据
参    数 ： 无
返 回 值 ： 无
*************************************************/
void PS2_ReadData(void)
{
	volatile uint8_t byte=0;
	volatile uint16_t ref=0x01;

	CS_L;

	PS2_Cmd(comd[0]);                      //开始命令
	PS2_Cmd(comd[1]);                      //请求数据

	for(byte=2;byte<9;byte++)              //开始接收数据
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			Delay_us(10);
			CLK_L;
			Delay_us(10);
			CLK_H;
		      if(DI)
		      data_ps2[byte] = ref|data_ps2[byte];
		}
        Delay_us(10);
	}
	CS_H;	
}

/************************************************
函数名称 ： PS2_DataKey
函数功能 ： 对读出来的PS2的数据进行处理，只处理了按键部分，默认数据是红灯模式，只有一个按键按下时
参    数 ： 无
返 回 值 ： 0 --------- 按键未按下
            index+1 --- 按键按下的键值
*************************************************/
uint8_t PS2_DataKey()
{
	uint8_t index;

	PS2_ClearData();
	PS2_ReadData();

	handkey=(data_ps2[4]<<8)|data_ps2[3];          
	for(index=0;index<16;index++)
	{	    
		if((handkey&(1<<(mask_ps2[index]-1)))==0)
		return index+1;
	}
	return 0; 
}
//
/************************************************
函数名称 ： PS2_AnologData
函数功能 ： 得到一个摇杆的模拟量，范围0~256
参    数 ： button - 需要读取的摇杆值
返 回 值 ： 摇杆模拟值
*************************************************/
uint8_t PS2_AnologData(uint8_t button)
{
	return data_ps2[button];
}

/************************************************
函数名称 ： PS2_ClearData
函数功能 ： 清除数据缓冲区
参    数 ： 无
返 回 值 ： 无
*************************************************/
void PS2_ClearData()
{
	uint8_t a;
	for(a=0;a<9;a++)
		data_ps2[a]=0x00;
}






