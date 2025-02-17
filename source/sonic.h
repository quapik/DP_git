/*
 * sonic.h
 *
 *  Created on: 14. 2. 2025
 *      Author: xsimav01
 */

#ifndef SONIC_H_
#define SONIC_H_

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"
#include <stdio.h>
#include "clock_config.h"
#include "fsl_adc16.h"

#include "delay.h"
#include "drive_control.h"

#define RYCHLOST_ZVUKU 0.034
#define COUNTER_TO_US 0.02083
#define COUNTER_MAX 0xFFFF

#define TPM0_BASEADDR TPM0
#define SRF05_1_channel kTPM_Chnl_5
#define SRF05_2_channel kTPM_Chnl_4
#define TPM0_INTERRUPT_NUMBER      TPM0_IRQn
#define TMP0_INTERRUPT_HANDLER TPM0_IRQHandler
#define SRF05_1_channel_INTERRUPT_ENABLE kTPM_Chnl5InterruptEnable
#define SRF05_1_CHANNEL_FLAG             kTPM_Chnl5Flag
#define SRF05_2_channel_INTERRUPT_ENABLE kTPM_Chnl4InterruptEnable
#define SRF05_2_CHANNEL_FLAG             kTPM_Chnl4Flag
#define TPM0_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgIrc48MClk)


void tmp0_init(void);
void TriggerPulse1(void);
void TriggerPulse2(void);
void TMP0_INTERRUPT_HANDLER(void);
uint32_t distanceCountF(uint32_t PW);


#endif /* SONIC_H_ */
