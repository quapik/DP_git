/*
 * interrupts.c
 *
 *  Created on: 20. 2. 2025
 *      Author: xsimav01
 */

#include "interrupts.h"


void enableInterruptsOnPorts(void)
{
	NVIC_SetPriority(PORTA_IRQn, 2);
	NVIC_EnableIRQ(PORTA_IRQn);
	NVIC_SetPriority(PORTB_PORTC_PORTD_PORTE_IRQn, 2);
	NVIC_EnableIRQ(PORTB_PORTC_PORTD_PORTE_IRQn);
}

void PORTA_IRQHandler(void)
{
	//Preruseni na tlacitku SW1 pri sestupne hrane
    if (PORT_GetPinsInterruptFlags(BOARD_INITPINS_SW1_PORT) & (1 << BOARD_INITPINS_SW1_PIN)) {
        SW1_pressed();
        PORT_ClearPinsInterruptFlags(BOARD_INITPINS_SW1_PORT, (1 << BOARD_INITPINS_SW1_PIN));
    }
}


void PORTB_PORTC_PORTD_PORTE_IRQHandler(void)
{
    if (PORT_GetPinsInterruptFlags(BOARD_INITPINS_SW3_PORT) & (1 << BOARD_INITPINS_SW3_PIN)) {
        SW3_pressed();
        PORT_ClearPinsInterruptFlags(BOARD_INITPINS_SW3_PORT, (1 << BOARD_INITPINS_SW3_PIN));
    }
}

