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
void irsensor_convert(uint32_t ir_orig);
void irsensor_mesure(void);


