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

extern int32 ADC_true_vaule[5];
extern int32 ADC_max_vaule[5];
extern int32 ADC_normal_vaule[5];
extern int32 last_ADC_normal_vaule[5];
extern int steer_offset;

extern void initADC();
extern int32 getErrorFromADC();
extern void showADCVaule();
extern int32 getADCVaule(ADCn_Ch_e ADC_channel);
extern void initADCUI();
extern void ADCSelteNext();
extern void ADCSelteBefore();
extern void showADCSeletPoint(uint16 colour);
extern void updateADCMaxVaule();
extern void updateADCVaule();
extern void showADCvaule();
extern void writeADCParamToFlash();
extern void readADCParamToFlash();

#endif //_ADC_H