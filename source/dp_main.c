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

#include "sonic.h"



uint16_t dc = 0;
uint16_t pct = 0;

/*
void TriggerPulse(void)
{
	PRINTF("SNED PULSE \r\n");
	GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger_GPIO, BOARD_INITPINS_SRF05_trigger_PIN, 1);
    SDK_DelayAtLeastUs(10U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
	//SysTick_DelayTicks(10U);
    GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger_GPIO, BOARD_INITPINS_SRF05_trigger_PIN, 0);
}
void TPM_INPUT_CAPTURE_HANDLER(void)
{
	counter++;
    uint32_t status = TPM_GetStatusFlags(DEMO_TPM_BASEADDR);
    uint32_t captureVal = DEMO_TPM_BASEADDR->CONTROLS[BOARD_TPM_INPUT_CAPTURE_CHANNEL].CnV;
    //PRINTF("status %u\r\n",status);
    if (status & kTPM_TimeOverflowFlag)
    {
        overflowCount++;
        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, kTPM_TimeOverflowFlag);
        //PRINTF("OVERFLOW\r\n");
    }

    if (status & TPM_CHANNEL_FLAG)
    {
        if (!firstEdgeCaptured)
        {
        	PRINTF("NASTUPNA\r\n");
            risingEdgeTime = captureVal;
            firstEdgeCaptured = true;
            overflowCount = 0;
            TPM_SetupInputCapture(DEMO_TPM_BASEADDR, BOARD_TPM_INPUT_CAPTURE_CHANNEL, kTPM_FallingEdge);
        }
        else
        {
        	PRINTF("SESTUPNA\r\n");
            fallingEdgeTime = captureVal;
            tpmIsrFlag = true;
            firstEdgeCaptured = false;
            TPM_SetupInputCapture(DEMO_TPM_BASEADDR, BOARD_TPM_INPUT_CAPTURE_CHANNEL, kTPM_RisingEdge);
        }

        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, TPM_CHANNEL_FLAG);
    }

    __DSB();
}
*/

int main(void)
{
	BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();



    PRINTF("APP START\r\n");
    SysTick_Init();
    tmp0_init();
    motors_init();

    /*
    PixyInit();
    PixySetLamp(1,1);
    SysTick_DelayTicks(1000U*100);
    PixySetServos(0, 400);

    */
    //irsensor_init();

    dutyCycle = MOTOR_MIN   ;

    uint16_t adc_value;
    float cm_value;



    bool sendpulse = true;
    int test = 0;
    TriggerPulse1();
    TriggerPulse2();

	while (1){

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
