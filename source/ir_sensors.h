/*
 * ir_tracker.h
 *
 *  Created on: 21. 2. 2025
 *      Author: xsimav01
 */
#include "globals.h"

void updateTrackerValues(uint8_t index);
void getTrackersValuesCount (void);
void irsensor_init(void);
uint16_t irsensor_mesure(void);
uint16_t irsensor_convert(uint16_t ir_orig);


