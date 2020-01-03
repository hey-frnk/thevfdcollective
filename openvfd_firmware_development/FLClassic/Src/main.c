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
#include "../../vfdco_sk6812.h"
#include "../../vfdco_color_lib.h"
#include "../../vfdco_lights.h"
#include "../../vfdco_time.h"
#include "../../vfdco_hid.h"
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
TIM_HandleTypeDef htim14;
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
static void MX_TIM14_Init(void);
/* USER CODE BEGIN PFP */
/*void user_pwm_setvalue(uint8_t value);
void setColorRGBW(uint8_t *);*/
void test1() {
  hsl_t *c1 = HSL_Init(85, 255, 127);
  hsl_d_t d = {-4, 0, 0};

  struct LED_Color *tester[18] = {NULL};
  int t = 3;

  int cs = 2;
  int cr = LED_COLOR_REPEAT_FOREVER;
  int ce = 4;
  int ct = 100;
  tester[0] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_SPLITACC);
  tester[1] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_SPLITACC);
  tester[2] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_SPLITLIN);
  tester[3] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, cs, cr, ce, c1, &d, ct, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_SPLITLIN);

  int rs = 5;
  int rr = LED_COLOR_REPEAT_FOREVER;
  int re = 6;
  int rt = 100;
  tester[4] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_RL_ACCELERATING);
  tester[5] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_RL_ACCELERATING);
  tester[6] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_RL_LINEAR);
  tester[7] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, rs, rr, re, c1, &d, rt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_RL_LINEAR);

  int ls = 0;
  int lr = LED_COLOR_REPEAT_FOREVER;
  int le = 6;
  int lt = 100;
  tester[8] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_LR_ACCELERATING);
  tester[9] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_LR_ACCELERATING);
  tester[10] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_FAST, LED_COLOR_CHASER_MODE_LR_LINEAR);
  tester[11] = (struct LED_Color *)LED_Color_Chaser_Init(t, LED_COLOR_BLEND_MODE_NORMAL, ls, lr, le, c1, &d, lt, LED_COLOR_CHASER_PRESERVING_DECAY_SLOW, LED_COLOR_CHASER_MODE_LR_LINEAR);

  for(uint8_t i = 0; i < 12; ++i) {
    while(tester[i]->Next(tester[i])) {
      uint8_t f1, f2, f3, f4;
      vfdco_hid_button_retrieve_all(&f1, &f2, &f3, &f4);

      if(f3 == BUTTON_STATE_SHORTPRESS) {
        tester[i]->Delete(tester[i]);
        vfdco_clr_set_all_RGBW(0, 0, 0, 0);

        break;
      }
    }
  }

  HSL_Delete(c1);
}

void test2() {
  struct Light_Pattern_Static ledInstance;
  Light_Pattern_Static_Init(&ledInstance);

  struct Light_Pattern *virtualMode = NULL;
  virtualMode = (struct Light_Pattern *)&ledInstance;

  while(1) {
    virtualMode->Update((void *)virtualMode);

    uint8_t c = vfdco_hid_button_retrieve(BUTTON_F3);
    vfdco_hid_button_reset(BUTTON_F3);
    switch(c) {
      case BUTTON_STATE_SHORTPRESS: virtualMode->F3((void *)virtualMode); break;
      case BUTTON_STATE_LONGPRESS: virtualMode->F3Var((void *)virtualMode); break;
      default: break;
    }
  }
}

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
  MX_TIM2_Init();
  MX_USB_PCD_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */

	vfdco_clr_init(6);

	vfdco_clr_set_all_RGBW(0, 0, 0, 0);
  HAL_Delay(10);
	vfdco_clr_render();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  	test2();

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
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 48-1;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 1000-1;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

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
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

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
