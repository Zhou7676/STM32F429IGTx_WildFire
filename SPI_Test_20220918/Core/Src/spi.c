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
#include <stdio.h>

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
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_6 ;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
		
    /* SPI5 interrupt Init */
    HAL_NVIC_SetPriority(SPI5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI5_IRQn);
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

    /* SPI5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(SPI5_IRQn);
  /* USER CODE BEGIN SPI5_MspDeInit 1 */

  /* USER CODE END SPI5_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
  * @brief  ����FLASH����
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* �������� */
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* ������������ָ��*/
  SPI_FLASH_SendByte(W25X_SectorErase);
  /*���Ͳ���������ַ�ĸ�8λ*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF000000) >> 24);
  /*���Ͳ���������ַ����ǰ8λ*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* ���Ͳ���������ַ���к�8λ */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* ���Ͳ���������ַ�ĵ�8λ */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();
  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd();
}


 /**
  * @brief  ����FLASH��������Ƭ����
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_BulkErase(void)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();

  /* ���� Erase */
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* �����������ָ��*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
  * @retval ��
  */
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();

  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* дҳдָ��*/
  SPI_FLASH_SendByte(W25X_PageProgram);

  /*����д��ַ�ĸ�8λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF000000) >> 24);
  /*����д��ַ����ǰ8λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*����д��ַ���к�8λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*����д��ַ�ĵ�8λ*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!");
  }

  /* д������*/
  while (NumByteToWrite--)
  {
    /* ���͵�ǰҪд����ֽ����� */
    SPI_FLASH_SendByte(*pBuffer);
    /* ָ����һ�ֽ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

  /* �ȴ�д�����*/
  SPI_FLASH_WaitForWriteEnd();
}


 /**
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = SPI_FLASH_PageSize - Addr;	
	/*�����Ҫд��������ҳ*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	 /* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
  if (Addr == 0) 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*�Ȱ�����ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				
				/*��д����ǰҳ*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;
				
				/*��дʣ�������*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*��ַ����������count�ֿ������������������*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;
			
			/*������ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

 /**
  * @brief  ��ȡFLASH����
  * @param 	pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();

  /* ���� �� ָ�� */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* ���� �� ��ַ��8λ */
  SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
  /* ���� �� ��ַ��ǰ8λ */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* ���� �� ��ַ�к�8λ */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* ���� �� ��ַ��8λ */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
  
	/* ��ȡ���� */
  while (NumByteToRead--)
  {
    /* ��ȡһ���ֽ�*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* ָ����һ���ֽڻ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();
}


 /**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* ��ʼͨѶ��CS�͵�ƽ */
  SPI_FLASH_CS_LOW();

  /* ����JEDECָ���ȡID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* ��ȡһ���ֽ����� */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* ��ȡһ���ֽ����� */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* ��ȡһ���ֽ����� */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* ֹͣͨѶ��CS�ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

	/*�����������������Ϊ�����ķ���ֵ*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

 /**
  * @brief  ��ȡFLASH Device ID
  * @param 	��
  * @retval FLASH Device ID
  */
uint32_t SPI_FLASH_ReadDeviceID(void)
{
  uint32_t Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  /* Read a byte from the FLASH */
  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  return Temp;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}


 /**
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  ��
  * @retval ���ؽ��յ�������
  */
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}


 /**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (__HAL_SPI_GET_FLAG( &hspi5, SPI_FLAG_TXE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  WRITE_REG(hspi5.Instance->DR, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (__HAL_SPI_GET_FLAG( &hspi5, SPI_FLAG_RXNE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return READ_REG(hspi5.Instance->DR);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
  
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Loop while DR register in not emplty */
  while (__HAL_SPI_GET_FLAG( &hspi5,  SPI_FLAG_TXE ) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
   }

  /* Send Half Word through the SPIx peripheral */
  WRITE_REG(hspi5.Instance->DR, HalfWord);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Wait to receive a Half Word */
  while (__HAL_SPI_GET_FLAG( &hspi5, SPI_FLAG_RXNE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
   }
  /* Return the Half Word read from the SPI bus */
  return READ_REG(hspi5.Instance->DR);
}


 /**
  * @brief  ��FLASH���� дʹ�� ����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{
  /* ͨѶ��ʼ��CS�� */
  SPI_FLASH_CS_LOW();

  /* ����дʹ������*/
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /*ͨѶ������CS�� */
  SPI_FLASH_CS_HIGH();
}

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  /* ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* ���� ��״̬�Ĵ��� ���� */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* ��FLASHæµ����ȴ� */
  do
  {
    /* ��ȡFLASHоƬ��״̬�Ĵ��� */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 

    {
      if((SPITimeout--) == 0) 
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* ����д���־ */

  /* ֹͣ�ź�  FLASH: CS �� */
  SPI_FLASH_CS_HIGH();
}


//�������ģʽ
void SPI_Flash_PowerDown(void)   
{ 
  /* ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* ���� ���� ���� */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* ֹͣ�ź�  FLASH: CS �� */
  SPI_FLASH_CS_HIGH();
}   

//����
void SPI_Flash_WAKEUP(void)   
{
  /*ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* ���� �ϵ� ���� */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* ֹͣ�ź� FLASH: CS �� */
  SPI_FLASH_CS_HIGH();                   //�ȴ�TRES1
}   

 /**
  * @brief  ʹ SPI_FLASH ���� 4 �ֽڵ�ַģʽ
  * @param  none
  * @retval none
  */
void SPI_FLASH_Mode_Init(void)
{
	uint8_t Temp;
	
	/*ѡ�� FLASH: CS �� */
	SPI_FLASH_CS_LOW();
	
	/* ����״̬�Ĵ��� 3 ���� */
	SPI_FLASH_SendByte(W25X_ReadStatusRegister3); 
	
	Temp = SPI_FLASH_SendByte(Dummy_Byte);
	
	/* ֹͣ�ź� FLASH: CS �� */
	SPI_FLASH_CS_HIGH();
	
	if((Temp&0x01) == 0)
	{
		/*ѡ�� FLASH: CS �� */
		SPI_FLASH_CS_LOW();
		
		/* ����4�ֽ�ģʽ */
		SPI_FLASH_SendByte(W25X_Enter4ByteMode);
		
		/* ֹͣ�ź� FLASH: CS �� */
		SPI_FLASH_CS_HIGH();
	}
}


/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
