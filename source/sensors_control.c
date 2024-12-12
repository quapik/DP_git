#include "sensors_control.h"
const uint32_t g_Adc16_12bitFullRange = 4096U;
uint16_t ir_sensor_value;

adc16_config_t adc16ConfigStruct;
adc16_channel_config_t adc16ChannelConfigStruct;

void irsensor_init(void){
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
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
}


uint16_t irsensor_mesure(void)
{
	//GETCHAR();
    /*
     When in software trigger mode, each conversion would be launched once calling the "ADC16_ChannelConfigure()"
     function, which works like writing a conversion command and executing it. For another channel's conversion,
     just to change the "channelNumber" field in channel's configuration structure, and call the
     "ADC16_ChannelConfigure() again.
    */
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
	ir_new = 6 + ((3333 - ir_orig)/3333) * 27;
	return ir_new;
}
