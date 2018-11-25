#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_psensor.h"



/* Private variables ---------------------------------------------------------*/

osThreadId pbCtrlHandle;

osThreadId defaultTaskHandle;
osThreadId taskOneHandle;
osThreadId taskTwoHandle;
osThreadId taskThreeHandle;
osThreadId taskFourHandle;
osThreadId taskFiveHandle;

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef handle;

int fputc(int ch, FILE *f) {
  while (HAL_OK != HAL_UART_Transmit(&handle, (uint8_t *) &ch, 1, 30000));
  return ch;
}
int fgetc(FILE *f) {
  uint8_t ch = 0;
  while (HAL_OK != HAL_UART_Receive(&handle, (uint8_t *)&ch, 1, 30000));
  return ch;
}

void UART_init() {	
	GPIO_InitTypeDef gpio_init;
	
	handle.Instance = USART1;
	handle.Init.BaudRate = 115200;
  handle.Init.WordLength = UART_WORDLENGTH_8B;
  handle.Init.StopBits = UART_STOPBITS_1;
  handle.Init.Parity = UART_PARITY_NONE;
  handle.Init.Mode = UART_MODE_TX_RX;
  handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  handle.Init.OverSampling = UART_OVERSAMPLING_16;
  handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();
	
  gpio_init.Pin = GPIO_PIN_6;
  gpio_init.Mode = GPIO_MODE_AF_PP;
  gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOB, &gpio_init);
	
	gpio_init.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &gpio_init);
	
	HAL_UART_Init(&handle);
}


























/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void pbCtrl(void const * argument);
void StartDefaultTask(void const * argument); //0:accelerator
void StartTempTask(void const * argument);//1
void StartHumTask(void const * argument);//2
void StartMagTask(void const * argument);//3
void StartPreTask(void const * argument);//4
void StartGyroscopeTask(void const * argument);//5
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/* USER CODE END PFP */
char send[3]={'Y','O','\n'};



int switchThread=1;
int timerFlag=1;


int sensorType =0; 

/*	0 off
	1 on */
int onoff = 1;
int pbtime = 0;//
int main(void)
{
 

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  UART_init();
 
  /*  Init */

  /* Configure the system clock */
  SystemClock_Config();
  /* SysInit */

  /* Initialize all configured peripherals */
 
  /* RTOS_MUTEX */
  	//SemaphoreHandle_t sp = xSemaphoreCreateMutex();
	//xSemasphoreTake(sp,0); //takes mutex
	//xSemaphoreGive(sp); //release semasphore


  /*  RTOS_SEMAPHORES */
  /* add semaphores, ... */
//


  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(pbCtrlThread, pbCtrl, osPriorityNormal, 0, 128);
  pbCtrlHandle = osThreadCreate(osThread(pbCtrlThread), NULL);
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */


  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  while (1)
  {
  }
}


/*
Waits till pb pressed
*/
void waitForPb(){
	osDelay(5000);
  	return; // return if pb pressed
}

//void StartDefaultTask(void const * argument)



void pbCtrl(void const * argument){



	UART_init();

	while(1){
	if(onoff==0){ // if sleep mode
  		waitForPb(); // wait for turn on signal
  		onoff=1;//if pb pressed, turn on sensor mode
      
  	}else{
  		if(sensorType==0&&switchThread==1){
  			switchThread=0;
  			printf("pbcontrol");//===================test====
  			osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  			defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  			waitForPb(); // wait for turn on signal
  		}
  		else if(sensorType==1&&switchThread==1){
  			switchThread=0;
  			osThreadDef(TaskOne, StartTempTask, osPriorityNormal, 0, 128);
  			taskOneHandle = osThreadCreate(osThread(TaskOne), NULL);
  			waitForPb(); // wait for turn on signal
  		}
  		else if(sensorType==2&&switchThread==1){
  			switchThread=0;
			osThreadDef(taskTwo, StartHumTask, osPriorityNormal, 0, 128);
  			taskTwoHandle = osThreadCreate(osThread(taskTwo), NULL);
  			waitForPb(); // wait for turn on signal
  		}
  		else if(sensorType==3&&switchThread==1){
  			switchThread=0;
			osThreadDef(taskThree, StartMagTask, osPriorityNormal, 0, 128);
  			taskThreeHandle = osThreadCreate(osThread(taskThree), NULL);
  			waitForPb(); // wait for turn on signal
  		}
  		else if(sensorType==4&&switchThread==1){
  			switchThread=0;
			osThreadDef(taskFour, StartPreTask, osPriorityNormal, 0, 128);
  			taskFourHandle = osThreadCreate(osThread(taskFour), NULL);
  			waitForPb(); // wait for turn on signal
  		}
  		else if(sensorType==5&&switchThread==1){
  			switchThread=0;
			osThreadDef(taskFive, StartGyroscopeTask, osPriorityNormal, 0, 128);
  			taskFiveHandle = osThreadCreate(osThread(taskFive), NULL);
  			waitForPb(); // wait for turn on signal
  		}
	
  		pbtime=0;//save current time;
  		while(123==133){ //while pb is pressed
  			if(timerFlag==1){
  				timerFlag=0;
  				pbtime++;
  			}
  		}

  		if(pbtime>=15){
  			onoff=0;// change to 0
  		}
  		if(onoff==1){
  			sensorType=(sensorType+1)%6; // rotate sensor type (5 sensors)
  			switchThread=1;
  		}
  	}
	}
}


























/* StartDefaultTask function */
void StartDefaultTask(void const * argument){

	UART_init();
	BSP_ACCELERO_Init();
	int16_t XYZ[3];
  	/* Infinite loop */
	for(;;){
		if(onoff==1&&sensorType==0){
			osDelay(200);
			BSP_ACCELERO_AccGetXYZ(XYZ);
			printf("X = %d, Y = %d, Z = %d\n", XYZ[0], XYZ[1], XYZ[2]);
		}else{
			BSP_ACCELERO_DeInit();
			osThreadTerminate(osThreadGetId());
		}
 	}
}





/* StartTempTask Function */
void StartTempTask(void const * argument){

	UART_init();
	BSP_TSENSOR_Init();
 
	float temperatue;
 
	/* Infinite loop */
  	for(;;){
		if(onoff==1&&sensorType==1){
			osDelay(200);
			temperatue = BSP_TSENSOR_ReadTemp();
			printf("temperatue = %f\n", temperatue);
		}else{
			osThreadTerminate(osThreadGetId());
		}
  	}
}



/* StartHumTask Function */
void StartHumTask(void const * argument){

	UART_init();
	BSP_HSENSOR_Init();
	float humidity;
 
	/* Infinite loop */
  	for(;;){
		if(onoff==1&&sensorType==2){
			osDelay(200);
			//BSP_HSENSOR_ReadID();  // ??????
			humidity = BSP_HSENSOR_ReadHumidity();
			printf("humidity = %f\n", humidity);
		}else{
			osThreadTerminate(osThreadGetId());
		}
 	}
}




/* StartMagTask Function */
void StartMagTask(void const * argument){

	UART_init();
	BSP_MAGNETO_Init();
	int16_t MagXYZ[3];
	/* Infinite loop */
  	for(;;){
		if(onoff==1&&sensorType==3){
			osDelay(200);
			BSP_MAGNETO_GetXYZ(MagXYZ);
			printf("MagX = %d, MagY = %d, MagZ = %d\n", MagXYZ[0], MagXYZ[1], MagXYZ[2]);
		}else{
			BSP_MAGNETO_DeInit();
			osThreadTerminate(osThreadGetId());
		}
  	}
}


/* StartPreTask Function */
void StartPreTask(void const * argument){

	UART_init();
	BSP_PSENSOR_Init();
	float pressure;
 
	/* Infinite loop */
  	for(;;){
		if(onoff==1&&sensorType==4){
			osDelay(200);
			//BSP_PSENSOR_ReadID();  // ??????
			pressure = BSP_PSENSOR_ReadPressure();
			printf("pressure = %f\n",pressure);
		}else{
			osThreadTerminate(osThreadGetId());
		}
  	}
}



/* StartGyroscopeTask Function */
void StartGyroscopeTask(void const * argument){

	UART_init();
	BSP_GYRO_Init();
 
	float XYZangular[3];
 
	/* Infinite loop */
  	for(;;){
		if(onoff==1&&sensorType==5){
			osDelay(200);
			BSP_GYRO_GetXYZ(XYZangular);
			printf("Xangular = %f, Yangular = %f, Zangular = %f\n", XYZangular[0], XYZangular[1], XYZangular[2]);
		}else{
			BSP_GYRO_DeInit();
			osThreadTerminate(osThreadGetId());
		}
  	}
}



























void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

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

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM17) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}


void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 

}
#endif /* USE_FULL_ASSERT */
