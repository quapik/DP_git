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

//COLOR SENSORS
#define COLOR_SENSORS_TMP TPM0
#define COLOR_SENSOR0_TPM_INPUT_CAPTURE_CHANNEL kTPM_Chnl_0
#define COLOR_SENSOR1_TPM_INPUT_CAPTURE_CHANNEL kTPM_Chnl_1

#define COLOR_SENSORS_TPM_INTERRUPT_NUMBER      TPM0_IRQn
#define COLOR_SENSORS_TPM_INPUT_CAPTURE_HANDLER TPM0_IRQHandler

#define COLOR_SENSOR1_TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl1InterruptEnable
#define COLOR_SENSOR1_TPM_CHANNEL_FLAG             kTPM_Chnl1Flag

#define COLOR_SENSOR0_TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define COLOR_SENSOR0_TPM_CHANNEL_FLAG             kTPM_Chnl0Flag

#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgIrc48MClk)


//ADC senzor na pinu PTE16
#define DEMO_ADC16_BASE          ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL  1U /* PTE16, A0-ADC0_SE1, J4-2 on FRDM-KL27Z. */


void irsensor_init(void);
uint16_t  irsensor_mesure(void);
float irsensor_convert(uint16_t);
void check_colors_sensors_interrupts(void);
void color_sensors_init(void);


















#endif /* SENSORS_CONTROL_H_ */
