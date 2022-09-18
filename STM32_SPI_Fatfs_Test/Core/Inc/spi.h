/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi5;

/* USER CODE BEGIN Private defines */
#define  sFLASH_ID         								0XEF4019     //W25Q128

#define SPI_FLASH_PageSize								256
#define SPI_FLASH_PerWritePageSize			256

#define W25X_WriteEnable		      				0x06 
#define W25X_WriteDisable		     				 	0x04 
#define W25X_ReadStatusReg		    				0x05 
#define W25X_WriteStatusReg		  					0x01 
#define W25X_ReadData			        				0x03 
#define W25X_FastReadData		      				0x0B 
#define W25X_FastReadDual		      				0x3B 
#define W25X_PageProgram		      				0x02 
#define W25X_BlockErase			      				0xD8 
#define W25X_SectorErase		      				0x20 
#define W25X_ChipErase			      				0xC7 
#define W25X_PowerDown			      				0xB9 
#define W25X_ReleasePowerDown	  					0xAB 
#define W25X_DeviceID			        				0xAB 
#define W25X_ManufactDeviceID   					0x90 
#define W25X_JedecDeviceID		    				0x9F 
#define W25X_Enter4ByteMode		    				0xB7
#define W25X_ReadStatusRegister3      		0x15

#define WIP_Flag													0x01
#define Dummy_Byte 												0xFF

#define	digitalHi(p,i)			    {p->BSRR=i;}			  //设置为高电平		
#define digitalLo(p,i)			    {p->BSRR=(uint32_t)i << 16;}				//输出低电平
#define SPI_FLASH_CS_LOW()      digitalLo(GPIOF, GPIO_PIN_6 )
#define SPI_FLASH_CS_HIGH()     digitalHi(GPIOF, GPIO_PIN_6 )

	

#define SPIT_FLAG_TIMEOUT         				((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         				((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

#define FLASH_INFO(fmt,arg...)          	printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)         	printf("<<-FLASH-ERROR->> "fmt"\n",##arg)

/* USER CODE END Private defines */

void MX_SPI5_Init(void);

/* USER CODE BEGIN Prototypes */
void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
void SPI_FLASH_Mode_Init(void);


uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
