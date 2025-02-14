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

#define DEMO_TPM_BASEADDR TPM0
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL kTPM_Chnl_5
#define TPM_INTERRUPT_NUMBER      TPM0_IRQn
#define TPM_INPUT_CAPTURE_HANDLER TPM0_IRQHandler
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl5InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl5Flag
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgIrc48MClk)

volatile bool tpmIsrFlag = false;
volatile uint32_t risingEdgeTime = 0;
volatile uint32_t fallingEdgeTime = 0;
volatile bool firstEdgeCaptured = false;
volatile uint32_t overflowCount = 0;
uint32_t counter = 0;
float test_distance = 0.0;

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


int main(void)
{
	BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();



    PRINTF("APP START\r\n");
    SysTick_Init();

    CLOCK_SetTpmClock(1U);
    tpm_config_t tpmInfo;
      uint32_t pulseWidth = 0;


    GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger_GPIO, BOARD_INITPINS_SRF05_trigger_PIN, 0);
    TPM_GetDefaultConfig(&tpmInfo);
        //tpmInfo.prescale = kTPM_Prescale_Divide_16;
    TPM_Init(DEMO_TPM_BASEADDR, &tpmInfo);

    TPM_SetupInputCapture(DEMO_TPM_BASEADDR, BOARD_TPM_INPUT_CAPTURE_CHANNEL, kTPM_RisingEdge);
	DEMO_TPM_BASEADDR->MOD = 0xFFFF;
	TPM_EnableInterrupts(DEMO_TPM_BASEADDR, TPM_CHANNEL_INTERRUPT_ENABLE | kTPM_TimeOverflowInterruptEnable);
	EnableIRQ(TPM_INTERRUPT_NUMBER);
	//NVIC_SetPriority(TPM_INTERRUPT_NUMBER, 2);
	TPM_StartTimer(DEMO_TPM_BASEADDR, kTPM_SystemClock);



    //motors_init();

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


	while (1){

	     TriggerPulse();
	        while (!tpmIsrFlag);
	        tpmIsrFlag = false;

	        pulseWidth = (fallingEdgeTime >= risingEdgeTime) ?
	                     (fallingEdgeTime - risingEdgeTime + (overflowCount * 0xFFFF)) :
	                     ((0xFFFF - risingEdgeTime) + fallingEdgeTime + (overflowCount * 0xFFFF));

	        uint32_t pulseTime_us = pulseWidth * (1e6 / TPM_SOURCE_CLOCK);
	        uint32_t distance = (pulseWidth* 0.02083 * 0.034) /  2;
	        test_distance = ((pulseWidth* 0.02083) * 0.34) /  200;
	        PRINTF("US %u Distance = %u cm\r\n",pulseTime_us, distance);


	        SDK_DelayAtLeastUs(100000U, CLOCK_GetFreq(kCLOCK_CoreSysClk));

		//PixyGetVectors();
		//SysTick_DelayTicks(1000*500);
		//motor_set_speed(0);

		 //SDK_DelayAtLeastUs(1000000U, CLOCK_GetFreq(kCLOCK_CoreSysClk));

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
/*
    	while(1)
    	{
    		getCharValue = GETCHAR() - 0x30U;
    		if (getCharValue == 0) dutyCycle = MOTOR_MIN;
    		if (getCharValue == 9) dutyCycle = MOTOR_MAX;
    		if (getCharValue == 1) dutyCycle = dutyCycle - 0.02;
    		if (getCharValue == 2) dutyCycle = dutyCycle + 0.02;
    		PRINTF("%d \r\n", (int)(dutyCycle*1000));
    		TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_MOTOR, (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR0, kTPM_CenterAlignedPwm, dutyCycle);
    		TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR_MOTOR, (tpm_chnl_t)BOARD_TPM_CHANNEL_MOTOR1, kTPM_CenterAlignedPwm, dutyCycle);
    	}
*/

    }
}
