#ifndef PID_H_
#define PID_H_

#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "HwDef.h"

#define T 1/60

typedef struct{
    float last_diff;
    float total_diff;
    float Kp;
    float Ki;
    float Kd;
    uint32_t PID_out;
} PID_controller;

/* Functions */
PID_controller pidInit(float Kp,float Ki,float Kd, float gain);
int Go(PID_controller PID, int pos_present, int pos_dest);

#endif
