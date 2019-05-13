#ifndef _SET_VAULE_H_
#define _SET_VAULE_H_

#include "include.h"
#include "common.h"

struct FlowValue
{
    char *name;
    uint8 *value;
    uint8 step;
};

extern struct FlowValue flow_value[];

extern void showFlowValue();
extern void nextFlowValue();
extern void beforeFlowValue();
extern void addFlowValue();
extern void subFlowValue();

#endif