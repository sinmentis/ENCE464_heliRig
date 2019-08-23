#ifndef GTASKSENSOR_H_
#define GTASKSENSOR_H_
/* gTaskSensor.h */
#include "drivers/HwDef.h"
#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "drivers/PIO.h"
#include "drivers/ADC.h"

#define BLOCK_TIME_MAX      portMAX_DELAY

// External ports and queue
extern IOPort PB,PD,PE,PF;
extern QueueHandle_t g_height_Queue;
extern QueueHandle_t g_yaw_Queue;

// Functions
int getHeiDest();
int getYawDest();
void sensorInit();
void sensorTask(xSemaphoreHandle *param);
#endif
