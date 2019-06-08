#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "common.h"
#include "include.h"

typedef enum
{
    T2L1L1 = 0,
    T1L5 = 1
} SoundType;

extern void initBuzzer();
extern void openBuzzer();
extern void closeBuzzer();
extern void turnBuzzer();
extern void tellMeRoadType(SoundType sound_type);

#endif //_BUZZER_H_