#include "pit_control.h"

#define POCET_PIT 2
volatile bool on = false;

// Správná alokace paměti pro časovače
static uint32_t pitHandler[POCET_PIT][HAL_TIMER_HANDLE_SIZE / sizeof(uint32_t)];

// Správné přiřazení handlerů
hal_timer_handle_t pitHandlers[POCET_PIT] = {
    (hal_timer_handle_t)pitHandler[0],
    (hal_timer_handle_t)pitHandler[1],
};

void PIT_Timer_Init(hal_timer_handle_t timerHandler, uint32_t timeout, hal_timer_callback_t callback, pit_chnl_t channel)
{
    hal_timer_config_t timerConfig;

    timerConfig.timeout = timeout;
    timerConfig.instance = 0;
    timerConfig.srcClock_Hz = CLOCK_GetFreq(kCLOCK_BusClk);

    if (HAL_TimerInit(timerHandler, &timerConfig) != kStatus_HAL_TimerSuccess)
    {
        return;  // Chybná inicializace
    }

    HAL_TimerInstallCallback(timerHandler, callback, timerHandler);
    PIT_SetTimerPeriod(PIT, channel, (uint32_t)USEC_TO_COUNT(timeout, timerConfig.srcClock_Hz));
    PIT_EnableInterrupts(PIT, channel, kPIT_TimerInterruptEnable);

    // Přerušení povolit pouze při prvním kanálu
    if (channel == kPIT_Chnl_0) {
        NVIC_EnableIRQ(PIT_IRQn);
    }

    HAL_TimerEnable(timerHandler);
}

void PIT_Callback_10us(void *param)
{
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
    HAL_TimerDisable(param);
}

void PIT_delay_10us(void)
{
    HAL_TimerEnable(pitHandlers[0]);
}

void PIT_Callback_100ms(void *param)
{
	led_off();
	steer_straight();
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
    HAL_TimerDisable(param);
    probihaZmena = false;
}

void PIT_delay_100ms(void)
{
    HAL_TimerEnable(pitHandlers[1]);
}


void PIT_Timers_Init()
{
	PIT_Timer_Init(pitHandlers[0], 10U, PIT_Callback_10us, kPIT_Chnl_0);
    PIT_Timer_Init(pitHandlers[1], 250000U, PIT_Callback_100ms, kPIT_Chnl_1);

}
