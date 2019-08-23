#include "main.h"

//*****************************************************************************
// The error routine that is called if the driver library encounters an error.
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line){}
#endif

//*****************************************************************************
// This hook is called by FreeRTOS when an stack overflow error is detected.
// This function can not return, so loop forever.  Interrupts are disabled
// on entry to this function, so no processor interrupts will interrupt
// this loop.
//*****************************************************************************
void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName){while(1){}}
void vApplicationIdleHook(){}
void vApplicationTickHook(){}
void vApplicationMallocFailedHook(){while(1){}}

//*****************************************************************************
//                                      MAIN
//*****************************************************************************

int main(void)
{
    // Init the clock
    SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ );

    // Create semaphore and mutex
    g_pUARTMutex = xSemaphoreCreateBinary();
    xSemaphoreHandle adcMutex = xSemaphoreCreateBinary();
    xSemaphoreHandle* sensorTaskParam = (xSemaphoreHandle*) {g_pUARTMutex, adcMutex};

    // Create Queue for current Height and Yaw.
    g_height_Queue = xQueueCreate(DATA_QUEUE_LENGTH, DATA_QUEUE_ITEM_SIZE);
    g_yaw_Queue = xQueueCreate(DATA_QUEUE_LENGTH, DATA_QUEUE_ITEM_SIZE);

    // Create tasks
    xTaskCreate(controllerTask, "controller Task",  128,    g_pUARTMutex,   PRIORITY_CONTROLLER_TASK,  NULL);
    xTaskCreate(sensorTask,     "sensor Task",      128,    sensorTaskParam,PRIORITY_SENSOR_TASK,  NULL);
    xTaskCreate(uartTask,       "UI Task",          1024,   adcMutex,       PRIORITY_UART_TASK,  NULL);

    // Start the scheduler.
    vTaskStartScheduler();

    // Never Enter this line.
    while(1){}
}
