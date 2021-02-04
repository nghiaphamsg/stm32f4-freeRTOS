<h1> FreeRTOS App Debugging Using SEGGER SystemView Tools </h1>

## Exercise
- Write a program to create 2 tasks with same priorities ([001_HelloWorld](https://github.com/nghiaphamsg/STM32F4_FreeRTOS/tree/master/001_HelloWorld)).
- Case 1: Single-shot recording mode
- Case 2: Continues recording mode

## 1. What is SEGGER SystemView ?
- SystemView is a software toolkit which is used to analyze the embedded software behaviour running on your target.
- The SystemView can be used to analyze how your embedded code is behaving on the target:
	+ You can analyze how many tasks are running and how much duration they consume on the CPU
	+ ISR entry and exit timings and duration of run on the CPU
	+ You can analyze other behaviour of tasks: like blocking, unblocking, notifying, yielding, etc
	+ You can analyze CPU idle time so that you can think of sending CPU to speed mode
	+ Total runtime behaviour of the application 
	+ It sheds light on what exactly happened in which order, which interrupt has triggered which task switch, which interrupt and task has called which API function of the underlying RTOS
	+ SystemView should be used to verify that the embedded system behaves as expected and can be used to find problems and inefficiencies, such as superfluous and spurious interrupts, and unexpected task changes

![Screenshot from 2021-02-04 22-09-36](https://user-images.githubusercontent.com/32474027/106897033-ba369900-6735-11eb-972f-d5ee2adcb005.png)

## 2. SystemView visualization modes
### 1. Real time recording (Continuous recording)
- With a SEGGER J-Link and its Real Time Transfer (RTT) technology SystemView can continuously record data, and analyze and visualize it in real time
- Real time mode can be achieved via ST-link instead of J-link . For that J-link firmware has to be flashed on ST-link circuitry of STM32 boards

![Screenshot from 2021-02-04 22-13-44](https://user-images.githubusercontent.com/32474027/106897460-4f399200-6736-11eb-9b80-a66ed2e52ed3.png)

### 2. Single-shot recording
- You need not to have JLINK or STLINK debugger for this
- In single-shot mode the recording is started manually in the application, which allows recording only specific parts, which are of interest

![Screenshot from 2021-02-04 22-14-39](https://user-images.githubusercontent.com/32474027/106897538-65dfe900-6736-11eb-9abe-c7780b6463cb.png)

## 3. Setup SEGGER SystemView
### Step 1: Download
- Download SW here:
```
	https://www.segger.com/downloads/systemview/
````
- Download targets sources:
```
	https://www.segger.com/downloads/systemview/SystemView_Src_V320.zip.sig
```
### Step 2: Including SEGGER SystemView in the application
- Copy below to `/Config`
```
/SystemView_Src_V320/Sample/FreeRTOSV10/Config/Cortex-M/SEGGER_SYSVIEW_Config_FreeRTOS.c
/SystemView_Src_V320/Config/*
```
- Copy below to `/OS`
```
/SystemView_Src_V320/Sample/FreeRTOSV10/SEGGER_SYSVIEW_FreeRTOS.c
/SystemView_Src_V320/Sample/FreeRTOSV10/SEGGER_SYSVIEW_FreeRTOS.h
```
- Copy below to `/Patch/FreeRTOSv10`
```
/SystemView_Src_V320/Sample/FreeRTOSV10/Patch/FreeRTOSV10_Core.patch
```
- Copy below to `/SEGGER`
```
/SystemView_Src_V320/SEGGER/SEGGER.h
/SystemView_Src_V320/SEGGER/SEGGER_RTT.c
/SystemView_Src_V320/SEGGER/SEGGER_RTT.h
/SystemView_Src_V320/SEGGER/SEGGER_SYSVIEW.c
/SystemView_Src_V320/SEGGER/SEGGER_SYSVIEW.h
/SystemView_Src_V320/SEGGER/SEGGER_RTT_ASM_ARMv7M.S
/SystemView_Src_V320/SEGGER/SEGGER_SYSVIEW_ConfDefaults.h
/SystemView_Src_V320/SEGGER/SEGGER_SYSVIEW_Int.h		
```
- Include header file (Don't forget)

```
Third_Party
└── SEGGER
		├── Config
		│   ├── Global.h
		│   ├── SEGGER_RTT_Conf.h
		│   ├── SEGGER_SYSVIEW_Conf.h
		│   └── SEGGER_SYSVIEW_Config_FreeRTOS.c
		├── OS
		│   ├── SEGGER_SYSVIEW_FreeRTOS.c
		│   └── SEGGER_SYSVIEW_FreeRTOS.h
		├── Patch
		│   └── FreeRTOSv10_1
		│       └── FreeRTOSV10_Core.patch
		└── SEGGER
		├── SEGGER.h
		├── SEGGER_RTT_ASM_ARMv7M.S
		├── SEGGER_RTT.c
		├── SEGGER_RTT.h
		├── SEGGER_SYSVIEW.c
		├── SEGGER_SYSVIEW_ConfDefaults.h
		├── SEGGER_SYSVIEW.h
		└── SEGGER_SYSVIEW_Int.h
```













