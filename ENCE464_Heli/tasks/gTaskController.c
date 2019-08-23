#include <tasks/gTaskController.h>
//*****************************************************************************
//
// gTaskController.c - A simple task that calc the PID and controll the PWM.
//
// All collected data are received and send out though Queue - g_height_Queue &&  g_yaw_Queue
// PID gain needs to adjust based on the enmulator
//
// This task is protected by *UARTMux.
//
//*****************************************************************************

// Global variables for PWM and PID
PWM pwm0 = {SYSPWM0,PWM0_BASE,0};
PWM pwm1 = {SYSPWM1,PWM1_BASE,0};
digPin PF1,PC5;
pwmPin pwmYaw,pwmHeight;
PID_controller PIDyaw,PIDheight;

void controllerInit()
{
    // pidInit(PID_controller PID, float Kp,float Ki,float Kd, float gain)
    // The profromance of the PID depends on STM32 enmulator somehow...
    PIDyaw = pidInit(1, 0.1, 0.1, 50);
    PIDheight = pidInit(2.2, 0, 0.1, 80); //

    PF1 = digPinInit(PF,1,OUT);
    pwmChan PWM1channel5 = {pwm1, PWMGEN2, PWM_OUT5};
    pwmYaw = pwmPinInit(PWM1channel5, PF1, PF1_M1_5);

    PC5 = digPinInit(PC,5,OUT);
    pwmChan PWM0channel7 = {pwm0, PWMGEN3, PWM_OUT7};
    pwmHeight = pwmPinInit(PWM0channel7, PC5, PC5_M0_7);
}

void controllerTask(void* param)
{
    controllerInit();
    xSemaphoreHandle *taskMux = (xSemaphoreHandle *) param;
    uint16_t height;
    int16_t yawPos;

    while(1){
        xSemaphoreTake(*taskMux,10);                                 // Protect controller task [GO]

        // Reading height from Queue
        xQueueReceive(g_height_Queue,   &height, ( TickType_t ) 10);
        xQueueReceive(g_yaw_Queue,      &yawPos, ( TickType_t ) 10);

        // Calc PID
        int heightOut = Go(PIDheight,   height, getHeiDest());
        int yawOut    = Go(PIDyaw,      yawPos, getYawDest());

        // Set PWM signal
        pwmDutySet(pwmHeight,heightOut);
        pwmDutySet(pwmYaw,yawOut);

        // Send result to UART for printing
        xQueueSend(g_height_Queue, &height,( TickType_t )  0);
        xQueueSend(g_yaw_Queue, &yawPos,( TickType_t )  0);

        xSemaphoreGive(*taskMux);                                    // Protect controller task [END]
        vTaskDelay(10);
    }
}
