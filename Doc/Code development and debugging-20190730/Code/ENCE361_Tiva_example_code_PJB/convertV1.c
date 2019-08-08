//*****************************************************************************
//
// convert.c - Simple interrupt driven program which samples with ADC0
//		***  Version 1 - calls API function within ISR ***
//
// Author:  P.J. Bones	UCECE
// Last modified:	15.3.2016
//
//*****************************************************************************
// Results: 
//  2 x 1.5V cells = 3.17 V (using voltmeter) = 1023 (saturated),
//   0V = ~3, 1 x 1.5V cell = ~544, noise level ~+- 2
//*****************************************************************************
// The setup for the ADC is based on Steve Weddell's 'my_adc.c'.
//*****************************************************************************

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "drivers/rit128x96x4.h"
#include "utils/ustdlib.h"
#include "circBuf.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define BUF_SIZE 10
#define SAMPLE_RATE_HZ 10

//*****************************************************************************
// Global variables
//*****************************************************************************
static circBuf_t g_inBuffer;		// Buffer of size BUF_SIZE integers (sample values)
static unsigned long g_ulSampCnt;	// Counter for the interrupts

//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt.
//
//*****************************************************************************
void
SysTickIntHandler(void)
{
    //
    // Initiate a conversion
    //
    ADCProcessorTrigger(ADC0_BASE, 3); 
    g_ulSampCnt++;
}

//*****************************************************************************
//
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
void
ADCIntHandler(void)
{
	unsigned long ulValue;
	
	//
	// Get the single sample from ADC0. (Yes, I know, a function call!!)
	ADCSequenceDataGet(ADC0_BASE, 3, &ulValue);
	//
	// Place it in the circular buffer (advancing write index)
	g_inBuffer.data[g_inBuffer.windex] = (int) ulValue;
	g_inBuffer.windex++;
	if (g_inBuffer.windex >= g_inBuffer.size)
		g_inBuffer.windex = 0;
	//
	// Clean up, clearing the interrupt
	ADCIntClear(ADC0_BASE, 3);                          
}

//*****************************************************************************
// Initialisation functions for the clock (incl. SysTick), ADC, display
//*****************************************************************************
void
initClock (void)
{
  //
  // Set the clock rate. From Section 19.1 in stellaris_peripheral_lib_UG.doc:
  //  "In order to use the ADC, the PLL must be used; the PLL output will be 
  //  used to create the clock required by the ADC." ADC rate = 8 MHz / 10.
  //  The processor clock rate = 20 MHz.
  SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);	
  //
  // Set up the period for the SysTick timer.  The SysTick timer period is
  // set as a function of the system clock.
  SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
  //
  // Register the interrupt handler
  SysTickIntRegister(SysTickIntHandler);
  //
  // Enable interrupt and device
  SysTickIntEnable();
  SysTickEnable();
}

void 
initADC (void)
{
  //
  // The ADC0 peripheral must be enabled for configuration and use.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    
  // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
  // will do a single sample when the processor sends a signal to start the
  // conversion.  
  ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
  
  //
  // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
  // single-ended mode (default) and configure the interrupt flag
  // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
  // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
  // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
  // sequence 0 has 8 programmable steps.  Since we are only doing a single
  // conversion using sequence 3 we will only configure step 0.  For more
  // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
  ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE |
                             ADC_CTL_END);    
                             
  //
  // Since sample sequence 3 is now configured, it must be enabled.
  ADCSequenceEnable(ADC0_BASE, 3);     
  
  //
  // Register the interrupt handler
  ADCIntRegister (ADC0_BASE, 3, ADCIntHandler);
  
  //
  // Enable interrupts for ADC0 sequence 3 (clears any outstanding interrupts)
  ADCIntEnable(ADC0_BASE, 3);
}

void
initDisplay (void)
{
  // intialise the OLED display
  RIT128x96x4Init(1000000);	
}

//*****************************************************************************
//
// Function to display the mean ADC value (10-bit value, note) and sample count.
//  See SW-EK-LM3S1968-Firmware-UG.pdf for information about the display driver
//  and the Micro Standard Library Module (ustdlib.c & ustdlib.h).
//
//*****************************************************************************
void
displayMeanVal(int meanVal, int count)
{
	char string[40];
	
    RIT128x96x4StringDraw("ADC0 (Pin 62)", 5, 24, 15);
    RIT128x96x4StringDraw("sampled at 10 Hz", 5, 34, 15);
	usprintf(string, "Mean value = %5d", meanVal);
    RIT128x96x4StringDraw(string, 5, 44, 15);
	usprintf(string, "Count = %7d", count);
    RIT128x96x4StringDraw(string, 5, 54, 15);
}


int
main(void)
{
	unsigned int i;
	int sum;
	
	initClock ();
	initADC ();
	initDisplay ();
	initCircBuf (&g_inBuffer, BUF_SIZE);

    //
    // Enable interrupts to the processor.
    IntMasterEnable();

	while (1)
	{
		//
		// Background task: calculate the (approximate) mean of the values in the
		// circular buffer and display it, together with the sample number.
		sum = 0;
		for (i = 0; i < BUF_SIZE; i++)
			sum = sum + readCircBuf (&g_inBuffer);
		displayMeanVal(sum / BUF_SIZE, (int) g_ulSampCnt);
	}
}

