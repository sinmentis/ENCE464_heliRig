#include "ADC.h"

adcPin adcPinInit (digPin pin, int adcCore, int adcChan)
{
    int sysADC = ADC_MAIN;
    if(adcCore) {sysADC++;}
    int baseADC = ADC0_BASE;
    if(adcCore) {baseADC+=0x1000;}
    if(adcChan>15) {adcChan = adcChan-16+0x100;}
    adcPin thisPin = {sysADC,baseADC,adcChan,pin,0};
    GPIOPinTypeADC(pin.io_port.BASEIO,pin.io_pin);
    SysCtlPeripheralEnable(sysADC);
    if(!pin.io_port.EN){portEnable(pin.io_port);}
    ADCSequenceConfigure(baseADC, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(baseADC, 3, 0, adcChan | ADC_CTL_IE |
                             ADC_CTL_END);
    ADCSequenceEnable(baseADC,3);
    return thisPin;
}

void adcIntAttach(adcPin pin, void(*handler)(void))
{
    ADCIntRegister(pin.io_BASEADC, 3, &(*handler));
    ADCIntEnable(pin.io_BASEADC, 3);
    ADCIntClear(pin.io_BASEADC,3);
}

void adcReadtoBuffer(adcPin pin, uint32_t *Buffer)
{
    ADCProcessorTrigger(pin.io_BASEADC, 3);
    ADCSequenceDataGet(pin.io_BASEADC, 3, Buffer);
    ADCIntClear(pin.io_BASEADC,3);
}
