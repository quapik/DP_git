/*
 * sensors_control.h
 *
 *  Created on: 11. 12. 2024
 *      Author: xsimav01
 */

#ifndef SENSORS_CONTROL_H_
#define SENSORS_CONTROL_H_

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"
#include <stdio.h>
#include "clock_config.h"
#include "fsl_adc16.h"

#include "delay.h"
#include "drive_control.h"

//ADC senzor na pinu PTE16
#define DEMO_ADC16_BASE          ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL  1U /* PTE16, A0-ADC0_SE1, J4-2 on FRDM-KL27Z. */




void irsensor_init(void);
uint16_t  irsensor_mesure(void);
float irsensor_convert(uint16_t);


















#endif /* SENSORS_CONTROL_H_ */
