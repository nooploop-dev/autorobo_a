#ifndef __PSTWO_H
#define __PSTWO_H
#include "main.h"

#define DI   HAL_GPIO_ReadPin(PS2_DAT_GPIO_Port,PS2_DAT_Pin)                       //PB14 输入

#define DO_H HAL_GPIO_WritePin(PS2_COM_GPIO_Port,PS2_COM_Pin,GPIO_PIN_SET)         //PB15 命令位高
#define DO_L HAL_GPIO_WritePin(PS2_COM_GPIO_Port,PS2_COM_Pin,GPIO_PIN_RESET)       //PB15 命令位低

#define CS_H HAL_GPIO_WritePin(PS2_ATT_GPIO_Port,PS2_ATT_Pin,GPIO_PIN_SET)         //PB12 CS拉高
#define CS_L HAL_GPIO_WritePin(PS2_ATT_GPIO_Port,PS2_ATT_Pin,GPIO_PIN_RESET)       //PB12 CS拉低

#define CLK_H HAL_GPIO_WritePin(PS2_CLK_GPIO_Port,PS2_CLK_Pin,GPIO_PIN_SET)        //PB13 时钟拉高
#define CLK_L HAL_GPIO_WritePin(PS2_CLK_GPIO_Port,PS2_CLK_Pin,GPIO_PIN_RESET)      //PB13 时钟拉低

//键值宏定义
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      26

#define PSS_RX 5                
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

extern uint8_t data_ps2[9];                   //数据存储数组
extern uint16_t mask_ps2[16];                 //按键值与按键名
extern uint16_t handkey;                      //键值暂存变量

extern uint8_t PS2_delay_count1;              //PS2读取延时计数变量1（10ms）
extern uint8_t PS2_delay_count2;              //PS2读取延时计数变量2（50ms）
extern uint8_t PS2_delay_flag1;               //PS2读取延时计数结束变量1（10ms）
extern uint8_t PS2_delay_flag2;               //PS2读取延时计数结束变量2（50ms）
extern uint8_t PS2_DataKey_flag;              //key1=PS2_DataKey()语句执行标志变量
extern uint8_t key1;                          //PS2键值变量
extern int x_data,y_data,z_data;              //PS2左右摇杆模拟量变量，xy为左摇杆左右、上下方向，z为右摇杆左右方向

uint8_t PS2_RedLight(void);                   //判断是否为红灯模式
void PS2_ReadData(void);                      //读取手柄数据
void PS2_Cmd(uint8_t CMD);		                //向手柄发送命令
uint8_t PS2_DataKey(void);		                //键值读取
uint8_t PS2_AnologData(uint8_t button);       //得到一个摇杆的模拟量
void PS2_ClearData(void);	                    //清除数据缓冲区
void PS2_Application(void);                   //PS2应用函数


#endif





