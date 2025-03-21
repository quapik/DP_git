#include "globals.h"


uint16_t dc = 0;
uint16_t pct = 0;

uint8_t g_tipString[] =
    "Uart functional API non-blocking example\r\nBoard sends characters\r\n";



int main(void)
{

	BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("APP START\r\n");


    SysTick_Init();

    enableInterruptsOnPorts();
    motors_init();

    PIT_Timer_Init();
    tmp0_init();
    LPTMR_Timer_Init();
    PIT_timer1_start();
    //irsensor_init();
    PixyStart();
    //uart_comm_init();

        /* Send g_tipString out non-blocking */
        //ART_WriteBlocking(UART2, g_tipString, sizeof(g_tipString) / sizeof(g_tipString[0]));

 	while (1){

 	 	//SDK_DelayAtLeastUs(1000*100, 4800000);
 	    // UART_WriteBlocking(UART2, "a", 1);

		//irsensor_mesure();



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
