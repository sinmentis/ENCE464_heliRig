/*
 * Written by (Shun Lyu && Ke Gao && Yat Chung LEUNG
 * Date:15/5/2018
 * Include all header file this program needs.
 * Include all global variables main program needs.
 */
#ifndef READ_HELI_HEIGHT_H_
#define READ_HELI_HEIGHT_H_

#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "circBufT.h"
#include "buttons4.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "motion.h"
#include "uart_display.h"
#include "init.h"

#define BUF_SIZE 10
#define SAMPLE_RATE_HZ 60

static circBuf_t g_inBuffer;    // Buffer of size BUF_SIZE integers (sample values)
static uint32_t g_ulSampCnt;    // Counter for the interrupts
static int32_t yaw_count;          // Use to store total number
static volatile bool phase_A;
static volatile bool phase_B;
static volatile bool phase_C = 0;
static volatile bool last_state_A;
static volatile bool last_state_B;
static volatile bool sys_init_flag = 0;
static volatile bool SW_on_flag = 0;
static volatile bool land_flag = 1;
static volatile bool calibration_flag = 0;
static int32_t base_line = 0;
static int32_t mean_ADC = 0;
static int32_t height_percen = 0;
static int32_t yaw_degree = 0;
static int32_t dest_height = 0;
static int32_t dest_yaw = 0;

#endif /* READ_HELI_HEIGHT_H_ */
