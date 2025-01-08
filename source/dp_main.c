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
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/
uint16_t dc = 0;
int main(void)
{


    /* Board pin, clock, debug console init */
	BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    color_sensors_init();






    PRINTF("APP START\r\n");


    SysTick_Init();
    motors_init();
    irsensor_init();

    dutyCycle = MOTOR_MAX;

    uint16_t adc_value;
    float cm_value;




	while (1){


		check_colors_sensors_interrupts();

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
    		if (getCharValue == 0) dutyCycle = MOTOR_MIN;
    		if (getCharValue == 9) dutyCycle = MOTOR_MAX;
    		if (getCharValue == 1) dutyCycle = dutyCycle - 0.02;
    		if (getCharValue == 2) dutyCycle = dutyCycle + 0.02;
    		TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_MOTOR, (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR, kTPM_CenterAlignedPwm, dutyCycle);
    	}
	*/

    }
}
