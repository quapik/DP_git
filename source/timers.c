#include <timers.h>

#define PIT_BASEADDR PIT
#define PIT_IRQ_HANDLER   PIT_IRQHandler
#define PIT_IRQ_ID        PIT_IRQn
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)

#define LPTMR_BASE   LPTMR0
#define LPTMR_IRQn   LPTMR0_IRQn
#define LPTMR_IRQ_HANDLER LPTMR0_IRQHandler
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)


volatile bool PIT_timer1_finished = false;
volatile bool PIT_timer0_finished = false;
volatile bool LPTMR_timer_finished = false;

uint32_t  c = 0;

bool LPTMR_first = true;
//LPTMR --------------------------------------------------------------------------------------------------

//Funkce pro inicialziaci LPTMR timeru (stara se o periodicky volani pro poslani logu pres UART)
void LPTMR_Timer_Init(void)
{
	lptmr_config_t lptmrConfig;
    LPTMR_GetDefaultConfig(&lptmrConfig);
    LPTMR_Init(LPTMR_BASE, &lptmrConfig);
    LPTMR_SetTimerPeriod(LPTMR_BASE, USEC_TO_COUNT(200000U, LPTMR_SOURCE_CLOCK));
    LPTMR_EnableInterrupts(LPTMR_BASE, kLPTMR_TimerInterruptEnable);
    EnableIRQ(LPTMR_IRQn);

    //PRINTF("LPTMR timer inicializace probehla uspesne\r\n");
    //LPTMR_timer_start();
}
//Funkce pro start a vypnuti lptimeru (lze vyzit dle potreby)
void LPTMR_StartPosilejUART(void)
{
	LPTMR_timer_finished = false;
	LPTMR_first = true;
	LPTMR_StartTimer(LPTMR_BASE);
}

void LPTMR_StopPosilejUART(void)
{
	LPTMR_timer_finished = true;
	LPTMR_first = false;
	LPTMR_StopTimer(LPTMR_BASE);
}

//Přerušení LPTImeru, co je periodicky vyvoláváno a je volán výpis na UART
void LPTMR_IRQ_HANDLER(void)
{
    LPTMR_ClearStatusFlags(LPTMR_BASE, kLPTMR_TimerCompareFlag);
    UART2_SendToHC05();
    __DSB();
    __ISB();
}
//PIT --------------------------------------------------------------------------------------------------
void PIT_IRQ_HANDLER(void)
{
	//PIT TIMER CHANNEL 0
	if((PIT_GetStatusFlags(PIT_BASEADDR, kPIT_Chnl_0)) == 1)
	{
		PIT_ClearStatusFlags(PIT_BASEADDR, kPIT_Chnl_0, kPIT_TimerFlag);
		PixyZpracujVektory();
		//PIT_StopTimer(PIT_BASEADDR, kPIT_Chnl_0);
	}

	//PIT TIMER CHANNEL 1 - delsi timeout
	if((PIT_GetStatusFlags(PIT_BASEADDR, kPIT_Chnl_1)) == 1)
	{
	PIT_timer1_finished = true;
	PIT_ClearStatusFlags(PIT_BASEADDR, kPIT_Chnl_1, kPIT_TimerFlag);

	//MERENI IR SENZORZ
	//irsensor_check();
	processColorSensorValue();
	}

    __DSB();
}
//Funkce pro zastavovani a spousteni timeru, pokud by bylo potreba vyuzit pri neperoidickem vyuzivani PIT timeru
void PIT_StartPixyZpracovavatVektory(void)
{
	PIT_StartTimer(PIT_BASEADDR, kPIT_Chnl_0);
	PIT_timer0_finished = false;
}

void PIT_StopPixyZpracovavatVektory(void)
{
	PIT_StopTimer(PIT_BASEADDR, kPIT_Chnl_0);
	PIT_timer0_finished = true;
}

void PIT_StartZpracujBarvuIRSensor(void)
{
	PIT_timer1_finished = false;
	PIT_StartTimer(PIT_BASEADDR, kPIT_Chnl_1);
}

void PIT_StopZpracujBarvuIRSensor(void)
{
	PIT_StopTimer(PIT_BASEADDR, kPIT_Chnl_1);
	PIT_timer1_finished = true;
}

//Funkce na inicializaci PIT TIMERU
void PIT_Timer_Init(void)
{
	uint32_t clockFreq = PIT_SOURCE_CLOCK;
		PRINTF("PIT Source Clock: %u Hz\r\n", clockFreq);

	pit_config_t pitConfig;
	PIT_GetDefaultConfig(&pitConfig);
	PIT_Init(PIT_BASEADDR, &pitConfig);


	//Jak casto se budou zpracovavat vektory
	PIT_SetTimerPeriod(PIT_BASEADDR, kPIT_Chnl_0, USEC_TO_COUNT(100000, PIT_SOURCE_CLOCK)); //33333 funguje

	//Jak casto se cte hodnota baervny senzoru + IR senzoru
	PIT_SetTimerPeriod(PIT_BASEADDR, kPIT_Chnl_1, USEC_TO_COUNT(500000, PIT_SOURCE_CLOCK));


	PIT_EnableInterrupts(PIT_BASEADDR, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	PIT_EnableInterrupts(PIT_BASEADDR, kPIT_Chnl_1, kPIT_TimerInterruptEnable);

	EnableIRQ(PIT_IRQ_ID);

}

