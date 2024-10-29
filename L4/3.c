
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdlib.h>
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/*/////////////////////////////////////////////////
####################################################################
QueueHandle_t myQueue;
QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );

QueueHandle_t xQueueCreateStatic( UBaseType_t uxQueueLength,
																	UBaseType_t uxItemSize,
																	uint8_t *pucQueueStorageBuffer,
																	StaticQueue_t *pxQueueBuffer );
####################################################################
BaseType_t xQueueReset( QueueHandle_t xQueue );
####################################################################
BaseType_t xQueueSendToFront( QueueHandle_t xQueue,
															const void * pvItemToQueue,
															TickType_t xTicksToWait );
####################################################################
BaseType_t xQueueSendToBack( QueueHandle_t xQueue,
														const void * pvItemToQueue,
														TickType_t xTicksToWait );	
####################################################################														
pdPASS  , errQUEUE_FULL
####################################################################
pdMS_TO_TICKS()
portMAX_DELAY
INCLUDE_vTaskSuspend
####################################################################
BaseType_t xQueueReceive( QueueHandle_t xQueue,
													void * const pvBuffer,
													TickType_t xTicksToWait );
pdPASS , errQUEUE_EMPTY
####################################################################
UBaseType_t uxQueueMessagesWaiting( QueueHandle_t xQueue );
####################################################################
*//////////////////////////////////////////////////

QueueHandle_t xQueue;

uint8_t txBuff[50];

static void vSenderTask( void *pvParameters )
{ 
		BaseType_t xStatus;
		const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );
		uint8_t i = 0;
		uint8_t *ad  = txBuff;
		for( ;; )
			{
			sprintf((char *)txBuff , "string%d\n" ,i++ );
			xStatus = xQueueSendToBack( xQueue, &ad, xTicksToWait );
			if( xStatus != pdPASS )
					printf( "Could not send to the queue.\r\n" );
			}
}
//////////////////////////////////////////////////

static void vReceiverTask( void *pvParameters )
{
	BaseType_t xStatus;
	
	uint8_t *add  ;
	for( ;; )
		{
	
				xStatus = xQueueReceive( xQueue, &add, 0 );
				if( xStatus == pdPASS )
					{
								printf( "%s\n", add);
					}
				else
						printf( "Could not receive from the queue.\r\n" );
		}
}
//////////////////////////////////////////////////
int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();


	xQueue = xQueueCreate( 1, sizeof(uint8_t *) );
	if( xQueue != NULL )
		{
			xTaskCreate( vSenderTask, "Task1", 64, NULL, 2, NULL );
			xTaskCreate( vReceiverTask, "Receiver", 128, NULL, 1, NULL );
			vTaskStartScheduler();
		}
	else
			printf( "Error.\r\n" );	
	


  while (1)
  {
  }

}

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


static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}


void Error_Handler(void)
{

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
