#include <tasks/gTaskSensor.h>
//*****************************************************************************
//
// gTaskSensor.c - A simple task that collect data from ADC, buttons and encoder.
//
// The buttons and encoder are configured by interrupt.
// The ADC is pulling.
// All collected data are sending out though Queue - g_height_Queue &&  g_yaw_Queue
// button step size is 5.
//
// The whole task is protected by *taskSem.
// The adc reading is protected by adcSem.
// The Encoder is protected by a mutex *encoderMux.
//*****************************************************************************

digPin PF4,PF0,PE0,PB0,PB1,PD2,PE3,PE4;
adcPin adcPin1,dummyAdcPin;
int height_dest = 0, yaw_dest = 0;

// Getters
int getHeiDest()
{
    if(height_dest>9)   {return height_dest;} // start motor if destination above 9
    else                {return 0;}
}

int getYawDest(){return yaw_dest;}

// Config button with interrupt
void ButtonUpHandler() {
    if(height_dest != 100) {height_dest += 5;}      // max height_dest = 100 %
    GPIOIntClear(PE0.io_port.BASEIO,PE0.io_pin);
}

void ButtonDownHandler() {
    if(height_dest != 0) {height_dest -= 5;}        // min height_dest =   0 %
    GPIOIntClear(PD2.io_port.BASEIO,PD2.io_pin);
}

void heightInthandler() {
    if(GPIOIntStatus(PF0.io_port.BASEIO,0) & PF0.io_pin) {yaw_dest -= 5;}
    else {yaw_dest += 5;}
    GPIOIntClear(PF0.io_port.BASEIO,PF0.io_pin);
    GPIOIntClear(PF4.io_port.BASEIO,PF4.io_pin);
}

void sensorInit()
{
    // Buttons
    PF4 = digPinInit(PF,4,IN);                  // RIGHT
    digPinPuPdSet(PF4,PULL_UP);
    digPinIntAttach(PF4,heightInthandler, RISE);
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R |= GPIO_PIN_0;              //PF0 unlocked
    GPIO_PORTF_LOCK_R = GPIO_LOCK_M;
    PF0 = digPinInit(PF,0,IN);                  // LEFT
    digPinPuPdSet(PF0,PULL_UP);
    digPinIntAttach(PF0,heightInthandler, RISE);
    PE0 = digPinInit(PE,0,IN);                  // UP
    digPinPuPdSet(PE0,PULL_DOWN);
    digPinIntAttach(PE0, ButtonUpHandler, RISE);
    PD2 = digPinInit(PD,2,IN);                  // DOWN
    digPinPuPdSet(PD2,PULL_DOWN);
    digPinIntAttach(PD2, ButtonDownHandler, RISE);

    // Encoder
    PB0 = digPinInit(PB,0,IN);
    PB1 = digPinInit(PB,1,IN);
    encoderInit(PB0, PB1);

    // Adc
    PE4 = digPinInit(PE,4,IN);
    adcPin1 = adcPinInit(PE4,0,9);
    PE3 = digPinInit(PE,3,IN);
    dummyAdcPin = adcPinInit(PE3,1,0); // potential meter ADC for Debug use only
}

void sensorTask(xSemaphoreHandle *param)
{
    xSemaphoreHandle taskSem = param[0];
    xSemaphoreHandle adcSem = param[1];
    xSemaphoreHandle encoderMux = xSemaphoreCreateMutex();
    sensorInit();

    while(1)
    {
        xSemaphoreTake(taskSem,10);                                 // Protect task [GO]

        xSemaphoreTake(adcSem,0);                                   // Protect ADC reading [GO]

        uint16_t adcBuffer;
        adcReadtoBuffer(adcPin1,&adcBuffer);
        adcBuffer = (3120 - adcBuffer) * 100 / 1360;
        xQueueSend(g_height_Queue, &adcBuffer, ( TickType_t ) 0);

        xSemaphoreGive(adcSem);                                     // Protect ADC reading [END]

        xSemaphoreTake(encoderMux,0);                               // Protect encoder reading [GO]

        int16_t YawCount;
        YawCount = getEncoderCount();
        xQueueSend(g_yaw_Queue, &YawCount, ( TickType_t ) 0);

        xSemaphoreGive(encoderMux);                                 // Protect encoder reading [END]

        xSemaphoreGive(taskSem);                                    // Protect task [END]
        vTaskDelay(10);
    }
}
