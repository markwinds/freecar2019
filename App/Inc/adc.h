#ifndef _ADC_H
#define _ADC_H

#include "common.h"
#include "include.h"

// typedef enum Inductance
// {
//     horizontal_middle = ADC0_DP3,
//     horizontal_left = ADC0_DP1,
//     horizontal_right = ADC1_DM1,
//     vertical_left = ADC0_DP0,
//     vertical_right = ADC0_DM1
// } Inductance;

extern void initADC();
extern int32 getErrorFromADC();
extern void showADCVaule();

#endif //_ADC_H