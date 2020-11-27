/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/************************************************
函数名称 ： User_Init
函数功能 ： 用户代码初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
void User_Init(void)
{
	PID_Init();                                         //PID参数初始化
	Ws2812b_Init();			                                //RGB灯条初始化

	HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);//AD校准
	HAL_ADC_Start_IT(&hadc1);                           //使能ADC1中断

	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);            //使能TIM4 通道1 PWM输出
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);            //使能TIM4 通道2 PWM输出
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);            //使能TIM4 通道3 PWM输出
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);            //使能TIM4 通道4 PWM输出

	HAL_UART_Receive_DMA(&huart1,u_rx_buf_1,sizeof(u_rx_buf_1));//开启DMA传输
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);         //开启uart1串口空闲中断

	HAL_UART_Receive_DMA(&huart2,u_rx_buf_2,sizeof(u_rx_buf_2));//开启DMA传输
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);         //开启uart2串口空闲中断

	HAL_UART_Receive_DMA(&huart3,u_rx_buf_3,sizeof(u_rx_buf_3));//开启DMA传输
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);         //开启uart3串口空闲中断

	HAL_UART_Receive_DMA(&huart4,u_rx_buf_4,sizeof(u_rx_buf_4));//开启DMA传输
	__HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);         //开启uart4串口空闲中断
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_DMA_Init();
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	User_Init();                                                                    //用户初始化代码

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		/*PS2读取函数 开始*/
		PS2_Application();			                                                      //调用PS2读取函数
		/*PS2读取函数 结束*/

		/*读取AOA测量值实现跟随控制 开始*/
		if(AOA_DMA_receive_flag == 1)                                                 //如果DMA接收了一帧uart2数据，执行频率与AOA刷新频率一致，默认50Hz
		{
			AOA_Application();                                                          //调用AOA应用函数
			AOA_DMA_receive_flag=0;                                                     //清除标志位
		}
		/*读取AOA测量值实现跟随控制 结束*/

		/*读取TOFSense测量值 开始*/
		if(TOF_DMA_receive_flag == 1)                                                 //如果DMA接收了一帧uart3数据，执行频率约为200Hz
		{
			TOF_Application();                                                          //调用TOFSense应用函数
			TOF_DMA_receive_flag=0;                                                     //清除标志位
		}
		/*读取TOFSense测量值 结束*/

		/*读取LinkTrack测量值 开始*/
		if(LT_DMA_receive_flag == 1)                                                  //如果DMA接收了一帧uart1数据，无数传帧情况下执行频率与LT刷新频率一致，默认50Hz
		{
			LT_Application();                                                           //调用LinkTrack应用函数
			LT_DMA_receive_flag=0;                                                      //清除标志位
		}
		/*读取LinkTrack测量值 结束*/

		/*读取外置9轴IMU测量值 开始*/
		if(WTIMU_DMA_receive_flag == 1)                                               //如果DMA接收了一帧uart4数据，执行频率与外置IMU刷新频率一致
		{
			WTIMU_Application();                                                        //调用外置9轴IMU应用函数
			WTIMU_DMA_receive_flag=0;                                                   //清除标志位
		}
		/*读取外置9轴IMU测量值 结束*/

		/*RGB灯条控制 开始*/
		RGB_Show(mode_selection);	                                                    //RGB灯条显示
		/*RGB灯条控制 结束*/

		/*电池低电压检测报警 开始*/
		Battery_Low_Voltage_Warning();                                                //电池低电压控制蜂鸣器报警防止过放
		/*电池低电压检测报警 结束*/

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Supply configuration update enable 
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source 
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_SPI1|RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 2;
  PeriphClkInitStruct.PLL2.PLL2N = 12;
  PeriphClkInitStruct.PLL2.PLL2P = 5;
  PeriphClkInitStruct.PLL2.PLL2Q = 5;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
  PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
