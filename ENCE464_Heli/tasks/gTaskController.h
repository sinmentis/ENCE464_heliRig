#ifndef GTASKCONTROLLER_H_
#define GTASKCONTROLLER_H_
/* gTaskController.h */
#include "drivers/HwDef.h"
#include <stdint.h>
#include <stdbool.h>
#include "../tasks/gTaskSensor.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "drivers/PIO.h"
#include "drivers/PWM.h"
#include "drivers/PID.h"

// External ports and queue
extern IOPort PC,PF;
extern QueueHandle_t g_height_Queue;
extern QueueHandle_t g_yaw_Queue;

// Functions
void controllerInit();
void controllerTask(void* param);

#endif
