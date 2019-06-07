#ifndef _PID_H_
#define _PID_H_

#include "common.h"
#include "include.h"

typedef struct ADCPid
{
    int32 kp;
    int32 kd;
    int32 error;
    int32 pre_error;
    int32 last_error;
} ADCPid;

extern ADCPid ADC_pid;

extern int32 getSteerPwmFromADCError();

#endif //_PID_H_