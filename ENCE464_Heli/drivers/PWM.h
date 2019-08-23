#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "HwDef.h"
#include "PIO.h"

#define PWM_PERIOD_FACTOR 256

void pwmDutySet(pwmPin pin, int duty);
void pwmPeriodSet(pwmPin pin, int period);
pwmPin pwmPinInit(pwmChan pwm, digPin pin, int link);

void breath(pwmPin pin);
#endif
