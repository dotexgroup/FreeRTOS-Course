
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
BaseType_t xQueueSend( QueueHandle_t xQueue,
											const void * pvItemToQueue,
											TickType_t xTicksToWait );
####################################################################
configUSE_QUEUE_SETS
QueueSetHandle_t xQueueCreateSet( const UBaseType_t uxEventQueueLength );
####################################################################
BaseType_t xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
													 QueueSetHandle_t xQueueSet );
####################################################################
QueueSetMemberHandle_t xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
																						const TickType_t xTicksToWait );
####################################################################
*//////////////////////////////////////////////////




QueueHandle_t xUint32tQueue;
QueueHandle_t xCharPointerQueue;
QueueSetHandle_t xQueueSet;




void vSenderTask1( void *pvParameters )
{
	const TickType_t xBlockTime = pdMS_TO_TICKS( 100 );
	const char * const pcMessage = "Message\r\n";

	for( ;; )
	{
		vTaskDelay( xBlockTime );
		xQueueSend( xCharPointerQueue, &pcMessage, 0 );
	}
}


void vSenderTask2( void *pvParameters )
{
	const TickType_t xBlockTime = pdMS_TO_TICKS( 200 );
	uint32_t x = 10;
	
	for( ;; )
	{
		vTaskDelay( xBlockTime );
		xQueueSend( xUint32tQueue, &x, 0 );
	}
}

void vReceiverTask( void *pvParameters )
{
	QueueSetMemberHandle_t xHandle;
	
	char *pcReceivedString;
	uint32_t ulRecievedValue;
	const TickType_t xDelay100ms = pdMS_TO_TICKS( 200 );
	for( ;; )
	{

		xHandle = xQueueSelectFromSet( xQueueSet, portMAX_DELAY );

		if( xHandle == NULL )
		{
			printf("Error");
		}
		else if( xHandle == ( QueueSetMemberHandle_t ) xCharPointerQueue )
		{
			xQueueReceive( xCharPointerQueue, &pcReceivedString, 0 );
			printf("%s", pcReceivedString );
		}
		else if( xHandle == ( QueueSetMemberHandle_t ) xUint32tQueue )
		{
			xQueueReceive(xUint32tQueue, &ulRecievedValue, 0 );
			printf("%d\n", ulRecievedValue );
		}	
	}
}
//////////////////////////////////////////////////
int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();


	xUint32tQueue = xQueueCreate( 1, sizeof(uint32_t ) );
  xCharPointerQueue = xQueueCreate( 1, sizeof(char *) );
	
	xQueueSet = xQueueCreateSet( 1+1);
	
	xQueueAddToSet( xUint32tQueue, xQueueSet );
  xQueueAddToSet( xCharPointerQueue, xQueueSet );
	
	xTaskCreate( vSenderTask1, "Sender1", 128, NULL, 1, NULL );
	xTaskCreate( vSenderTask2, "Sender2", 64, NULL, 1, NULL );
	
	xTaskCreate( vReceiverTask, "Receiver", 64, NULL, 2, NULL );
	
	vTaskStartScheduler();


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
