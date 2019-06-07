#include "common.h"
#include "include.h"

int buzzer_music[][10] = {
    {100, 100, 100, 0}

};

void initBuzzer()
{
    gpio_init(PTD2, GPO, 0);
}

void openBuzzer()
{
    gpio_set(PTD2, 1);
}

void closeBuzzer()
{
    gpio_set(PTD2, 0);
}

void turnBuzzer()
{
    gpio_turn(PTD2);
}

void tellMeRoadType(RoadType road_type)
{
    int *p = buzzer_music[road_type];
    while (*p != 0)
    {
        turnBuzzer();
        DELAY_MS(*p++);
    }
    turnBuzzer();
}