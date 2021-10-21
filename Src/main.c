/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "dma.h"
#include "sdio.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "my_printf.h"
#include "string.h"
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
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_SDIO_SD_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  SDIO_ShowInfo();
  uint8_t t = 0;
  HAL_UART_Receive(&huart1, &t, 1, 1000000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t intx = 0;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    const uint32_t SEC_CNT = 2;
    const uint32_t BUF_SIZE = SEC_CNT*512;
    __align(4) uint8_t buf[BUF_SIZE];
    for (int i=0;i<BUF_SIZE;i++)
    {
        buf[i] = 'A' + i%5;
    }
    DBG_Print(DBG_INFO, "Raw Data\r\n");
    DBG_Print(DBG_INFO, "%s", buf);
    DBG_Print(DBG_INFO, "\r\n\r\n");
    HAL_Delay(100);

    HAL_StatusTypeDef state;

    state = SDIO_Write_Sector(0, SEC_CNT, buf);
    if(state == HAL_OK)
        DBG_Print(DBG_INFO, "SDIO_Write_Sector 0 OK\r\n\r\n");
    else
        DBG_Print(DBG_INFO, "SDIO_Write_Sector 0 Err\r\n\r\n");
    HAL_Delay(100);

    state = SDIO_Write_Buf(2, buf, BUF_SIZE);
    if(state == HAL_OK)
        DBG_Print(DBG_INFO, "SDIO_Write_Buf 2 OK\r\n\r\n");
    else
        DBG_Print(DBG_INFO, "SDIO_Write_Buf 2 Err\r\n\r\n");
    HAL_Delay(100);



    memset(buf, 0, BUF_SIZE);
    state = SDIO_Read_Sector(0, SEC_CNT, buf);
    if(state == HAL_OK)
    {
        DBG_Print(DBG_INFO, "SDIO_Read_Sector 0 OK\r\n");
        DBG_Print(DBG_INFO, "%s", buf);
        DBG_Print(DBG_INFO, "\r\n");
    }
    else
        DBG_Print(DBG_INFO, "SDIO_Read_Sector 0 Err\r\n\r\n");
    HAL_Delay(100);

    memset(buf, 0, BUF_SIZE);
    state = SDIO_Read_Sector(2, SEC_CNT, buf);
    if(state == HAL_OK)
    {
        DBG_Print(DBG_INFO, "SDIO_Read_Sector 2 OK\r\n");
        DBG_Print(DBG_INFO, "%s", buf);
        DBG_Print(DBG_INFO, "\r\n");
    }
    else
        DBG_Print(DBG_INFO, "SDIO_Read_Sector 2 Err\r\n\r\n");
    HAL_Delay(100);


    memset(buf, 0, BUF_SIZE);
    state = SDIO_Read_Buf(0, buf, BUF_SIZE);
    if(state == HAL_OK)
    {
        DBG_Print(DBG_INFO, "SDIO_Read_Buf 0 OK\r\n");
        DBG_Print(DBG_INFO, "%s", buf);
        DBG_Print(DBG_INFO, "\r\n");
    }
    else
        DBG_Print(DBG_INFO, "SDIO_Read_Buf 0 Err\r\n\r\n");
    HAL_Delay(100);

    memset(buf, 0, BUF_SIZE);
    state = SDIO_Read_Buf(2, buf, BUF_SIZE);
    if(state == HAL_OK)
    {
        DBG_Print(DBG_INFO, "SDIO_Read_Buf 2 OK\r\n");
        DBG_Print(DBG_INFO, "%s", buf);
        DBG_Print(DBG_INFO, "\r\n");
    }
    else
        DBG_Print(DBG_INFO, "SDIO_Read_Buf 2 Err\r\n\r\n");
    HAL_Delay(100);

    
    HAL_Delay(20000000);
    
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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
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