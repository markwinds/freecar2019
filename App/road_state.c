#include "common.h"
#include "include.h"

/**状态保护位*/
uint8 protect_flag = 0;

void checkSmallCircleLeft();

/******************************************定时器操作************************************/
void setRoadTypeTimer(int16 time_long, void (*deal_function)())
{
    if (time_long <= 0)
    {
        return;
    }
    pit_init_ms(PIT1, time_long);
    set_vector_handler(PIT1_VECTORn, deal_function);
    enable_irq(PIT1_IRQn);
}

void resetTimer()
{
    PIT_Flag_Clear(PIT1); //清中断标志位
    disable_irq(PIT1_IRQn);
    protect_flag = 0;
}

/******************************************函数************************************/

void updataRoadState(RoadState *new_road_state)
{
    last_road_state = now_road_state;
    now_road_state->out_state();
    now_road_state = new_road_state;
    /**开启状态保护*/
    protect_flag = new_road_state->protect_time <= 0 ? 0 : 1;
    now_road_state->enter_state();
    //setRoadTypeTimer(now_road_state->protect_time, now_road_state->deal_timer);
    if (now_road_state->protect_time <= 0)
    {
        return;
    }
    pit_init_ms(PIT1, now_road_state->protect_time);
    set_vector_handler(PIT1_VECTORn, checkSmallCircleLeft);
    enable_irq(PIT1_IRQn);
}

/*********************************************doNothing************************************/
void doNothings()
{
}

/****************************************small_circle_left************************************/
void makeSound()
{
    // if (last_road_state == &small_circle_left)
    // {
    //     setSteer(-50);
    //     DELAY_MS(2000);
    // }
    //if (fuck_you < 1)
    tellMeRoadType(now_road_state->sound_type);
    printf("buzzer\n");
}
void justTurnLED()
{
    resetTimer();
    led_turn(LED1);
}

void checkSmallCircleLeft()
{

    resetTimer();
    int32 mid = getADCVaule(horizontal_middle);
    int32 left = getADCVaule(horizontal_left);
    //int32 right = getADCVaule(horizontal_right);
    if (mid + left > 1800)
    {
        fuck_you++;
        setSteer(-50);
        printf("turn\n");
        DELAY_MS(2000);
    }
    //resetTimer();
}

/*********************************************定义道路类型************************************/
RoadState straighta_way = {0, doNothings, doNothings, T2L1L1, doNothings};
// RoadState big_circle_left = {0, doNothings, doNothings, T2L1L1, doNothings};
// RoadState big_circle_right = {0, doNothings, doNothings, T2L1L1, doNothings};
RoadState small_circle_left = {0, makeSound, doNothings, T1L5, justTurnLED};
RoadState small_circle_left1 = {3000, makeSound, doNothings, T1L5, checkSmallCircleLeft};
// RoadState small_circle_right = {0, doNothings, doNothings, T1L5, doNothings};

RoadState *now_road_state = &straighta_way;
RoadState *last_road_state = &straighta_way;

/*********************************************提供给外部的判别接口************************************/
void judgeRoadFromADC(int32 mid, int32 left, int32 right)
{
    if (protect_flag)
    {
        return;
    }
    if (mid + left > 1900)
    {
        updataRoadState(&small_circle_left1);
    }
    else if (mid + right > 1900)
    {
        //updataRoadState(&small_circle_right);
    }
}