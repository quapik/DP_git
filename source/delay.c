#include "delay.h"

volatile uint32_t g_systickCounter;

void SysTick_Init(void)
{
	if (SysTick_Config(SystemCoreClock / 1000U))
		{
			while (1)
			{
			}
		}
}
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}
