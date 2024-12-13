/* USER CODE BEGIN Header */
#include "arm_math.h"
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define NS  128


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

uint32_t noisy_ecg[NS] = {
	    3481, 2271, 1646, 719, 367, 0, 565, 611, 747, 587, 443, 921, 1106, 1547, 1595,
	    1694, 1301, 1151, 909, 1043, 1379, 975, 684, 1011, 627, 576, 1045, 788, 979, 1167,
	    974, 1085, 587, 1045, 1346, 1035, 628, 901, 849, 1031, 1183, 1391, 1342, 1301, 1172,
	    1217, 631, 964, 1065, 897, 1995, 2798, 3540, 3838, 2855, 1927, 972, 885, 274, 666,
	    878, 905, 1221, 949, 1294, 1616, 1588, 1812, 1945, 1780, 1802, 1274, 1531, 948, 786,
	    1430, 1147, 1253, 1096, 993, 633, 1350, 1127, 1148, 1272, 913, 1054, 1431, 1325, 954,
	    774, 1102, 1261, 1507, 1559, 1437, 1745, 1339, 1152, 1108, 996, 1189, 1062, 1903, 2778,
	    3700, 4095, 3533, 2503, 1483, 820, 554, 500, 1647, 1459, 1319, 1076, 1081, 1655, 2059,
	    1555, 1874, 1936, 1464, 1867, 1405, 1654, 1197
	};

//uint32_t filtered_ecg[NS] = {
//	    0, 897, 2419, 3364, 2875, 1617, 773, 751, 1172, 1509, 1538, 1387, 1327, 1519, 1881,
//	    2205, 2341, 2264, 2035, 1783, 1651, 1704, 1810, 1761, 1569, 1417, 1368, 1403, 1498, 1596,
//	    1662, 1703, 1690, 1596, 1508, 1572, 1728, 1722, 1521, 1378, 1448, 1646, 1834, 1937, 1938,
//	    1863, 1754, 1613, 1474, 1436, 1554, 1860, 2445, 3245, 3908, 4005, 3379, 2348, 1468, 1044,
//	    1000, 1153, 1363, 1530, 1607, 1644, 1753, 1954, 2147, 2259, 2290, 2233, 2082, 1880, 1678,
//	    1499, 1428, 1530, 1671, 1676, 1534, 1358, 1308, 1441, 1608, 1646, 1554, 1451, 1471, 1609,
//	    1656, 1487, 1293, 1334, 1598, 1852, 1937, 1898, 1827, 1711, 1538, 1394, 1360, 1444, 1688,
//	    2212, 3013, 3791, 4095, 3668, 2687, 1643, 962, 782, 1043, 1512, 1799, 1693, 1420, 1384,
//	    1698, 2049, 2130, 2013, 1902, 1847, 1809, 1749
//	};

// Define filter parameters
#define FILTER_ORDER     4
#define BLOCK_SIZE      128  // Same as your LUT size

// Filter coefficients (make non-const for ARM DSP lib)
static float32_t b[] = {0.206370f, 0.000000f, -0.412740f, 0.000000f, 0.206370f};
static float32_t a[] = {1.000000f, -2.369106f, 1.935069f, -0.761901f, 0.195939f};

static float32_t coeffs[] = {
    0.206370f, 0.000000f, -0.412740f,  // b0, b1, b2 (first section)
    2.369106f, -1.935069f,             // -a1, -a2 (first section)
    0.000000f, 0.206370f, 0.0f,        // b0, b1, b2 (second section)
    0.761901f, -0.195939f              // -a1, -a2 (second section)
};


// Filter state buffer
static float32_t state[4 * FILTER_ORDER];
arm_biquad_casd_df1_inst_f32 filter;

// Function to initialize filter
void filter_init(void)
{
    arm_biquad_cascade_df1_init_f32(&filter, FILTER_ORDER/2, coeffs, state);
}

// Function to filter the signal

//void filter_signal(const uint32_t* input, uint32_t* output, uint32_t size)
//{
//    // Temporary buffers for float conversion
//    float32_t float_input[BLOCK_SIZE];
//    float32_t float_output[BLOCK_SIZE];
//
//    // Convert uint32_t to float32_t
//    for(uint32_t i = 0; i < size; i++)
//    {
//        float_input[i] = (float32_t)input[i];
//    }
//
//    // Apply filter
//    arm_biquad_cascade_df1_f32(&filter, float_input, float_output, size);
//
//    // Convert back to uint32_t and ensure bounds
//    for(uint32_t i = 0; i < size; i++)
//    {
//        int32_t temp = (int32_t)float_output[i];
//        if(temp < 0) temp = 0;
//        if(temp > 4095) temp = 4095;
//        output[i] = (uint32_t)temp;
//    }
//}

void filter_signal(const uint32_t* input, uint32_t* output, uint32_t size)
{
    float32_t float_input[BLOCK_SIZE];
    float32_t float_output[BLOCK_SIZE];

    // Convert to float but keep original scale
    for(uint32_t i = 0; i < size; i++)
    {
        float_input[i] = (float32_t)input[i];
    }

    // Apply filter
    arm_biquad_cascade_df1_f32(&filter, float_input, float_output, size);

    // Simple scaling - maintain relative amplitudes but ensure within DAC range
    for(uint32_t i = 0; i < size; i++)
    {
        // Scale by 0.5 to reduce amplitude a bit but maintain signal character
        float32_t scaled = float_output[i] * 0.0001f;
        // Ensure within DAC range
        if(scaled > 4095.0f) scaled = 4095.0f;
        if(scaled < 0.0f) scaled = 0.0f;
        output[i] = (uint32_t)scaled;
    }
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac1;
DAC_HandleTypeDef hdac2;
DMA_HandleTypeDef hdma_dac1_ch1;
DMA_HandleTypeDef hdma_dac2_ch1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_DAC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_DAC2_Init(void);
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
  MX_DMA_Init();
  MX_DAC1_Init();
  MX_TIM2_Init();
  MX_DAC2_Init();
  /* USER CODE BEGIN 2 */
  // Initialize filter
  filter_init();

  // Create buffer for filtered signal
  uint32_t filtered_buffer[NS];

  // Apply filter to noisy signal
  filter_signal(noisy_ecg, filtered_buffer, NS);

  HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t*)filtered_buffer, 128, DAC_ALIGN_12B_R);
  HAL_DAC_Start_DMA(&hdac2, DAC_CHANNEL_1, (uint32_t*)noisy_ecg, 128, DAC_ALIGN_12B_R);
  HAL_TIM_Base_Start(&htim2);


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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
  sConfig.DAC_DMADoubleDataMode = DISABLE;
  sConfig.DAC_SignedFormat = DISABLE;
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief DAC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC2_Init(void)
{

  /* USER CODE BEGIN DAC2_Init 0 */

  /* USER CODE END DAC2_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC2_Init 1 */

  /* USER CODE END DAC2_Init 1 */

  /** DAC Initialization
  */
  hdac2.Instance = DAC2;
  if (HAL_DAC_Init(&hdac2) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
  sConfig.DAC_DMADoubleDataMode = DISABLE;
  sConfig.DAC_SignedFormat = DISABLE;
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac2, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC2_Init 2 */

  /* USER CODE END DAC2_Init 2 */

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

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 520;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
