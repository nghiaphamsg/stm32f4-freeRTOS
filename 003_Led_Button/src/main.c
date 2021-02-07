/**
  ******************************************************************************
  * @file    main.c
  * @author  NghiaPham
  * @version V1.0
  * @date    2020/02/07
  * @brief   Button task should continously poll the button, if pressed it
  *          should update the flag. 
  *          Led task should turn on the LED if button flag is SET, otherwise turn off.
  *          LED PA5
  *          Button  PC13
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include <string.h>
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#define USE_USART_DEBUG 1
#define NOT_PRESSED     0
#define PRESSED         1

void preInitialization(void);
void usartSetup(void);
void sendData(char *);
void vLed_Task_Handler(void *);
void vButton_Task_Handler(void *);
void gpioSettup(void);

uint8_t button_flag;
char msg[256] = {0};

int main(void)
{
  /* Resets the RCC clock configuration to the default reset state */
  /* HSI On, HSE & PLL Off */
  RCC_DeInit();

  /* Update SystemCoreClock variable according to Clock Register Values */
  /* System clock/CPU clock = 16MHz */
  SystemCoreClockUpdate();

  /* Initialization area */
  preInitialization();

  /* Create led task & button task*/
  xTaskCreate(vLed_Task_Handler, "Led-Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(vButton_Task_Handler, "Button-Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  /* Start scheduler */
  vTaskStartScheduler();

}

void vLed_Task_Handler(void *pvParameters)
{
  while (1)
  {
    if (button_flag == PRESSED)
      GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
    else
      GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);    
  }
}

void vButton_Task_Handler(void *pvParameters)
{
  while (1)
  {
    if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
      button_flag = NOT_PRESSED;
    else
      button_flag = PRESSED;
  }
}

void preInitialization(void)
{
#if USE_USART_DEBUG
  /* Setup USART for debug */
  usartSetup();
#endif  /* USE_USART_DEBUG */
  gpioSettup();
}

void gpioSettup(void)
{
  /* Enable AHB1 peripheral clock for GPIOA & GPIOC */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* Initializes the GPIOA peripheral as LED & GPIOC peripheral as button */
  GPIO_InitTypeDef LED_Init, Button_Init;
  memset(&LED_Init, 0, sizeof(LED_Init));
  memset(&Button_Init, 0, sizeof(Button_Init));

  /* Configure GPIO Led PA5 */
  LED_Init.GPIO_Pin = GPIO_Pin_5;
  LED_Init.GPIO_Mode = GPIO_Mode_OUT;
  LED_Init.GPIO_OType = GPIO_OType_PP;
  LED_Init.GPIO_Speed = GPIO_Low_Speed;
  LED_Init.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_Init(GPIOA, &LED_Init);

  /* Configure GPIO Button PC13 */
  Button_Init.GPIO_Pin = GPIO_Pin_13;
  Button_Init.GPIO_Mode = GPIO_Mode_IN;
  Button_Init.GPIO_OType = GPIO_Mode_IN;
  Button_Init.GPIO_Speed = GPIO_Mode_IN;
  Button_Init.GPIO_PuPd = GPIO_Mode_IN;

  GPIO_Init(GPIOC, &Button_Init);
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

  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET);
}
