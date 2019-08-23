#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"
#include "PIO.h"
#include "HwDef.h"

int getEncoderCount();
uint8_t getEncoderState();
void encoderInit(digPin pinA, digPin pinB);
void encoderIntHandler();

#endif
