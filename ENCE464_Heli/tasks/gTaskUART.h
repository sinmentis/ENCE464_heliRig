#ifndef GTASKUART_H_
#define GTASKUART_H_
/* gTaskUART.h */
#include "../tasks/gTaskSensor.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "UART.h"


//VT100 commands
#define VT100_ESC 0x1B
#define VT100_HOME "[H"
#define VT100_CLS "[2J"
#define VT100_CLR "[K"
#define VT100_FG_YELLOW "[33m"
#define VT100_FG_WHITE "[37m"
#define VT100_BG_BLACK "[40m"
#define VT100_BG_MAGENTA "[45m"

// Functions
void uartTask(void* param);

#endif
