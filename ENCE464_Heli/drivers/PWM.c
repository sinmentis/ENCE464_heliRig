#include "PWM.h"


void pwmDutySet(pwmPin pin, int duty)
{
    PWMPulseWidthSet(pin.io_pwm.PWMCORE.BASEPWM,pin.io_pwm.OUT, duty*PWM_PERIOD_FACTOR);
    PWMSyncUpdate(pin.io_pwm.PWMCORE.BASEPWM,pin.io_pwm.GEN/4);
    pin.duty = duty;
}

void pwmPeriodSet(pwmPin pin, int period)
{
    PWMGenPeriodSet(pin.io_pwm.PWMCORE.BASEPWM, pin.io_pwm.GEN, period);
}

pwmPin pwmPinInit(pwmChan pwm, digPin pin, int link)
{
    pwmPin thisPin = {pwm, pin, pwm.GEN, 1000*PWM_PERIOD_FACTOR, 0};
    if(!pin.io_port.EN) {portEnable(pin.io_port);}
    if(!pwm.PWMCORE.EN) {SysCtlPeripheralEnable(pwm.PWMCORE.SYSPWM);}
    GPIOPinTypePWM(pin.io_port.BASEIO,pin.io_pin);
    PWMGenConfigure(pwm.PWMCORE.BASEPWM, pwm.GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    pwmPeriodSet(thisPin,1000*PWM_PERIOD_FACTOR);
    pwmDutySet(thisPin,0);
    digPinAFset(pin,link);
    PWMGenEnable(pwm.PWMCORE.BASEPWM, pwm.GEN);
    PWMOutputState(pwm.PWMCORE.BASEPWM,1<<(pwm.OUT&0x0000000f),1);
    pwm.PWMCORE.EN = 1;
    return thisPin;
}

