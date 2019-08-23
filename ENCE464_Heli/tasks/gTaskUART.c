#include <tasks/gTaskUART.h>
//*****************************************************************************
//
// gTaskUART.c - A simple task that update the state of the HeliRig.
//
// The code is use VT100 commands and test in Tera Term.
// The sequence is always [ESC + COMMAND].
//
// This task is protected by *UARTMux.
//
//*****************************************************************************
void uartTask(void* param)
{
    xSemaphoreHandle *UARTMux = (xSemaphoreHandle *) param;
    uartInit();
    char message[12];

    while(1)
    {
        // Secure Uart printing task complete without interrupted.
        xSemaphoreTake(*UARTMux,10);

        // Reading current height and yaw from queue
        uint16_t adc;
        int16_t yawPos;
        xQueueReceive(g_height_Queue, &adc, ( TickType_t ) 10);
        xQueueReceive(g_yaw_Queue, &yawPos, ( TickType_t ) 10);

        // Print out with VT100 command.
        sprintf(message, "%c%s", VT100_ESC, VT100_HOME);
        uartSend(message);
        sprintf(message, "C.H: %d |C.Y: %d |D.H: %d |D.Y: %d\n", adc, yawPos, getHeiDest(), getYawDest());
        uartSend(message);

        // release
        xSemaphoreGive(*UARTMux);
    }
}
