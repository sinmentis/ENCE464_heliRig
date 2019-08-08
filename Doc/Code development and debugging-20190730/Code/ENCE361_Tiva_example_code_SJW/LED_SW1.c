// **************************************************************
// Author: Texas Instruments
// Modifications: Steve Weddell
// Date: 24 February 2017
// Purpose: This program will flash an LED on Pin 3 of Port F.
// Description: This program is based on hello.c and was modified
// for the purpose of this first ENCE361 microcontroller lab.
// **************************************************************
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//TivaWare Library Includes:
#include "inc/hw_memmap.h"          //Has GPIO_PORTF_BASE
#include "inc/tm4c123gh6pm.h"       //Has Interrupt assignment numbers, and register address for direct read-write method

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"



#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

/*
 * LED_SW1.c
 */
int main(void) {
//
// Turn on the LED

	// Set up the system clock (refer to pp.220 of the TM4C123 datasheet for an overview). 
	// Many options exist here but basically you want boost a 16MHz crystal oscillator, 
	// to 400Mz via a DLL, then divide by down by 2, and then again by 2.5, i.e., to 80Mz.
	SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_2_5);

	SysCtlDelay(100);  // let the oscillator settle down
	
	uint32_t clock = SysCtlClockGet();  // define a local variable
	 
    // register that we want to use GPIO port F
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  
	
	// Set up the specific port pin as medium strength current & pull-down config.
	// Refer to TivaWare peripheral lib user manual for set up for configuration options
	GPIOPadConfigSet(GPIO_PORTF_BASE, GREEN_LED, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);
	
	// set data direction register as output
	GPIODirModeSet(GPIO_PORTF_BASE, GREEN_LED, GPIO_DIR_MODE_OUT);

	//write a zero to the output pin 3 on port F 
	GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED, 0x00);
//
	SysCtlDelay(clock/12);     // Delay

	// enter a gadfly loop
	while(1)
	{
	    //
	    // Turn off the LED
	    //
	    GPIOPinWrite(GPIO_PORTF_BASE,  GREEN_LED, 0x00);

	    //
	    // Delay
	    //
	    SysCtlDelay(clock/12);

	    //
	    // Turn on the LED
	    //
	    GPIOPinWrite(GPIO_PORTF_BASE,  GREEN_LED, GREEN_LED);

	    //
	    // Delay
	    //
	    SysCtlDelay(clock/12);

	}
}
