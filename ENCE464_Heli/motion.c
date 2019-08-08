/*
 * Written by Shun Lyu & Ke Gao & Yat Chung LEUNG
 * Date:15/5/2018
 * Use to calculate PID control value and set PWM
 */
#include "motion.h"

static int tot_error_height = 0,
last_error_height = 0,
last_error_yaw = 0,
tot_error_yaw = 0;

void setPWM (uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period =
            SysCtlClockGet() / PWM_DIVIDER / PWM_START_RATE_HZ;

    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, ui32Period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM,
                     ui32Period * ui32Duty / 100);
}


void setPWM2 (uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period =
            SysCtlClockGet() / PWM_DIVIDER / PWM_START_RATE_HZ;

    PWMGenPeriodSet(PWM_SEC_BASE, PWM_SEC_GEN, ui32Period);
    PWMPulseWidthSet(PWM_SEC_BASE, PWM_SEC_OUTNUM,
                     ui32Period * ui32Duty / 100);
}

/*
 * calculate and output PWM signal to the main motor
 *
 * pos = position of the helicopter
 * dest = destination of the helicopter
 *
 * the control signal was calculated by find the ratio between
 * the error and the full range (1000), then multiply with the range of the
 * pwm(100) output thus a P control with Kp = 1 would be ((error)/1000)*100 = (error)/10
 */

void HPIDcontrol(int pos, int dest)
{
    float P_PWM = Kp*(pos-dest)/10;

    float D_PWM = ((Kd/T)*((pos-dest) - last_error_height))/10;

    float I_PWM = Ki*tot_error_height*T/10;

    int tot_PWM = (int)(P_PWM + D_PWM + I_PWM);

    if(tot_PWM<=95 && tot_PWM>=5)
    {
        setPWM(tot_PWM);
        tot_error_height += (pos-dest);
    }
    else if                 (tot_PWM < 5)       {setPWM(5);}
    else            {setPWM(95);}

    last_error_height = (pos-dest);
}

/*
 * calculate and output PWM signal to the tail motor
 *
 * pos = yaw of the helicopter
 * dest = destination of the helicopter
 *
 * the control signal was calculated by find the ratio between
 * the error and the full range (448), then multiply with the range of the
 * pwm(100) output thus a P control with Kp = 1 would be ((error)/448)*100
 */
void YawControl(int yaw,int dest)
{
    float P_PWM = Kpy*(dest-yaw)*100/448;

    float D_PWM = ((Kdy/T)*((dest-yaw) - last_error_yaw))*100/448;

    float I_PWM = (Kiy*tot_error_yaw*T*100)/448;

    int tot_PWM = (int)(P_PWM + D_PWM + I_PWM);

    if(tot_PWM<=95 && tot_PWM>=5)
    {
        setPWM2(tot_PWM);
        tot_error_yaw += dest-yaw;
    }
    else if                 (tot_PWM < 5)       {setPWM2(5);}
    else            {setPWM2(95);}

    last_error_yaw = dest-yaw;
}


