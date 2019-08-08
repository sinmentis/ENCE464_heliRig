//********************************************************
//
// uartDemo.c - Example code for ENCE361
//
// Link with modules:  buttons2, OrbitOLEDInterface
//
// Author:  P.J. Bones	UCECE
// Last modified:	26.3.2017
//

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "stdio.h"
#include "stdlib.h"
#include "OrbitOLEDInterface.h"
#include "buttons2.h"

//********************************************************
// Constants
//********************************************************
#define SYSTICK_RATE_HZ 100
#define SLOWTICK_RATE_HZ 4
#define MAX_STR_LEN 16
//---USB Serial comms: UART0, Rx:PA0 , Tx:PA1
#define BAUD_RATE 9600
#define UART_USB_BASE           UART0_BASE
#define UART_USB_PERIPH_UART    SYSCTL_PERIPH_UART0
#define UART_USB_PERIPH_GPIO    SYSCTL_PERIPH_GPIOA
#define UART_USB_GPIO_BASE      GPIO_PORTA_BASE
#define UART_USB_GPIO_PIN_RX    GPIO_PIN_0
#define UART_USB_GPIO_PIN_TX    GPIO_PIN_1
#define UART_USB_GPIO_PINS      UART_USB_GPIO_PIN_RX | UART_USB_GPIO_PIN_TX

//********************************************************
// Prototypes
//********************************************************
void SysTickIntHandler (void);
void initClock (void);
void initSysTick (void);
void initDisplay (void);
void initialiseUSB_UART (void);
void UARTSend (char *pucBuffer);
void displayButtonState (char *butStr, char *stateStr,
	uint8_t numPushes, uint8_t charLine);

//********************************************************
// Global variables
//********************************************************
char statusStr[MAX_STR_LEN + 1];
volatile uint8_t slowTick = false;

//*******************************************************************
//
// The interrupt handler for the SysTick interrupt.
//
//*******************************************************************
void
SysTickIntHandler (void)
{
	static uint8_t tickCount = 0;
	const uint8_t ticksPerSlow = SYSTICK_RATE_HZ / SLOWTICK_RATE_HZ;

    updateButtons ();		// Poll the buttons
    if (++tickCount >= ticksPerSlow)
    {						// Signal a slow tick
    	tickCount = 0;
    	slowTick = true;
    }

}

//********************************************************
// Initialisation functions: clock, SysTick, display, UART
//********************************************************
void
initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
}

//*******************************************************************
void
initSysTick (void)
{
    //
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet (SysCtlClockGet () / SYSTICK_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister (SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable ();
    SysTickEnable ();
}

// *******************************************************
void
initDisplay (void)
{
  // intialise the Orbit OLED display
	OLEDInitialise ();
}


//********************************************************
// initialiseUSB_UART - 8 bits, 1 stop bit, no parity
//********************************************************
void
initialiseUSB_UART (void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    //
    SysCtlPeripheralEnable(UART_USB_PERIPH_UART);
    SysCtlPeripheralEnable(UART_USB_PERIPH_GPIO);
    //
    // Select the alternate (UART) function for these pins.
    //
    GPIOPinTypeUART(UART_USB_GPIO_BASE, UART_USB_GPIO_PINS);
    UARTConfigSetExpClk(UART_USB_BASE, SysCtlClockGet(), BAUD_RATE,
    		UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
			UART_CONFIG_PAR_NONE);
    UARTFIFOEnable(UART_USB_BASE);
    UARTEnable(UART_USB_BASE);
}


//**********************************************************************
// Transmit a string via UART0
//**********************************************************************
void
UARTSend (char *pucBuffer)
{
    // Loop while there are more characters to send.
    while(*pucBuffer)
    {
        // Write the next character to the UART Tx FIFO.
        UARTCharPut(UART0_BASE, *pucBuffer);
        pucBuffer++;
    }
}

//********************************************************
// Function to display a status message on the OLED
//********************************************************
void
displayButtonState (char *butStr, char *stateStr, uint8_t numPushes, uint8_t charLine)
{
    char string[MAX_STR_LEN + 1]; //Display fits 16 characters per line.
	
    OLEDStringDraw ("                ", 0, charLine);
    sprintf (string, "%s - %s %d", butStr, stateStr, numPushes);
    OLEDStringDraw (string, 0, charLine);
}


int
main(void)
{
	int8_t upPushes = 0, downPushes = 0;
	uint8_t butState;
	
	initClock ();
	initDisplay ();
	initButtons ();
	initialiseUSB_UART ();
	initSysTick ();

    OLEDStringDraw ("UART demo", 0, 0);
    displayButtonState ("UP  ", "RELS", upPushes, 2);
    displayButtonState ("DOWN", "RELS", downPushes, 3);

	while(1)
	{
        // check state of each button and display if a change is detected
        butState = checkButton (UP);
        switch (butState)
        {
        case PUSHED:
        	displayButtonState ("UP  ", "PUSH", ++upPushes, 2);
        	break;
        case RELEASED:
        	displayButtonState ("UP  ", "RELS", upPushes, 2);
        	break;
        // Do nothing if state is NO_CHANGE
        }
        butState = checkButton (DOWN);
        switch (butState)
        {
        case PUSHED:
        	displayButtonState ("DOWN", "PUSH", ++downPushes, 3);
        	break;
        case RELEASED:
        	displayButtonState ("DOWN", "RELS", downPushes, 3);
        	break;
        // Do nothing if state is NO_CHANGE
        }

		// Is it time to send a message?
        if (slowTick)
        {
        	slowTick = false;
        	// Form and send a status message to the console
        	sprintf (statusStr, "UP=%2d DN=%2d | ", upPushes, downPushes);
        	UARTSend (statusStr);
        }
	}
}

