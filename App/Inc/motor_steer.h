#ifndef __MOTOR_STEER_H__
#define __MOTOR_STEER_H__

#include "common.h"
#include "include.h"

#define STEER_MID 1340
#define STEER_MAX_LEFT 1470
#define STEER_MAX_RIGHT 1190

extern int steerTable[101]; //将方向100等分

extern void initMotorSteer();
extern void setSpeedLeft(int speed);
extern void setSpeedRight(int speed);

#endif //__MOTOR_STEER_H__