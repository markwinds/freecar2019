#include "common.h"
#include "include.h"

int steerTable[101]; //将方向100等分
int speed_different = 0;

void initMotorSteer()
{
    //舵机初始化  FTM0
    ftm_pwm_init(FTM0, FTM_CH6, 100, STEER_MID);
    //左电机初始化
    ftm_pwm_init(FTM3, FTM_CH0, 10000, 0);
    ftm_pwm_init(FTM3, FTM_CH1, 10000, 0);
    //右电机初始化
    ftm_pwm_init(FTM3, FTM_CH2, 10000, 0);
    ftm_pwm_init(FTM3, FTM_CH3, 10000, 0);
    float temp = ((float)(STEER_MAX_LEFT - STEER_MID) / 50.0);
    for (int i = 0; i < 50; i++)
    {
        steerTable[i] = STEER_MID + (int)(temp * (50 - i));
        // DELAY_MS(20);
        // printf("%d,%d ", i, steerTable[i]);
    }
    temp = fabs((STEER_MAX_RIGHT - STEER_MID) / 50.0);
    for (int i = 51; i < 101; i++)
    {
        steerTable[i] = STEER_MID + (int)(temp * (50 - i));
        // DELAY_MS(20);
        // printf("%d,%d ", i, steerTable[i]);
    }
    steerTable[50] = STEER_MID;
}

/**
输入的dir为正时是右转，满转50
输入的dir为负时是左转，满转50
*/
void setSteer(int dir)
{
    dir += 50;
    dir = dir < 0 ? 0 : dir;
    dir = dir > 100 ? 100 : dir;
    ftm_pwm_duty(FTM0, FTM_CH6, steerTable[dir]);
}

/**
输入的speed为正时是正转，满速10000
输入的speed为负时是反转，满速10000
*/
void setSpeedLeft(int speed)
{
    //正转
    if (speed >= 0)
    {
        ftm_pwm_init(FTM3, FTM_CH0, 10000, (uint32)speed);
        ftm_pwm_init(FTM3, FTM_CH1, 10000, 0);
    }
    else
    {
        speed = -1 * speed;
        ftm_pwm_init(FTM3, FTM_CH0, 10000, 0);
        ftm_pwm_init(FTM3, FTM_CH1, 10000, (uint32)speed);
    }
}

/**
输入的speed为正时是正转，满速10000
输入的speed为负时是反转，满速10000
*/
void setSpeedRight(int speed)
{
    //正转
    if (speed >= 0)
    {
        ftm_pwm_init(FTM3, FTM_CH2, 10000, (uint32)speed);
        ftm_pwm_init(FTM3, FTM_CH3, 10000, 0);
    }
    else
    {
        speed = -1 * speed;
        ftm_pwm_init(FTM3, FTM_CH2, 10000, 0);
        ftm_pwm_init(FTM3, FTM_CH3, 10000, (uint32)speed);
    }
}