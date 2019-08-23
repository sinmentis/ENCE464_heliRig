#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"

// Drivers
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"
#include "drivers/PIO.h"
#include "drivers/PID.h"
#include "drivers/HwDef.h"
#include "drivers/PWM.h"
#include "drivers/ADC.h"
#include "drivers/uart_display.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "priorities.h"

// Tasks
#include "tasks/gTaskController.h"
#include "tasks/gTaskSensor.h"
#include "tasks/gTaskUART.h"


// Head define for semaphore, mutex and queue
#define DATA_QUEUE_LENGTH               1
#define DATA_QUEUE_ITEM_SIZE            sizeof(uint32_t)
#define SEMAPHORE_MAX_COUNT         3
#define COUNTING_SEMAPHORE_INIT_COUNT   0

//*****************************************************************************
// Variables
//*****************************************************************************

IOPort PA = {SYSIOA,PORTA_BASE,0};
IOPort PB = {SYSIOB,PORTB_BASE,0};
IOPort PC = {SYSIOC,PORTC_BASE,0};
IOPort PD = {SYSIOD,PORTD_BASE,0};
IOPort PE = {SYSIOE,PORTE_BASE,0};
IOPort PF = {SYSIOF,PORTF_BASE,0};
IOPort PG = {SYSIOG,PORTG_BASE,0};
IOPort PH = {SYSIOH,PORTH_BASE,0};
IOPort PJ = {SYSIOJ,PORTJ_BASE,0};


digPin PF1,PF2,PF3;
digPin PE1,PE2,PE4;
digPin PB0,PB1;
digPin PC5;

// Buttons(UP,DOWN,LEFT,RIGHT)
digPin PE0,PD2,PF4,PF0;

// Semaphore, mutex and queue
xSemaphoreHandle g_pUARTMutex;
xSemaphoreHandle g_pADCSemaphore;
xSemaphoreHandle g_pPIDSemaphore;
QueueHandle_t g_height_Queue;
QueueHandle_t g_yaw_Queue;

int main(void);

#endif
