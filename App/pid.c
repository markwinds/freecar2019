#include "common.h"
#include "include.h"

ADCPid ADC_pid = {60, 0, 0, 0, 0};

int32 getSteerPwmFromADCError()
{
    ADC_pid.pre_error = ADC_pid.error;
    ADC_pid.error = getErrorFromADC();
    int32 ans_pwm = ADC_pid.kp * ADC_pid.error + ADC_pid.kd * (ADC_pid.error - ADC_pid.pre_error);
    //LCDShowNumDefule(100, 60, (ans_pwm >> 10) / 3);
    return (ans_pwm >> 5) / 5;
}