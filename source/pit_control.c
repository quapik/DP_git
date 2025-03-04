#include "pit_control.h"

#define PIT_BASEADDR PIT
#define PIT_IRQ_HANDLER   PIT_IRQHandler
#define PIT_IRQ_ID        PIT_IRQn
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)


volatile bool PIT_timer1_finished = false;
volatile bool PIT_timer0_finished = false;

void PIT_IRQ_HANDLER(void)
{
	//PIT TIMER CHANNEL 2 - kartky timeout
	if((PIT_GetStatusFlags(PIT_BASEADDR, kPIT_Chnl_0)) == 1)
	{
		//PRINTF("\r\n Channel No.0 interrupt is occurred !");
		PIT_ClearStatusFlags(PIT_BASEADDR, kPIT_Chnl_0, kPIT_TimerFlag);
		PIT_StopTimer(PIT_BASEADDR, kPIT_Chnl_0);
		PIT_timer0_finished = true;
		if(isTriggerTriggering)
		{
			if(actualTrigger == 1)
				{
					GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger1_GPIO, BOARD_INITPINS_SRF05_trigger1_PIN, 0);

				}
				else
				{
					GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger2_GPIO, BOARD_INITPINS_SRF05_trigger2_PIN, 0);
				}
			isTriggerTriggering = false;
		}
	}

	//PIT TIMER CHANNEL 2 - delsi timeout
	if((PIT_GetStatusFlags(PIT_BASEADDR, kPIT_Chnl_1)) == 1)
	{
	//PRINTF("\r\n Channel No.1 interrupt is occurred !");
	PIT_ClearStatusFlags(PIT_BASEADDR, kPIT_Chnl_1, kPIT_TimerFlag);
	PIT_StopTimer(PIT_BASEADDR, kPIT_Chnl_1);
	PIT_timer1_finished = true;
	probihaZmena = false;
	steer_straight();

	}

    __DSB();
}

void PIT_timer0_start(void)
{
	PIT_StartTimer(PIT_BASEADDR, kPIT_Chnl_0);
	PIT_timer0_finished = false;

}


void PIT_timer1_start(void)
{
	PIT_StartTimer(PIT_BASEADDR, kPIT_Chnl_1);
	PIT_timer1_finished = false;
	probihaZmena = true;
	steer_straight();

}

//Funkce na inicializaci timeru
void PIT_Timer_Init(void)
{

	pit_config_t pitConfig;
	PIT_GetDefaultConfig(&pitConfig);
	PIT_Init(PIT_BASEADDR, &pitConfig);


	PIT_SetTimerPeriod(PIT_BASEADDR, kPIT_Chnl_0, USEC_TO_COUNT(10, PIT_SOURCE_CLOCK));
	PIT_SetTimerPeriod(PIT_BASEADDR, kPIT_Chnl_1, USEC_TO_COUNT(500000, PIT_SOURCE_CLOCK));


	PIT_EnableInterrupts(PIT_BASEADDR, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	PIT_EnableInterrupts(PIT_BASEADDR, kPIT_Chnl_1, kPIT_TimerInterruptEnable);


	EnableIRQ(PIT_IRQ_ID);


	//Nezapinat timery aby to nebezelo periodicky
	//PIT_StartTimer(PIT_BASEADDR, kPIT_Chnl_0);
	//PIT_StartTimer(PIT_BASEADDR, kPIT_Chnl_1);
	PRINTF("PIT TIMERS inits finished\r\n");
}

