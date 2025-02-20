/*
 * ir_tracker.c
 *
 *  Created on: 21. 2. 2025
 *      Author: xsimav01
 */

#include "ir_tracker.h"


void updateTrackerValues(uint8_t index)
{
	index = index - 1;
	if(ir_trackers[index] == false)
	{
		ir_trackers[index] = true;
		PRINTF("TRACKER %u TRUE \r\n", index);
		getTrackersValuesCount();
	}
	else
	{
		ir_trackers[index] = false;
		PRINTF("TRACKER %u FALSE \r\n", index);
		getTrackersValuesCount();
	}
}

void  getTrackersValuesCount (void)
{
	uint8_t cnt = 0;
	for(uint8_t i = 0; i < 6; i++)
	{
		if(ir_trackers[i] == false) cnt++;
	}
	PRINTF("FALSE COUNT %u \r\n", cnt);
	//return cnt;
}



