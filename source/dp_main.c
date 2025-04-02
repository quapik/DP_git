#include "globals.h"


uint16_t dc = 0;
uint16_t pct = 0;

uint8_t g_tipString[] =
    "Uart functional API non-blocking example\r\nBoard sends characters\r\n";


void sendTextCharByChar(UART_Type *base, const char *text)
{
    size_t len = strlen(text);
    for (size_t i = 0; i < len; i++)
    {
        /* Odeslání jednoho znaku, velikost 1 bajt */
        UART_WriteBlocking(base, (const uint8_t *)&text[i], 1);
    }
}
void sendMessage(UART_Type *base, const char *message)
{
    UART_WriteBlocking(base, (uint8_t *)message, strlen(message));
}


int main(void)
{
	BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("NXP CUP & VOJTECH SIMA MASTER DEGREE THESIS APP, WELCOME \r\n");


    SysTick_Init();
    HallResetValues();
    enableInterruptsOnPorts();
    motors_init();



    //COLOR SENSORY A  ULTRASONIC
   // tmp0_init();

    PIT_Timer_Init();
    //LPTMR_Timer_Init();
   // PIT_timer1_start();
    //PIT_timer0_start();
    //irsensor_init();


   PixyStart();

    /*
    uart_comm_init();
    const char *myText = "Tento text bude odeslan znak po znaku";
    */

        /* Send g_tipString out non-blocking */
        //ART_WriteBlocking(UART2, g_tipString, sizeof(g_tipString) / sizeof(g_tipString[0]));
    dutyCycle = SERVO_MIDDLE;
 	while (1){

 		/*
 		sendMessage(UART2, myText);
 	    SDK_DelayAtLeastUs(1000U*1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
 	    */

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
		if (getCharValue == 3) dutyCycle = SERVO_MIDDLE;
		if (getCharValue == 1) dutyCycle = dutyCycle - 0.10;
		if (getCharValue == 2) dutyCycle = dutyCycle + 0.10;
		TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, dutyCycle);
		if (dutyCycle > SERVO_MAX) dutyCycle  = SERVO_MAX;
		if (dutyCycle < SERVO_MIN) dutyCycle  = SERVO_MIN;

		if (getCharValue == 6 ) pct = 0;
		if (getCharValue == 9 ) pct = 100;
		if (getCharValue == 4)  pct = pct -1;
		if (getCharValue == 5) pct = pct +1;
		if(getCharValue == 6 || getCharValue == 5 || getCharValue == 4 || getCharValue == 9)
		{
			PRINTF("MOTOR %d \r\n", (int)(pct));
			motor_set_speed(pct);
		}
		else
		{
			PRINTF("SERVO %d \r\n", (int)(dutyCycle*100));
		}
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
