#include "globals.h"



uint16_t dc = 0;
uint16_t pct = 0;

int main(void)
{
	BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("APP START\r\n");

    enableInterruptsOnPorts();

    SysTick_Init();



    motors_init();

    tmp0_init();

    irsensor_init();



/*
    PixyInit();
    PixySetLamp(1,1);
    PixySetServos(0, 400);
    PixySetLamp(0,0);
*/

    //


    uint16_t test = 0;

	while (1){

		test = irsensor_mesure();
		test = irsensor_convert(test);

		//PixyGetVectors();
		//SDK_DelayAtLeastUs(100000, 48000000);

		/*
		//IR SENSOR
		adc_value  = irsensor_mesure();
		if (adc_value > 1000)
		{
		PRINTF("STOP\r\n");
		motor_set_speed(0);
		}
		else motor_set_speed(15);

		cm_value = irsensor_convert(adc_value);
		SysTick_DelayTicks(20U);
		*/



/*
		getCharValue = GETCHAR()  - 0x30U;

		if (getCharValue == 1) dutyCycle = dutyCycle - 0.10;
		if (getCharValue == 2) dutyCycle = dutyCycle + 0.10;
		TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, dutyCycle);
		if (dutyCycle > SERVO_MAX) dutyCycle  = SERVO_MAX;
		if (dutyCycle < SERVO_MIN) dutyCycle  = SERVO_MIN;
*/


    	// LEVY TADBU 6.5 a 12.9 max
/*
    	while(1)
    	{
    		getCharValue = GETCHAR() - 0x30U;
    		if (getCharValue == 0) pct = 0;
    		if (getCharValue == 9) pct = 100;
    		if (getCharValue == 1) pct = pct -1;
    		if (getCharValue == 2) pct = pct +1;
    		PRINTF("%d \r\n", (int)(pct));
    		motor_set_speed(pct);
    	}
*/

    }
}
