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

static uint8_t WriteStatus = 0;
static uint8_t ReadStatus = 0;
/* USER CODE END 0 */

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;

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

    /* SDIO DMA Init */
    /* SDIO_RX Init */
    hdma_sdio_rx.Instance = DMA2_Stream3;
    hdma_sdio_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio_rx.Init.Mode = DMA_PFCTRL;
    hdma_sdio_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    hdma_sdio_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio_rx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio_rx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&hdma_sdio_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(sdHandle,hdmarx,hdma_sdio_rx);

    /* SDIO_TX Init */
    hdma_sdio_tx.Instance = DMA2_Stream6;
    hdma_sdio_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sdio_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio_tx.Init.Mode = DMA_PFCTRL;
    hdma_sdio_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    hdma_sdio_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio_tx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio_tx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&hdma_sdio_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(sdHandle,hdmatx,hdma_sdio_tx);

    /* SDIO interrupt Init */
    HAL_NVIC_SetPriority(SDIO_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);
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

    /* SDIO DMA DeInit */
    HAL_DMA_DeInit(sdHandle->hdmarx);
    HAL_DMA_DeInit(sdHandle->hdmatx);

    /* SDIO interrupt Deinit */
    HAL_NVIC_DisableIRQ(SDIO_IRQn);
  /* USER CODE BEGIN SDIO_MspDeInit 1 */

  /* USER CODE END SDIO_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

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

HAL_StatusTypeDef SDIO_GetCardState(void)
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

//一次写一个Sector约700-900us，一次写10个Sector约1600us，但开机后第一次写入单个Sector耗时可能较长约2-5ms
HAL_StatusTypeDef SDIO_Write_Sector(uint32_t sector_addr, uint32_t sector_cnt, uint8_t* buf)
{
    static HAL_StatusTypeDef state;
    uint32_t timeout;
        
    WriteStatus = 0;
    state = HAL_SD_WriteBlocks_DMA(&hsd, buf, sector_addr, sector_cnt);
    if(state == HAL_OK)
    {
        timeout = HAL_GetTick();
        while((WriteStatus == 0) && ((HAL_GetTick() - timeout) < SDIO_TIMEOUT))
        {
        }
        /* incase of a timeout return error */
        if (WriteStatus == 0)
        {
          state = HAL_ERROR;
        }
        else
        {
          WriteStatus = 0;
          timeout = HAL_GetTick();
          state = HAL_ERROR;
          while((HAL_GetTick() - timeout) < SDIO_TIMEOUT)
          {
            if (SDIO_GetCardState() == HAL_OK)
            {
              state = HAL_OK;
              break;
            }
          }
        }
    }
    return(state);
}

static uint32_t SDIO_Min(uint32_t a, uint32_t b)
{
    if(a<=b)
        return(a);
    return(b);
}

//该函数是对SDIO_Write_Sector的封装，主要是为了确保buf是4字节对齐的
//该函数无论数据长度是多上，都只能一个一个Sector地发送
//所以一次写一个Sector约710--910us，一次写10个Sector约7100--9100us
//所以，尽量不要使用该函数；应由用户保证提供的buf是4字节对其的。
HAL_StatusTypeDef SDIO_Write_Buf(uint32_t sector_addr, uint8_t* buf, uint32_t len)
{
    uint32_t sector_cnt = SDIO_BufLen2SecCnt(len);
    static HAL_StatusTypeDef state;

    for(uint32_t n=0;n<sector_cnt;n++)
    {
        memset(cpy_buf, 0, BLOCK_SIZE);
        memcpy(cpy_buf, buf, SDIO_Min(BLOCK_SIZE, len));
        state = SDIO_Write_Sector(sector_addr+n, 1, cpy_buf);//单个sector的写操作
        if (state != HAL_OK)
            break;
        if(len > BLOCK_SIZE)
        {
            buf += BLOCK_SIZE;
            len -= BLOCK_SIZE;
        }
    }
    return(state);
}

//一次读一个Sector约200--240us，一次读10个Sector约600--800us，但开机后第一次读取单个Sector耗时可能较长约2--5ms
HAL_StatusTypeDef SDIO_Read_Sector(uint32_t sector_addr, uint32_t sector_cnt, uint8_t* buf)
{
    static HAL_StatusTypeDef state;
    uint32_t timeout;

    ReadStatus = 0;
    state = HAL_SD_ReadBlocks_DMA(&hsd, buf, sector_addr, sector_cnt);
    if (state == HAL_OK)
    {
        /* Wait that the reading process is completed or a timeout occurs */
        timeout = HAL_GetTick();
        while((ReadStatus == 0) && ((HAL_GetTick() - timeout) < SDIO_TIMEOUT))
        {
        }
        /* incase of a timeout return error */
        if (ReadStatus == 0)
        {
          state = HAL_ERROR;
        }
        else
        {
          ReadStatus = 0;
          timeout = HAL_GetTick();
          state = HAL_ERROR;
          while((HAL_GetTick() - timeout) < SDIO_TIMEOUT)
          {
            if (SDIO_GetCardState() == HAL_OK)
            {
                state = HAL_OK;
                break;
            }
          }
        }
    }
    return(state);
}

//该函数是对SDIO_Read_Sector的封装，主要是为了确保buf是4字节对齐的
//该函数无论数据长度是多上，都只能一个一个Sector地读取
//所以一次读一个Sector约215--255us，一次写10个Sector约2150--2550us
//所以，尽量不要使用该函数；应由用户保证提供的buf是4字节对其的。
HAL_StatusTypeDef SDIO_Read_Buf(uint32_t sector_addr, uint8_t* buf, uint32_t len)
{
    uint32_t sector_cnt = SDIO_BufLen2SecCnt(len);
    static HAL_StatusTypeDef state;

    for(uint32_t n=0;n<sector_cnt;n++)
    {
        memset(cpy_buf, 0, BLOCK_SIZE);
        state = SDIO_Read_Sector(sector_addr+n, 1, cpy_buf);//单个sector的读操作
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
    return(state);
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hsd: SD handle
  */
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
  WriteStatus = 1;
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: SD handle
  */
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
  ReadStatus = 1;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
