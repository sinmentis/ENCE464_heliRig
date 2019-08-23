#ifndef PIO_H_
#define PIO_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "HwDef.h"

enum {IN = 0, OUT = 1, HW = 2};
enum {PULL_UP,PULL_DOWN}; // pull-up, pull-down

typedef enum{
	RISE = GPIO_RISING_EDGE,
	FALL = GPIO_FALLING_EDGE, 
	BOTH = GPIO_BOTH_EDGES,
	}triggerEdge;
	
digPin digPinInit(IOPort port, int pinNum, int mode);
void digPinSet(digPin pin, int status);
uint8_t digPinRead(digPin pin);
void digPinPuPdSet(digPin pin, int PP);
void portEnable(IOPort port);
void digPinAFset(digPin pin, int config);
void digPinIntAttach(digPin pin, void(*handler)(void), triggerEdge edge);
#endif
