/*
 * sonic.h
 *
 *  Created on: 14. 2. 2025
 *      Author: xsimav01
 */

#ifndef SONIC_COLOR_H_
#define SONIC_COLOR_H_

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"
#include <stdio.h>
#include "clock_config.h"
#include "fsl_adc16.h"


#include "drive_control.h"

#define RYCHLOST_ZVUKU 0.0343
#define COUNTER_TO_US 2.67 // prescaler128
#define COUNTER_MAX 0xFFFF

#define TPM0_BASEADDR TPM0

#define SRF05_1_channel kTPM_Chnl_5
#define SRF05_2_channel kTPM_Chnl_4
#define COLOR_1_channel kTPM_Chnl_0
#define COLOR_2_channel kTPM_Chnl_1

#define TPM0_INTERRUPT_NUMBER      TPM0_IRQn
#define TMP0_INTERRUPT_HANDLER TPM0_IRQHandler

#define SRF05_1_channel_INTERRUPT_ENABLE kTPM_Chnl5InterruptEnable
#define SRF05_1_CHANNEL_FLAG             kTPM_Chnl5Flag
#define SRF05_2_channel_INTERRUPT_ENABLE kTPM_Chnl4InterruptEnable
#define SRF05_2_CHANNEL_FLAG             kTPM_Chnl4Flag
#define COLOR_1_channel_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define COLOR_1_CHANNEL_FLAG   			kTPM_Chnl0Flag
#define COLOR_2_channel_INTERRUPT_ENABLE kTPM_Chnl1InterruptEnable
#define COLOR_2_CHANNEL_FLAG   kTPM_Chnl1Flag
#define TPM0_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgIrc48MClk)


void TMP0Init(void);
void TriggerPulse1(void);
void TriggerPulse2(void);
uint32_t checkMaxDistance(uint32_t d);
void TMP0_INTERRUPT_HANDLER(void);
uint32_t distanceCountF(uint32_t PW);
void isObstacle(uint32_t d1, uint32_t d2);
uint32_t pulseWidthLength(uint32_t rising, uint32_t falling, uint32_t overflow);
uint32_t pulseWidthToUs(uint32_t PW);
void checkColorSensorValue(uint32_t PW_US, uint8_t i);
void processColorSensorValue();
void TMP0Reset(void);
void SonicReset(void);

extern volatile bool color_event_flag;
extern volatile uint32_t last_color_pw;
extern volatile uint8_t last_color_index;


#endif /* SONIC_COLOR_H_ */
