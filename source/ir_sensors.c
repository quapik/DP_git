/*
 * ir_tracker.c
 *
 *  Created on: 21. 2. 2025
 *      Author: xsimav01
 */

#include <ir_sensors.h>

#define IR_ADC_BASE          ADC0
#define IR_ADC_CHANNEL_GROUP 0U
#define IR_ADC_CHANNEL  1U /* PTE16, A0-ADC0_SE1, J4-2 on FRDM-KL27Z. */

#define IR_ADC_IRQ             ADC0_IRQn
#define IR_ADC_IRQ_HANDLER ADC0_IRQHandler

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool IR_mesure_done = false;
const uint32_t IR_ADC_range = 4096U;

adc16_config_t IR_ADC_config_struct;
adc16_channel_config_t IR_ADC_channel_config_struct;


//funkce ktera se stara o obsluhu preruseni pro IR senzor na ADC0 pinu
void IR_ADC_IRQ_HANDLER(void)
{
	IR_mesure_done = true;
	IR_sensor_raw = ADC16_GetChannelConversionValue(IR_ADC_BASE, IR_ADC_CHANNEL_GROUP);
	//ADC16_SetChannelConfig(IR_ADC_BASE, IR_ADC_CHANNEL_GROUP, &IR_ADC_channel_config_struct);
	SDK_ISR_EXIT_BARRIER;
}

//Prevod ADC digital value na centimetry
void irsensor_convert(uint32_t ir_orig)
{
	IR_sensor_cm = 6 + ((4095.0 - ir_orig)/4095.0) * 27;
}

void irsensor_init(void)
{

	EnableIRQ(IR_ADC_IRQ);
	    ADC16_GetDefaultConfig(&IR_ADC_config_struct);
	#ifdef BOARD_ADC_USE_ALT_VREF
	    IR_ADC_config_struct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
	#endif
	    ADC16_Init(IR_ADC_BASE, &IR_ADC_config_struct);
	    ADC16_EnableHardwareTrigger(IR_ADC_BASE, false); /* Make sure the software trigger is used. */
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
	#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */

	    IR_ADC_channel_config_struct.channelNumber                        = IR_ADC_CHANNEL;
	    IR_ADC_channel_config_struct.enableInterruptOnConversionCompleted = true; /* Enable the interrupt. */
	#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
	    IR_ADC_channel_config_struct.enableDifferentialConversion = false;
	#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */

	    ADC16_SetChannelConfig(IR_ADC_BASE, IR_ADC_CHANNEL_GROUP, &IR_ADC_channel_config_struct);
}


void irsensor_mesure(void)
{
	//SDK_DelayAtLeastUs(100000, 48000000);
	IR_mesure_done = false;
	ADC16_SetChannelConfig(IR_ADC_BASE, IR_ADC_CHANNEL_GROUP, &IR_ADC_channel_config_struct);
	while (!IR_mesure_done)
	{
	}
	irsensor_convert(IR_sensor_raw);
	PRINTF("ADC Value: %d\r\n", IR_sensor_raw);
}


void updateTrackerValues(uint8_t index)
{	/*
	index = index - 1;
	if(ir_trackers[index] == false)
	{
		ir_trackers[index] = true;
		PRINTF("TRACKER %u TRUE \r\n", index);
		//getTrackersValuesCount();
	}
	else
	{
		ir_trackers[index] = false;
		//PRINTF("TRACKER %u FALSE \r\n", index);
		//getTrackersValuesCount();
	}
	*/
}

void getTrackersValuesCount (void)
{
	uint8_t cnt = 0;
	for(uint8_t i = 0; i < 6; i++)
	{
		if(ir_trackers[i] == true) cnt++;
	}

	//PRINTF("TRUE COUNT %u \r\n", cnt);

	if(cnt > 2)
	{
		led_R();
		motor_set_speed(0);
		steer_straight();
		PRINTF("LINE DETECTED \r\n");
		startMotorsButtonPressed = false;
		driving = false;
	}




}



