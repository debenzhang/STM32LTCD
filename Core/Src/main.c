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
#include "main.h"
#include "crc.h"
#include "dma2d.h"
#include "fatfs.h"
#include "ltdc.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"
#include "fmc.h"
#include "app_touchgfx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "sdram_fmc_drv.h"
#include "touch_800x480.h"
#include "usbh_core.h"
#include "USBDisk.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE {
    while ((USART1->SR & 0X40) == 0);
    USART1->DR = (uint8_t) ch;
    return ch;
}
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
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define EXT_SDRAM_ADDR		((uint32_t)0xD0000000)
#define EXT_SDRAM_SIZE		(32 * 1024 * 1024)

uint32_t bsp_TestExtSDRAM(void);

uint8_t timer_irq = 0;
uint8_t touch_cnt = 0;

uint8_t rUSB = 0;
uint8_t pre_state;
uint16_t usbCounter = 0;

void timer_loop(void) {
	if(timer_irq == 0) return;
	timer_irq = 0;

	touch_cnt++;
	if(touch_cnt > 20) {
		touch_cnt = 0;
		Touch_Scan();
	}

	usbCounter++;
	if(usbCounter > 3000) {
		usbCounter = 0;
		printf("usbCounter. \r\n");
		if(rUSB == 1) {
			printf("Explore_Disk. \r\n");
			Explore_Disk(USBHPath, 1);
		}
	}

	MX_USB_HOST_Process();

	if (pre_state != Appli_state) {
		switch(Appli_state) {
			case APPLICATION_DISCONNECT: //USB flash disk remove
				/* Register the file system object to the FatFs module */
				printf("APPLICATION_DISCONNECT. \r\n");
				if(f_mount(NULL, "", 0) != FR_OK) {
					USBH_UsrLog("ERROR : Cannot exit FatFs! \n");
				}
				break;
			case APPLICATION_READY:   //USB flash disk plugin
				/* Open or create a log file and ready to append */
				printf("APPLICATION_READY. \r\n");
				if(f_mount(&USBDISKFatFs, (TCHAR const*)USBHPath, 0) != FR_OK) {
					printf("f_mount fails. \r\n");
					break;
				} else {
					printf("f_mount success. \r\n");
					rUSB = 1;
				}
//				if(f_mount(&USBDISKFatFs, (TCHAR const*)USBHPath, 0) != FR_OK) {
//					printf("f_mount fails. \r\n");
//					break;
//				} else {
//					if(USBH_MSC_IsReady(&USBDISKFatFs))
////					printf("Explore_Disk. \r\n");
////					Explore_Disk("0:/", 1);
////					Explore_Disk(USBHPath, 1);
//				}
				break;
			default:
				break;
		}
	}
	pre_state = Appli_state;

//	switch(Appli_state) {
//		case APPLICATION_READY:
////			if(rUSB == 0) {
////	            printf("APPLICATION_READY.\r\n");
//				// MSC_Application();
//				Explore_Disk("0:/", 1);
////			}
//			// Appli_state = APPLICATION_DISCONNECT;
//			rUSB = 1;
//			break;
//		case APPLICATION_DISCONNECT:
//			f_mount(NULL, "", 0);
//			break;
//		default:
//			break;
//	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  Appli_state = APPLICATION_IDLE;
  pre_state = Appli_state;
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
  MX_FMC_Init();
  MX_TIM2_Init();
  MX_LTDC_Init();
  MX_DMA2D_Init();
  MX_CRC_Init();
  MX_USART1_UART_Init();
  MX_USB_HOST_Init();
  MX_FATFS_Init();
  MX_TouchGFX_Init();
  /* USER CODE BEGIN 2 */

  // 点亮 LED
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  HAL_TIM_Base_Start_IT(&htim2);

  SDRAM_Init();

  HAL_Delay(100);

  // 点亮 LCD
  HAL_GPIO_WritePin(LED_BL_GPIO_Port, LED_BL_Pin, GPIO_PIN_SET);

  printf("System running... \r\n");

  Touch_Init();				// 触摸屏初始化

//  uint32_t i;
//  uint32_t *pSRAM;
//
//  /* 写SDRAM */
//  pSRAM = (uint32_t *)EXT_SDRAM_ADDR;
//  for (i = 0; i < 800 * 50; i++)
//  {
//      *pSRAM++ = 0x33;
//  }


//  if (bsp_TestExtSDRAM() == 0) {
//    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//  } else {
//    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    // MX_USB_HOST_Process();
	timer_loop();
    MX_TouchGFX_Process();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 60;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  if(GPIO_Pin == TOUCH_INT_Pin)
  {
    // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    // 清除标志
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
  }
}


uint32_t bsp_TestExtSDRAM(void)
{
    uint32_t i;
    uint32_t *pSRAM;
    uint8_t *pBytes;
    uint32_t err;
    const uint8_t ByteBuf[4] = {0x55, 0xA5, 0x5A, 0xAA};

    /* 写SDRAM */
    pSRAM = (uint32_t *)EXT_SDRAM_ADDR;
    for (i = 0; i < EXT_SDRAM_SIZE / 4; i++)
    {
        *pSRAM++ = i;
    }

    /* 读SDRAM */
    err = 0;
    pSRAM = (uint32_t *)EXT_SDRAM_ADDR;
    for (i = 0; i < EXT_SDRAM_SIZE / 4; i++)
    {
        if (*pSRAM++ != i)
        {
            err++;
        }
    }

    if (err >  0)
    {
        return  (4 * err);
    }

    /* 对SDRAM 的数据求反并写入 */
    pSRAM = (uint32_t *)EXT_SDRAM_ADDR;
    for (i = 0; i < EXT_SDRAM_SIZE / 4; i++)
    {
        *pSRAM = ~*pSRAM;
        pSRAM++;
    }

    /* 再次比较SDRAM的数�????????????? */
    err = 0;
    pSRAM = (uint32_t *)EXT_SDRAM_ADDR;
    for (i = 0; i < EXT_SDRAM_SIZE / 4; i++)
    {
        if (*pSRAM++ != (~i))
        {
            err++;
        }
    }

    if (err >  0)
    {
        return (4 * err);
    }

    /* 测试按字节方式访�?????????????, 目的是验�????????????? FSMC_NBL0 �????????????? FSMC_NBL1 口线 */
    pBytes = (uint8_t *)EXT_SDRAM_ADDR;
    for (i = 0; i < sizeof(ByteBuf); i++)
    {
        *pBytes++ = ByteBuf[i];
    }

    /* 比较SDRAM的数�????????????? */
    err = 0;
    pBytes = (uint8_t *)EXT_SDRAM_ADDR;
    for (i = 0; i < sizeof(ByteBuf); i++)
    {
        if (*pBytes++ != ByteBuf[i])
        {
            err++;
        }
    }
    if (err >  0)
    {
        return err;
    }
    return 0;
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
