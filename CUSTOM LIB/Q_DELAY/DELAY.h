#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

void Delay_Init(void);
void delay_us(uint32_t time);
void delay_ms(uint32_t time1);

#endif
