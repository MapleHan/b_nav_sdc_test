/**
  ******************************************************************************
  * File Name          : SDIO.c
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

/* Includes ------------------------------------------------------------------*/
#include "sdio.h"

/* USER CODE BEGIN 0 */
#include "my_printf.h"
#include "string.h"

#define BLOCK_SIZE  (512)
__align(4) static uint8_t cpy_buf[BLOCK_SIZE];

/* USER CODE END 0 */

SD_HandleTypeDef hsd;

/* SDIO init function */

void MX_SDIO_SD_Init(void)
{

  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
  if (HAL_SD_Init(&hsd) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SD_MspInit(SD_HandleTypeDef* sdHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(sdHandle->Instance==SDIO)
  {
  /* USER CODE BEGIN SDIO_MspInit 0 */

  /* USER CODE END SDIO_MspInit 0 */
    /* SDIO clock enable */
    __HAL_RCC_SDIO_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SDIO GPIO Configuration    
    PC8     ------> SDIO_D0
    PC9     ------> SDIO_D1
    PC10     ------> SDIO_D2
    PC11     ------> SDIO_D3
    PC12     ------> SDIO_CK
    PD2     ------> SDIO_CMD 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN SDIO_MspInit 1 */

  /* USER CODE END SDIO_MspInit 1 */
  }
}

void HAL_SD_MspDeInit(SD_HandleTypeDef* sdHandle)
{

  if(sdHandle->Instance==SDIO)
  {
  /* USER CODE BEGIN SDIO_MspDeInit 0 */

  /* USER CODE END SDIO_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SDIO_CLK_DISABLE();
  
    /**SDIO GPIO Configuration    
    PC8     ------> SDIO_D0
    PC9     ------> SDIO_D1
    PC10     ------> SDIO_D2
    PC11     ------> SDIO_D3
    PC12     ------> SDIO_CK
    PD2     ------> SDIO_CMD 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

  /* USER CODE BEGIN SDIO_MspDeInit 1 */

  /* USER CODE END SDIO_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

//关闭所有中断(但是不包括fault和NMI中断)
__asm static void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm static void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}

//通过串口打印SD卡相关信息
void SDIO_ShowInfo(void)
{
	switch(hsd.SdCard.CardType)
	{
		case CARD_SDSC:DBG_Print(DBG_INFO, "SD Standard Capacity <2G\r\n");break;
		case CARD_SDHC_SDXC:DBG_Print(DBG_INFO, "SD High Capacity <32G,SD Extended Capacity <2T\r\n");break;
		case CARD_SECURED:DBG_Print(DBG_INFO, "CARD_SECURED\r\n");break;
        default:break;
	}
    DBG_Print(DBG_INFO, "Card Version:%d\r\n", hsd.SdCard.CardVersion + 1);	            //卡版本
  	DBG_Print(DBG_INFO, "Card Class:%d\r\n", hsd.SdCard.Class);	                        //卡类别
 	DBG_Print(DBG_INFO, "Card RCA:%d\r\n", hsd.SdCard.RelCardAdd);                      //卡相对地址
	DBG_Print(DBG_INFO, "Card Block Capacity:%d MB\r\n",hsd.SdCard.BlockNbr);           //显示块容量
 	DBG_Print(DBG_INFO, "Card Block Size:%d\r\n\r\n", hsd.SdCard.BlockSize);			//显示块大小
	DBG_Print(DBG_INFO, "Card Logical Block Capacity:%d MB\r\n",hsd.SdCard.LogBlockNbr);//显示块容量
 	DBG_Print(DBG_INFO, "Card Logical Block Size:%d\r\n\r\n", hsd.SdCard.LogBlockSize); //显示块大小
}

HAL_StatusTypeDef SD_GetCardState(void)
{
  return((HAL_SD_GetCardState(&hsd)==HAL_SD_CARD_TRANSFER )?HAL_OK:HAL_BUSY);
}

static uint32_t SDIO_BufLen2SecCnt(uint32_t len)
{
    uint32_t sector_cnt = 0;
    if (len == 0)
        sector_cnt = 0;
    else if ((len % BLOCK_SIZE) == 0)
        sector_cnt = len/BLOCK_SIZE;
    else
        sector_cnt = len/BLOCK_SIZE + 1;
    return(sector_cnt);
}

HAL_StatusTypeDef SDIO_Write_Sector(uint32_t sector_addr, uint32_t sector_cnt, uint8_t* buf)
{
    static HAL_StatusTypeDef state;

    INTX_DISABLE();
    while(SD_GetCardState() != HAL_OK) ;
    state = HAL_SD_WriteBlocks(&hsd, buf, sector_addr, sector_cnt, SDIO_TIMEOUT * sector_cnt);
    INTX_ENABLE();
    return(state);
}

static uint32_t SDIO_Min(uint32_t a, uint32_t b)
{
    if(a<=b)
        return(a);
    return(b);
}

HAL_StatusTypeDef SDIO_Write_Buf(uint32_t sector_addr, uint8_t* buf, uint32_t len)
{
    uint32_t sector_cnt = SDIO_BufLen2SecCnt(len);
    static HAL_StatusTypeDef state;

    INTX_DISABLE();
    for(uint32_t n=0;n<sector_cnt;n++)
    {
        memset(cpy_buf, 0, BLOCK_SIZE);
        memcpy(cpy_buf, buf, SDIO_Min(BLOCK_SIZE, len));
        while(SD_GetCardState() != HAL_OK) ;
        state = HAL_SD_WriteBlocks(&hsd, cpy_buf, sector_addr+n, 1, SDIO_TIMEOUT * sector_cnt);//单个sector的写操作
        if (state != HAL_OK)
            break;
        if(len > BLOCK_SIZE)
        {
            buf += BLOCK_SIZE;
            len -= BLOCK_SIZE;
        }
    }
    INTX_ENABLE();
    return(state);
}

HAL_StatusTypeDef SDIO_Read_Sector(uint32_t sector_addr, uint32_t sector_cnt, uint8_t* buf)
{
    static HAL_StatusTypeDef state;

    INTX_DISABLE();
    while(SD_GetCardState() != HAL_OK) ;
    state = HAL_SD_ReadBlocks(&hsd, buf, sector_addr, sector_cnt, SDIO_TIMEOUT * sector_cnt);
    INTX_ENABLE();
    return(state);
}

HAL_StatusTypeDef SDIO_Read_Buf(uint32_t sector_addr, uint8_t* buf, uint32_t len)
{
    uint32_t sector_cnt = SDIO_BufLen2SecCnt(len);
    static HAL_StatusTypeDef state;

    INTX_DISABLE();
    for(uint32_t n=0;n<sector_cnt;n++)
    {
        memset(cpy_buf, 0, BLOCK_SIZE);
        while(SD_GetCardState() != HAL_OK) ;
        state = HAL_SD_ReadBlocks(&hsd, cpy_buf, sector_addr+n, 1, SDIO_TIMEOUT * sector_cnt);//单个sector的写操作
        if (state != HAL_OK)
            break;
        else
        {
            memcpy(buf, cpy_buf, SDIO_Min(BLOCK_SIZE, len));
        }
        if(len > BLOCK_SIZE)
        {
            buf += BLOCK_SIZE;
            len -= BLOCK_SIZE;
        }
    }
    INTX_ENABLE();
    return(state);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
