#ifndef __NVIC_H__
#define __NVIC_H__

#include "common.h"
#include "include.h"

#define NVIC_PriorityGroup_0 ((uint32)0x7) // 0 bits for pre-emption priority
#define NVIC_PriorityGroup_1 ((uint32)0x6) // 1 bits for pre-emption priority
#define NVIC_PriorityGroup_2 ((uint32)0x5) // 2 bits for pre-emption priority
#define NVIC_PriorityGroup_3 ((uint32)0x4) // 3 bits for pre-emption priority
#define NVIC_PriorityGroup_4 ((uint32)0x3) // 4 bits for pre-emption priority

extern void initNVIC();

#endif //__NVIC_H__