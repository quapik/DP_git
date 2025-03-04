/*
 * sonic.c
 *
 *  Created on: 14. 2. 2025
 *      Author: xsimav01
 */

#include <sonic_color.h>
volatile uint32_t tmp0_interrupt_status;

volatile bool probehloUspesneMereniSRF05_1 = false;
volatile bool probehloUspesneMereniSRF05_2 = false;
volatile bool probehloUspesneMereniColor_1 = false;
volatile bool probehloUspesneMereniColor_2 = false;

volatile uint32_t risingEdgeTime1 = 0;
volatile uint32_t fallingEdgeTime1 = 0;
volatile uint32_t risingEdgeTime2 = 0;
volatile uint32_t fallingEdgeTime2 = 0;

volatile uint32_t risingEdgeTimeColor1 = 0;
volatile uint32_t fallingEdgeTimeColor1 = 0;
volatile uint32_t risingEdgeTimeColor2 = 0;
volatile uint32_t fallingEdgeTimeColor2 = 0;

volatile bool risigneEdgeCaptured1 = false;
volatile bool risigneEdgeCaptured2 = false;
volatile bool risigneEdgeCapturedColor1 = false;
volatile bool risigneEdgeCapturedColor2 = false;

volatile uint32_t overflowCount1 = 0;
volatile uint32_t overflowCount2 = 0;
volatile uint32_t overflowCountColor1 = 0;
volatile uint32_t overflowCountColor2 = 0;


volatile uint32_t timerVal1;
volatile uint32_t timerVal2;
volatile uint32_t timerValColor1;
volatile uint32_t timerValColor2;

volatile uint32_t pulseWidth1 = 0;
volatile uint32_t pulseWidth2 = 0;
volatile uint32_t pulseWidthColor1 = 0;
volatile uint32_t pulseWidthColor2 = 0;

uint32_t distance1 = 450;
uint32_t distance2 = 450;

uint32_t distance1_sum;
uint32_t distance2_sum;

volatile bool color_event_flag = false;
volatile uint32_t last_color_pw = 0;
volatile uint8_t last_color_index = 0;

#define POCET_MERENI 10
uint8_t pocet_mereni1 = 1;
uint8_t pocet_mereni2 = 1;
bool prumerovani = false;
uint32_t counter = 0;

tpm_config_t tmp0info;

void tmp0_reset(void)
{
	   TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_1_channel, kTPM_RisingEdge);
	   TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_2_channel, kTPM_RisingEdge);
	   TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_1_channel, kTPM_RisingEdge);
	   TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_2_channel, kTPM_RisingEdge);
	   overflowCount1 = 0;
	   overflowCount2 = 0;
	   overflowCountColor1 = 0;
	   overflowCountColor2 = 0;
	   risigneEdgeCaptured1 = false;
	   risigneEdgeCaptured2 = false;
	   risigneEdgeCapturedColor1 = false;
	   risigneEdgeCapturedColor2 = false;
	   EnableIRQ(TPM0_INTERRUPT_NUMBER);
	   TPM_StartTimer(TPM0_BASEADDR, kTPM_SystemClock);
	   //V inicializace se hned pusti uvodni dva pulzy (eliminuje se tim ze by pak pri prvnim mereni byly namereny nejake spatne hodnoty
	   TriggerPulse1();
	   TriggerPulse2();
}

//Funkce na inicializace TMP0 - obsluhuje senzory na ruznych channelech
void tmp0_init(void)
{
   CLOCK_SetTpmClock(1U);
   //SRF TRIGGER pin na 0 pro jistotu
   GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger1_GPIO, BOARD_INITPINS_SRF05_trigger1_PIN, 0);
   GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger2_GPIO, BOARD_INITPINS_SRF05_trigger2_PIN, 0);

   //TMP INIT
   TPM_GetDefaultConfig(&tmp0info);
   tmp0info.prescale = kTPM_Prescale_Divide_128;
   TPM_Init(TPM0_BASEADDR, &tmp0info);


   //defaultni nastaveni na to aby prvne byla zachycavana Rising edge
   TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_1_channel, kTPM_RisingEdge);
   TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_2_channel, kTPM_RisingEdge);
   TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_1_channel, kTPM_RisingEdge);
   TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_2_channel, kTPM_RisingEdge);

   //Nastaveni do kolika bude pocitanbo, pak preteceni
   TPM0_BASEADDR->MOD = COUNTER_MAX;

   TPM_EnableInterrupts(TPM0_BASEADDR, COLOR_1_channel_INTERRUPT_ENABLE | COLOR_2_channel_INTERRUPT_ENABLE | SRF05_1_channel_INTERRUPT_ENABLE |SRF05_2_channel_INTERRUPT_ENABLE | kTPM_TimeOverflowInterruptEnable);
   //TPM_EnableInterrupts(TPM0_BASEADDR, SRF05_1_channel_INTERRUPT_ENABLE |SRF05_2_channel_INTERRUPT_ENABLE | kTPM_TimeOverflowInterruptEnable);

   EnableIRQ(TPM0_INTERRUPT_NUMBER);
	//NVIC_SetPriority(TPM0_INTERRUPT_NUMBER, 2);
   TPM_StartTimer(TPM0_BASEADDR, kTPM_SystemClock);
   //V inicializace se hned pusti uvodni dva pulzy (eliminuje se tim ze by pak pri prvnim mereni byly namereny nejake spatne hodnoty
   TriggerPulse1();
   //TriggerPulse2();
   PRINTF("TMP0 INIT FINISHED\r\n");
}


//Funkce, která vyšle na GPIO pinu 10s dlouhý signál, kdy SRF05 senzor vysílá ultrazuvkový signál
void TriggerPulse1(void)
{	isTriggerTriggering = true;
	GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger1_GPIO, BOARD_INITPINS_SRF05_trigger1_PIN, 1);
	//SDK_DelayAtLeastUs(10U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
	//GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger1_GPIO, BOARD_INITPINS_SRF05_trigger1_PIN, 0);
	actualTrigger = 1;
	PIT_timer0_start();
}

void TriggerPulse2(void)
{
	isTriggerTriggering = true;
	GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger2_GPIO, BOARD_INITPINS_SRF05_trigger2_PIN, 1);
    //SDK_DelayAtLeastUs(10U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    //GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger2_GPIO, BOARD_INITPINS_SRF05_trigger2_PIN, 0);
	actualTrigger = 2;
	PIT_timer0_start();
}

//Pokud je zmerena vzdalenost delsi nez 450, zmeni se na maximalni meritelnou hodnotu senzoru
uint32_t checkMaxDistance(uint32_t d)
{
	return ((d > 450) ? 450 : d);
}

//Funkce, co počítá na základě počátečního a koncového času kdy je signál v 1 (rising a falling edge)
uint32_t pulseWidthLength(uint32_t rising, uint32_t falling, uint32_t overflow)
{
	return ((falling >= rising) ?
            (falling - rising + (overflow * COUNTER_MAX)) :
            ((COUNTER_MAX - rising) + falling + (overflow * COUNTER_MAX)));
}

// Funkce, která počítá v závislosti na počtu ticků vzdálenost SRF05 ultrasonic senzoru
// Delka pulzu je prevedena na casovy udaj v US a proveden výpočet
uint32_t distanceCountF(uint32_t PW)
{
	uint32_t d = (pulseWidthToUs(PW) * RYCHLOST_ZVUKU) /  2;
	d = checkMaxDistance(d);
	return d;
}
//Převod délky pulsu v ticks na US
uint32_t pulseWidthToUs(uint32_t PW)
{
	//COUNTER_TO_US == 2.67 při prescaler == 128
	return (PW *  COUNTER_TO_US);
}

void checkColorSensorValue(uint32_t PW_US, uint8_t i)
{
	//Zaznamenae merici honoty, pirmo na ledky, NIC je pohled do prazdna na zem cca metr
	//100% S0 S1 H H | NIC - cca 53, BILA 26-23, CERNA PASKA 16, FIXA 18
	//2%   S0 S1 L H | NIC - cca 2880, BILA 163, CERNA PASKA 980 - 1150, FIXA 980-1110
	//20%  S0 S1 H L | NIC - cca 280, BILA 13, CERNA PASKA 60-100, FIXA cca 90
	// S2 H
	// S3 L  for clear (no filter)
	 last_color_pw = PW_US;
	 last_color_index = i;
	 color_event_flag = true;

}

//VOLANO Z MAINU
void processColorSensorValue()
{

	if(color_event_flag)
	{
		color_event_flag = false;
		uint16_t color_treshold1_WHITE = 400;
		uint16_t color_treshold1_BLACK = 1000;
		uint16_t color_treshold2_WHITE = 1000;
		uint16_t color_treshold2_BLACK = 5000;

		if(last_color_index == 1)
		{
			COLOR1_value_global = last_color_pw;
			//PRINTF("COLOR1 value = %u \r\n", PW_US);
			if(probihaZmena == false)
			{
				if(COLOR1_value_global < color_treshold1_BLACK)
				{
					if(COLOR1_value_global > color_treshold1_WHITE)
						{
							PRINTF("C1 %u A \r\n", COLOR1_value_global);
							probihaZmena = true;

							steer_left(50);
							PIT_timer1_start();
							led_B();
							//PIT_delay_100ms();
						}
				}
			}

		}
		else
		{

			COLOR2_value_global = last_color_pw;
			if(probihaZmena == false)
			{
				if(COLOR2_value_global < color_treshold2_BLACK)
				{
					if(COLOR2_value_global > color_treshold2_WHITE)
					{
						PRINTF("C2 %u A \r\n", COLOR2_value_global);
						probihaZmena = true;
						steer_right(50);
						PIT_timer1_start();
						led_B();




					}


				}
			}
		}
	}
}


void TMP0_INTERRUPT_HANDLER(void)
{

    tmp0_interrupt_status = TPM_GetStatusFlags(TPM0_BASEADDR);
    //PRINTF("status %u\r\n",tmp0_interrupt_status);

    //Overflow kdyz pretece counter, je potreba pocitat abyc se pak zaznemanl spravne delka mezi prerusenimi
    if (tmp0_interrupt_status & kTPM_TimeOverflowFlag)
    {
        overflowCount1++;
        overflowCount2++;
        overflowCountColor1++;
        overflowCountColor2++;
        TPM_ClearStatusFlags(TPM0_BASEADDR, kTPM_TimeOverflowFlag);
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
            pulseWidth1 = pulseWidthLength(risingEdgeTime1,fallingEdgeTime1,overflowCount1);
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
            		SRF_distance1_global = distance1_sum / POCET_MERENI;
            		//PRINTF("AVG Distance1 = %u cm\r\n",SRF_distance1_global);
            		pocet_mereni1 = 1;
            		distance1_sum = 0;
            		isObstacle(SRF_distance1_global,SRF_distance2_global);

            	}
            }

            else
            {
            	SRF_distance1_global = distance1;
            	//PRINTF("Distance1 = %u cm (%u) \r\n",distance1, distance2);
            	isObstacle(SRF_distance1_global,SRF_distance2_global);
            }

			//SDK_DelayAtLeastUs(1000U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
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
                pulseWidth2 = pulseWidthLength(risingEdgeTime2,fallingEdgeTime2,overflowCount2);

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
                		SRF_distance2_global = distance2_sum / POCET_MERENI;
                		PRINTF("AVG Distance2 = %u cm\r\n",SRF_distance2_global);
                		pocet_mereni2 = 1;
                		distance2_sum = 0;
                		isObstacle(SRF_distance1_global,SRF_distance2_global);
                	}
                }
                else
                {
                	SRF_distance2_global = distance2;
                	//PRINTF("Distance2 = %u cm (%u) \r\n",distance2, distance1);
                	isObstacle(SRF_distance1_global,SRF_distance2_global);
                }
    			//SDK_DelayAtLeastUs(100000U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    			TriggerPulse1();
            }
            TPM_ClearStatusFlags(TPM0_BASEADDR, SRF05_2_CHANNEL_FLAG);
        }

    if (tmp0_interrupt_status & COLOR_1_CHANNEL_FLAG)
       {
       	timerValColor1 = TPM0_BASEADDR->CONTROLS[COLOR_1_channel].CnV;
       	//Zachycena nastupna hrana signalu - echo signal senzoru, delka pulza pak znamena vzdalenost
           if (!risigneEdgeCapturedColor1)
           {
        	   risingEdgeTimeColor1 = timerValColor1;
               risigneEdgeCapturedColor1 = true;
               overflowCountColor1 = 0;
               //Zmena zachycavani na sestupnou hranu
               TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_1_channel, kTPM_FallingEdge);
           }
           else
           {
               fallingEdgeTimeColor1 = timerValColor1;
               probehloUspesneMereniColor_1 = true;
               risigneEdgeCapturedColor1 = false;
               TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_1_channel, kTPM_RisingEdge);
               //Na zaklade hodnot ze zaznemanaych casovych hodnoty a za zaklade poctu preteceni se vypocita delka pulzu

               pulseWidthColor1 = pulseWidthLength(risingEdgeTimeColor1,fallingEdgeTimeColor1,overflowCountColor1);
               checkColorSensorValue(pulseWidthToUs(pulseWidthColor1),1);

           }
           TPM_ClearStatusFlags(TPM0_BASEADDR, COLOR_1_CHANNEL_FLAG);
       }
    if (tmp0_interrupt_status & COLOR_2_CHANNEL_FLAG)
           {
           	timerValColor2 = TPM0_BASEADDR->CONTROLS[COLOR_2_channel].CnV;
           	//Zachycena nastupna hrana signalu - echo signal senzoru, delka pulza pak znamena vzdalenost
               if (!risigneEdgeCapturedColor2)
               {
               	//PRINTF("NASTUPNA HRANA SRF05_1\r\n");
                   risingEdgeTimeColor2 = timerValColor2;
                   risigneEdgeCapturedColor2 = true;
                   overflowCountColor2 = 0;
                   //Zmena zachycavani na sestupnou hranu
                   TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_2_channel, kTPM_FallingEdge);
               }
               else
               {
               	//PRINTF("SESTUPNA HRANA SRF05_1\r\n");
                   fallingEdgeTimeColor2 = timerValColor2;
                   probehloUspesneMereniColor_2 = true;
                   risigneEdgeCapturedColor2 = false;
                   TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_2_channel, kTPM_RisingEdge);

                   //Na zaklade hodnot ze zaznemanaych casovych hodnoty a za zaklade poctu preteceni se vypocita delka pulzu
                   pulseWidthColor2 = pulseWidthLength(risingEdgeTimeColor2,fallingEdgeTimeColor2,overflowCountColor2);
                   checkColorSensorValue(pulseWidthToUs(pulseWidthColor2),2);

               }
               TPM_ClearStatusFlags(TPM0_BASEADDR, COLOR_2_CHANNEL_FLAG);

           }

    __DSB();
}

void isObstacle(uint32_t d1, uint32_t d2)
{
	//TODO ZPOMALOVANI PODLE VZDALENOSTI
	if(driving)
	{
		uint16_t hranice = 10;
		if(d1 < hranice | d2 < hranice)
		{
		led_R();
		//Vypnuti motoru a nastaveni pro
		motor_set_speed(0);
		steer_straight();
		PRINTF("OBSTACLE DETECTED \r\n");
		startMotorsButtonPressed = false;
		isObstacleDetected = true;
		driving = false;
		}
	}
	/*
	else if(d1 < 100 | d2 < 100)
	{
		led_Y();
		if (driving)
		{
			motor_set_speed(10);
		}

	}
	*/


}
