/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "ff.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
	{
		FAILED = 0,
		PASSED
	}TestStatus;

FATFS fs; 					//FatFs file system object
FIL fnew;					//File object
FRESULT res_flash;			//File operation result
UINT fnum;					//Num to count successful opeartion of reading and writing

char fpath[100];			//File path
char readbuffer[512];

	
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FLASH_WriteAddress		0x000000
#define FLASH_ReadAddress			FLASH_WriteAddress
#define FLASH_SectorToErase	 	FLASH_WriteAddress

#define countof(a)						(sizeof(a) / sizeof(*(a)))
#define BufferSize						(countof(Tx_Buffer) - 1)	
	
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus = FAILED;

uint8_t Tx_Buffer[] = "STM32F429IG SPI FLASH TEST";
uint8_t Rx_Buffer[BufferSize];

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
  MX_SPI5_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  
  printf("STM32 SPI FLASH File System Test!\n");
  res_flash = f_mount(&fs, "1:", 1);
  if(res_flash != FR_OK)
  {
	  printf("!!Mount external FLASH failure! (%d)\r\n", res_flash);
	  printf("!!SPI Init failure!\r\n");
	  
	  while(1);
  }
  else
  {
	  printf(">File system is mounted successfully and can continue to test\r\n");
  }
  
  res_flash = miscellaneous();
  
  printf("\n******************** File info test ************************\r\n");
  res_flash = file_check();

  printf("********************** File Scan test ************************\r\n");
  strcpy(fpath, "1:");
  scan_files(fpath);
  
  f_mount(NULL, "1:", 1);
  
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  /** Initializes the CPU, AHB and APB buses clocks
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
/**
 * @brief
 * @param
 * @retval
 *
 */
static FRESULT miscellaneous(void)
{
	DIR dir;
	FATFS *pfs;
	DWORD fre_clust, fre_sect, tot_sect;
	
	printf("\n****************Device Info ************\r\n");
	//get device information and size of clust
	res_flash = f_getfree("1", &fre_clust, &pfs);
	
	//Calculate totol sectors and empty sectors
	tot_sect = (pfs->n_fatent - 2) * pfs->csize;
	fre_sect = fre_clust * pfs->csize;
	
	printf(">> Device total space : %10lu KB.\n >> free space : %10lu KB. \n", tot_sect * 4, fre_sect * 4);
	
	
	printf("\n *********************** File location and format to write test ***************\r\n");
	res_flash = f_open(&fnew, "1:FatFs write and read test.txt", FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
	
	if(res_flash == FR_OK)
	{
		//File location 
		res_flash = f_lseek(&fnew, f_size(&fnew));
		if(res_flash == FR_OK)
		{
			f_printf(&fnew, "\n Add a new line in the file");
			f_printf(&fnew, "> Device total space: %10lu KB. \n > free space: %10lu KB.\n", tot_sect * 4, fre_sect * 4);
			
			//Locate to the starting place of the file
			res_flash = f_lseek(&fnew, 0);
			
			res_flash = f_read(&fnew, readbuffer, f_size(&fnew), &fnum);
			if(res_flash == FR_OK)
			{
				printf("> Context: \n%s\n", readbuffer);
			}
			
		}
		
		f_close(&fnew);
		
		printf(\n********************* Create directory and name function test *****************\r\n);
	
		res_flash = f_opendir(&dir, "1:TestDir");
		if(res_flash != FR_OK)
		{
			res_flash = f_mkdir("1:TestDir");
		}
		else
		{
			res_flash =  f_close(&dir)
			
			f_unlink("1:TestDir/testdir.txt");
		}
		
		if(res_flash == FR_OK)
		{
			res_flash = f_rename("1:FatFs write and read test.txt", "1:TestDir/testdir.txt");
		}
	}
	else
	{
		printf("!!Fail to open file: %d\n", res_flash);
	}
	
	return res_flash;
	
}

FILINFO fno;
static FRESULT file_check(void)
{
	res_flash = f_stat("1:TestDir/testdir.txt", &fno);
	if(res_flash == FR_OK)
	{
		printf("testdir.txt file info: \n");
		pirntf("> file size: %ld byte \n", fno.size);
	}
	
	return res_flash;
}

/**
  * @brief  scan_files 递归扫描FatFs内的文件
  * @param  path:初始扫描路径
  * @retval result:文件系统的返回值
  */
static FRESULT scan_files (char* path) 
{ 
  FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
  FILINFO fno; 
  DIR dir; 
  int i;            
  char *fn;        // 文件名	
	
#if _USE_LFN 
  /* 长文件名支持 */
  /* 简体中文需要2个字节保存一个“字”*/
  static char lfn[_MAX_LFN*2 + 1]; 	
  fno.lfname = lfn; 
  fno.lfsize = sizeof(lfn); 
#endif 
  //打开目录
  res = f_opendir(&dir, path); 
  if (res == FR_OK) 
	{ 
    i = strlen(path); 
    for (;;) 
		{ 
      //读取目录下的内容，再读会自动读下一个文件
      res = f_readdir(&dir, &fno); 								
      //为空时表示所有项目读取完毕，跳出
      if (res != FR_OK || fno.fname[0] == 0) break; 	
#if _USE_LFN 
      fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      fn = fno.fname; 
#endif 
      //点表示当前目录，跳过			
      if (*fn == '.') continue; 	
      //目录，递归读取      
      if (fno.fattrib & AM_DIR)         
			{ 			
        //合成完整目录名        
        sprintf(&path[i], "/%s", fn); 		
        //递归遍历         
        res = scan_files(path);	
        path[i] = 0;         
        //打开失败，跳出循环        
        if (res != FR_OK) 
					break; 
      } 
			else 
			{ 
				printf("%s/%s\r\n", path, fn);								//输出文件名	
        /* 可以在这里提取特定格式的文件路径 */        
      }//else
    } //for
  } 
  return res; 
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
