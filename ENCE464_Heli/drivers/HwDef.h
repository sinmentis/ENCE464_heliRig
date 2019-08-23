#ifndef HWDEF_H_
#define HWDEF_H_

#include "stdint.h"

#define SYSIOA     0xf0000800  // GPIO A
#define SYSIOB     0xf0000801  // GPIO B
#define SYSIOC     0xf0000802  // GPIO C
#define SYSIOD     0xf0000803  // GPIO D
#define SYSIOE     0xf0000804  // GPIO E
#define SYSIOF     0xf0000805  // GPIO F
#define SYSIOG     0xf0000806  // GPIO G
#define SYSIOH     0xf0000807  // GPIO H
#define SYSIOJ     0xf0000808  // GPIO J

#define PORTA_BASE         0x40004000  // GPIO Port A
#define PORTB_BASE         0x40005000  // GPIO Port B
#define PORTC_BASE         0x40006000  // GPIO Port C
#define PORTD_BASE         0x40007000  // GPIO Port D
#define PORTE_BASE         0x40024000  // GPIO Port E
#define PORTF_BASE         0x40025000  // GPIO Port F
#define PORTG_BASE         0x40026000  // GPIO Port G
#define PORTH_BASE         0x40027000  // GPIO Port H
#define PORTJ_BASE         0x4003D000  // GPIO Port J

#define SYSPWM0      		  0xf0004000  // PWM 0
#define SYSPWM1      		  0xf0004001  // PWM 1
#define PWMBASE0              0x40028000  // Pulse Width Modulator (PWM)
#define PWMBASE1              0x40029000  // Pulse Width Modulator (PWM)

#define PWMGEN0               0x00000040  // Offset address of Gen0
#define PWMGEN1               0x00000080  // Offset address of Gen1
#define PWMGEN2               0x000000C0  // Offset address of Gen2
#define PWMGEN3               0x00000100  // Offset address of Gen3

/* GPIOPinConfig values for PWM pins */
#define PB4_M0_2         0x00011004
#define PB5_M0_3         0x00011404
#define PB6_M0_0         0x00011804
#define PB7_M0_1         0x00011C04

#define PC4_M0_6         0x00021004
#define PC5_M0_7         0x00021404

#define PD0_M0_6         0x00030004
#define PD1_M0_7         0x00030404

#define PE4_M0_4         0x00041004
#define PE5_M0_5         0x00041404

#define PA6_M1_2         0x00001805
#define PA7_M1_3         0x00001C05

#define PD0_M1_0         0x00030005
#define PD1_M1_1         0x00030405

#define PE4_M1_2         0x00041005
#define PE5_M1_3         0x00041405

#define PF0_M1_4         0x00050005
#define PF1_M1_5         0x00050405
#define PF2_M1_6         0x00050805
#define PF3_M1_7         0x00050C05

#define PWM_OUT0               0x00000040  // Encoded offset address of PWM0
#define PWM_OUT1               0x00000041  // Encoded offset address of PWM1
#define PWM_OUT2               0x00000082  // Encoded offset address of PWM2
#define PWM_OUT3               0x00000083  // Encoded offset address of PWM3
#define PWM_OUT4               0x000000C4  // Encoded offset address of PWM4
#define PWM_OUT5               0x000000C5  // Encoded offset address of PWM5
#define PWM_OUT6               0x00000106  // Encoded offset address of PWM6
#define PWM_OUT7               0x00000107  // Encoded offset address of PWM7

#define ADC_MAIN     0xf0003800  // ADC 0
#define ADC0_BASE               0x40038000  // ADC0
#define ADC1_BASE               0x40039000  // ADC1

typedef struct {
	int SYSIO;
	int BASEIO;
	int EN;
} IOPort;

typedef struct {
	int SYSPWM;
	int BASEPWM;
	int EN;
} PWM;

typedef struct {
	PWM PWMCORE;
	int GEN;
	int OUT;
} pwmChan;

typedef struct {
	IOPort io_port;
	int io_pin;
	int io_config;
	int io_status;
} digPin;

typedef struct {
	pwmChan io_pwm;
	digPin  io_pin;
	int io_gen;
	int period;
	int duty;
} pwmPin;

typedef struct {
	int io_SYSADC;
	int io_BASEADC;
	int io_channel;
	digPin io_pin;
	int val;
} adcPin;

typedef struct{
	digPin A;
	digPin B;
	int count;
	uint8_t state;
} encoder;



#endif
