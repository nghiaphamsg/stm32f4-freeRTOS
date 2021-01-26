<h1> FreeRTOS On STM32F4xx With Debugging </h1>

## 1. Introduce
<p align="center"> <img src="https://user-images.githubusercontent.com/32474027/105848287-1c024f00-6022-11eb-8a6f-6bdae761b44d.jpg" /> </p>

- FreeRTOS is a real-time operating system kernel for embedded devices that has been ported to 35 microcontroller platforms
- FreeRTOS is designed to be small and simple. To make the code readable, easy to port, and maintainable, it is written mostly in C, but there are a few assembly functions included where needed (mostly in architecture-specific scheduler routines).
- FreeRTOS provides methods for multiple threads or tasks, mutexes, semaphores and software timers. A tick-less mode is provided for low power applications.

## 2. Goal
- Understanding various RTOS concepts with FreeRTOS Programming and Debugging
- Using STM32 Standard Peripheral Driver APIs to configure peripherals
- FreeRTOS Debugging using SEGGER SystemView Software
- Using FreeRTOS API with code examples
- Learn Complete Step by step method to run FreeRTOS on STM32 MCUs using OpenSTM32 System Workbench
- ...

## 3. Prepare

| Software              | Hardware               |       
|-----------------------|------------------------|
| [OpenSTM32 System Workbench](https://www.openstm32.org/Installing%2BSystem%2BWorkbench%2Bfor%2BSTM32%2Bfrom%2BEclipse)   | [Board NUCLEO-F446RE](https://www.st.com/en/evaluation-tools/nucleo-f446re.html) |
| [SEGGER SystemView](https://www.segger.com/downloads/systemview/)  | Cable |
| [FreeRTOS](https://sourceforge.net/projects/freertos/files/FreeRTOS/) | Led |
| STM32F4 standard peripherals library | Button |
| minicom |

 
