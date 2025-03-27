/*
 * delay.c
 *
 *  Created on: 10. 1. 2025
 *  Author: xsimav01
 *  Soubor s implementací blokjícího čekání, využito bylo vzorových examplů ze SDK pro desku KL27Z
 */


#include "delay.h"


volatile uint32_t g_systickCounter;
void SysTick_Init(void)
{
	if (SysTick_Config(SystemCoreClock/1000000U)) //uS
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
