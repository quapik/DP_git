/*
 * globals.h
 *
 *  Created on: 20. 2. 2025
 *      Author: xsimav01
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_


#include "fsl_debug_console.h"
#include "fsl_pit.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"
#include <stdio.h>
#include "clock_config.h"
#include "fsl_adc16.h"
#include "fsl_adapter_timer.h"
#include "fsl_port.h"
#include "fsl_clock.h"

#include "delay.h"
#include "drive_control.h"
#include "pixySPI.h"
#include <sonic_color.h>
#include "led_button.h"
#include "interrupts.h"
#include "pit_control.h"

extern bool jedouMotory;
extern uint8_t pctMotory;
extern int8_t pctServo;
extern bool isObstacleDetected;
extern bool startMotorsButtonPressed;

extern bool ir_trackers[6];
extern uint16_t IR_sensor_cm;
extern uint32_t IR_sensor_raw;

extern bool driving;

extern uint16_t COLOR1_value_global;
extern uint16_t COLOR2_value_global;

extern uint16_t SRF_distance1_global;
extern uint16_t SRF_distance2_global;

extern bool probihaZmena;

#endif /* GLOBALS_H_ */
