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
#include "hardwareAPI.h"
#include "STM32F1.h"
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
struct systemTime_t
{
    uint32_t ms;
    uint32_t ns;
};
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
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
errno_t initHardware(void)
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
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  // "Please pay special attention to the static variable SystemCoreClock. This variable might 
  // be used throughout the whole system initialization and runtime to calculate frequency/time 
  // related values. Thus one must assure that the variable always reflects the actual system 
  // clock speed. Be aware that a value stored to SystemCoreClock during low level initializaton 
  // (i.e. SystemInit()) might get overwritten by C libray startup code and/or .bss section 
  // initialization. Thus its highly recommended to call SystemCoreClockUpdate at the beginning 
  // of the user main() routine." (from CMSIS-Core (Cortex-M) System and Clock Configuration,
  // https://www.keil.com/pack/doc/cmsis/Core/html/group__system__init__gr.html)
  //
  SystemCoreClockUpdate();

  /* USER CODE END 2 */
  return 0;

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_BUILTIN_Pin */
  GPIO_InitStruct.Pin = LED_BUILTIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_BUILTIN_GPIO_Port, &GPIO_InitStruct);

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

void assert_failed(const char * file, uint32_t line)
{
    // Turn on the on-board LED to indicate a failed assert
    //
    HAL_GPIO_WritePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin, GPIO_PIN_RESET);
    
    // Enter infinite loop; halt the processor if a debugger is attached
    //
    while(1)
    {
        HALT_IF_DEBUGGING();
    }
}

p_systemTime_t systemTime_create(void)
{
    return (p_systemTime_t)calloc(1, sizeof(struct systemTime_t));
}

errno_t getSystemTime(p_systemTime_t p_thisTime)
{
    uint32_t SysTick_Value;

    ASSERT(p_thisTime != NULL);

    // Ensure uwTick (accessed through HAL_GetTick) isn't updated while retrieving SysTick->VAL
    //
    do
    {
        p_thisTime->ms = HAL_GetTick();
        SysTick_Value = SysTick->VAL;
    } while( p_thisTime->ms != HAL_GetTick() );

    // NOTE: Order of operations, currently enforced with parentheses, is CRITICAL to prevent
    // overflow.
    //
    p_thisTime->ns = ( ( SysTick->LOAD - SysTick_Value ) * ( 1000000000.0 / (float)SystemCoreClock ) );

    return 0;
}

errno_t elapsedSystemTime_ns(uint32_t * time_ns)
{
    errno_t err = 0;
    uint32_t ms;
    uint32_t SysTick_Value;

    ASSERT( time_ns != NULL );

    // Ensure uwTick (accessed through HAL_GetTick) isn't updated while retrieving SysTick->VAL
    //
    do
    {
        ms = HAL_GetTick();
        SysTick_Value = SysTick->VAL;
    } while ( ms != HAL_GetTick() );

    // NOTE: Order of operations, currently enforced with parentheses, is CRITICAL to prevent
    // overflow.
    //
    *time_ns = ( ( 1000000 * ms ) + ( ( SysTick->LOAD - SysTick_Value ) * ( 1000000000.0 / (float)SystemCoreClock ) ) );

    return err;
}

uint32_t systemTimeDiff_ns(p_systemTime_t p_startTime, p_systemTime_t p_endTime)
{
    ASSERT(p_startTime != NULL);
    ASSERT(p_endTime != NULL);

    // NOTE: No checks are made for overflow, meaning the results of this function are undefined
    // if the difference in nanoseconds between p_startTime and p_endTime is greater than UINT32_MAX
    // (i.e. greater than 4,294,967,295, or 4.294967295 seconds).
    uint32_t diff_ms = p_endTime->ms - p_startTime->ms;
    uint32_t diff_ns = p_endTime->ns - p_startTime->ns;

    return ( ( 1000000 * diff_ms ) + diff_ns );
}

void printResults(uint32_t iterations, float executionTime_sin_ns_avg, float executionTime_sin_LUT_ns_avg, float absoluteError_sin_LUT_avg, float percentError_sin_LUT_avg)
{
    while(1)
    {
        HALT_IF_DEBUGGING();
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
