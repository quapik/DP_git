#include "sensors_control.h"

unsigned long counter = 0;
volatile uint32_t overflowCount = 0;

//SENZORY BARVY
volatile bool tpmIsrFlag0 = false;
volatile bool tpmIsrFlag1 = false;
volatile uint32_t rising0 = 0;
volatile uint32_t falling0 = 0;
volatile uint32_t rising1 = 0;
volatile uint32_t falling1 = 0;
volatile bool rising0_detected = false;
volatile bool rising1_detected = false;

//SFR senzor
volatile uint32_t start_time = 0;
volatile uint32_t end_time = 0;
volatile uint32_t pulse_width_us = 0;
volatile bool SRF_pulse_measured = false;
volatile bool tpmIsrFlag = false;
volatile uint32_t risingEdgeTime = 0;
volatile uint32_t fallingEdgeTime = 0;
volatile bool firstEdgeCaptured = false;

//IR SENSOR
const uint32_t g_Adc16_12bitFullRange = 4096U;
uint16_t ir_sensor_value;
adc16_config_t adc16ConfigStruct;
adc16_channel_config_t adc16ChannelConfigStruct;

void TriggerPulse(void)
{
	PRINTF("SNED PULSE4 \r\n");
	GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger_GPIO, BOARD_INITPINS_SRF05_trigger_PIN, 1);
    SDK_DelayAtLeastUs(10U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
	//SysTick_DelayTicks(10U);
    GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger_GPIO, BOARD_INITPINS_SRF05_trigger_PIN, 0);
}

void GetSRF5_distacne(void)
{
	uint32_t pulseWidth = 0;
    pulseWidth = (fallingEdgeTime >= risingEdgeTime) ?
                     (fallingEdgeTime - risingEdgeTime + (overflowCount * 0xFFFF)) :
                     ((0xFFFF - risingEdgeTime) + fallingEdgeTime + (overflowCount * 0xFFFF));

        uint32_t pulseTime_us = pulseWidth * (1e6 / TPM_SOURCE_CLOCK);
        uint32_t distance = (pulseWidth* 0.02083 * 0.034) /  2;
        float test_distance = ((pulseWidth* 0.02083) * 0.34) /  200;
        PRINTF("US %u Distance = %u cm\r\n",pulseTime_us, distance);}

void color_sensors_init(void)
{
    CLOCK_SetTpmClock(1U);
    tpm_config_t tpm0Info;
    TPM_GetDefaultConfig(&tpm0Info);
    tpm0Info.prescale = kTPM_Prescale_Divide_1;
    TPM_Init(COLOR_SENSORS_TMP, &tpm0Info);

    //Defaulte cekame na prvni rising edge (pak se to preklopi na falling a zase na rising)
    //TPM_SetupInputCapture(COLOR_SENSORS_TMP, COLOR_SENSOR0_TPM_INPUT_CAPTURE_CHANNEL, kTPM_RisingEdge);
   // TPM_SetupInputCapture(COLOR_SENSORS_TMP, COLOR_SENSOR1_TPM_INPUT_CAPTURE_CHANNEL, kTPM_RisingEdge);
    TPM_SetupInputCapture(COLOR_SENSORS_TMP, SRF05_TMP_TPM_INPUT_CAPTURE_CHANNEL, kTPM_RisingEdge);

    //nastaveni counter (do kolika pocita, pak pretece, 65555, vice nejde, 16bit)
    COLOR_SENSORS_TMP->MOD = 0xFFFF;
    //TPM_EnableInterrupts(COLOR_SENSORS_TMP, COLOR_SENSOR1_TPM_CHANNEL_INTERRUPT_ENABLE);
    //TPM_EnableInterrupts(COLOR_SENSORS_TMP, COLOR_SENSOR0_TPM_CHANNEL_INTERRUPT_ENABLE);
    TPM_EnableInterrupts(COLOR_SENSORS_TMP, SRF05_TMP_TPM_CHANNEL_INTERRUPT_ENABLE | kTPM_TimeOverflowInterruptEnable);


    EnableIRQ(COLOR_SENSORS_TPM_INTERRUPT_NUMBER);

    TPM_StartTimer(COLOR_SENSORS_TMP, kTPM_SystemClock);
}

//Zachytava preruseni na TMP0 (sensory)
void COLOR_SENSORS_TPM_INPUT_CAPTURE_HANDLER(void)
{
    uint32_t status = TPM_GetStatusFlags(COLOR_SENSORS_TMP);
    uint32_t captureVal = COLOR_SENSORS_TMP->CONTROLS[SRF05_TMP_TPM_INPUT_CAPTURE_CHANNEL].CnV;
    PRINTF("status %u\r\n",status);
    //if(status != 256)PRINTF("status %u\r\n",status);
   //PRINTF("status %u\r\n",status & SRF05_TMP_TPM_INPUT_CAPTURE_CHANNEL);

    if (status & kTPM_TimeOverflowFlag)
        {
            overflowCount++;
            TPM_ClearStatusFlags(COLOR_SENSORS_TMP, kTPM_TimeOverflowFlag);
            //PRINTF("OVERFLOW\r\n");
        }

    if (status & SRF05_TMP_TPM_INPUT_CAPTURE_CHANNEL)
       {
       	PRINTF("JSEM KURVA TADY?\r\n");
       	if (!firstEdgeCaptured)
       	        {
       	        	PRINTF("NASTUPNA\r\n");
       	            risingEdgeTime = captureVal;
       	            firstEdgeCaptured = true;
       	            overflowCount = 0;
       	            TPM_SetupInputCapture(COLOR_SENSORS_TMP, SRF05_TMP_TPM_INPUT_CAPTURE_CHANNEL, kTPM_FallingEdge);
       	        }
       	        else
       	        {
       	        	PRINTF("SESTUPNA\r\n");
       	            fallingEdgeTime = captureVal;
       	            SRF_pulse_measured = true;
       	            firstEdgeCaptured = false;
       	            TPM_SetupInputCapture(COLOR_SENSORS_TMP, SRF05_TMP_TPM_INPUT_CAPTURE_CHANNEL, kTPM_RisingEdge);
       	        }

       	        TPM_ClearStatusFlags(COLOR_SENSORS_TMP, SRF05_TMP_TPM_INPUT_CAPTURE_CHANNEL);


       }
    /*
    if (status & COLOR_SENSOR0_TPM_CHANNEL_FLAG)
    {

        if (!rising0_detected)
        {
        	counter++;
            //Ulozeni kdy doslo k nastupne hrane (hodnota tmpcounteru)
        	rising0 = COLOR_SENSORS_TMP->CONTROLS[COLOR_SENSOR0_TPM_INPUT_CAPTURE_CHANNEL].CnV;
        	rising0_detected = true;

            // Přepnout na detekci sestupné hrany
            TPM_SetupInputCapture(COLOR_SENSORS_TMP, COLOR_SENSOR0_TPM_INPUT_CAPTURE_CHANNEL, kTPM_FallingEdge);
        }
        else
        {
            // Zachytit časovou značku pro sestupnou hranu
            falling0 = COLOR_SENSORS_TMP->CONTROLS[COLOR_SENSOR0_TPM_INPUT_CAPTURE_CHANNEL].CnV;
            tpmIsrFlag0 = true; // Signalizace, že obě hrany byly detekovány

            // Přepnout zpět na detekci nástupné hrany
            TPM_SetupInputCapture(COLOR_SENSORS_TMP, COLOR_SENSOR0_TPM_INPUT_CAPTURE_CHANNEL, kTPM_RisingEdge);
        }

        // Vyčistit příznak přerušení
        TPM_ClearStatusFlags(COLOR_SENSORS_TMP, COLOR_SENSOR0_TPM_CHANNEL_FLAG);
    }

    if (status & COLOR_SENSOR1_TPM_CHANNEL_FLAG)
        {
            if (!rising1_detected)
            {
            	counter++;
                //Ulozeni kdy doslo k nastupne hrane (hodnota tmpcounteru)
            	rising1 = COLOR_SENSORS_TMP->CONTROLS[COLOR_SENSOR1_TPM_INPUT_CAPTURE_CHANNEL].CnV;
            	rising1_detected = true;

                // Přepnout na detekci sestupné hrany
                TPM_SetupInputCapture(COLOR_SENSORS_TMP, COLOR_SENSOR1_TPM_INPUT_CAPTURE_CHANNEL, kTPM_FallingEdge);
            }
            else
            {
                // Zachytit časovou značku pro sestupnou hranu
                falling1 = COLOR_SENSORS_TMP->CONTROLS[COLOR_SENSOR1_TPM_INPUT_CAPTURE_CHANNEL].CnV;
                tpmIsrFlag1 = true; // Signalizace, že obě hrany byly detekovány

                // Přepnout zpět na detekci nástupné hrany
                TPM_SetupInputCapture(COLOR_SENSORS_TMP, COLOR_SENSOR1_TPM_INPUT_CAPTURE_CHANNEL, kTPM_RisingEdge);
            }

            // Vyčistit příznak přerušení
            TPM_ClearStatusFlags(COLOR_SENSORS_TMP, COLOR_SENSOR1_TPM_CHANNEL_FLAG);
        }
        */

    __DSB();
}

void check_colors_sensors_interrupts(void)
{
	PRINTF("JSEMasda TU\r\n");
	 if (tpmIsrFlag0)
	        {
	            tpmIsrFlag0 = false;

	            // Vypočítat délku impulzu
	            uint32_t pulse0;
	            if (falling0 >= rising0)
	            {
	            	pulse0 = falling0 - rising0;
	            }
	            else
	            {
	                // Přetečení časovače
	            	pulse0 = (65535 - rising0) + falling0 + 1;
	            }
	            PRINTF("COLOR 0 PW %u ", pulse0);
	            if(pulse0 > 35000) PRINTF("BILA\r\n");
	            else if (pulse0 < 33000) PRINTF("CERNA\r\n");
	            else PRINTF("JINA\r\n");

	            rising0_detected = false;
	        }
	 if (tpmIsrFlag1)
	        {
	            tpmIsrFlag1 = false;

	            // Vypočítat délku impulzu
	            uint32_t pulse1;
	            if (falling1 >= rising1)
	            {
	            	pulse1 = falling1 - rising1;
	            }
	            else
	            {
	                // Přetečení časovače
	            	pulse1 = (65535 - rising1) + falling1 + 1;
	            }
	            PRINTF("COLOR 1 PW %u ", pulse1);
	            if(pulse1 > 35000) PRINTF("BILA\r\n");
	            else if (pulse1 < 33000) PRINTF("CERNA\r\n");
	            else PRINTF("JINA\r\n");

	            rising1_detected = false;
	        }
}

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
    //PRINTF("Press any key to get user channel's ADC value ...\r\n");

    adc16ChannelConfigStruct.channelNumber                        = DEMO_ADC16_USER_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
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
