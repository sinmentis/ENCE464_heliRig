#include "PID.h"
/*
 * calculate and output PWM signal to the main motor
 *
 * the control signal was calculated by find the ratio between
 * the error and the full range (1000), then multiply with the range of the
 * pwm(100) output thus a P control with Kp = 1 would be ((error)/1000)*100 = (error)/10
 *
 * the control signal was calculated by find the ratio between
 * the error and the full range (448), then multiply with the range of the
 * pwm(100) output thus a P control with Kp = 1 would be ((error)/448)*100
 */
 
PID_controller pidInit(float Kp,float Ki,float Kd, float gain) {
    PID_controller PID;
    PID.Kd = Kd*gain;
    PID.Ki = Ki*gain;
    PID.Kp = Kp*gain;
    PID.last_diff = 0;
    PID.total_diff = 0;
    PID.PID_out = 0;
    return PID;
}

int Go(PID_controller PID, int pos_present, int pos_dest) {
    float diff = pos_dest - pos_present;
    float P_PWM = PID.Kp * diff;
    float D_PWM = (PID.Kd * (diff -PID.last_diff))  ;
    float I_PWM = (PID.Ki * PID.total_diff * T)     ;
    PID.last_diff   = diff;
    PID.total_diff += diff;
    float out = P_PWM + D_PWM + I_PWM;
    if (out >= 950) {out = 950;}
    if (out <= 5) {out = 0;}
    PID.PID_out = (int)out;
}
