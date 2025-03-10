#include <timers.h>

#define PIT_BASEADDR PIT
#define PIT_IRQ_HANDLER   PIT_IRQHandler
#define PIT_IRQ_ID        PIT_IRQn
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)

#define LPTMR_BASE   LPTMR0
#define LPTMR_IRQn   LPTMR0_IRQn
#define LPTMR_IRQ_HANDLER LPTMR0_IRQHandler
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgInternalRefClk)


volatile bool PIT_timer1_finished = false;
volatile bool PIT_timer0_finished = false;
volatile bool LPTMR_timer_finished = false;


void LPTMR_Timer_Init(void)
{
	/*
	uint32_t clockFreq = LPTMR_SOURCE_CLOCK;
	PRINTF("LPTMR Source Clock: %u Hz\r\n", clockFreq);
	*/

	uint32_t currentCounter = 0U;
	lptmr_config_t lptmrConfig;
    LPTMR_GetDefaultConfig(&lptmrConfig);
    LPTMR_Init(LPTMR_BASE, &lptmrConfig);
    LPTMR_SetTimerPeriod(LPTMR_BASE, USEC_TO_COUNT(10U, LPTMR_SOURCE_CLOCK));
    LPTMR_EnableInterrupts(LPTMR_BASE, kLPTMR_TimerInterruptEnable);
    EnableIRQ(LPTMR_IRQn);

    PRINTF("LPTMR timer init finished\r\n");
    //LPTMR_StartTimer(LPTMR_BASE);
}

void LPTMR_timer_start(void)
{
	LPTMR_timer_finished = false;
	LPTMR_StartTimer(LPTMR_BASE);
}


void LPTMR_IRQ_HANDLER(void)
{
    LPTMR_ClearStatusFlags(LPTMR_BASE, kLPTMR_TimerCompareFlag);

    LPTMR_timer_finished = true;

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


    LPTMR_StopTimer(LPTMR_BASE);
    __DSB();
    __ISB();
}


void PIT_IRQ_HANDLER(void)
{
	//PRINTF("PIT IQR\r\n");
	//PIT TIMER CHANNEL 0
	if((PIT_GetStatusFlags(PIT_BASEADDR, kPIT_Chnl_0)) == 1)
	{
		//PRINTF("\r\n Channel No.0 interrupt is occurred !");
		PIT_timer0_finished = true;
		PIT_ClearStatusFlags(PIT_BASEADDR, kPIT_Chnl_0, kPIT_TimerFlag);
		PixyZpracujVektory();

		//PIT_StopTimer(PIT_BASEADDR, kPIT_Chnl_0);
	}

	//PIT TIMER CHANNEL 1 - delsi timeout
	if((PIT_GetStatusFlags(PIT_BASEADDR, kPIT_Chnl_1)) == 1)
	{
	//PRINTF("\r\n Channel No.1 interrupt is occurred !");
	PIT_timer1_finished = true;
	probihaZmena = false;
	PIT_ClearStatusFlags(PIT_BASEADDR, kPIT_Chnl_1, kPIT_TimerFlag);
	PIT_StopTimer(PIT_BASEADDR, kPIT_Chnl_1);
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
	PIT_timer1_finished = false;
	probihaZmena = true;
	PIT_StartTimer(PIT_BASEADDR, kPIT_Chnl_1);
}

//Funkce na inicializaci timeru
void PIT_Timer_Init(void)
{
	uint32_t clockFreq = PIT_SOURCE_CLOCK;
		PRINTF("PIT Source Clock: %u Hz\r\n", clockFreq);

	pit_config_t pitConfig;
	PIT_GetDefaultConfig(&pitConfig);
	PIT_Init(PIT_BASEADDR, &pitConfig);


	PIT_SetTimerPeriod(PIT_BASEADDR, kPIT_Chnl_0, USEC_TO_COUNT(100000, PIT_SOURCE_CLOCK));
	PIT_SetTimerPeriod(PIT_BASEADDR, kPIT_Chnl_1, USEC_TO_COUNT(500000, PIT_SOURCE_CLOCK));


	PIT_EnableInterrupts(PIT_BASEADDR, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	PIT_EnableInterrupts(PIT_BASEADDR, kPIT_Chnl_1, kPIT_TimerInterruptEnable);


	EnableIRQ(PIT_IRQ_ID);


	//Nezapinat timery aby to nebezelo periodicky
	//PIT_StartTimer(PIT_BASEADDR, kPIT_Chnl_0);
	//PIT_StartTimer(PIT_BASEADDR, kPIT_Chnl_1);
	PRINTF("PIT TIMERS inits finished\r\n");
}

