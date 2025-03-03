/*
 * pit_control.h
 *
 *  Created on: 3. 3. 2025
 *      Author: xsimav01
 */

#ifndef PIT_CONTROL_H_
#define PIT_CONTROL_H_
#include "globals.h"


#endif /* PIT_CONTROL_H_ */
void PIT_Timer_Init(hal_timer_handle_t timerHandler, uint32_t timeout, hal_timer_callback_t callback, pit_chnl_t channel);
void PIT_Timers_Init();
void PIT_Callback_10us(void *param);
void PIT_Callback_100ms(void *param);
void PIT_delay_10us(void);
void PIT_delay_100ms(void);
