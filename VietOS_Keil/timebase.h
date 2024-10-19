#ifndef __TIMEBASE_H__
#define __TIMEBASE_H__

#include <stdint.h>

void timebase_init(void);
void delay(uint32_t delay);
uint32_t get_tick(void);

#endif