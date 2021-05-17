/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <app_cfg.h>
#include <cpu_core.h>
#include <os.h>
#include "stdio.h"
#include <math.h>
#include "lib_str.h"
#include "stdarg.h"
#include "tareas.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#warning "Modify this value to match the number of external interrupts in your MCU"
#define EXT_INT_MAX_NBR 16u
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */
static void StartupTask 			(void *p_arg);
static void LecturaPulsadores		(void *p_arg);
static void ActualizarDisplay		(void *p_arg);
static void ControlMotor			(void *p_arg);



static OS_STK StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];
static OS_STK Task1Stk[APP_CFG_TASK1_STK_SIZE];
static OS_STK Task2Stk[APP_CFG_TASK2_STK_SIZE];
static OS_STK Task3Stk[APP_CFG_TASK3_STK_SIZE];
static OS_STK Task4Stk[APP_CFG_TASK4_STK_SIZE];
static OS_STK Task5Stk[APP_CFG_TASK5_STK_SIZE];
static OS_STK Task6Stk[APP_CFG_TASK6_STK_SIZE];

static void App_TaskCreate (void);
static void App_Task2Create (void);
static void App_Task3Create (void);
static void App_Task4Create (void);
static void App_Task5Create (void);
static void App_Task6Create (void);
void UsbPrintf (CPU_CHAR  *p_fmt, ...);





/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
#if (OS_TASK_NAME_EN > 0u)
CPU_INT08U os_err;
#endif
CPU_INT16U int_id;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  CPU_IntDis();
  for (int_id = CPU_INT_EXT0; int_id <= (EXT_INT_MAX_NBR - 1u); int_id++)
  {
  /* Set all external intr. to KA interrupt priority boundary */
  CPU_IntSrcPrioSet(int_id, CPU_CFG_KA_IPL_BOUNDARY, CPU_INT_KA);
  }

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */


  //HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);



  OSInit();
  OSTaskCreateExt( StartupTask,
                   0,
				   &StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE - 1],
				   APP_CFG_STARTUP_TASK_PRIO,
				   APP_CFG_STARTUP_TASK_PRIO,
				   &StartupTaskStk[0],
				   APP_CFG_STARTUP_TASK_STK_SIZE,
				   0,
				   (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
  #if (OS_TASK_NAME_EN > 0u)
   OSTaskNameSet( APP_CFG_STARTUP_TASK_PRIO,
    	  	      (INT8U *)"Startup task",
				  &os_err);
  #endif

  OSStart();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 1-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 36000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 18000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin
                          |LCD_RS_Pin|LCD_RW_Pin|LCD_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PUL_Pin|DIR_Pin|ENA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D4_Pin LCD_D5_Pin LCD_D6_Pin LCD_D7_Pin
                           LCD_RS_Pin LCD_RW_Pin LCD_EN_Pin */
  GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin
                          |LCD_RS_Pin|LCD_RW_Pin|LCD_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Sensor_rejillas_Pin Fc_Inf_Pin Fc_Sup_Pin PULSADOR1_Pin
                           PULSADOR2_Pin PULSADOR3_Pin PULSADOR4_Pin */
  GPIO_InitStruct.Pin = Sensor_rejillas_Pin|Fc_Inf_Pin|Fc_Sup_Pin|PULSADOR1_Pin
                          |PULSADOR2_Pin|PULSADOR3_Pin|PULSADOR4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PUL_Pin DIR_Pin ENA_Pin */
  GPIO_InitStruct.Pin = PUL_Pin|DIR_Pin|ENA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/*
*************************************************************************
* STM32Cube HAL FUNCTIONS
*************************************************************************
*/
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
/* define as empty to prevent the system tick being initialized before
    the OS starts */
 return (HAL_OK);
}

uint32_t HAL_GetTick(void)
{
 CPU_INT32U os_tick_ctr;

 #if (OS_VERSION >= 30000u)
  OS_ERR os_err;
  os_tick_ctr = OSTimeGet(&os_err);
 #else
  os_tick_ctr = OSTimeGet();
 #endif

 return os_tick_ctr;
}

/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void StartupTask (void *p_arg)
{
 CPU_INT32U cpu_clk;
 (void)p_arg;
 cpu_clk = HAL_RCC_GetHCLKFreq();
 /* Initialize and enable System Tick timer */
 OS_CPU_SysTickInitFreq(cpu_clk);

 #if (OS_TASK_STAT_EN > 0)
  OSStatInit();                                               /* Determine CPU capacity.                              */
 #endif

// App_EventCreate();                                          /* Create application events.                           */
// App_TaskCreate();
  App_TaskCreate();			/* Create application tasks.               */
  App_Task2Create();		/* Create application tasks.  			   */
  App_Task3Create();		/* Create application tasks.  			   */
  App_Task4Create();
  App_Task5Create();
  App_Task6Create();

 while (DEF_TRUE){
   HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
   //UsbPrintf("uCOS-II Running...\n");
   OSTimeDlyHMSM(0u, 0u, 1u, 0u);
  }
}





/*
**************************************************************************************************************************
*                                               UsbPrintfMsg()
*
* Description :
* Argument(s) : none
* Return(s)   : none.
* Caller(s)   :
* Note(s)     : none.
**************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                 static void App_TaskCreate (void)
*
* Description : Creacion de las tareas
*
* Argument(s) : none
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void App_TaskCreate (void)
{
	CPU_INT08U os_err;

	os_err = OSTaskCreateExt((void (*)(void *)) LecturaPulsadores,
							 (void			* )0,
							 (OS_STK		* )&Task1Stk[APP_CFG_TASK1_STK_SIZE - 1],
							 (INT8U			  ) APP_CFG_TASK1_PRIO,
							 (INT16U		  ) APP_CFG_TASK1_PRIO,
							 (OS_STK		* )&Task1Stk[0],
							 (INT32U		  ) APP_CFG_TASK1_STK_SIZE,
							 (void			* )0,
							 (INT16U		  )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

	OSTaskNameSet(APP_CFG_TASK1_PRIO, (unsigned char *)"Pulsadores", &os_err);
}

static void App_Task2Create (void)
{
	CPU_INT08U os_err;

	os_err = OSTaskCreateExt((void (*)(void *)) ActualizarDisplay,
							 (void			* )0,
							 (OS_STK		* )&Task2Stk[APP_CFG_TASK2_STK_SIZE - 1],
							 (INT8U			  ) APP_CFG_TASK2_PRIO,
							 (INT16U		  ) APP_CFG_TASK2_PRIO,
							 (OS_STK		* )&Task2Stk[0],
							 (INT32U		  ) APP_CFG_TASK2_STK_SIZE,
							 (void			* )0,
							 (INT16U		  )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

	OSTaskNameSet(APP_CFG_TASK2_PRIO, (unsigned char *)"Display", &os_err);
}

static void App_Task3Create (void)
{
	CPU_INT08U os_err;

	os_err = OSTaskCreateExt((void (*)(void *)) ControlMotor,
							 (void			* )0,
							 (OS_STK		* )&Task3Stk[APP_CFG_TASK3_STK_SIZE - 1],
							 (INT8U			  ) APP_CFG_TASK3_PRIO,
							 (INT16U		  ) APP_CFG_TASK3_PRIO,
							 (OS_STK		* )&Task3Stk[0],
							 (INT32U		  ) APP_CFG_TASK3_STK_SIZE,
							 (void			* )0,
							 (INT16U		  )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

	OSTaskNameSet(APP_CFG_TASK3_PRIO, (unsigned char *)"Control Motor", &os_err);
}

static void App_Task4Create (void)
{
	CPU_INT08U os_err;

	os_err = OSTaskCreateExt((void (*)(void *)) Detener,
							 (void			* )0,
							 (OS_STK		* )&Task4Stk[APP_CFG_TASK4_STK_SIZE - 1],
							 (INT8U			  ) APP_CFG_TASK4_PRIO,
							 (INT16U		  ) APP_CFG_TASK4_PRIO,
							 (OS_STK		* )&Task4Stk[0],
							 (INT32U		  ) APP_CFG_TASK4_STK_SIZE,
							 (void			* )0,
							 (INT16U		  )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

	OSTaskNameSet(APP_CFG_TASK4_PRIO, (unsigned char *)"Detener", &os_err);
}

static void App_Task5Create (void)
{
	CPU_INT08U os_err;

	os_err = OSTaskCreateExt((void (*)(void *)) Rampa,
							 (void			* )0,
							 (OS_STK		* )&Task5Stk[APP_CFG_TASK5_STK_SIZE - 1],
							 (INT8U			  ) APP_CFG_TASK5_PRIO,
							 (INT16U		  ) APP_CFG_TASK5_PRIO,
							 (OS_STK		* )&Task5Stk[0],
							 (INT32U		  ) APP_CFG_TASK5_STK_SIZE,
							 (void			* )0,
							 (INT16U		  )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

	OSTaskNameSet(APP_CFG_TASK5_PRIO, (unsigned char *)"Rampa", &os_err);
}

static void App_Task6Create (void)
{
	CPU_INT08U os_err;

	os_err = OSTaskCreateExt((void (*)(void *)) Piezas,
							 (void			* )0,
							 (OS_STK		* )&Task6Stk[APP_CFG_TASK6_STK_SIZE - 1],
							 (INT8U			  ) APP_CFG_TASK6_PRIO,
							 (INT16U		  ) APP_CFG_TASK6_PRIO,
							 (OS_STK		* )&Task6Stk[0],
							 (INT32U		  ) APP_CFG_TASK6_STK_SIZE,
							 (void			* )0,
							 (INT16U		  )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

	OSTaskNameSet(APP_CFG_TASK6_PRIO, (unsigned char *)"Piezas", &os_err);
}
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
