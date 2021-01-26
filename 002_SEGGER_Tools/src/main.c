/**
  ******************************************************************************
  * @file    main.c
  * @author  NghiaPham
  * @version V1.0
  * @date    24-Jan-2021
  * @brief   Hello world function. Discuss about create task & debug
  * @note    PA2: USART2_TX AF7
  *          PA3: USART2_RX AF7
  ******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#define USE_USART_DEBUG
// #define USE_SEMIHOSTING
#define AVAILABLE          1
#define NOT_AVAILABLE      0

/* Used for semihosting */
extern void initialise_monitor_handles();

void preInitialization(void);
void usartSetup(void);
void sendData(char *);
void vTask1_Handler(void *);
void vTask2_Handler(void *);

TaskHandle_t xTaskHandler1 = NULL;
TaskHandle_t xTaskHandler2 = NULL;

char msg[256] = {0};
uint8_t KEY = AVAILABLE;

int main(void)
{
#ifdef USE_SEMIHOSTING

  /* Semihosting init */
  initialise_monitor_handles();
  printf("Semihosting is running\n");

#endif  /* USE_SEMIHOSTING */

  /* Resets the RCC clock configuration to the default reset state */
  /* HSI On, HSE & PLL Off */
  RCC_DeInit();

  /* Update SystemCoreClock variable according to Clock Register Values */
  /* System clock/CPU clock = 16MHz */
  SystemCoreClockUpdate();
	
  /* Initialization area */
  preInitialization();

  sprintf(msg, "App is running\r\n");
  sendData(msg);

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
#ifdef USE_SEMIHOSTING
   printf("Hello world from task-1\n");
#endif  /* USE_SEMIHOSTING */
    if (KEY == AVAILABLE)
    {
      KEY = NOT_AVAILABLE;
      sendData("Hello world from task-1\r\n");
      KEY = AVAILABLE;
      taskYIELD()
    }
  }
}

void vTask2_Handler(void *pvParameters)
{
  while(1)
  {
#ifdef USE_SEMIHOSTING
   printf("Hello world from task-2\n");
#endif  /* USE_SEMIHOSTING */
    if (KEY == AVAILABLE)
    {
      KEY = NOT_AVAILABLE;
      sendData("Hello world from task-2\r\n");
      KEY = AVAILABLE;
      taskYIELD();
    }
  }
}

void preInitialization(void)
{
#ifdef USE_USART_DEBUG
  /* Setup USART for debug */
  usartSetup();
#endif  /* USE_USART_DEBUG */
}

void usartSetup(void)
{
  /* Enable APB1/AHB1 peripheral clock for USART2 & GPIOA */
  /* By default the USART2 communication is enabled */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Initializes the GPIOA peripheral as USART2 */
  GPIO_InitTypeDef GPIOA_Init;
  memset(&GPIOA_Init, 0, sizeof(GPIOA_Init));

  GPIOA_Init.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIOA_Init.GPIO_Mode = GPIO_Mode_AF;
  GPIOA_Init.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_Init(GPIOA, &GPIOA_Init);

  /* AF configure mode 7 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

  /* Initializes the USART2 peripheral */
  USART_InitTypeDef USART2_Init;
  memset(&USART2_Init, 0, sizeof(USART2_Init));

  USART2_Init.USART_BaudRate = 115200;
  USART2_Init.USART_WordLength = USART_WordLength_8b;
  USART2_Init.USART_StopBits = USART_StopBits_1;
  USART2_Init.USART_Parity = USART_Parity_No;
  USART2_Init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART2_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  
  USART_Init(USART2, &USART2_Init);

  /* Enable USART2 peripheral */
  USART_Cmd(USART2, ENABLE);
}

void sendData(char *msg)
{
  for (uint16_t i = 0; i < strlen(msg); i++)
  {
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);
    USART_SendData(USART2, msg[i]);
  }
}