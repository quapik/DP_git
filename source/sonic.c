/*
 * sonic.c
 *
 *  Created on: 14. 2. 2025
 *      Author: xsimav01
 */
/*
#include "sonic.h"

volatile bool tpmIsrFlag = false;
volatile uint32_t risingEdgeTime = 0;
volatile uint32_t fallingEdgeTime = 0;
volatile bool firstEdgeCaptured = false;
volatile uint32_t overflowCount = 0;
uint32_t counter = 0;
float test_distance = 0.0;

tpm_config_t tpmInfo;
uint32_t pulseWidth = 0;

void tmp0_init(void)
{

    CLOCK_SetTpmClock(1U);
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

}

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

