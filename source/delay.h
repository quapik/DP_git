#ifndef DELAY_H
#define DELAY_H

#include "clock_config.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"
#include <stdio.h>
#include <stdint.h>


extern volatile uint32_t g_systickCounter; 

void SysTick_Init(void);
void SysTick_Handler(void);
void SysTick_DelayTicks(uint32_t n);

#endif