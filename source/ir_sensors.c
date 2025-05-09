/*
 * 	ir_sensors.c
 *
 * Soubor starající se o obsluhu IR senzor (vzdálenosti a trackeru)
 *
 *  Created on: 14. 1. 2025
 *  Author: Vojtěch Šíma
 *  Diplomová práce  Samořiditelný model autíčka pro NXP Cup
 *  2024/2025
 */

#include <ir_sensors.h>


uint8_t pocitadlo = 0;
uint16_t IR_sensor_sum;
uint16_t IR_sensor_avg;

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
void irSensorConvert(uint32_t ir_orig)
{
	IR_sensor_cm = 6 + ((4095.0 - ir_orig)/4095.0) * 26;
}

void irSensorInit(void)
{

	EnableIRQ(IR_ADC_IRQ);
	ADC16_GetDefaultConfig(&IR_ADC_config_struct);
	#ifdef BOARD_ADC_USE_ALT_VREF
	    IR_ADC_config_struct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
	#endif
	    ADC16_Init(IR_ADC_BASE, &IR_ADC_config_struct);
	    ADC16_EnableHardwareTrigger(IR_ADC_BASE, false);
	#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION


	#endif

	    IR_ADC_channel_config_struct.channelNumber                        = IR_ADC_CHANNEL;
	    IR_ADC_channel_config_struct.enableInterruptOnConversionCompleted = true;
	#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
	    IR_ADC_channel_config_struct.enableDifferentialConversion = false;
	#endif
	ADC16_SetChannelConfig(IR_ADC_BASE, IR_ADC_CHANNEL_GROUP, &IR_ADC_channel_config_struct);
}

//Funkce, kdy pokud bylo dokončeno měření, je hodnota převedena na CM a je opět zavoláno měření
void irSensorCheck(void)
{

	if(IR_mesure_done)
		{
		if(driving && dokoncenoKolo)
			{
			if(pocitadlo < 5)
			{

				pocitadlo++;
				IR_sensor_sum = IR_sensor_sum + IR_sensor_raw;

			}
			else
			{
				IR_sensor_avg = IR_sensor_sum / pocitadlo;
				/*
				if(IR_sensor_avg >2300)
				{
					PRINTF("ir sensor stop %d\r\n",IR_sensor_avg);
					UART2_SendTextToHC05("IRV");
					StopAll();
					led_C();
				}
				*/
				pocitadlo = 0;
				IR_sensor_sum = 0;
			}

			}

			irSensorConvert(IR_sensor_raw);
			IR_mesure_done = false;
			ADC16_SetChannelConfig(IR_ADC_BASE, IR_ADC_CHANNEL_GROUP, &IR_ADC_channel_config_struct);


			//PRINTF("ADC Value: %d and CM value %d\r\n", IR_sensor_raw,IR_sensor_cm);
		}

}

void updateTrackerValues(uint8_t index)
{
	index = index - 1;
	if(ir_trackers[index] == false)
	{

		ir_trackers[index] = true;
		//PRINTF("TRACKER %u TRUE \r\n", index+1);
		//getTrackersValuesCount();
		//Pokud jedeme a jeste jsme nedokoncili prvni kolo (pak uz pouze na kameru)
		if(driving && !dokoncenoKolo)
		{
			if((index)==0||(index)==1) //index 2 je pokaženej takže vynechavame
			{
				//UART2_SendTextToHC05("IRC");
				SteerLeft(75);
			}
			if((index)==3||(index)==4 ||(index)==5)
			{
				//UART2_SendTextToHC05("IRC");;
				SteerRight(75);
			}

		}


	}
	else
	{
		ir_trackers[index] = false;
		//PRINTF("TRACKER %u FALSE \r\n", index);
		//getTrackersValuesCount();
	}

}
//funkce co zjistí kolik IR senzorů je v logické 1 (pro detekci cílové čáry, nakonec nevyužitp)
void getTrackersValuesCount (void)
{
	uint8_t cnt = 0;
	for(uint8_t i = 0; i < 6; i++)
	{
		if(ir_trackers[i] == true) cnt++;
	}
	//Zastavi pokud prejede 2 cary (aspon 2 true hodnty), problematicke v pripade kdy se jen vyjede z drahy (senzory musi byt osazeny rovne)
	if(cnt > 2 && driving)
	{
		led_R();
		MotorSetSpeed(0);
		SteerStraight();
		PRINTF("LINE DETECTED \r\n");
		startMotorsButtonPressed = false;
		driving = false;
	}

}



