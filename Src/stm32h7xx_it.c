/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

	/*PS2读取延时处理1 开始*/
	if(PS2_delay_count1 >= 1)              				//开始10ms延时计时
	{
		PS2_delay_count1++;
	}
	if(PS2_delay_count1 == 11)
	{
		PS2_delay_flag1=1;                   				//10ms延时结束，标志位置1
		PS2_delay_count1=0;                  				//重置计数变量

	}
	/*PS2读取延时处理1 结束*/

	/*PS2读取延时处理2 开始*/
	if(PS2_delay_count2 >= 1)              				//开始50ms延时计时
	{
		PS2_delay_count2++;
	}
	if(PS2_delay_count2 == 51)
	{
		PS2_delay_flag2=1;                   				//50ms延时结束，标志位置1
		PS2_delay_count2=0;                  				//重置计数变量

	}
	/*PS2读取延时处理2 结束*/

	/*TOFSense查询周期计算 开始*/
	if(TOF_inquire_cycle_count >= 0)              //每1ms计数变量自加一次
	{
		TOF_inquire_cycle_count++;
	}
	if(TOF_inquire_cycle_count == 5)              //如果达到了5ms的查询周期，查询一轮的时间为x*5ms
	{
		if(TOF_inquire_number == 6)                 //如果当前查询的模块编号变为6（即完成一轮查询）
		{
			TOF_inquire_number=0;
		}
		u_tx_buf_3[4]=TOF_inquire_number;           //将发送缓存数组中的查询模块ID改为当前需要查询的编号
		u_tx_buf_3[7]=TOF_inquire_number+0x63;      //更新校验和
		HAL_UART_Transmit_DMA(&huart3,u_tx_buf_3,sizeof(u_tx_buf_3));//开启usart3发送DMA传输
		TOF_inquire_number++;                       //当前查询的模块编号+1
		TOF_inquire_cycle_count=0;                  //重置计数变量
	}
	/*TOFSense查询周期计算 结束*/

	/*TOF避障开环控制延时 开始*/
	if(avoid_danger_shift_time != 0)              //如果避障开环控制横移延时不等于0
	{
		avoid_danger_shift_time--;
	}
	else
	{
		avoid_danger_status=0;                    	//避障状态标志位置0
	}

	/*TOF避障开环控制延时 结束*/

	/*TOF避障转弯开环控制延时 开始*/
	if(avoid_danger_turn_flag == 1)               //如果3TOF都小于阈值开始逆时针转圈
	{
		avoid_danger_turn_count++;
		if(avoid_danger_turn_count == 500)          //如果转了xms以后
		{
			avoid_danger_turn_count=0;							  //清空计数变量
			avoid_danger_turn_flag=2;								  //准备开始顺时针转圈
		}
	}

	/*TOF避障转弯开环控制延时 结束*/

	/*RGB灯条控制延时 开始*/

	if(RGB_delay != 0)                            //如果RGB延时不等于0
	{
		RGB_delay--;                                //延时自减
	}

	/*RGB灯条控制延时 结束*/

	/*AD电压转换计时 开始*/

	if(ADC_converted_count >= 0)                  //如果AD转换计时变量大于等于0
	{
		ADC_converted_count++;                      //延时自加
	}
	if(ADC_converted_count == 1000)               //如果AD转换计时变量等于1000（1000ms转换一次）
	{
		HAL_ADC_Start(&hadc1);                      //启动AD转换
		if(battery_voltage<LOW_VOLTAGE)             //如果电池电压小于报警电压
		{
			battery_low_voltage_count++;              //电池低电压计数变量加1
			if(battery_low_voltage_count == 5)        //如果连续x次采样电池电压都小于报警电压
			{
				battery_low_voltage_flag=1;             //电池低电压标志位置1
				battery_low_voltage_count=0;            //清零电池低电压计数变量
			}

		}
		else
		{
			battery_low_voltage_count=0;              //清零电池低电压计数变量
		}
		ADC_converted_count=0;                      //清零计时变量
	}

	/*AD电压转换计时 结束*/

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */

  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

  /* USER CODE END DMA1_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_rx);
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

  /* USER CODE END DMA1_Stream3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
  /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

  /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

  /* USER CODE END DMA1_Stream5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
  /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

  /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 global interrupts.
  */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

	if(__HAL_ADC_GET_FLAG(&hadc1, ADC_FLAG_EOC ) != RESET)                     //确认发生了中断
	{
		ADC_converted_value=HAL_ADC_GetValue(&hadc1);                            //取AD转换值
		battery_voltage=(float)(((double)ADC_converted_value*3.3/65536)*63/24);  //计算电池电压值
	}

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

	//确保是否产生了EXTI Line中断
	if(__HAL_GPIO_EXTI_GET_IT(KEY1_Pin) != RESET)
	{
		LED2_TOGGLE;                       				   //LED状态反转
		mode_selection++;                  				   //切换遥控/角度跟随/角度距离全跟随模式/定位目标点移动模式
		Reinit_XYZ_Output();                         //重新初始化输出量防止模式切换后小车失控
		RGB_delay=0;                       				   //清零RGB延时变量，防止模式切换后RGB灯光失控
		RGB_delay_count=0;                 				   //清零RGB延时计数变量，防止模式切换后RGB灯光失控
		if(mode_selection == 4)            				   //最后一个模式时按下按键切换到第一个模式
		{
			mode_selection=0;
		}

		__HAL_GPIO_EXTI_CLEAR_IT(KEY1_Pin);          //清除中断标志位
	}

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

	// DMA加串口空闲中断接收语句 开始
	if(__HAL_UART_GET_IT( &huart1, UART_IT_IDLE ) != RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);       	//清除串口空闲中断标志位
		HAL_UART_AbortReceive(&huart1);           	//关闭DMA传输
		LT_DMA_receive_flag=1;		                	//DMA完成一帧数据接收
		LT_data_length  = sizeof(u_rx_buf_1) - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);   //计算接收到的数据长度
		HAL_UART_Receive_DMA(&huart1,u_rx_buf_1,sizeof(u_rx_buf_1));//开启DMA传输

	}
	//DMA加串口空闲中断接收语句 结束

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

	// DMA加串口空闲中断接收语句 开始
	if(__HAL_UART_GET_IT( &huart2, UART_IT_IDLE ) != RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);       	//清除串口空闲中断标志位
		HAL_UART_AbortReceive(&huart2);           	//关闭DMA传输
		AOA_DMA_receive_flag=1;		                	//DMA完成一帧数据接收
		AOA_data_length  = sizeof(u_rx_buf_2) - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);   //计算接收到的数据长度
		HAL_UART_Receive_DMA(&huart2,u_rx_buf_2,sizeof(u_rx_buf_2));//开启DMA传输

	}
	//DMA加串口空闲中断接收语句 结束

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

	// DMA加串口空闲中断接收语句 开始
	if(__HAL_UART_GET_IT( &huart3, UART_IT_IDLE ) != RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart3);       	//清除串口空闲中断标志位
		HAL_UART_AbortReceive(&huart3);           	//关闭DMA传输
		TOF_DMA_receive_flag=1;		                	//DMA完成一帧数据接收
		TOF_data_length  = sizeof(u_rx_buf_3) - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);   //计算接收到的数据长度
		HAL_UART_Receive_DMA(&huart3,u_rx_buf_3,sizeof(u_rx_buf_3));//开启DMA传输

	}

	//DMA加串口空闲中断接收语句 结束

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */

	// DMA加串口空闲中断接收语句 开始
	if(__HAL_UART_GET_IT( &huart4, UART_IT_IDLE ) != RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart4);       	//清除串口空闲中断标志位
		HAL_UART_AbortReceive(&huart4);           	//关闭DMA传输
		WTIMU_DMA_receive_flag=1;		              	//DMA完成一帧数据接收
		WTIMU_data_length  = sizeof(u_rx_buf_4) - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);   //计算接收到的数据长度
		HAL_UART_Receive_DMA(&huart4,u_rx_buf_4,sizeof(u_rx_buf_4));//开启DMA传输

	}
	//DMA加串口空闲中断接收语句 结束

  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream5 global interrupt.
  */
void DMA2_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream5_IRQn 0 */

  /* USER CODE END DMA2_Stream5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA2_Stream5_IRQn 1 */

  /* USER CODE END DMA2_Stream5_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream7_IRQn 0 */

  /* USER CODE END DMA2_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
  /* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

  /* USER CODE END DMA2_Stream7_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
