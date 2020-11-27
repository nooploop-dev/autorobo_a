#include "ws2812b.h"

uint8_t r_buffer[PIXEL_MAX]={0};                                 //红色RGB值
uint8_t g_buffer[PIXEL_MAX]={0};                                 //绿色RGB值
uint8_t b_buffer[PIXEL_MAX]={0};                                 //蓝色RGB值

uint16_t RGB_delay=0;                                            //RGB灯条定时器延时时间
uint16_t RGB_delay_count=0;                                      //RGB灯条定时器延时时间计数变量，表示经过了多少个延时周期

/************************************************
函数名称 ： RGB_Show
函数功能 ： RGB灯条根据模式显示不同的效果
参    数 ： mode ------ 模式
返 回 值 ： 无
*************************************************/
void RGB_Show(uint8_t mode)
{
	switch(mode)                                                   //根据模式采用不同的RGB灯光效果
	{
		case 0:				
			ColorWipe(Color(0, 255, 0), 200);
			break;
		case 1:
			Rainbow(200);
			break;
		case 2:
			Rainbow(25);
			break;
		case 3:
			RainbowCycle(25);
			break;
	}	
}

/************************************************
函数名称 ： Send_8_Bits
函数功能 ： 发送8位数据
参    数 ： data ------ 需要发送的数据
返 回 值 ： 无
*************************************************/
void Send_8_Bits(uint8_t data)                                   //向RGB信号线发送8位数据
{
  uint8_t j=0;
  static uint8_t CodeOne=0x7c;                                   //采用SPI发送一个字节来模拟编码1的时序 7c，3e
  static uint8_t CodeZero=0x70;                                  //采用SPI发送一个字节来模拟编码0的时序 70，38
  
  for (j=0;j<8;j++)
  {
    if((data & 0x80)==0x80)
    {
      HAL_SPI_Transmit(&hspi1, &CodeOne, 1, 10);
    }
    else
    {
      HAL_SPI_Transmit(&hspi1, &CodeZero, 1, 10); 
    }
    data=data<<1;
  }
}

/************************************************
函数名称 ： Send_24_Bits
函数功能 ： 发送24位数据控制一个像素点（按照G--R--B的顺序发送）
参    数 ： RData ------ 红色
            GData ------ 绿色
            BData ------ 蓝色
返 回 值 ： 无
*************************************************/
void Send_24_Bits(uint8_t RData,uint8_t GData,uint8_t BData)     //连续发送3个8位数据，控制一个像素的RGB显示
{   
  Send_8_Bits(GData);  
  Send_8_Bits(RData); 
  Send_8_Bits(BData);
} 

/************************************************
函数名称 ： Pixel_Update
函数功能 ： 复位函数
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Pixel_Update(void)                                          //复位函数，应该 >24us
{
  uint8_t a[24]={0};
  HAL_SPI_Transmit(&hspi1, a, 24, 10);
}

/************************************************
函数名称 ： Ws2812b_Init
函数功能 ： 初始化函数
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Ws2812b_Init(void)                                          //初始化函数，应该 >50us
{
  uint8_t Res_Code[50]={0};
  HAL_SPI_Transmit(&hspi1, Res_Code, 50, 10);
  SetAllPixelColor(0, 0, 0);
  HAL_Delay (1);
  SetAllPixelColor(0, 0, 0);
  HAL_Delay (1);
}

/************************************************
函数名称 ： SetAllPixelColor
函数功能 ： 将所有像素点设置为同一种颜色
参    数 ： R ------ 红色
            G ------ 绿色
            B ------ 蓝色
返 回 值 ： 无
*************************************************/
void SetAllPixelColor(uint8_t R, uint8_t G, uint8_t B)           //将所有像素点设置为同一种颜色
{ 
  uint8_t i=0;
  for(i=0;i<PIXEL_MAX;i++)
  {
    r_buffer[i]=0;
    g_buffer[i]=0;
    b_buffer[i]=0;
  }
  for(i=0;i<PIXEL_MAX;i++)
  {
    r_buffer[i]=R;
    g_buffer[i]=G;
    b_buffer[i]=B;
  }

  for(i=0;i<PIXEL_MAX;i++)
  {							  
    Send_24_Bits(r_buffer[i],g_buffer[i],b_buffer[i]);
  }
	Pixel_Update();
}

/************************************************
函数名称 ： setPixelColor
函数功能 ： 单独设置第X+1个像素点的颜色,其余像素点全都熄灭
参    数 ： X ------ 第X+1个像素点
            R ------ 红色
            G ------ 绿色
            B ------ 蓝色
返 回 值 ： 无
*************************************************/
void setPixelColor(uint16_t X, uint8_t R, uint8_t G, uint8_t B)  //单独设置第X+1个像素点的颜色,其余像素点全都熄灭
{	 
  uint8_t i=0;

  for(i=0;i<PIXEL_MAX;i++)
  {
    r_buffer[i]=0;
    g_buffer[i]=0;
    b_buffer[i]=0;
  }
  r_buffer[X]=R;
  g_buffer[X]=G;
  b_buffer[X]=B;
  for(i=0;i<PIXEL_MAX;i++)
  {							  
    Send_24_Bits(r_buffer[i],g_buffer[i],b_buffer[i]);
  }
  Pixel_Update();
}

/************************************************
函数名称 ： SetPixelColor
函数功能 ： 单独设置第X+1个像素点的颜色,其余像素点保持不变
参    数 ： X ------ 第X+1个像素点
            D ------ RGB颜色
返 回 值 ： 无
*************************************************/
void SetPixelColor(uint16_t X, uint32_t D)                       //单独设置第X+1个像素点的颜色,其余像素点保持不变
{	 
  uint8_t i=0;
	  
  r_buffer[X]=(uint8_t)(D>>16);
  g_buffer[X]=(uint8_t)(D>>8);
  b_buffer[X]=(uint8_t)D;

  for(i=0;i<PIXEL_MAX;i++)
  {							  
    Send_24_Bits(r_buffer[i],g_buffer[i],b_buffer[i]);
  }
	Pixel_Update();
}

/************************************************
函数名称 ： Color
函数功能 ： 将RGB 3个颜色合并成一个变量
参    数 ： R ------ 红色
            G ------ 绿色
            B ------ 蓝色
返 回 值 ： 合并的uint32_t变量
*************************************************/
uint32_t Color(uint8_t R, uint8_t G, uint8_t B)                  //将RGB 3个颜色合并成一个变量
{
  return ((uint32_t)R << 16) | ((uint32_t)G <<  8) | B;
}

/************************************************
函数名称 ： Wheel
函数功能 ： 对变量进行处理
参    数 ： Wheel_Pos ------ 需要处理的变量
返 回 值 ： 处理后的uint32_t变量
*************************************************/
uint32_t Wheel(uint8_t Wheel_Pos)
{
  Wheel_Pos = 255 - Wheel_Pos;
  if(Wheel_Pos < 85) 
  {
    return Color(255 - Wheel_Pos * 3, 0, Wheel_Pos * 3);
  }
  if(Wheel_Pos < 170) 
	{
    Wheel_Pos -= 85;
    return Color(0, Wheel_Pos * 3, 255 - Wheel_Pos * 3);
  }
  Wheel_Pos -= 170;
  return Color(Wheel_Pos * 3, 255 - Wheel_Pos * 3, 0);
}

/************************************************
函数名称 ： Rainbow
函数功能 ： 彩虹模式
参    数 ： time ------ 每次改变像素的间隔时间，单位ms
返 回 值 ： 无
*************************************************/
void Rainbow(uint16_t time)                                       //彩虹模式
{
  uint16_t i;
  
	if(RGB_delay == 0)
	{
		for(i=0; i<PIXEL_MAX; i++)
    {
      SetPixelColor(i, Wheel((i+RGB_delay_count) & 255));
    }
//    Pixel_update();
		RGB_delay=time;
		RGB_delay_count++;
		if(RGB_delay_count == 256)
		{
			RGB_delay_count=0;
		}
		
	}
	
}

/************************************************
函数名称 ： RainbowCycle
函数功能 ： 彩虹循环模式
参    数 ： time ------ 每次改变像素的间隔时间，单位ms
返 回 值 ： 无
*************************************************/
void RainbowCycle(uint16_t time)                                  //彩虹循环模式
{
  uint16_t i;
  
	if(RGB_delay == 0)
	{
		for(i=0; i< PIXEL_MAX; i++) 
    {
      SetPixelColor(i, Wheel(((i * 256 / PIXEL_MAX) + RGB_delay_count) & 255));
    }
//    Pixel_update();
		RGB_delay=time;
		RGB_delay_count++;
		if(RGB_delay_count == 256*5)
		{
			RGB_delay_count=0;
		}

	}

}

/************************************************
函数名称 ： ColorWipe
函数功能 ： 依次点亮模式
参    数 ： D --------- RGB颜色
						time ------ 每次改变像素的间隔时间，单位ms
返 回 值 ： 无
*************************************************/
void ColorWipe(uint32_t D, uint16_t time)                         //依次点亮模式
{	
	if(RGB_delay == 0)
	{
		if(RGB_delay_count == PIXEL_MAX)
		{
			SetAllPixelColor( 0, 0, 0);
		}
		else
		{
			SetPixelColor(RGB_delay_count, D);			
		}
//		Pixel_update();
		RGB_delay=time;
		RGB_delay_count++;
		if(RGB_delay_count == PIXEL_MAX+1)
		{
			RGB_delay_count=0;
		}
	}
	
}

/************************************************
函数名称 ： Ws2812b_Test
函数功能 ： RGB灯条测试程序
参    数 ： 无
返 回 值 ： 无
*************************************************/
void Ws2812b_Test(void)
{
  SetAllPixelColor(255, 0, 0);
  HAL_Delay (500);
  SetAllPixelColor(0, 255, 0);
  HAL_Delay (500);
  SetAllPixelColor(0, 0, 255);
  HAL_Delay (500);
  
  SetAllPixelColor(0, 0, 0);
  HAL_Delay (500);
  
  setPixelColor(0, 0, 255, 0);
  HAL_Delay (500);
  setPixelColor(2, 0, 0, 255);
  HAL_Delay (500); 
  setPixelColor(4, 255, 0, 0);
  HAL_Delay (500);
  setPixelColor(6, 125, 125, 125);
  HAL_Delay (500);    
  setPixelColor(5, 0, 255, 0);
  HAL_Delay (500);
  setPixelColor(3, 0, 0, 255);
  HAL_Delay (500); 
  setPixelColor(1, 255, 0, 0);
  HAL_Delay (500);
  SetAllPixelColor(0, 0, 0);
  HAL_Delay (50);
}






