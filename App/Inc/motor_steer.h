#ifndef __MOTOR_STEER_H__
#define __MOTOR_STEER_H__

#include "common.h"
#include "include.h"

#define STEER_MID 1300
#define STEER_MAX_LEFT 1450
#define STEER_MAX_RIGHT 1150
// #define STEER_MID 652
// #define STEER_MAX_LEFT 730
// #define STEER_MAX_RIGHT 570

extern int steerTable[101]; //将方向100等分
extern int speed_different;

extern void initMotorSteer();
extern void setSteer(int dir);
extern void setSpeedLeft(int speed);
extern void setSpeedRight(int speed);

#endif //__MOTOR_STEER_H__