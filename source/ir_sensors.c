/*
 * ir_tracker.c
 *
 *  Created on: 21. 2. 2025
 *      Author: xsimav01
 */

#include <ir_sensors.h>

#define DEMO_ADC16_BASE          ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL  1U /* PTE16, A0-ADC0_SE1, J4-2 on FRDM-KL27Z. */

#define DEMO_ADC16_IRQn             ADC0_IRQn
#define DEMO_ADC16_IRQ_HANDLER_FUNC ADC0_IRQHandler

volatile bool g_Adc16ConversionDoneFlag = false;
volatile uint32_t g_Adc16ConversionValue;
volatile uint32_t g_Adc16InterruptCounter;
const uint32_t g_Adc16_12bitFullRange = 4096U;

adc16_channel_config_t adc16ChannelConfigStruct;
uint16_t ir_sensor_value;

void DEMO_ADC16_IRQ_HANDLER_FUNC(void)
{
    g_Adc16ConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
    g_Adc16InterruptCounter++;
    SDK_ISR_EXIT_BARRIER;
}

uint16_t irsensor_convert(uint16_t ir_orig)
{
	uint16_t ir_new;
	ir_new = 6 + ((4095.0 - ir_orig)/4095.0) * 27;
	PRINTF("Vzdalenost: %d\r\n", ir_new);
	return  ir_new;
}

void irsensor_init(void)
{
	adc16_config_t adc16ConfigStruct;
	adc16_channel_config_t adc16ChannelConfigStruct;
	 EnableIRQ(DEMO_ADC16_IRQn);
	ADC16_GetDefaultConfig(&adc16ConfigStruct);
	#ifdef BOARD_ADC_USE_ALT_VREF
	    adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
	#endif
	    ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
	    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false); /* Make sure the software trigger is used. */
	#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
	    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASE))
	    {
	        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
	    }
	    else
	    {
	        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
	    }
	#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */

	    PRINTF("ADC Full Range: %d\r\n", g_Adc16_12bitFullRange);
	    PRINTF("Press any key to get user channel's ADC value ...\r\n");

	    adc16ChannelConfigStruct.channelNumber                        = DEMO_ADC16_USER_CHANNEL;
	    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true; /* Enable the interrupt. */
	#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
	    adc16ChannelConfigStruct.enableDifferentialConversion = false;
	#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */

	    g_Adc16InterruptCounter = 0U;
}

uint16_t irsensor_mesure(void)
{
    ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)))
    {
    }
    ir_sensor_value = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
    PRINTF("ADC Value: %d\r\n", ir_sensor_value);
    return ir_sensor_value;
}


/*
void irsensor_init(void){
    ADC16_GetDefaultConfig(&adc16ConfigStruct);
#ifdef BOARD_ADC_USE_ALT_VREF
    adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
#endif
    ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false);
#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASE))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }
#endif

    PRINTF("ADC Full Range: %d\r\n", g_Adc16_12bitFullRange);
    //PRINTF("Press any key to get user channel's ADC value ...\r\n");

    adc16ChannelConfigStruct.channelNumber                        = DEMO_ADC16_USER_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif
}

uint16_t irsensor_mesure(void)
{
    ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)))
    {
    }
    ir_sensor_value = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
    PRINTF("ADC Value: %d\r\n", ir_sensor_value);
    return ir_sensor_value;
}


float irsensor_convert(uint16_t ir_orig)
{
	float ir_new;
	ir_new = 6 + ((4095.0 - ir_orig)/4095.0) * 27;
	return ir_new;
}

*/

void updateTrackerValues(uint8_t index)
{
	index = index - 1;
	if(ir_trackers[index] == false)
	{
		ir_trackers[index] = true;
		//PRINTF("TRACKER %u TRUE \r\n", index);
		getTrackersValuesCount();
	}
	else
	{
		ir_trackers[index] = false;
		//PRINTF("TRACKER %u FALSE \r\n", index);
		getTrackersValuesCount();
	}
}

void  getTrackersValuesCount (void)
{
	uint8_t cnt = 0;
	for(uint8_t i = 0; i < 6; i++)
	{
		if(ir_trackers[i] == true) cnt++;
	}
	//PRINTF("TRUE COUNT %u \r\n", cnt);

	if(cnt > 2)
	{	led_R();
		motor_set_speed(0);
		steer_straight();
		PRINTF("LINE DETECTED \r\n");
		startMotorsButtonPressed = false;
		driving = false;
	}

	//return cnt;
}



