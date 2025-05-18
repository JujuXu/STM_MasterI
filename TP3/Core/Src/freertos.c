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
#include <stdio.h>
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for printThreadTask */
osThreadId_t printThreadTaskHandle;
const osThreadAttr_t printThreadTask_attributes = {
  .name = "printThreadTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for processThread1T */
osThreadId_t processThread1THandle;
const osThreadAttr_t processThread1T_attributes = {
  .name = "processThread1T",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for processThread2T */
osThreadId_t processThread2THandle;
const osThreadAttr_t processThread2T_attributes = {
  .name = "processThread2T",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for printStrThreadT */
osThreadId_t printStrThreadTHandle;
const osThreadAttr_t printStrThreadT_attributes = {
  .name = "printStrThreadT",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for processStrTask */
osThreadId_t processStrTaskHandle;
const osThreadAttr_t processStrTask_attributes = {
  .name = "processStrTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for printQueue */
osMessageQueueId_t printQueueHandle;
const osMessageQueueAttr_t printQueue_attributes = {
  .name = "printQueue"
};
/* Definitions for printStrQueue */
osMessageQueueId_t printStrQueueHandle;
const osMessageQueueAttr_t printStrQueue_attributes = {
  .name = "printStrQueue"
};
/* Definitions for uartMutex */
osMutexId_t uartMutexHandle;
const osMutexAttr_t uartMutex_attributes = {
  .name = "uartMutex",
  osMutexRecursive | osMutexPrioInherit, NULL, 0
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
typedef struct{
	char c;
	uint32_t val;
} impData_t;

typedef struct{
	char* c;
	uint32_t val;
} impDataStr_t;
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void printThread(void *argument);
void processThread1(void *argument);
void processThread2(void *argument);
void printStrThread(void *argument);
void processStr(void *argument);

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
  /* creation of uartMutex */
  uartMutexHandle = osMutexNew(&uartMutex_attributes);

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
  /* creation of printQueue */
  printQueueHandle = osMessageQueueNew (32, sizeof(impData_t), &printQueue_attributes);

  /* creation of printStrQueue */
  printStrQueueHandle = osMessageQueueNew (32, sizeof(impDataStr_t), &printStrQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of printThreadTask */
  printThreadTaskHandle = osThreadNew(printThread, NULL, &printThreadTask_attributes);

  /* creation of processThread1T */
  processThread1THandle = osThreadNew(processThread1, NULL, &processThread1T_attributes);

  /* creation of processThread2T */
  processThread2THandle = osThreadNew(processThread2, NULL, &processThread2T_attributes);

  /* creation of printStrThreadT */
  printStrThreadTHandle = osThreadNew(printStrThread, NULL, &printStrThreadT_attributes);

  /* creation of processStrTask */
  processStrTaskHandle = osThreadNew(processStr, NULL, &processStrTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

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

/* USER CODE BEGIN Header_printThread */
/**
* @brief Function implementing the printThreadTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_printThread */
void printThread(void *argument)
{
  /* USER CODE BEGIN printThread */
  /* Infinite loop */
  for(;;)
  {
	  impData_t data;
	  osMessageQueueGet(printQueueHandle, &data, 0, osWaitForever);

	  char buffer[20];
	  sprintf(buffer, "%c %u\r\n", data.c, (unsigned int) data.val);

	  osMutexAcquire(uartMutexHandle, osWaitForever);

	  HAL_UART_Transmit(&huart3, (uint8_t *) buffer, strlen(buffer), 30);

	  osMutexRelease(uartMutexHandle);
	  osDelay(1);
  }
  /* USER CODE END printThread */
}

/* USER CODE BEGIN Header_processThread1 */
/**
* @brief Function implementing the processThread1T thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_processThread1 */
void processThread1(void *argument)
{
  /* USER CODE BEGIN processThread1 */
  /* Infinite loop */
  for(;;)
  {
	  impData_t data = {'a',1};
//	  strcpy(data.c, "process");

	  osMessageQueuePut(printQueueHandle, &data, 0, osWaitForever);

	  osDelay(500);
  }
  /* USER CODE END processThread1 */
}

/* USER CODE BEGIN Header_processThread2 */
/**
* @brief Function implementing the processThread2T thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_processThread2 */
void processThread2(void *argument)
{
  /* USER CODE BEGIN processThread2 */
  /* Infinite loop */
  for(;;)
  {
		impData_t data = {'b', 2};
		//strcpy(data.c, "process2");

		osMessageQueuePut(printQueueHandle, &data, 0, osWaitForever);

		osDelay(500);
  }
  /* USER CODE END processThread2 */
}

/* USER CODE BEGIN Header_printStrThread */
/**
* @brief Function implementing the printStrThreadT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_printStrThread */
void printStrThread(void *argument)
{
  /* USER CODE BEGIN printStrThread */
  /* Infinite loop */
  for(;;)
  {
	  impDataStr_t data;
	  osMessageQueueGet(printQueueHandle, &data, 0, osWaitForever);

	  char buffer[64];
	  sprintf(buffer, "%s %lu\r\n", data.c, data.val);

	  osMutexAcquire(uartMutexHandle, osWaitForever);

	  HAL_UART_Transmit(&huart3, (uint8_t *) buffer, strlen(buffer), 30);

	  osMutexRelease(uartMutexHandle);

	  osDelay(1);
  }
  /* USER CODE END printStrThread */
}

/* USER CODE BEGIN Header_processStr */
/**
* @brief Function implementing the processStrTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_processStr */
void processStr(void *argument)
{
  /* USER CODE BEGIN processStr */
  /* Infinite loop */
  for(;;)
  {
	  char str[] = "Hello World!";
	  impDataStr_t data = {str, 69};
	  //strcpy(data.c, "process2");

	  osMessageQueuePut(printQueueHandle, &data, 0, osWaitForever);

	  osDelay(500);
  }
  /* USER CODE END processStr */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

