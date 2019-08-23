#include "PIO.h"

uint8_t digPinRead(digPin pin) {return GPIOPinRead(pin.io_port.BASEIO, pin.io_pin) == pin.io_pin;}

void portEnable(IOPort port) 
{ 
	SysCtlPeripheralEnable(port.SYSIO); 
	port.EN = 1;
}

digPin digPinInit(IOPort port, int pinNum, int mode) 
{
	int pinBin = 1 << pinNum;
	if(!port.EN) 	  {portEnable(port);}
	if(mode==OUT) 	  {GPIOPinTypeGPIOOutput(port.BASEIO, pinBin);}
    else if(mode==IN) {GPIOPinTypeGPIOInput(port.BASEIO, pinBin);}
	digPin thisPin = {port, pinBin, mode, 0};
	digPinSet(thisPin,0);
	return thisPin;
}

void digPinSet(digPin pin, int status) 
{
	if(status) 
		{GPIOPinWrite(pin.io_port.BASEIO, pin.io_pin, pin.io_pin);}
	else 
		{GPIOPinWrite(pin.io_port.BASEIO, pin.io_pin, 0);}
	pin.io_status = status;
}

void digPinPuPdSet(digPin pin, int PP)
{
	if(PP == PULL_UP) 
		{GPIOPadConfigSet(pin.io_port.BASEIO, pin.io_pin, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);}
	else 
		{GPIOPadConfigSet(pin.io_port.BASEIO, pin.io_pin, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);}
}

void digPinAFset(digPin pin, int config)
{
	GPIOPinConfigure(config);
	pin.io_config = config;
}

void digPinIntAttach(digPin pin, void(*handler)(void), triggerEdge edge)
{
	GPIOIntRegister(pin.io_port.BASEIO, handler);
	GPIOIntTypeSet(pin.io_port.BASEIO, pin.io_pin, edge);
	GPIOIntEnable(pin.io_port.BASEIO, pin.io_pin);
}
