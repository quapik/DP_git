#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"
#include <stdio.h>
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_TPM_BASEADDR TPM2
#define BOARD_TPM_CHANNEL  0U

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl0Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM2_IRQn
#define TPM_SERVO_HANDLER    TPM2_IRQHandler

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgIrc48MClk)

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t getCharValue = 0U;
volatile uint8_t angle = 0U; //Uhel natoceni serva - rozsah 0 - 180 stupnu
float dutyCycle = 0.0;
volatile uint32_t g_systickCounter;

/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}
int main(void)
{
    tpm_config_t tpmInfo;
    tpm_chnl_pwm_signal_param_t tpmParam;

#ifndef TPM_SERVO_ON_LEVEL
#define TPM_SERVO_ON_LEVEL kTPM_HighTrue
#endif


    /* Configure tpm params with frequency 50Hz for servo control */
    tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL;
    tpmParam.level            = TPM_SERVO_ON_LEVEL;
    tpmParam.dutyCyclePercent = 7.5;
    /*0
     Nastaveni serva do uvodni polohy - uhel 0 ma
      */

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Select the clock source for the TPM counter as MCGPLLCLK */
    CLOCK_SetTpmClock(1U);

    /* Initialize TPM module */
    TPM_GetDefaultConfig(&tpmInfo);
    tpmInfo.prescale = kTPM_Prescale_Divide_32;
    TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

    /* Set up PWM with 50Hz frequency */
    TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_EdgeAlignedPwm, 50U, TPM_SOURCE_CLOCK);
    TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
    angle = 0;
    PRINTF("APP START\r\n");
    /* Set systick reload value to generate 1ms interrupt */
	if (SysTick_Config(SystemCoreClock / 1000U))
	{
		while (1)
		{
		}
	}

	// INICIALIZACE MOTORU
	PRINTF("SET MAX\r\n");
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_EdgeAlignedPwm, 13.0);
	SysTick_DelayTicks(2000U);
	PRINTF("SET MIN\r\n");
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_EdgeAlignedPwm, 6.5);
	SysTick_DelayTicks(2000U);
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_EdgeAlignedPwm, 7.5);
	SysTick_DelayTicks(2000U);
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_EdgeAlignedPwm, 6.5);


	while (1)
    {
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

    	/*
    	//SERVO MOTOR RIZENI!!
    	getCharValue = GETCHAR();
    	angle = angle + 1;


    	if(angle>180) angle = 0;


        dutyCycle = 2.5 + (angle / 180.0) * 10.0; // Duty cycle pro úhel
        TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_EdgeAlignedPwm, dutyCycle);

        PRINTF("Servo angle set to %d degrees!\r\n", angle);
        */


    	/*
    	//MOTORY
    	dutyCycle = 13.0;
    	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_EdgeAlignedPwm, dutyCycle);
    	PRINTF("Nastavena max hodnota\r\n");
    	getCharValue = GETCHAR();
    	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_EdgeAlignedPwm, dutyCycle);
    	PRINTF("Nastavena min hodnota\r\n");
    	while(1)
    	{
    		getCharValue = GETCHAR() - 0x30U;
    		if (getCharValue == 0) dutyCycle = 6.5;
    		if (getCharValue == 9) dutyCycle = 12.9;
    		if (getCharValue == 1) dutyCycle = dutyCycle - 0.10;
    		if (getCharValue == 2) dutyCycle = dutyCycle + 0.10;
    		TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_EdgeAlignedPwm, dutyCycle);
    	}
    	*/


    }
}
