/*******************************************************************
 *
 * pwm100.c - Example program to output PWM on the main rotor pin.
 *
 * Author:  P.J. Bones	UCECE
 * Last modified:	12.3.2015
 *
 ******************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

/*******************************************
 *      PWM config details.
 *******************************************/
#define PWM_RATE_HZ  200
#define PWM_START_PC  10
#define PWM_DIVIDER_CODE  SYSCTL_PWMDIV_2
#define PWM_DIVIDER  2

/*******************************************
 *      PWM Hardware Details.
 *******************************************/
//---Main Rotor PWM: M0PWM7,PC5, J4-05
#define PWM_MAIN_BASE			PWM0_BASE
#define PWM_MAIN_GEN			PWM_GEN_3	//covers outputs 6 and 7
#define PWM_MAIN_OUTNUM			PWM_OUT_7
#define PWM_MAIN_OUTBIT			PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM		SYSCTL_PERIPH_PWM0	//module 0
#define PWM_MAIN_PERIPH_GPIO	SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE		GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG	GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN		GPIO_PIN_5



/****************************************************
 * initialisePWM
 * M0PWM7 (J4-05, PC5) is used for the main rotor motor
 ***************************************************/
void initialisePWM(void)
{
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);

    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    // Calculate the PWM period corresponding to PWM_RATE_HZ.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / PWM_RATE_HZ;

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, ui32Period);

    // Set the pulse width for PWM_START_PC % duty cycle.
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, ui32Period * PWM_START_PC / 100);

    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
}

int
main(void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

    // Set the PWM clock rate to 10 MHz (which means 80 Hz min rate)
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);

    // As a precaution, make sure that the peripherals used are reset
    SysCtlPeripheralReset(PWM_MAIN_PERIPH_GPIO); //Used for PWM output
    SysCtlPeripheralReset(PWM_MAIN_PERIPH_PWM);  //Main Rotor PWM

    initialisePWM();

    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);

    while(1)
    {

    }
}
