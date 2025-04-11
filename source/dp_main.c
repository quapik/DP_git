#include "globals.h"
volatile uint8_t getCharValue = 0U;

uint16_t dc = 0;
uint16_t pct = 0;

bool pixyMainFeatures = false;


int main(void)
{
	logujJenomVektory = false;
	BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("NXP CUP & VOJTECH SIMA MASTER DEGREE THESIS APP, WELCOME \r\n");


    HallResetValues();
    enableInterruptsOnPorts();
    motors_init();



    //COLOR SENSORY A  ULTRASONIC
    tmp0_init();
    UART2_Init();

    PIT_Timer_Init();
    LPTMR_Timer_Init();
    HallResetValues();
    irsensor_init();

    //LPTMR_StartPosilejUART();
    PixyStart();
    PIT_StartZpracujBarvuIRSensor();

    dutyCycle = SERVO_MIDDLE;
    PRINTF("Vsechny init funkce dokonceny\r\n");
    led_R();
 	while (1){



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
 		/*
    	// LEVY TADBU 6.5 a 12.9 max
    	while(1)
    	{
    		getCharValue = GETCHAR() - 0x30U;
    		if (getCharValue == 0) {pct = 0; LPTMR_timer_stop(); HallResetValues();}
    		if (getCharValue == 9) {LPTMR_timer_start(); pct = 100;}
    		if (getCharValue == 1) pct = pct -1;
    		if (getCharValue == 2) pct = pct +1;
    		PRINTF("%d \r\n", (int)(pct));
    		motor_set_speed(pct);
    	}
    	*/

    }
}
