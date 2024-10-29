
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdlib.h>
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/*/////////////////////////////////////////////////
####################################################################
TimerHandle_t xTimerCreate( const char * const pcTimerName,
														TickType_t xTimerPeriodInTicks,
														UBaseType_t uxAutoReload,
														void * pvTimerID,
														TimerCallbackFunction_t pxCallbackFunction );
####################################################################
BaseType_t xTimerStart( TimerHandle_t xTimer, TickType_t xTicksToWait );
####################################################################
TickType_t xTaskGetTickCount( void );
####################################################################
void vTimerSetTimerID( const TimerHandle_t xTimer, void *pvNewID );
####################################################################
void *pvTimerGetTimerID( TimerHandle_t xTimer );
####################################################################
BaseType_t xTimerStop( TimerHandle_t xTimer, TickType_t xTicksToWait );
####################################################################
BaseType_t xTimerChangePeriod( TimerHandle_t xTimer,
																TickType_t xNewTimerPeriodInTicks,
																TickType_t xTicksToWait );
####################################################################
BaseType_t xTimerReset( TimerHandle_t xTimer, TickType_t xTicksToWait );
*//////////////////////////////////////////////////




#define mainONE_SHOT_TIMER_PERIOD pdMS_TO_TICKS( 3333 )
#define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS( 500 )
#define NEW_TIMER_PERIOD pdMS_TO_TICKS( 200 )

TimerHandle_t xAutoReloadTimer, xOneShotTimer;
BaseType_t xTimer1Started, xTimer2Started;
//////////////////////////////////////////////////
uint32_t timerId = 0;
void TimerCallback( TimerHandle_t xTimer )
	{
	TickType_t xTimeNow;
	uint32_t id = 0;
	xTimeNow = xTaskGetTickCount();
		
		
		
	if(xTimer == xOneShotTimer)
		{
			printf( "One-shot timer callback executing = %d\n", xTimeNow );
			xTimerChangePeriod(xAutoReloadTimer ,NEW_TIMER_PERIOD , 0 );
		}
	else if (xTimer == xAutoReloadTimer)
		{
			id = ( uint32_t )pvTimerGetTimerID(xTimer);

			printf( "Auto-reload timer callback executing = %d\n", xTimeNow );
			vTimerSetTimerID(xTimer ,( void *)timerId) ;
			timerId++;
			if(id == 15)
				xTimerStop(xTimer,0);
		}

	}



//////////////////////////////////////////////////
int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();


	xOneShotTimer = xTimerCreate("OneShot",
																mainONE_SHOT_TIMER_PERIOD,
																pdFALSE,
																( uint32_t *)0,
																TimerCallback );
	xAutoReloadTimer = xTimerCreate("AutoReload",
																mainAUTO_RELOAD_TIMER_PERIOD,
																pdTRUE,
																( uint32_t *)0,
																TimerCallback );
	
	
	if( ( xOneShotTimer != NULL ) && ( xAutoReloadTimer != NULL ) )
	{	
		xTimer1Started = xTimerStart( xOneShotTimer, 0 );
		xTimer2Started = xTimerStart( xAutoReloadTimer, 0 );
		if( ( xTimer1Started == pdPASS ) && ( xTimer2Started == pdPASS ) )
		{
			vTaskStartScheduler();
		}
	}
	
	for( ;; );
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
