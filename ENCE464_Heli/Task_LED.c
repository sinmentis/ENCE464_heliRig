/*
 * Task_LED.c
 *
 *  Created on: 8/08/2019
 *      Author: sly31
 */
#include "main.h"

xQueueHandle g_pLEDQueue;
extern xSemaphoreHandle g_pUARTSemaphore;

static void LEDTask(void *pvParameters) {
    while(1) {
        if(xQueueReceive(g_pLEDQueue, 0) == pdPASS) {
            xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
            xSemaphoreGive(g_pUARTSemaphore);
        }
    }
}

uint32_t LEDTaskInit(void) {
    // Init GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    if(xTaskCreate(LEDTask, (const portCHAR *)"LED", LEDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_LED_TASK, NULL) != pdTRUE) {return(1);}
    return(0);

}
/*
 * TO BE CONTINUED
 */