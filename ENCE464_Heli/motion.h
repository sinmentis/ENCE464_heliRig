/*
 * Written by (Shun Lyu && Ke Gao && Yat Chung LEUNG
 * Date:15/5/2018
 * Use to calculate PID control value and set PWM
 */
#ifndef MOTION_H_
#define MOTION_H_

#include "main.h"

#define BUF_SIZE 10
#define SAMPLE_RATE_HZ 60

#define Kp 0.4 // heli3: 0.6
#define Ki 0.2 // heli3: 0.3
#define Kd 0.1 // heli3: 0.2

#define Kpy 1.6  // heli3: 2
#define Kiy 0.2// heli3: 0.3
#define Kdy 0.1//heli3: 0.15


#define T 1/60
#define HOLD_DUTY 50
#define OUTPUT_MAX 98
#define OUTPUT_MIN 2

#define Heli_Height_MAX     800
#define Heli_Height_StepSize 200
#define Heli_Yaw_StepSize    19
// PWM configuration
#define PWM_START_RATE_HZ  250
#define PWM_RATE_STEP_HZ   50
#define PWM_RATE_MIN_HZ    50
#define PWM_RATE_MAX_HZ    400

#define PWM_START_DUTY     0
#define PWM_DUTY_MIN_HZ    5
#define PWM_DUTY_MAX_HZ    95

#define PWM_DIVIDER_CODE   SYSCTL_PWMDIV_4
#define PWM_DIVIDER        4

#define PWM_FRE_FIXED      200
#define PWM_DUTY_FIXED     50

#define PWM_MAIN_BASE        PWM0_BASE
#define PWM_MAIN_GEN         PWM_GEN_3
#define PWM_MAIN_OUTNUM      PWM_OUT_7
#define PWM_MAIN_OUTBIT      PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM  SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE   GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN    GPIO_PIN_5

//  PWM Hardware Details M1PWM5 (gen 2)
//  ---Sec Rotor PWM: PF1, J3-10
#define PWM_SEC_BASE        PWM1_BASE
#define PWM_SEC_GEN         PWM_GEN_2
#define PWM_SEC_OUTNUM      PWM_OUT_5
#define PWM_SEC_OUTBIT      PWM_OUT_5_BIT
#define PWM_SEC_PERIPH_PWM  SYSCTL_PERIPH_PWM1
#define PWM_SEC_PERIPH_GPIO SYSCTL_PERIPH_GPIOF
#define PWM_SEC_GPIO_BASE   GPIO_PORTF_BASE
#define PWM_SEC_GPIO_CONFIG GPIO_PF1_M1PWM5
#define PWM_SEC_GPIO_PIN    GPIO_PIN_1


void setPWM (uint32_t ui32Duty);
void setPWM2 (uint32_t ui32Duty);
void HPIDcontrol(int pos, int dest);
void YawControl(int yaw,int dest);

#endif
