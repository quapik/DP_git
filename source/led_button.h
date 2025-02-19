/*
 * led_button.h
 *
 *  Created on: 19. 2. 2025
 *      Author: xsimav01
 */

#ifndef LED_BUTTON_H_
#define LED_BUTTON_H_

#include "pin_mux.h"
#include "board.h"
#include "fsl_port.h"
#include "board.h"
#include "fsl_debug_console.h"
void led_on(void);
void led_off(void);
void led_set(uint8_t R,uint8_t G,uint8_t B);
void led_B(void);
void led_R(void);
void led_G(void);
void led_Y(void);
void led_C(void);
void led_M(void);

void button_IRQ_init();



#endif /* LED_BUTTON_H_ */
