#ifndef ADC_H_
#define ADC_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "HwDef.h"
#include "PIO.h"
#include "inc/tm4c123gh6pm.h"

adcPin adcPinInit (digPin pin, int adcCore, int adcChan);
void adcIntAttach(adcPin pin, void(*handler)(void));
void adcReadtoBuffer(adcPin pin, uint32_t *Buffer);
#endif
