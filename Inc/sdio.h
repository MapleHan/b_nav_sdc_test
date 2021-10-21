/**
  ******************************************************************************
  * File Name          : SDIO.h
  * Description        : This file provides code for the configuration
  *                      of the SDIO instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sdio_H
#define __sdio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SD_HandleTypeDef hsd;

/* USER CODE BEGIN Private defines */

#define SDIO_TIMEOUT    10

/* USER CODE END Private defines */

void MX_SDIO_SD_Init(void);

/* USER CODE BEGIN Prototypes */

void SDIO_ShowInfo(void);
HAL_StatusTypeDef SD_GetCardState(void);
HAL_StatusTypeDef SDIO_Write_Sector(uint32_t sector_addr, uint32_t sector_cnt, uint8_t* buf);
HAL_StatusTypeDef SDIO_Write_Buf(uint32_t sector_addr, uint8_t* buf, uint32_t len);
HAL_StatusTypeDef SDIO_Read_Sector(uint32_t sector_addr, uint32_t sector_cnt, uint8_t* buf);
HAL_StatusTypeDef SDIO_Read_Buf(uint32_t sector_addr, uint8_t* buf, uint32_t len);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ sdio_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
