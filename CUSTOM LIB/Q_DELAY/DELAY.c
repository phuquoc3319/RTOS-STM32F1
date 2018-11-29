#include "DELAY.h"

extern volatile uint32_t Val_time_Stick;

void Delay_Init(void)
{
	if(SysTick_Config(SystemCoreClock/1000))
	{
		while(1);
	}
}


void delay_us(uint32_t time)
{
	uint32_t temp=0;
	temp = Val_time_Stick+(time/1000);
	while(Val_time_Stick<temp);
}
void delay_ms(uint32_t time1)
{
	uint32_t temp1=0;
	temp1 = Val_time_Stick+time1;
	while(Val_time_Stick<temp1);
}