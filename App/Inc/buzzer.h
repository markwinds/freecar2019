#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "common.h"
#include "include.h"

typedef enum
{
    circle = 0
} RoadType;

extern void initBuzzer();
extern void openBuzzer();
extern void closeBuzzer();
extern void turnBuzzer();
extern void tellMeRoadType(RoadType road_type);

#endif //_BUZZER_H_