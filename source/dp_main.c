#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"
#include <stdio.h>
#include "clock_config.h"

#include "delay.h"
#include "drive_control.h"
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

    /* Select the clock source for the TPM counter as MCGPLLCLK */


    /* Set up PWM with 50Hz frequency */

    int angle = 0;
    PRINTF("APP START\r\n");
    //Inicializace pro DELAY!!!

    SysTick_Init();
    motors_init();


    dutyCycle = 0.0;
	while (1){
		/*
			for(int i = 0; i < 10; i++)
			{
				motor_set_speed(1);
						SysTick_DelayTicks(250U);
						motor_set_speed(0);
						SysTick_DelayTicks(250U);
			}

			SysTick_DelayTicks(1550U);
		*/
    	/*
        do
        {
            PRINTF("\r\nEnter an angle between 0 and 180:\r\n");
            PRINTF("Angle: ");
            getCharValue = GETCHAR() - 0x30U;
            angle = getCharValue * 10; // Přepočítá vstup na 10x hodnotu
            PRINTF("%d", angle);
            PRINTF("\r\n");
        } while (angle > 180U);
		*/


    	//SERVO MOTOR RIZENI!!
		/*
    	getCharValue = GETCHAR();
    	angle = angle + 1;


    	if(angle>180) angle = 0;


        dutyCycle = 3.0 + (angle / 180.0) * 10.0; // Duty cycle pro úhel
        TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, dutyCycle);

        PRINTF("Servo angle set to %d degrees!\r\n", angle);
        */

		/*
		getCharValue = GETCHAR()  - 0x30U;

		if (getCharValue == 1) dutyCycle = dutyCycle - 0.10;
		if (getCharValue == 2) dutyCycle = dutyCycle + 0.10;
		TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_SERVO, (tpm_chnl_t)BOARD_TPM_CHANNEL_SERVO, kTPM_EdgeAlignedPwm, dutyCycle);
		if (dutyCycle > 50.0) dutyCycle  = 0.0;
		*/


    	// LEVY TADBU 6.5 a 12.9 max

    	while(1)
    	{
    		getCharValue = GETCHAR() - 0x30U;
    		if (getCharValue == 0) dutyCycle = 5.9;
    		if (getCharValue == 9) dutyCycle = 12.9;
    		if (getCharValue == 1) dutyCycle = dutyCycle - 0.02;
    		if (getCharValue == 2) dutyCycle = dutyCycle + 0.02;
    		TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_MOTOR, (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR, kTPM_CenterAlignedPwm, dutyCycle);
    	}


    }
}
