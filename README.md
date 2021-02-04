<h1> FreeRTOS On STM32F4xx With Debugging </h1>

## 1. Introduce
<p align="center"> <img src="https://user-images.githubusercontent.com/32474027/105848287-1c024f00-6022-11eb-8a6f-6bdae761b44d.jpg" /> </p>

- FreeRTOS is a real-time operating system kernel for embedded devices that has been ported to 35 microcontroller platforms
- FreeRTOS is designed to be small and simple. To make the code readable, easy to port, and maintainable, it is written mostly in C, but there are a few assembly functions included where needed (mostly in architecture-specific scheduler routines).
- FreeRTOS provides methods for multiple threads. A tick-less mode is provided for low power applications.
- FreeRTOS kernel features:
  + Pre-emptive or co-operative operation
  + Very flexible task priority assignment
  + Software timers
  + Queues
  + Binarysemaphores
  + Countingsemaphores
  + Recursivesemaphores
  + Mutexes
  + Tick hook functions
  + Idle hook functions
  + Stack overflow checking
  + Trace hook macros
 

## 2. FreeRTOS Structure & Hierarchy
- Download sources:
```shell
 https://sourceforge.net/projects/freertos/files/FreeRTOS/
```
```
FreeRTOS
  ├── Demo             (Contains demo application for every official FreeRTOS port)
  ├── Test             (contains the tests performed on common code and the portable layer code)
  └── Source           (Core FreeRTOS kernel file)
      ├── include      (Consists of The core FreeRTOS kernel header files)
      └── portable     (Consists of Processor specific code)
          ├── Compiler (Contains all the ports created by compiler)
          ├── MemMang  (Contains The sample heap implementations)
          └── ...
```

- Structure & Hierarchy:
```
FreeRTOS.h
   ├── FreeRTOSConfig.h        (Application specific configuration options)
   ├── projdefs.h              (Basic FreeRTOS definitions)
   └── portable.h              (Definitions specific to the port being used)
         ├── portmacro.h
         └── mpu_wrappers.h

FreeRTOS.h
   ├── list.c
   ├── croutine.c
   ├── timers.c
   ├── heap_?.c
   ├── queue.c
   └── tasks.c
```

## 3. Goal
- Understanding various RTOS concepts with FreeRTOS Programming and Debugging
- Using STM32 Standard Peripheral Driver APIs to configure peripherals
- FreeRTOS Debugging using SEGGER SystemView Software
- Using FreeRTOS API with code examples
- Learn Complete Step by step method to run FreeRTOS on STM32 MCUs using OpenSTM32 System Workbench
- ...

## 4. Prepare

| Software              | Hardware               |       
|-----------------------|------------------------|
| [OpenSTM32 System Workbench](https://www.openstm32.org/Installing%2BSystem%2BWorkbench%2Bfor%2BSTM32%2Bfrom%2BEclipse)   | [Board NUCLEO-F446RE](https://www.st.com/en/evaluation-tools/nucleo-f446re.html) |
| [SEGGER SystemView](https://www.segger.com/downloads/systemview/)  | Cable |
| [FreeRTOS v10.1.1](https://sourceforge.net/projects/freertos/files/FreeRTOS/) | Led |
| STM32F4 standard peripherals library | Button |
| minicom |

 
