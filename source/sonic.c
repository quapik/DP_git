/*
 * sonic.c
 *
 *  Created on: 14. 2. 2025
 *      Author: xsimav01
 */

#include "sonic.h"

volatile bool probehloUspesneMereniSRF05_1 = false;
volatile bool probehloUspesneMereniSRF05_2 = false;
volatile uint32_t risingEdgeTime1 = 0;
volatile uint32_t fallingEdgeTime1 = 0;
volatile uint32_t risingEdgeTime2 = 0;
volatile uint32_t fallingEdgeTime2 = 0;
volatile bool risigneEdgeCaptured1 = false;
volatile bool risigneEdgeCaptured2 = false;
volatile uint32_t overflowCount1 = 0;
volatile uint32_t overflowCount2 = 0;

volatile uint32_t tmp0_interrupt_status;
volatile uint32_t timerVal1;
volatile uint32_t timerVal2;

volatile uint32_t pulseWidth1 = 0;
volatile uint32_t pulseWidth2 = 0;

uint32_t distance1;
uint32_t distance2;

uint32_t distance1_sum;
uint32_t distance2_sum;

#define POCET_MERENI 10
uint8_t pocet_mereni1 = 1;
uint8_t pocet_mereni2 = 1;
bool prumerovani = true;

tpm_config_t tmp0info;



//Funkce na inicializace TMP0 - obsluhuje senzory na ruznych channelech
void tmp0_init(void)
{
   CLOCK_SetTpmClock(1U);
   //SRF TRIGGER pin na 0 pro jistotu
   GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger1_GPIO, BOARD_INITPINS_SRF05_trigger1_PIN, 0);
   GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger2_GPIO, BOARD_INITPINS_SRF05_trigger2_PIN, 0);

   //tmp inicializace
   TPM_GetDefaultConfig(&tmp0info);
   TPM_Init(TPM0_BASEADDR, &tmp0info);

   //defaultni nastaveni na to aby prvne byla zachycavana Rising edge
   TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_1_channel, kTPM_RisingEdge);
   TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_2_channel, kTPM_RisingEdge);
   TPM0_BASEADDR->MOD = COUNTER_MAX;

   TPM_EnableInterrupts(TPM0_BASEADDR, SRF05_1_channel_INTERRUPT_ENABLE |SRF05_2_channel_INTERRUPT_ENABLE | kTPM_TimeOverflowInterruptEnable);
   EnableIRQ(TPM0_INTERRUPT_NUMBER);
	//NVIC_SetPriority(TPM0_INTERRUPT_NUMBER, 2);
   TPM_StartTimer(TPM0_BASEADDR, kTPM_SystemClock);

}

void TriggerPulse1(void)
{
	//PRINTF("Byl poslan pulse na SRF05_1 \r\n");
	GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger1_GPIO, BOARD_INITPINS_SRF05_trigger1_PIN, 1);
    SDK_DelayAtLeastUs(10U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
	//SysTick_DelayTicks(10U);
    GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger1_GPIO, BOARD_INITPINS_SRF05_trigger1_PIN, 0);
}

void TriggerPulse2(void)
{
	//PRINTF("Byl poslan pulse na SRF05_2 \r\n");
	GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger2_GPIO, BOARD_INITPINS_SRF05_trigger2_PIN, 1);
    SDK_DelayAtLeastUs(10U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
	//SysTick_DelayTicks(10U);
    GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger2_GPIO, BOARD_INITPINS_SRF05_trigger2_PIN, 0);
}

uint32_t distanceCountF(uint32_t PW)
{
	return (PW* COUNTER_TO_US * RYCHLOST_ZVUKU) /  2;
}


void TMP0_INTERRUPT_HANDLER(void)
{

    tmp0_interrupt_status = TPM_GetStatusFlags(TPM0_BASEADDR);
    //PRINTF("status %u\r\n",status);

    //Overflow kdyz pretece counter, je potreba pocitat abyc se pak zaznemanl spravne delka mezi prerusenimi
    if (tmp0_interrupt_status & kTPM_TimeOverflowFlag)
    {
        overflowCount1++;
        overflowCount2++;
        TPM_ClearStatusFlags(TPM0_BASEADDR, kTPM_TimeOverflowFlag);
        //PRINTF("OVERFLOW\r\n");
    }

    if (tmp0_interrupt_status & SRF05_1_CHANNEL_FLAG)
    {
    	timerVal1 = TPM0_BASEADDR->CONTROLS[SRF05_1_channel].CnV;
    	//Zachycena nastupna hrana signalu - echo signal senzoru, delka pulza pak znamena vzdalenost
        if (!risigneEdgeCaptured1)
        {
        	//PRINTF("NASTUPNA HRANA SRF05_1\r\n");
            risingEdgeTime1 = timerVal1;
            risigneEdgeCaptured1 = true;
            overflowCount1 = 0;
            //Zmena zachycavani na sestupnou hranu
            TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_1_channel, kTPM_FallingEdge);
        }
        else
        {
        	//PRINTF("SESTUPNA HRANA SRF05_1\r\n");
            fallingEdgeTime1 = timerVal1;
            probehloUspesneMereniSRF05_1 = true;
            risigneEdgeCaptured1 = false;
            TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_1_channel, kTPM_RisingEdge);

            //Na zaklade hodnot ze zaznemanaych casovych hodnoty a za zaklade poctu preteceni se vypocita delka pulzu
            pulseWidth1 = (fallingEdgeTime1 >= risingEdgeTime1) ?
            	         (fallingEdgeTime1 - risingEdgeTime1 + (overflowCount1 * COUNTER_MAX)) :
            	         ((COUNTER_MAX - risingEdgeTime1) + fallingEdgeTime1 + (overflowCount1 * COUNTER_MAX));


            distance1 = distanceCountF(pulseWidth1);

            //Vypis a pocitani v zavistlosti na tom zda se prumeruje nebo ne (kvuli eliminace duplicit
            if(prumerovani)
            {
            	if(pocet_mereni1 <  POCET_MERENI)
            	{
            		distance1_sum += distance1;
            		pocet_mereni1++;
            	}
            	else
            	{
            		distance1_sum += distance1;
            		distance1 = distance1_sum / POCET_MERENI;
            		PRINTF("AVG Distance1 = %u cm\r\n",distance1);
            		pocet_mereni1 = 1;
            		distance1_sum = 0;
            	}
            }

            else
            {
            	PRINTF("Distance1 = %u cm (%u) \r\n",distance1, distance2);
            }

			SDK_DelayAtLeastUs(100000U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
			TriggerPulse2();
        }
        TPM_ClearStatusFlags(TPM0_BASEADDR, SRF05_1_CHANNEL_FLAG);
    }
    if (tmp0_interrupt_status & SRF05_2_CHANNEL_FLAG)
        {
        	timerVal2 = TPM0_BASEADDR->CONTROLS[SRF05_2_channel].CnV;
        	//Zachycena nastupna hrana signalu - echo signal senzoru, delka pulza pak znamena vzdalenost
            if (!risigneEdgeCaptured2)
            {
            	//PRINTF("NASTUPNA HRANA SRF05_1\r\n");
                risingEdgeTime2 = timerVal2;
                risigneEdgeCaptured2 = true;
                overflowCount2 = 0;
                //Zmena zachycavani na sestupnou hranu
                TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_2_channel, kTPM_FallingEdge);
            }
            else
            {
            	//PRINTF("SESTUPNA HRANA SRF05_1\r\n");
                fallingEdgeTime2 = timerVal2;
                probehloUspesneMereniSRF05_2 = true;
                risigneEdgeCaptured2 = false;
                TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_2_channel, kTPM_RisingEdge);

                //Na zaklade hodnot ze zaznemanaych casovych hodnoty a za zaklade poctu preteceni se vypocita delka pulzu
                pulseWidth2 = (fallingEdgeTime2 >= risingEdgeTime2) ?
                	         (fallingEdgeTime2 - risingEdgeTime2+ (overflowCount2 * COUNTER_MAX)) :
                	         ((COUNTER_MAX - risingEdgeTime2) + fallingEdgeTime2 + (overflowCount2 * COUNTER_MAX));


                distance2 = distanceCountF(pulseWidth2);

                //Vypis a pocitani v zavistlosti na tom zda se prumeruje nebo ne (kvuli eliminace duplicit
                if(prumerovani)
                {
                	if(pocet_mereni2 <  POCET_MERENI)
                	{
                		distance2_sum += distance2;
                		pocet_mereni2++;
                	}
                	else
                	{
                		distance2_sum += distance2;
                		distance2 = distance2_sum / POCET_MERENI;
                		PRINTF("AVG Distance2 = %u cm\r\n",distance2);
                		pocet_mereni2 = 1;
                		distance2_sum = 0;
                	}
                }

                else
                {
                	PRINTF("Distance2 = %u cm (%u) \r\n",distance2, distance1);
                }

    			SDK_DelayAtLeastUs(100000U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    			TriggerPulse1();
            }
            TPM_ClearStatusFlags(TPM0_BASEADDR, SRF05_2_CHANNEL_FLAG);
        }

    __DSB();
}


