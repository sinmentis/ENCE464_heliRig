/*
 * Written by (Shun Lyu && Ke Gao && Yat Chung LEUNG
 * Date:15/5/2018
 */
#ifndef INIT_H_
#define INIT_H_

#include "main.h"

/*
 * Use to initialize two PWM
 * PWM_MAIN_GEN && PWM_SEC_GEN
 */
void initPWM();

/*
 * initialize clock related
 */
void initClock();

/*
 * When switch is not ready, serial port printing "Switch off"
 * The whole program won't start unless swiwch is on.
 */
void initSwitch();

void initADC();

void initGPIO();
/*
 *ADCProcessorTrigger
 *updateButtons
 */
void SysTickIntHandler();


#endif /* INIT_H_ */
