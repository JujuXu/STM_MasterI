/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct{
	uint8_t symbols;
	uint8_t n;
} morseData;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FLAG_EMPTY	0x0001
#define FLAG_SENT	0x0002
const uint8_t timeUnit = 200;

const morseData morseLUT[128] = {
    ['a'] = {0b00000010, 2}, // .-
    ['b'] = {0b00000001, 4}, // -...
    ['c'] = {0b00000101, 4}, // -.-.
    ['d'] = {0b00000001, 3}, // -..
    ['e'] = {0b00000000, 1}, // .
    ['f'] = {0b00000100, 4}, // ..-.
    ['g'] = {0b00000011, 3}, // --.
    ['h'] = {0b00000000, 4}, // ....
    ['i'] = {0b00000000, 2}, // ..
    ['j'] = {0b00001110, 4}, // .---
    ['k'] = {0b00000101, 3}, // -.-
    ['l'] = {0b00000100, 4}, // .-..
    ['m'] = {0b00000011, 2}, // --
    ['n'] = {0b00000001, 2}, // -.
    ['o'] = {0b00000111, 3}, // ---
    ['p'] = {0b00000110, 4}, // .--.
    ['q'] = {0b00001011, 4}, // --.-
    ['r'] = {0b00000010, 3}, // .-.
    ['s'] = {0b00000000, 3}, // ...
    ['t'] = {0b00000001, 1}, // -
    ['u'] = {0b00000010, 3}, // ..-
    ['v'] = {0b00000010, 4}, // ...-
    ['w'] = {0b00000110, 3}, // .--
    ['x'] = {0b00000101, 4}, // -..-
    ['y'] = {0b00001001, 4}, // -.--
    ['z'] = {0b00000011, 4}, // --..

    ['0'] = {0b00011111, 5}, // -----
    ['1'] = {0b00011110, 5}, // .----
    ['2'] = {0b00001110, 5}, // ..---
    ['3'] = {0b00000110, 5}, // ...--
    ['4'] = {0b00000010, 5}, // ....-
    ['5'] = {0b00000000, 5}, // .....
    ['6'] = {0b00000001, 5}, // -....
    ['7'] = {0b00000011, 5}, // --...
    ['8'] = {0b00000111, 5}, // ---..
    ['9'] = {0b00001111, 5}  // ----.
};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for serialRXTask */
osThreadId_t serialRXTaskHandle;
const osThreadAttr_t serialRXTask_attributes = {
  .name = "serialRXTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for asciiToMorseTas */
osThreadId_t asciiToMorseTasHandle;
const osThreadAttr_t asciiToMorseTas_attributes = {
  .name = "asciiToMorseTas",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for morseToLEDTask */
osThreadId_t morseToLEDTaskHandle;
const osThreadAttr_t morseToLEDTask_attributes = {
  .name = "morseToLEDTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for emptyRXTask */
osThreadId_t emptyRXTaskHandle;
const osThreadAttr_t emptyRXTask_attributes = {
  .name = "emptyRXTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for serialTXTask */
osThreadId_t serialTXTaskHandle;
const osThreadAttr_t serialTXTask_attributes = {
  .name = "serialTXTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for rxQueue */
osMessageQueueId_t rxQueueHandle;
const osMessageQueueAttr_t rxQueue_attributes = {
  .name = "rxQueue"
};
/* Definitions for ledQueue */
osMessageQueueId_t ledQueueHandle;
const osMessageQueueAttr_t ledQueue_attributes = {
  .name = "ledQueue"
};
/* Definitions for txQueue */
osMessageQueueId_t txQueueHandle;
const osMessageQueueAttr_t txQueue_attributes = {
  .name = "txQueue"
};
/* Definitions for serialMut */
osMutexId_t serialMutHandle;
const osMutexAttr_t serialMut_attributes = {
  .name = "serialMut"
};
/* Definitions for ledMut */
osMutexId_t ledMutHandle;
const osMutexAttr_t ledMut_attributes = {
  .name = "ledMut"
};
/* Definitions for morseEvents */
osEventFlagsId_t morseEventsHandle;
const osEventFlagsAttr_t morseEvents_attributes = {
  .name = "morseEvents"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void serialRX(void *argument);
void asciiToMorse(void *argument);
void morseToLED(void *argument);
void emptyRXsendTX(void *argument);
void serialTX(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of serialMut */
  serialMutHandle = osMutexNew(&serialMut_attributes);

  /* creation of ledMut */
  ledMutHandle = osMutexNew(&ledMut_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of rxQueue */
  rxQueueHandle = osMessageQueueNew (16, sizeof(uint8_t), &rxQueue_attributes);

  /* creation of ledQueue */
  ledQueueHandle = osMessageQueueNew (16, sizeof(morseData), &ledQueue_attributes);

  /* creation of txQueue */
  txQueueHandle = osMessageQueueNew (16, sizeof(uint32_t), &txQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of serialRXTask */
  serialRXTaskHandle = osThreadNew(serialRX, NULL, &serialRXTask_attributes);

  /* creation of asciiToMorseTas */
  asciiToMorseTasHandle = osThreadNew(asciiToMorse, NULL, &asciiToMorseTas_attributes);

  /* creation of morseToLEDTask */
  morseToLEDTaskHandle = osThreadNew(morseToLED, NULL, &morseToLEDTask_attributes);

  /* creation of emptyRXTask */
  emptyRXTaskHandle = osThreadNew(emptyRXsendTX, NULL, &emptyRXTask_attributes);

  /* creation of serialTXTask */
  serialTXTaskHandle = osThreadNew(serialTX, NULL, &serialTXTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of morseEvents */
  morseEventsHandle = osEventFlagsNew(&morseEvents_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_serialRX */
/**
* @brief Function implementing the serialRXTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_serialRX */
void serialRX(void *argument)
{
  /* USER CODE BEGIN serialRX */
  /* Infinite loop */
  for(;;)
  {
	  osMutexAcquire(serialMutHandle, osWaitForever);

	  uint8_t rxC;

	  if (HAL_UART_Receive(&huart3, &rxC, 1, osWaitForever) == HAL_OK) {
		  osMessageQueuePut(rxQueueHandle, &rxC, 0, osWaitForever);
	  } else {
		  osEventFlagsSet(morseEventsHandle, FLAG_EMPTY);
	  }

	  osMutexRelease(serialMutHandle);
  }
  /* USER CODE END serialRX */
}

/* USER CODE BEGIN Header_asciiToMorse */
/**
* @brief Function implementing the asciiToMorseTas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_asciiToMorse */
void asciiToMorse(void *argument)
{
  /* USER CODE BEGIN asciiToMorse */
  /* Infinite loop */
  for(;;)
  {
	  uint8_t rxC;
	  osMessageQueueGet(rxQueueHandle, &rxC, 0, osWaitForever);

	  morseData data = morseLUT[rxC];

	  osMessageQueuePut(ledQueueHandle, &data, 0, osWaitForever);
  }
  /* USER CODE END asciiToMorse */
}

/* USER CODE BEGIN Header_morseToLED */
/**
* @brief Function implementing the morseToLEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_morseToLED */
void morseToLED(void *argument)
{
  /* USER CODE BEGIN morseToLED */
  /* Infinite loop */
  for(;;)
  {
	  morseData data;
	  osMessageQueueGet(ledQueueHandle, &data, 0, osWaitForever);

	  osMutexAcquire(ledMutHandle, osWaitForever);

	  for (uint8_t i = 0; i<data.n; i++) {
		  if (data.symbols & (1<<i)) {
			  // dash
			  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			  osDelay(3*timeUnit); // 3* dash
			  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		  } else {
			  // dott
			  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			  osDelay(timeUnit); // 1* dott
			  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		  }

		  osDelay(timeUnit); // 1* between 2 symbols
	  }

	  osDelay(3*200); // 3* between letters

	  osMutexRelease(ledMutHandle);
	  osEventFlagsSet(morseEventsHandle, FLAG_SENT);
  }
  /* USER CODE END morseToLED */
}

/* USER CODE BEGIN Header_emptyRXsendTX */
/**
* @brief Function implementing the emptyRXTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_emptyRXsendTX */
void emptyRXsendTX(void *argument)
{
  /* USER CODE BEGIN emptyRXsendTX */
  /* Infinite loop */
  for(;;)
  {
	  // empty buffer 0001
	  // sent done 0010
	  // FlagsWait sensitive on 0011 => 0x1 and 0x2 !
	  // use #define and make something like that: FLAG_EMPTY | FLAG_DONE => or means 0x3
	  uint32_t flags = osEventFlagsWait(morseEventsHandle, FLAG_EMPTY|FLAG_SENT, osFlagsWaitAny, osWaitForever);

	  if(flags & FLAG_EMPTY) { // emptybuffer
		  char txt[] = "RX Buffer empty!\r\n";
		  osMutexAcquire(serialMutHandle, osWaitForever);

		  osMessageQueuePut(ledQueueHandle, &txt, 0, osWaitForever);

		  osMutexRelease(serialMutHandle);
	  }

	  if (flags & FLAG_SENT) { // sent done
		  char txt[] = "Sent done!\r\n";
		  osMutexAcquire(serialMutHandle, osWaitForever);

		  osMessageQueuePut(ledQueueHandle, &txt, 0, osWaitForever);

		  osMutexRelease(serialMutHandle);

	  }
  }
  /* USER CODE END emptyRXsendTX */
}

/* USER CODE BEGIN Header_serialTX */
/**
* @brief Function implementing the serialTXTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_serialTX */
void serialTX(void *argument)
{
  /* USER CODE BEGIN serialTX */
  /* Infinite loop */
  for(;;)
  {
	  char* txt;
	  osMessageQueueGet(txQueueHandle, &txt, 0, osWaitForever);

	  osMutexAcquire(serialMutHandle, osWaitForever);
	  HAL_UART_Transmit(&huart3, (uint8_t *) txt, strlen(txt), 30);
	  osMutexRelease(serialMutHandle);
  }
  /* USER CODE END serialTX */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

