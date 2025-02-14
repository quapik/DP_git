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

#define DEMO_TPM_BASEADDR TPM0
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL kTPM_Chnl_5
#define TPM_INTERRUPT_NUMBER      TPM0_IRQn
#define TPM_INPUT_CAPTURE_HANDLER TPM0_IRQHandler
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl5InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl5Flag
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgIrc48MClk)

void tmp0_init(void);
void TriggerPulse(void);
void TPM_INPUT_CAPTURE_HANDLER(void);


#endif /* SONIC_H_ */
