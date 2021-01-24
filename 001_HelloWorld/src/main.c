/**
  ******************************************************************************
  * @file    main.c
  * @author  NghiaPham
  * @version V1.0
  * @date    24-Jan-2021
  * @brief   Hello world function. Discuss about create task & debug
  ******************************************************************************
*/

#include <stdio.h>
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

/* Used for semihosting */
extern void initialise_monitor_handles();

void vTask1_Handler(void *);
void vTask2_Handler(void *);

TaskHandle_t xTaskHandler1 = NULL;
TaskHandle_t xTaskHandler2 = NULL;

int main(void)
{
  /* Semihosting init */
  initialise_monitor_handles();
  printf("Semihosting is runing\n");

  /* Resets the RCC clock configuration to the default reset state */
  /* HSI On, HSE & PLL Off */
  RCC_DeInit();

  /* Update SystemCoreClock variable according to Clock Register Values */
  /* System clock/CPU clock = 16MHz */
  SystemCoreClockUpdate();
	
  /* Create two tasks */
  xTaskCreate(vTask1_Handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandler1);
  xTaskCreate(vTask2_Handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandler2);
  
  /* Start scheduler */
  vTaskStartScheduler();

  for(;;);
}

void vTask1_Handler(void *pvParameters)
{
  while(1)
  {
    printf("Hello world from task-1\n");
  }
}

void vTask2_Handler(void *pvParameters)
{
  while(1)
  {
    printf("Hello world from task-2\n");
  }
}
