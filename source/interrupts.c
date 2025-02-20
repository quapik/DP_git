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
	//SW3 stisknuti interrupt
    if (PORT_GetPinsInterruptFlags(BOARD_INITPINS_SW3_PORT) & (1 << BOARD_INITPINS_SW3_PIN))
    {
        SW3_pressed();
        PORT_ClearPinsInterruptFlags(BOARD_INITPINS_SW3_PORT, (1 << BOARD_INITPINS_SW3_PIN));
    }

    //IR TRACKER 1-6 nastupna/sestupna hrana interrupt
    if (PORT_GetPinsInterruptFlags(BOARD_INITPINS_tracker1_PORT) & (1 << BOARD_INITPINS_tracker1_PIN))
    {
    	updateTrackerValues(1);
        PORT_ClearPinsInterruptFlags(BOARD_INITPINS_tracker1_PORT, (1 << BOARD_INITPINS_tracker1_PIN));
    }
    if (PORT_GetPinsInterruptFlags(BOARD_INITPINS_tracker2_PORT) & (1 << BOARD_INITPINS_tracker2_PIN))
    {
    	updateTrackerValues(2);
        PORT_ClearPinsInterruptFlags(BOARD_INITPINS_tracker2_PORT, (1 << BOARD_INITPINS_tracker2_PIN));
    }
    if (PORT_GetPinsInterruptFlags(BOARD_INITPINS_tracker3_PORT) & (1 << BOARD_INITPINS_tracker3_PIN))
    {
    	updateTrackerValues(3);
        PORT_ClearPinsInterruptFlags(BOARD_INITPINS_tracker3_PORT, (1 << BOARD_INITPINS_tracker3_PIN));
    }
    if (PORT_GetPinsInterruptFlags(BOARD_INITPINS_tracker4_PORT) & (1 << BOARD_INITPINS_tracker4_PIN))
    {
    	updateTrackerValues(4);
        PORT_ClearPinsInterruptFlags(BOARD_INITPINS_tracker4_PORT, (1 << BOARD_INITPINS_tracker4_PIN));
    }
    if (PORT_GetPinsInterruptFlags(BOARD_INITPINS_tracker5_PORT) & (1 << BOARD_INITPINS_tracker5_PIN))
    {
    	updateTrackerValues(5);
    	PORT_ClearPinsInterruptFlags(BOARD_INITPINS_tracker5_PORT, (1 << BOARD_INITPINS_tracker5_PIN));
    }
    if (PORT_GetPinsInterruptFlags(BOARD_INITPINS_tracker6_PORT) & (1 << BOARD_INITPINS_tracker6_PIN))
    {
    	updateTrackerValues(6);
        PORT_ClearPinsInterruptFlags(BOARD_INITPINS_tracker6_PORT, (1 << BOARD_INITPINS_tracker6_PIN));
    }
}

