/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "vfdco_sk6812.h"
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
TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim2_ch1;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_DMA_Init(void);
/* USER CODE BEGIN PFP */
/*void user_pwm_setvalue(uint8_t value);
void setColorRGBW(uint8_t *);*/


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Faster color wheel with HSL transform and fixed point arithmetics
// https://stackoverflow.com/questions/2353211
static inline int32_t _mdisplay_fpmul(int32_t x, int32_t y) { return ((int64_t)x * (int64_t)y) / 0x01000000; }
static int32_t _mdisplay_hslp_hue2rgb(int32_t p, int32_t q, int32_t t) {
  if(t < 0x00000000) t += 0x01000000;
  if(t > 0x01000000) t -= 0x01000000;
  if(t < 0x002AAAAB) return p + _mdisplay_fpmul(0x06000000, _mdisplay_fpmul((q - p), t));
  if(t < 0x00800000) return q;
  if(t < 0x00AAAAAB) return p + _mdisplay_fpmul(0x06000000, _mdisplay_fpmul((q - p), 0x00AAAAAB - t));
  return p;
}

uint32_t ledPhase(uint8_t h, uint8_t s, uint8_t l) {
  int32_t hFP = _mdisplay_fpmul((int32_t)h << 23, 0x20202);
  int32_t sFP = _mdisplay_fpmul((int32_t)s << 23, 0x20202);
  int32_t lFP = _mdisplay_fpmul((int32_t)l << 23, 0x20202);

  int32_t rFP, gFP, bFP;
  if(s == 0) return ((uint32_t)l) | (((uint32_t)l) << 8) | (((uint32_t)l) << 16);
  else{
    int32_t q = (lFP < 0x00800000) ? _mdisplay_fpmul(lFP, 0x01000000 + sFP) : (lFP + sFP - _mdisplay_fpmul(lFP, sFP));
    int32_t p = _mdisplay_fpmul(0x02000000, lFP) - q;
    rFP = _mdisplay_hslp_hue2rgb(p, q, hFP + 0x00555555);
    gFP = _mdisplay_hslp_hue2rgb(p, q, hFP);
    bFP = _mdisplay_hslp_hue2rgb(p, q, hFP - 0x00555555);
  }

  return ((uint32_t)(_mdisplay_fpmul(0x3FC00000, gFP) >> 6 ) & 0x00FF0000) | \
         ((uint32_t)(_mdisplay_fpmul(0x3FC00000, rFP) >> 14) & 0x0000FF00) | \
         ((uint32_t)(_mdisplay_fpmul(0x3FC00000, bFP) >> 22) & 0x000000FF);
}

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
  MX_USB_PCD_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

	// HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	// HAL_Delay(100);

	vfdco_clr_init(6);

	uint8_t LED7_dp = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		LED7_dp+= 1; // Just let it overflow and begin from 0 :p
		HAL_Delay(20);

    // Cycle position

    /*for(uint8_t i = 0; i < 6; i++) {
    	uint32_t phase = ledPhase(LED7_dp + (i * 7), 255, 128);
    	vfdco_clr_set_RGB(i, (phase >> 16) & 0xFF, (phase >> 8) & 0xFF, phase & 0xFF);
    }*/

		uint32_t phase = ledPhase(LED7_dp, 255, 128);

		for(uint8_t i = 0; i < 6; ++i) {
			vfdco_clr_set_RGB(i, (phase >> 16) & 0xFF, (phase >> 8) & 0xFF, phase & 0xFF);
		}

		/*const uint8_t led_Presets[][18] = {{  0, 200, 255,  // Rainbow colors!
				                                       0,   0, 255,
				                                     255,   0,   0,
				                                     128, 255,   0,
				                                      30, 255,   0,
				                                       0, 255,   0},

				                                    {128, 255, 255,  // Pastel rainbow!
				                                     128,  50, 255,
				                                     255,   0, 128,
				                                     255, 128, 128,
				                                     255, 255, 128,
				                                     100, 255, 128},

				                                    {255,   0,   0,  // Green to blue!
				                                     240,   0,  64,
				                                     216,   0, 128,
				                                     128,   0, 216,
				                                      64,   0, 240,
				                                       0,   0, 255},

				                                    {  0, 255,   3,  // Red to blue!
				                                       0, 255,  10,
				                                       0, 240,  25,
				                                       0, 200,  80,
				                                       0, 100, 150,
				                                       0,  50, 255},

				                                    {  3, 255,   0,  // Red to green!
				                                      30, 255,   0,
				                                      60, 240,   0,
				                                     100, 180,   0,
				                                     180, 180,   0,
				                                     255,  20,   0}
				                                     };

				for(uint8_t j = 0; j < 5; ++j) {
					for(uint8_t i = 0; i < 6; i++) {
								rgb_arr[4 * i] = led_Presets[j][i * 3];
								rgb_arr[4 * i + 1] = led_Presets[j][i * 3 + 1];
								rgb_arr[4 * i + 2] = led_Presets[j][i * 3 + 2];
								rgb_arr[4 * i + 3] = 0;
					}
			    vfdco_clr_render();
			    HAL_Delay(1500);
				}*/

		vfdco_clr_render();

		/*for(uint8_t i = 0; i < 6; i++) {
		    	uint32_t phase = ledPhase(LED7_dp + (i * 20), 255, 128);
		    	rgb_arr[4 * i] = (uint8_t)((phase >> 8) & 0xFF);     // G
		    	rgb_arr[4 * i + 1] = (uint8_t)((phase >> 16) & 0xFF);  // R
		    	rgb_arr[4 * i + 2] = (uint8_t)(phase & 0xFF);         // B
		    	rgb_arr[4 * i + 3] = 0;
		    }*/



 // vfdco_clr_render(i, (uint8_t)((phase >> 16) & 0xFF), (uint8_t)((phase >> 8) & 0xFF), (uint8_t)(phase & 0xFF), 0);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 60-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PF0 PF1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 PA3 PA4 
                           PA5 PA6 PA7 PA8 
                           PA9 PA10 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4 
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB3 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : F1_Pin F2_Pin F3_Pin F4_Pin */
  GPIO_InitStruct.Pin = F1_Pin|F2_Pin|F3_Pin|F4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/*void setColorRGBW(uint8_t *rgbw) {
	for(uint8_t j = 0; j < 4; ++j) {
		for(uint8_t i = 0; i < 8; ++i) {
				user_pwm_setvalue((*rgbw & 0x01) ? DUTY1 : DUTY0);
				*rgbw >>= 1;
		}
		++rgbw;
	}
}

void user_pwm_setvalue(uint8_t value) {
    TIM_OC_InitTypeDef sConfigOC;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}*/
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
