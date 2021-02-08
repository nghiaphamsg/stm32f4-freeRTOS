/**
  ******************************************************************************
  * @file    main.c
  * @author  NghiaPham
  * @version V1.0
  * @date    2020/02/08
  * @brief   Led task should turn on the LED if button flag is SET, otherwise turn off.
  *          Button interrupt handler must update the button status flag
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
void button_Handler(void);
void gpioSettup(void);

uint8_t button_flag = NOT_PRESSED;
char msg[256] = {0};

int main(void)
{
  /* Enable cycles counter in DWT_CYCCNT regiter */
  DWT->CTRL |= (1 << 0);

  /* Resets the RCC clock configuration to the default reset state */
  /* HSI On, HSE & PLL Off */
  RCC_DeInit();

  /* Update SystemCoreClock variable according to Clock Register Values */
  /* System clock/CPU clock = 16MHz */
  SystemCoreClockUpdate();

  /* Initialization area */
  preInitialization();

  /* Start the recording of events */
  SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();

  /* Create led task & button task*/
  xTaskCreate(vLed_Task_Handler, "Led-Task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

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

void button_Handler(void)
{
  button_flag ^= 1;
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
  /* Enable AHB1 peripheral clock for GPIOA & GPIOC
     Enable APB2 peripheral clock for EXTI 13 */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

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

  /* Interrupt configuration for the button */
  /* Selects the GPIO pin used as EXTI Line */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

  EXTI_InitTypeDef EXTI13_Init;
  // memset(&EXTI13_Init, 0, sizeof(EXTI13_Init));

  EXTI13_Init.EXTI_Line = EXTI_Line13;
  EXTI13_Init.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI13_Init.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI13_Init.EXTI_LineCmd = ENABLE;

  EXTI_Init(&EXTI13_Init);

  /* (NVIC) Set interrupt priority 
     According by RF, EXTI Line[15:10] of Interrupt number is 40 
     and set priority 5 */
  NVIC_SetPriority(EXTI15_10_IRQn, 5);
  NVIC_EnableIRQ(EXTI15_10_IRQn);
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

void EXTI15_10_IRQHandler(void)
{
  /* Trace for debug */
  traceISR_ENTER();

  /* When come interrupt handler then should be clear 
     interrupt pending bit of the EXTI 13 */
  EXTI_ClearITPendingBit(EXTI_Line13);
  button_Handler();

  traceISR_EXIT();
}