/*
 * globals.h
 *
 *  Created on: 20. 2. 2025
 *      Author: xsimav01
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <sonic_color.h>
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"
#include <stdio.h>
#include "clock_config.h"
#include "fsl_adc16.h"

#include "delay.h"
#include "drive_control.h"
#include "sensors_control.h"
#include "pixySPI.h"

#include "led_button.h"
#include "interrupts.h"

extern bool jedouMotory;
extern uint8_t pctMotory;
extern int8_t pctServo;
extern bool isObstacleDetected;
extern bool startMotorsButtonPressed;

extern bool ir_trackers[6];

extern bool driving;

extern uint16_t COLOR1_value_global;
extern uint16_t COLOR2_value_global;


#endif /* GLOBALS_H_ */
