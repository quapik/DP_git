/*
 * led_button.c
 *
 *  Created on: 19. 2. 2025
 *      Author: xsimav01
 */
#include "led_button.h"

void led_set(uint8_t R,uint8_t G,uint8_t B)
{
	if(R == 1) R = 0;
	else R = 1;
	if(G == 1) G = 0;
	else G = 1;
	if(B == 1) B = 0;
	else B = 1;
	GPIO_PinWrite(BOARD_INITPINS_LED_GREEN_GPIO, BOARD_INITPINS_LED_GREEN_PIN, G);
	GPIO_PinWrite(BOARD_INITPINS_LED_RED_GPIO, BOARD_INITPINS_LED_RED_PIN, R);
	GPIO_PinWrite(BOARD_INITPINS_LED_BLUE_GPIO, BOARD_INITPINS_LED_BLUE_PIN, B);
}
void led_off(void){led_set(0,0,0);}
void led_on(void){led_set(1,1,1);}
void led_G(void){led_set(0,1,0);}
void led_R(void){led_set(1,0,0);}
void led_B(void){led_set(0,0,1);}
void led_M(void){led_set(1,0,1);}
void led_C(void){led_set(0,1,1);}
void led_Y(void){led_set(1,1,0);}

void button_IRQ_init()
{
	   NVIC_SetPriority(PORTA_IRQn, 2);
	    NVIC_EnableIRQ(PORTA_IRQn);
	    /*
	    NVIC_SetPriority(PORTC_IRQn, 2);
	    NVIC_EnableIRQ(PORTACIRQn);
	    */
}
/*
void PORTA_IRQHandler(void) {
    if (PORT_GetPinsInterruptFlags(BOARD_INITPINS_SW1_PORT) & (1 << BOARD_INITPINS_SW1_PIN)) {
        PRINTF("SW1 stisknuto!\r\n");
        PORT_ClearPinsInterruptFlags(BOARD_INITPINS_SW1_PORT, (1 << BOARD_INITPINS_SW1_PIN));
    }
}
*/
/*
void PORTC_IRQHandler(void) {
    if (PORT_GetPinsInterruptFlags(BOARD_INITPINS_SW3_PORT) & (1 << BOARD_INITPINS_SW3_PIN)) {
        PRINTF("SW1 stisknuto!\r\n");
        PORT_ClearPinsInterruptFlags(BOARD_INITPINS_SW3_PORT, (1 << BOARD_INITPINS_SW3_PIN));
    }
}
*/
