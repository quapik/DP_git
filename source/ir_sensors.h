/*
 * ir_tracker.h
 *
 *  Created on: 21. 2. 2025
 *      Author: xsimav01
 */
#include "globals.h"

#define IR_ADC_BASE          ADC0
#define IR_ADC_CHANNEL_GROUP 0U
#define IR_ADC_CHANNEL 14U //2. pin shora

#define IR_ADC_IRQ             ADC0_IRQn
#define IR_ADC_IRQ_HANDLER ADC0_IRQHandler

void updateTrackerValues(uint8_t index);
void getTrackersValuesCount (void);
void irsensor_init(void);
void irsensor_convert(uint32_t ir_orig);
void irsensor_check(void);


