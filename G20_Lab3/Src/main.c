#include "main.h"
#include "stm32l4xx_hal.h"
#define TS_CAL1					(uint16_t*) 0x1FFF75A8
#define TS_CAL2					(uint16_t*) 0x1FFF75CA

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_UART_DMA_Init(void);
static void MX_DMA_Init(void);
int UART_Print_String(UART_HandleTypeDef *huart1, char *arr, int size);

/* GLOBAL VARIABLE FOR THE SOFTWARE FLAG */
int softFlag = 0;
int result;
uint16_t sensorValue;	
uint16_t actualTemp;

/* GLOBAL VARIABLE TO STORE THE TEMPERATURE READING FOR DMA */
char temp_reading[30];
int temp_counter = 0;

int main(void)
{	
	char ch[1] = {'Y'};
	char buf[1];
	char msg[18] = {'T', 'e', 'm', 'p', 'e', 'r', 'a', 't', 'u', 'r', 'e', ' ', '=', ' ', ' ', ' ', 'C', '\n'};


	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();
	//HAL_ADC_DeInit(&hadc1);
	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_USART1_UART_Init();
	MX_DMA_Init();
	
	/* Infinite loop */
	while (1)
	{
		/* Prints Y whenever X is received */
		/*
		HAL_UART_Receive(&huart1, (uint8_t *)&buf[0], 1, 30000);
		HAL_Delay(100);
		if(buf[0] == 'X'){
			HAL_UART_Transmit(&huart1, (uint8_t *)&ch[0], 1, 30000);
		} 
		*/
				
		/*if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK){
			sensorValue = (uint8_t) HAL_ADC_GetValue(&hadc1);
			actualTemp = (110 - 30) / (*TS_CAL2 - *TS_CAL1) * (sensorValue - *TS_CAL1) + 30;
		}*/
		
		/*
		HAL_ADC_Start(&hadc1);
		if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK){
			sensorValue = (uint16_t) HAL_ADC_GetValue(&hadc1);
			result = __LL_ADC_CALC_TEMPERATURE(3300, sensorValue, LL_ADC_RESOLUTION_8B);
		}
		
		msg[14] = (result / 10) + 48;
		msg[15] = (result % 10) + 48;
		
		UART_Print_String(&huart1, msg, 18);
		HAL_Delay(100);
		*/
		
		
		if(softFlag){							// check interrupt
			HAL_ADC_Start(&hadc1);				// Start ADC for reading the temperature
			//HAL_DMA_Start(&hdma_usart1_tx);
			if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK){
				sensorValue = (uint16_t) HAL_ADC_GetValue(&hadc1);					// Keeps polling for the reading of the sensor readings
				result = __LL_ADC_CALC_TEMPERATURE(3300, sensorValue, LL_ADC_RESOLUTION_8B);
			}
			// Transmits through UART immediately 
			
			msg[14] = (result / 10) + 48;		// write the first digit to the string
			msg[15] = (result % 10) + 48;		// write the secon digit to the string
			
			
			//Update the memory first and access using DMA afterwards	
			
			//temp_reading[temp_counter] = (result / 10) + 48;
			//temp_reading[temp_counter + 1] = (result % 10) + 48;
			//temp_reading[temp_counter + 2] = '\n';
			temp_counter += 3;
			
			if(temp_counter >= 30){
				temp_counter = 0;
				HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&msg[0], 18);
			}		
			
			//UART_Print_String(&huart1, msg, 18);
			softFlag = 0;						// reset the interrupt flag
		}
	}
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 32;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/10);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

static void MX_ADC1_Init(void)
{
	ADC_ChannelConfTypeDef sConfig;
	
	__HAL_RCC_ADC_CLK_ENABLE();
	
	hadc1.Instance = ADC1;
	//STATUS = HAL_ERROR @ LINE 533 OF STM32L4XX_HAL_ADC.C
	
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	// According to the requirement, we should use 8 bit here
	hadc1.Init.Resolution = ADC_RESOLUTION_8B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	
	// Error check
	if(HAL_ADC_Init(&hadc1) != HAL_OK){
		_Error_Handler(__FILE__, __LINE__);
	}
  
	sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
	sConfig.Offset = 0;
	
	// Error check
	if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){
		_Error_Handler(__FILE__, __LINE__);
	}
}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
	HAL_DMA_Init(&hdma_usart1_tx);

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

int UART_Print_String(UART_HandleTypeDef *huart1, char *arr, int size){
	HAL_UART_Transmit(huart1, (uint8_t *)&arr[0], size, 100);
	return 0;
}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();
}


void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
}
#endif /* USE_FULL_ASSERT */
