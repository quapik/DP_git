#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"
#include <stdio.h>
#include "clock_config.h"
#include "fsl_adc16.h"

#include "delay.h"
#include "drive_control.h"
#include "sensors_control.h"
#include "pixySPI.h"


uint16_t dc = 0;


int main(void)
{


    /* Board pin, clock, debug console init */
	BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    //color_sensors_init();






    PRINTF("APP START\r\n");
    SysTick_Init();
    //GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger_GPIO, BOARD_INITPINS_SRF05_trigger_PIN, 0);
    SysTick_DelayTicks(100U);
    motors_init();
    //irsensor_init();

    /*
    PixyInit();
    PixySetLamp(1,1);
    SysTick_DelayTicks(1000U*1000);
    PixySetServos(0,400);
    SysTick_DelayTicks(1000U*1000);
    PixySetServos(0,160);
    SysTick_DelayTicks(1000U*1000);
    PixySetLamp(0,0);
    */
    dutyCycle = MOTOR_MIN;

    uint16_t adc_value;
    float cm_value;

    bool sendpulse = true;
    int test = 0;


	while (1){


		/*
		if(sendpulse) {
			sendpulse = false;
			//PRINTF("ON\n");
				GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger_GPIO, BOARD_INITPINS_SRF05_trigger_PIN, 1);
				SysTick_DelayTicks(10U);
				//PRINTF("OF\n");
				GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger_GPIO, BOARD_INITPINS_SRF05_trigger_PIN, 0);
		}*/


		/*
		 if (pulse_measured)
		        {
		            // Zpracujte změřený impulz
		            PRINTF("Pulse width: %u us\r\n", pulse_width_us);

		            // Resetujte příznak
		            pulse_measured = false;
		            sendpulse = true;
		        }
		*/
		//check_colors_sensors_interrupts();

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

    	while(1)
    	{
    		getCharValue = GETCHAR() - 0x30U;
    		if (getCharValue == 0) dutyCycle = MOTOR_MIN;
    		if (getCharValue == 9) dutyCycle = MOTOR_MAX;
    		if (getCharValue == 1) dutyCycle = dutyCycle - 0.02;
    		if (getCharValue == 2) dutyCycle = dutyCycle + 0.02;
    		PRINTF("%d \r\n", (int)(dutyCycle*1000));
    		TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_MOTOR, (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR, kTPM_CenterAlignedPwm, dutyCycle);
    	}


    }
}
