
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdlib.h>

TIM_HandleTypeDef htim2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/*/////////////////////////////////////////////////
####################################################################
EventGroupHandle_t xEventGroupCreate( void );			//xEventGroupCreateStatic()
####################################################################
EventBits_t xEventGroupSetBits( EventGroupHandle_t xEventGroup,
																const EventBits_t uxBitsToSet );
####################################################################
BaseType_t xEventGroupSetBitsFromISR( EventGroupHandle_t xEventGroup,
																			const EventBits_t uxBitsToSet,
																			BaseType_t *pxHigherPriorityTaskWoken );
####################################################################
EventBits_t xEventGroupWaitBits( const EventGroupHandle_t xEventGroup,
																const EventBits_t uxBitsToWaitFor,
																const BaseType_t xClearOnExit,
																const BaseType_t xWaitForAllBits,
																TickType_t xTicksToWait );
####################################################################	
EventBits_t xEventGroupSync( EventGroupHandle_t xEventGroup,
														const EventBits_t uxBitsToSet,
														const EventBits_t uxBitsToWaitFor,
														TickType_t xTicksToWait );

*//////////////////////////////////////////////////
#define mainFIRST_TASK_BIT ( 1UL << 0UL ) 
#define mainSECOND_TASK_BIT ( 1UL << 1UL ) 
#define mainTHIRD_TASK_BIT  ( 1UL << 2UL )

EventGroupHandle_t xEventGroup;
//////////////////////////////////////////////////

static void vSyncingTask( void *pvParameters )
{
	const TickType_t xMaxDelay = pdMS_TO_TICKS( 4000UL );
	const TickType_t xMinDelay = pdMS_TO_TICKS( 200UL );
	TickType_t xDelayTime;
	EventBits_t uxThisTasksSyncBit;
	const EventBits_t uxAllSyncBits = ( mainFIRST_TASK_BIT |
																			mainSECOND_TASK_BIT |
																			mainTHIRD_TASK_BIT );
	uxThisTasksSyncBit = ( EventBits_t ) pvParameters;
	for( ;; )
	{
		xDelayTime = ( rand() % xMaxDelay ) + xMinDelay;
		vTaskDelay( xDelayTime );
		printf(pcTaskGetTaskName( NULL ));
		printf( "\treached sync point\n" );
		xEventGroupSync(  xEventGroup,
											uxThisTasksSyncBit,
											uxAllSyncBits,
											portMAX_DELAY );

		printf( pcTaskGetTaskName( NULL ) );
		printf( "\texited sync point\n" );
		}
}


//////////////////////////////////////////////////
int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();
	__HAL_TIM_CLEAR_FLAG(&htim2, TIM_SR_UIF);
	HAL_TIM_Base_Start_IT(&htim2);
	
	xEventGroup = xEventGroupCreate();

	xTaskCreate( vSyncingTask, "Task 1", 64, (void*)mainFIRST_TASK_BIT, 1, NULL );
	xTaskCreate( vSyncingTask, "Task 2", 64,(void*) mainSECOND_TASK_BIT, 1, NULL );
	xTaskCreate( vSyncingTask, "Task 3", 64, (void*)mainTHIRD_TASK_BIT, 1, NULL );

	vTaskStartScheduler();
	
	for( ;; );
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 35999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 2000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
	else  if(htim->Instance==TIM2)
  {  
		//
  }
  
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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