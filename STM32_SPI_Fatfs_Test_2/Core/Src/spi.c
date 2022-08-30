/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "spi.h"

/* USER CODE BEGIN 0 */

static __IO uint32_t SPITimeout = SPIT_LONG_TIMEOUT;

/* USER CODE END 0 */

SPI_HandleTypeDef hspi5;

/* SPI5 init function */
void MX_SPI5_Init(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi5.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI5_Init 2 */
	__HAL_SPI_ENABLE(&hspi5);
	
	SPI_FLASH_Mode_Init();
  /* USER CODE END SPI5_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI5)
  {
  /* USER CODE BEGIN SPI5_MspInit 0 */

  /* USER CODE END SPI5_MspInit 0 */
    /* SPI5 clock enable */
    __HAL_RCC_SPI5_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**SPI5 GPIO Configuration
    PF7     ------> SPI5_SCK
    PF8     ------> SPI5_MISO
    PF9     ------> SPI5_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
		
  /* USER CODE BEGIN SPI5_MspInit 1 */

  /* USER CODE END SPI5_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI5)
  {
  /* USER CODE BEGIN SPI5_MspDeInit 0 */

  /* USER CODE END SPI5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI5_CLK_DISABLE();

    /**SPI5 GPIO Configuration
    PF7     ------> SPI5_SCK
    PF8     ------> SPI5_MISO
    PF9     ------> SPI5_MOSI
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);

  /* USER CODE BEGIN SPI5_MspDeInit 1 */

  /* USER CODE END SPI5_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
	* @brief SPI Timeout Callback function
	* @param 
	* @retval None
	*/
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
	FLASH_ERROR("SPI Timeout! ErrorCode = %d", errorCode);
	
	return 0;
}

/**
* @brief Use SPI send one byte
* @param byte
*	@retval Return received data
*/
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待发送缓冲区为空，TXE事件 */
  while (__HAL_SPI_GET_FLAG( &hspi5, SPI_FLAG_TXE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  WRITE_REG(hspi5.Instance->DR, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待接收缓冲区非空，RXNE事件 */
  while (__HAL_SPI_GET_FLAG( &hspi5, SPI_FLAG_RXNE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* 读取数据寄存器，获取接收缓冲区数据 */
  return READ_REG(hspi5.Instance->DR);

}

/**
*	@brief Use SPI to read one byte
*	@param 
* @retval None
*/
uint8_t SPI_FLASH_ReadByte(void)
{
	return (SPI_FLASH_SendByte(Dummy_Byte));
}

/**
	* @brief Read Device ID
	* @param
	* @retval
	*/
uint32_t SPI_FLASH_ReadDeviceID(void)
{
	uint32_t temp = 0;
	
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(W25X_DeviceID);
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);
	
	temp = SPI_FLASH_SendByte(Dummy_Byte);
	
	SPI_FLASH_CS_HIGH();
	
	return temp;
	
}

/**
	* @brief Read FLASH ID
	* @param None
	* @retval FLASH ID
	*/
uint32_t SPI_FLASH_ReadID(void)
{
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	
	//Start Communicatioon: CS is LOW
	SPI_FLASH_CS_LOW();
	
	//Send JEDEC instruction, read ID
	SPI_FLASH_SendByte(W25X_JedecDeviceID);
	
	//Read one byte
	Temp0 = SPI_FLASH_SendByte(Dummy_Byte);
	
	//Read one byte
	Temp1 = SPI_FLASH_SendByte(Dummy_Byte);
	
	//Read one byte
	Temp2 = SPI_FLASH_SendByte(Dummy_Byte);
	
	//Stop Communication: CS is HIGH
	SPI_FLASH_CS_HIGH();
	
	//Assemble data
	Temp = (Temp << 16) | (Temp1 << 8) | Temp2;
	
	return Temp;
	
}

/**
	* @brief Send Write Enable to FLASH
	* @param None
	* @retval None
	*/
void SPI_FLASH_WriteEnable(void)
{
	//Start Communication: CS is LOW
	SPI_FLASH_CS_LOW();
	
	//Send write enable 
	SPI_FLASH_SendByte(W25X_WriteEnable);
	
	//Stop Communication; CS is HIGH
	SPI_FLASH_CS_HIGH();
}

/**
	* @brief Wait WIP_Flag to be clear, i.e., Flash write is finish
	* @param None
	* @retval	None
	*/
void SPI_FLASH_WaitForWriteEnd(void)
{
	uint8_t FLASH_Status = 0;
	
	//Start Communication: CS is LOW
	SPI_FLASH_CS_LOW();
	
	//Send Read Reg instruction 
	SPI_FLASH_SendByte(W25X_ReadStatusReg);
	
	//Wait FLASH is free
	do
	{
		FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
	}
	while((FLASH_Status & WIP_Flag) == SET);
	
	//Stop Communication
	SPI_FLASH_CS_HIGH();
	
}

/**
 * @brief
 * @param
 * @retval
 */
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	while(__HAL_SPI_GET_FLAG(&hspi5, SPI_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(2);
		}
	}
	
	WRITE_REG(hspi5.Instance->DR, HalfWord);
	
	while(__HAL_SPI_GET_FLAG(&hspi5, SPI_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(3);
		}
	}
	
	return READ_REG(hspi5.Instance->DR);
	
}

/**
 * @brief
 * @param
 * @retval
 */
 void SPI_FLASH_PowerDown(void)
 {
	//Select FLASH
	SPI_FLASH_CS_LOW();
	
	//Send Power down instruction
	SPI_FLASH_SendByte(W25X_PowerDown);
	
	//Stop
	SPI_FLASH_CS_HIGH();
	 
 }

/**
 * @brief 
 * @param
 * @retval
 */
void SPI_FLASH_WAKEUP(void)
{
	//Select FLASH
	SPI_FLASH_CS_LOW();
	
	//Send Realse power down instruction
	SPI_FLASH_SendByte(W25X_ReleasePowerDown);
	
	//Stop
	SPI_FLASH_CS_HIGH();
	
}

/**
 * @brief Erase FLASH Sector
 * @param None
 * @retval 	None
 */
void SPI_FLASH_BulkErase(word)
{
	//Write Enable 
	SPI_FLASH_WriteEnable();
	
	//Select FLASH
	SPI_FLASH_CS_LOW();
	
	//Send Erase instruction
	SPI_FLASH_SendByte(W25X_ChipErase);
	
	//Stop
	SPI_FLASH_CS_HIGH();
	
	//Wait until finish
	SPI_FLASH_WaitForWriteEnd();
}


/**
	* @brief Erase FLASH sector
	* @param SectorAddr
	* @retval None
	*/
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	//Send Enable
	SPI_FLASH_WriteEnable();
	
	SPI_FLASH_WaitForWriteEnd();
	
	//Select FLASH: CS is LOW
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(W25X_SectorErase);
	
	//Erase high bits in sector
	SPI_FLASH_SendByte((SectorAddr & 0xFF000000) >> 24);
	
	//Erase middle hig 8 bits sector
	SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	
	//Erase middle bits in sector
	SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
	
	//Erase low bits in sector
	SPI_FLASH_SendByte(SectorAddr & 0xFF);
	
	//Stop Communication: CS is HIGH
	SPI_FLASH_CS_HIGH();
	
	//Wait Erase is finish
	SPI_FLASH_WaitForWriteEnd();
}

/**
	* @brief Write data by page, need to erase sector firstly
	* @param pBuff
	*				 WriteAddr
	*				 NumByteToWrite
	* @retval 	None
	*/
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	//Send Enable instruction
	SPI_FLASH_SendByte(W25X_WriteEnable);
	
	//Select FLASH: CS is LOW
	SPI_FLASH_CS_LOW();
	
	//Send Write instruction
	SPI_FLASH_SendByte(W25X_PageProgram);
	
	//Send Write address high 8 bits
	SPI_FLASH_SendByte((WriteAddr & 0xFF000000) >> 24);
	
	//Send Write address middle high 8 bits
	SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	
	//Send Write address middle bits
	SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
	
	//Send Write address low bits
	SPI_FLASH_SendByte(WriteAddr & 0xFF);
	
	if(NumByteToWrite > SPI_FLASH_PER_WRITE_PAGESIZE)
	{
		NumByteToWrite =  SPI_FLASH_PER_WRITE_PAGESIZE;
		FLASH_ERROR("SPI_FLASH_PAGEWRITE too large!");
	}
	
	//Write data
	while(NumByteToWrite--)
	{
		SPI_FLASH_SendByte(*pBuffer);
		pBuffer++;
	}
	
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}


/**
	* @brief Write non-fix length data, need to erase sector firstly
	* @param pBuffer
	*				 WriteAddr
	*				 NumByteToWrite
  * @retval None 
	*/
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	Addr  =  WriteAddr % SPI_FLASH_PAGESIZE;
	count = SPI_FLASH_PAGESIZE - Addr;
	NumOfPage = NumByteToWrite / SPI_FLASH_PER_WRITE_PAGESIZE;
	NumOfSingle = NumByteToWrite % SPI_FLASH_PER_WRITE_PAGESIZE;
	
	if(Addr == 0)
	{
		if(NumOfPage == 0)
		{
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else
		{
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PER_WRITE_PAGESIZE);
				WriteAddr++;
				pBuffer++;
			}
			
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else
	{
		if(NumOfPage == 0)
		{
			if(NumOfSingle > count)
			{
				temp = NumOfSingle - count;
				
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				WriteAddr++;
				pBuffer++;
				
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
				
			}
			else
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else
		{
			NumByteToWrite -= count;
			NumOfPage = NumByteToWrite / SPI_FLASH_PER_WRITE_PAGESIZE;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PER_WRITE_PAGESIZE;
			
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			WriteAddr++;
			pBuffer++;
			
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PAGESIZE);
				WriteAddr++;
				pBuffer++;
			}
			
			if(NumOfSingle != 0)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
			
		}
		
			
	}
	
}

/**
	* @brief Read FLASH 
	* @param pBuffer
	*				 ReadAddr
	*				 NumByteToRead
	* @retval 	None
	*/
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	//Selcet FLASH: CS is LOW
	SPI_FLASH_CS_LOW();
	
	//Send Read instruction
	SPI_FLASH_SendByte(W25X_ReadData);
	
	//Send address high 8 bits
	SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
	
	//Send address middle 8 high bits
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	
	//Send address middle bits
	SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
	
	//Send address low bits
	SPI_FLASH_SendByte(ReadAddr & 0xFF);
	
	while(NumByteToRead--)
	{
		*pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
		pBuffer++;
		
	}
	
	//Stop 
	SPI_FLASH_CS_HIGH();
}

/**
 * @brief 
 * @param
 * retval
 */
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
	//Select FLASH
	SPI_FLASH_CS_LOW();
	
	//Send Read instruction
	SPI_FLASH_SendByte(W25X_ReadData);
	
	//Send the 24-bit address to read from
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	
	SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
	
	SPI_FLASH_SendByte(ReadAddr & 0xFF);
	
}



/**
  * @brief  使 SPI_FLASH 进入 4 字节地址模式
  * @param  none
  * @retval none
  */
void SPI_FLASH_Mode_Init(void)
{
	uint8_t Temp;
	
	/*选择 FLASH: CS 低 */
	SPI_FLASH_CS_LOW();
	
	/* 发送状态寄存器 3 命令 */
	SPI_FLASH_SendByte(W25X_ReadStatusRegister3); 
	
	Temp = SPI_FLASH_SendByte(Dummy_Byte);
	
	/* 停止信号 FLASH: CS 高 */
	SPI_FLASH_CS_HIGH();
	
	if((Temp&0x01) == 0)
	{
		/*选择 FLASH: CS 低 */
		SPI_FLASH_CS_LOW();
		
		/* 进入4字节模式 */
		SPI_FLASH_SendByte(W25X_Enter4ByteMode);
		
		/* 停止信号 FLASH: CS 高 */
		SPI_FLASH_CS_HIGH();
	}
}



/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
