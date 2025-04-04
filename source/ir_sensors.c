/*
 * ir_tracker.c
 *
 *  Created on: 21. 2. 2025
 *      Author: xsimav01
 */

#include <ir_sensors.h>



adc16_config_t IR_ADC_config_struct;
adc16_channel_config_t IR_ADC_channel_config_struct;


//funkce ktera se stara o obsluhu preruseni pro IR senzor na ADC0 pinu
void IR_ADC_IRQ_HANDLER(void)
{
	IR_mesure_done = true;
	IR_sensor_raw = ADC16_GetChannelConversionValue(IR_ADC_BASE, IR_ADC_CHANNEL_GROUP);
	SDK_ISR_EXIT_BARRIER;
}

//Prevod ADC digital value na centimetry
void irsensor_convert(uint32_t ir_orig)
{
	IR_sensor_cm = 6 + ((4095.0 - ir_orig)/4095.0) * 26;
}

void irsensor_init(void)
{

	EnableIRQ(IR_ADC_IRQ);
	ADC16_GetDefaultConfig(&IR_ADC_config_struct);
	#ifdef BOARD_ADC_USE_ALT_VREF
	    IR_ADC_config_struct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
	#endif
	    ADC16_Init(IR_ADC_BASE, &IR_ADC_config_struct);
	    ADC16_EnableHardwareTrigger(IR_ADC_BASE, false);
	#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
	    /*
	    if (kStatus_Success == ADC16_DoAutoCalibration(IR_ADC_BASE))
	    {
	        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
	    }
	    else
	    {
	        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
	    }
	    */
	#endif

	    IR_ADC_channel_config_struct.channelNumber                        = IR_ADC_CHANNEL;
	    IR_ADC_channel_config_struct.enableInterruptOnConversionCompleted = true;
	#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
	    IR_ADC_channel_config_struct.enableDifferentialConversion = false;
	#endif
	ADC16_SetChannelConfig(IR_ADC_BASE, IR_ADC_CHANNEL_GROUP, &IR_ADC_channel_config_struct);
}

//Funkce, kdy pokud bylo dokončeno měření, je hodnota převedena na CM a je opět zavoláno měření
void irsensor_check(void)
{
	if(IR_mesure_done)
		{	if(IR_sensor_raw > 2300) PRINTF ("STOOOOOOOOOOOOP\r\n");
			irsensor_convert(IR_sensor_raw);
			IR_mesure_done = false;
			ADC16_SetChannelConfig(IR_ADC_BASE, IR_ADC_CHANNEL_GROUP, &IR_ADC_channel_config_struct);
			PRINTF("ADC Value: %d and CM value %d\r\n", IR_sensor_raw,IR_sensor_cm);


		}

}


void updateTrackerValues(uint8_t index)
{	/*
	index = index - 1;
	if(ir_trackers[index] == false)
	{
		ir_trackers[index] = true;
		PRINTF("TRACKER %u TRUE \r\n", index+1);
		getTrackersValuesCount();
	}
	else
	{
		ir_trackers[index] = false;
		//PRINTF("TRACKER %u FALSE \r\n", index);
		getTrackersValuesCount();
	}
	*/
}

void getTrackersValuesCount (void)
{
	/*
	uint8_t cnt = 0;
	for(uint8_t i = 0; i < 6; i++)
	{
		if(ir_trackers[i] == true) cnt++;
	}

	PRINTF("TRUE COUNT %u \r\n", cnt);

	if(cnt > 2 && driving)
	{
		led_R();
		motor_set_speed(0);
		steer_straight();
		PRINTF("LINE DETECTED \r\n");
		startMotorsButtonPressed = false;
		driving = false;
	}
	*/



}



