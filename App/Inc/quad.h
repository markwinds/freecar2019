#ifndef __QUAD_H__
#define __QUAD_H__

#include "common.h"
#include "include.h"

extern long quad_val_left;
extern long quad_val_right;

extern void Quad_Init(void);
extern void PIT0_IRQHandler(void);

#endif //__QUAD_H__