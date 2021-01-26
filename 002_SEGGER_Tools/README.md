<h1> Creating FreeRTOS Based Project </h1>

## Exercise
- Write a program to create 2 tasks with same priorities
	+ Task-1: print "Hello world from task 1" 
	+ Task-2: print "Hello world from task 2"
- Case 1: using ARM semi-hosting feature to print log on the console
- Case 2: using UART peripheral

## 1. Tasks Introduction 
- A real time application that uses an RTOS can be structured as a set of independent tasks. Each task executes within its own context with no coincidental dependency on other tasks within the system or the RTOS scheduler itself. 
- Only one task within the application can be executing at any point in time and the real time RTOS scheduler is responsible for deciding which task this should be. 
- The RTOS scheduler may therefore repeatedly start and stop each task (swap each task in and out) as the application executes. 
- As a task has no knowledge of the RTOS scheduler activity it is the responsibility of the real time RTOS scheduler to ensure that the processor context (register values, stack contents, etc) when a task is swapped in is exactly that as when the same task was swapped out. To achieve this each task is provided with its own stack. 
- When the task is swapped out the execution context is saved to the stack of that task so it can also be exactly restored when the same task is later swapped back in.

![Screenshot from 2021-01-23 23-56-10](https://user-images.githubusercontent.com/32474027/105630034-e0cd1800-5e89-11eb-92b0-69077df38084.png)

![Screenshot from 2021-01-23 23-55-24](https://user-images.githubusercontent.com/32474027/105630042-ecb8da00-5e89-11eb-8e04-cfb1a562f89d.png)

## 2. Task States
- **Running**
When a task is actually executing it is said to be in the Running state. It is currently utilising the processor. If the processor on which the RTOS is running only has a single core then there can only be one task in the Running state at any given time.

- **Ready**
Ready tasks are those that are able to execute (they are not in the Blocked or Suspended state) but are not currently executing because a different task of equal or higher priority is already in the Running state.

- **Blocked**
A task is said to be in the Blocked state if it is currently waiting for either a temporal or external event. For example, if a task calls vTaskDelay() it will block (be placed into the Blocked state) until the delay period has expired – a temporal event. Tasks can also block to wait for queue, semaphore, event group, notification or semaphore event. Tasks in the Blocked state normally have a ‘timeout’ period, after which the task will be timeout, and be unblocked, even if the event the task was waiting for has not occurred.
Tasks in the Blocked state do not use any processing time and cannot be selected to enter the Running state.

- **Suspended**
Like tasks that are in the Blocked state, tasks in the Suspended state cannot be selected to enter the Running state, but tasks in the Suspended state do not have a time out. Instead, tasks only enter or exit the Suspended state when explicitly commanded to do so through the vTaskSuspend() and xTaskResume() API calls respectively.

## 3. Create Tasks and Scheduler
- Create a new task and add it to the list of tasks that are ready to run.
```shell
#include "task.h"

BaseType_t xTaskCreate(  TaskFunction_t pvTaskCode,
                         const char * const pcName,
                         configSTACK_DEPTH_TYPE usStackDepth,
                         void *pvParameters,
                         UBaseType_t uxPriority,
                         TaskHandle_t *pxCreatedTask
                       );
```
![Screenshot from 2021-01-24 20-01-05](https://user-images.githubusercontent.com/32474027/105630348-022f0380-5e8c-11eb-9e29-2532ace77e2a.png)

- Starts the RTOS scheduler. After calling the RTOS kernel has control over which tasks are executed and when. The idle task and optionally the timer daemon task are created automatically when the RTOS scheduler is started.

```shell
#include "task.h"
void vTaskStartScheduler( void );
```
![Screenshot from 2021-01-24 19-59-07](https://user-images.githubusercontent.com/32474027/105630358-1410a680-5e8c-11eb-8454-395451468c35.png)

## 4. Running
### 1. Setup Semihosting on System Workbench
- Step 1: Linker argument settings

Properties > C/C++ Build > Settings > MCU GCC Linker > Miscellaneous > Linker
flags (`-specs=rdimon.specs -lc -lrdimon`) > Apply > OK 

- Step 2: Debug configuration of your application

Debug as > Debug configurations > Startup tab > Run commands (`monitor arm semihosting enable`) >
Apply > Close

- Step 3: Configure in `main.c` use below codes
```shell
#define USE_SEMIHOSTING
extern void initialise_monitor_handles();
initialise_monitor_handles();
```
- Step 4: `syscalls.c` > properties > check "Exclude resource from build"

- Right click > Debug as > C/C++ Application
![Screenshot from 2021-01-24 21-32-12](https://user-images.githubusercontent.com/32474027/105630535-335c0380-5e8d-11eb-90f5-4add963bbf2a.png)

### 2. Debug via USART2
- Step 1: Install minicom (host machine) and setup port `/dev/ttyACM0`
```
#Plugged in cable in to host
dmesg | grep /dev/tty
```

```shell
sudo apt-get install -y minicom
sudo minicom -s
```
![Screenshot from 2021-01-26 21-20-07](https://user-images.githubusercontent.com/32474027/105845190-b744f580-601d-11eb-85f0-55ec15891dc5.png)

- Step 2: Configure in `main.c` source
```shell
#define USE_USART_DEBUG

# Macro for forcing a context switch
taskYIELD() 
```
USART2 configuration parameters (BaudRate: 115200, Word: 8, Stop bits: 1, Parity: none)

- Step 3: Testing

Right click > Run as > C/C++ Application
![Screenshot from 2021-01-26 21-25-44](https://user-images.githubusercontent.com/32474027/105845220-c461e480-601d-11eb-82d4-6f7852e11e10.png)
