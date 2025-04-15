/*
 * 	sonic.c
 *
 * 	Kod starajici se obsluhu utlrasonickych senzoru vzdalenosti + color sensoru
 * 	Zachytavani delky hran pro kazdy senzor, vyuzito TMP pinu a timeru
 *
 *  Created on: 14. 2. 2025
 *  Author: xsimav01
 */

#include <sonic_color.h>

volatile uint32_t tmp0_interrupt_status;
uint16_t hranice;

//Promenne uchovavajici hodnotu zda bylo provedeno uspesne mereni
volatile bool probehloUspesneMereniSonic1 = false;
volatile bool probehloUspesneMereniSonic2 = false;
volatile bool probehloUspesneMereniColor_1 = false;
volatile bool probehloUspesneMereniColor_2 = false;

//Promenne uchovavajici casy rising/falling hrran pro ulstrasonic senzory
volatile uint16_t risingEdgeTimeSonic1 = 0;
volatile uint16_t fallingEdgeTimeSonic1 = 0;
volatile uint16_t risingEdgeTimeSonic2 = 0;
volatile uint16_t fallingEdgeTimeSonic2 = 0;

//Promenne uchovavajici casy rising/falling hrran pro color sensory
volatile uint16_t risingEdgeTimeColor1 = 0;
volatile uint16_t fallingEdgeTimeColor1 = 0;
volatile uint16_t risingEdgeTimeColor2 = 0;
volatile uint16_t fallingEdgeTimeColor2 = 0;

//Bool promnne uchovavajici zda byly zachyceny nastupne hrany (po nich se meri delka pulzu)
volatile bool risigneEdgeCapturedSonic1 = false;
volatile bool risigneEdgeCapturedSonic2 = false;
volatile bool risigneEdgeCapturedColor1 = false;
volatile bool risigneEdgeCapturedColor2 = false;


//Promenne uchovavajici pocet preteceni timeru pro jednotlive senzory (dulezita informace pro spravne vypocitani delky pulzu)
volatile uint16_t overflowCountSonic1 = 0;
volatile uint16_t overflowCountSonic2 = 0;
volatile uint16_t overflowCountColor1 = 0;
volatile uint16_t overflowCountColor2 = 0;

//Promenne pro ulozeni aktualnich hodnot casovace pro jednotlive channel
volatile uint16_t aktualniHodnotaCasovaceSonic1;
volatile uint16_t aktualniHodnotaCasovaceSonic2;
volatile uint16_t aktualniHodnotaCasovaceColor1;
volatile uint16_t aktualniHodnotaCasovaceColor2;

//Promenne pro ulozeni delky pulzu senzoru
volatile uint32_t pulseWidthSonic1 = 0;
volatile uint32_t pulseWidthSonic2 = 0;
volatile uint32_t pulseWidthColor1 = 0;
volatile uint32_t pulseWidthColor2 = 0;


//Promenne pro vypoctenou vzdalenost a pro akumulovanou vzdalenost pri prumerovani
uint32_t distance1 = 450;
uint32_t distance2 = 450;
uint32_t distance1_sum;
uint32_t distance2_sum;

volatile bool color_event_flag = false;
volatile uint32_t last_color_pw = 0;
volatile uint8_t last_color_index = 0;

#define POCET_MERENI 5
uint8_t pocet_mereni1 = 1;
uint8_t pocet_mereni2 = 1;
uint32_t counter = 0;
tpm_config_t tmp0info;

bool prumerovani = false;


bool SONIC1_ocekavano = false;
bool SONIC2_ocekavano = false;


volatile uint32_t COLOR1_PW = 0;
volatile uint32_t COLOR2_PW = 0;



//Funkce pro znovuzapnutí TMP, nevyužito, zanecháno pro případ potřeby
void TMP0Reset(void)
{
	   TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_1_channel, kTPM_RisingEdge);
	   TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_2_channel, kTPM_RisingEdge);
	   TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_1_channel, kTPM_RisingEdge);
	   TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_2_channel, kTPM_RisingEdge);
	   overflowCountSonic1 = 0;
	   overflowCountSonic2 = 0;
	   overflowCountColor1 = 0;
	   overflowCountColor2 = 0;
	   risigneEdgeCapturedSonic1 = false;
	   risigneEdgeCapturedSonic2 = false;
	   risigneEdgeCapturedColor1 = false;
	   risigneEdgeCapturedColor2 = false;
	   EnableIRQ(TPM0_INTERRUPT_NUMBER);
	   TPM_StartTimer(TPM0_BASEADDR, kTPM_SystemClock);
}
//Funkce pro resetování pouze SRF senzorů a jejich měření, využití dle potřeby
void SonicReset(void)
{
	overflowCountSonic1 = 0;
	overflowCountSonic2 = 0;
	TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_1_channel, kTPM_RisingEdge);
	TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_2_channel, kTPM_RisingEdge);
	risigneEdgeCapturedSonic1 = false;
	risigneEdgeCapturedSonic2 = false;
	TriggerPulse1();
	//TriggerPulse2();
}

//Funkce na inicializace TMP0 - obsluhuje senzory na ruznych channelech
void TMP0Init(void)
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

}


//Funkce, která vyšle na GPIO pinu 10s dlouhý signál, kdy SRF05 senzor vysílá ultrazuvkový signál
void TriggerPulse1(void)
{	isTriggerTriggering = true;

	//risigneEdgeCapturedSonic1 = false;
	//TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_1_channel, kTPM_RisingEdge);

	GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger1_GPIO, BOARD_INITPINS_SRF05_trigger1_PIN, 1);
	SDK_DelayAtLeastUs(10U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
	GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger1_GPIO, BOARD_INITPINS_SRF05_trigger1_PIN, 0);
	//actualTrigger = 1;
	//LPTMR_timer_start();

   	SONIC1_ocekavano = true;
	SONIC2_ocekavano = false;
}

void TriggerPulse2(void)
{
	isTriggerTriggering = true;
	//risigneEdgeCapturedSonic2 = false;
	//TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_2_channel, kTPM_RisingEdge);

	GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger2_GPIO, BOARD_INITPINS_SRF05_trigger2_PIN, 1);
	SDK_DelayAtLeastUs(10U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    GPIO_PinWrite(BOARD_INITPINS_SRF05_trigger2_GPIO, BOARD_INITPINS_SRF05_trigger2_PIN, 0);
	//actualTrigger = 2;
	//LPTMR_timer_start();

   	SONIC1_ocekavano = false;
	SONIC2_ocekavano = true;
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

//Ulozeni uklada na zaklade preruseni na TMP channelech do proenych pro budouci zpracovani
void checkColorSensorValue(uint32_t PW_US, uint8_t i)
{
	 last_color_pw = PW_US;
	 last_color_index = i;
	 color_event_flag = true;
	 if (i == 1)
	 {
		 COLOR1_value_global = PW_US;
	 }
	 else
		 {
		 COLOR2_value_global = PW_US;
		 }
}

//Funkce ktera  zpracovava periodicky ulozene hodnoty v promenych a kontroluje barvu
//Volaana z mainu
void processColorSensorValue()
{
	DisableIRQ(TPM0_INTERRUPT_NUMBER);
	//Zaznamenae merici honoty, pirmo na ledky, NIC je pohled do prazdna na zem cca metr
	//100% S0 S1 H H | NIC - cca 53, BILA 26-23, CERNA PASKA 16, FIXA 18
	//2%   S0 S1 L H | NIC - cca 2880, BILA 163, CERNA PASKA 980 - 1150, FIXA 980-1110
	//20%  S0 S1 H L | NIC - cca 280, BILA 13, CERNA PASKA 60-100, FIXA cca 90
	// S2 H
	// S3 L  for clear (no filter)

	//PRINTF("C1 %u  (c2 %u) \r\n", COLOR1_value_global,COLOR2_value_global);

	if(driving && !dokoncenoKolo)
	{
		if(COLOR1_value_global > 100 && COLOR1_value_global < 500)
		{
			SteerLeft(75);
		}

		if(COLOR2_value_global > 100 && COLOR2_value_global < 500)
		{
			SteerRight(75);
		}

		COLOR1_value_global = 0;
		COLOR2_value_global = 0;
	}
	EnableIRQ(TPM0_INTERRUPT_NUMBER);
}

//Funkce která ja automaticky volána když je přerušení vyvoláno (nástupné a sesupn0 hranz senzoru]
void TMP0_INTERRUPT_HANDLER(void)
{

    tmp0_interrupt_status = TPM_GetStatusFlags(TPM0_BASEADDR);
    //PRINTF("status %u\r\n",tmp0_interrupt_status);

    //Overflow kdyz pretece counter, je potreba pocitat abyc se pak zaznemanl spravne delka mezi prerusenimi
    if (tmp0_interrupt_status & kTPM_TimeOverflowFlag)
    {

        overflowCountSonic1++;
        overflowCountSonic2++;
        overflowCountColor1++;
        overflowCountColor2++;
        //Občas se měřeni nějak zadrhne a začně přetykat, takže je třeba restartovat
        if( overflowCountSonic1==4 || overflowCountSonic2==4)
        	{

        	//PRINTF("OF %u OF2 %u\r\n", overflowCountSonic1,overflowCountSonic2);
        	SonicReset();
        	}

        TPM_ClearStatusFlags(TPM0_BASEADDR, kTPM_TimeOverflowFlag);
    }
//-------------------SRF ULTRASONIC 2x----------------------------------------------------------------------------------
    if (tmp0_interrupt_status & SRF05_1_CHANNEL_FLAG)
    {
    	if(SONIC1_ocekavano)
    	{
    		aktualniHodnotaCasovaceSonic1 = TPM0_BASEADDR->CONTROLS[SRF05_1_channel].CnV;
			//Zachycena nastupna hrana signalu - echo signal senzoru, delka pulzu pak znamena vzdalenost
			if (!risigneEdgeCapturedSonic1)
			{
				//PRINTF("NASTUPNA HRANA SRF05_1\r\n");
				risingEdgeTimeSonic1 = aktualniHodnotaCasovaceSonic1;
				risigneEdgeCapturedSonic1 = true;
				overflowCountSonic1 = 0;
				//Zmena zachycavani na sestupnou hranu
				TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_1_channel, kTPM_FallingEdge);
			}
			else
			{
				//PRINTF("SESTUPNA HRANA SRF05_1\r\n");
				fallingEdgeTimeSonic1 = aktualniHodnotaCasovaceSonic1;
				probehloUspesneMereniSonic1 = true;
				risigneEdgeCapturedSonic1 = false;
				//Zmena zachycavani na nastupnou hranu (bude cekat na prijem signalu pro triggeru)
				TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_1_channel, kTPM_RisingEdge);

				//Na zaklade hodnot ze zaznemanaych casovych hodnoty a za zaklade poctu preteceni se vypocita delka pulzu
				pulseWidthSonic1 = pulseWidthLength(risingEdgeTimeSonic1,fallingEdgeTimeSonic1,overflowCountSonic1);
				distance1 = distanceCountF(pulseWidthSonic1);
				if(distance1 > 5)
						{
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
										//PRINTF("Distance1 = %u cm \r\n",distance1);
										isObstacle(SRF_distance1_global,SRF_distance2_global);
									}

						}


				TriggerPulse2(); //Vyvolani triggeru a merenim na druhem senzoru, stridani kvuli zajisteni funkcnosti
			}
    	}
        TPM_ClearStatusFlags(TPM0_BASEADDR, SRF05_1_CHANNEL_FLAG);
    }
    if (tmp0_interrupt_status & SRF05_2_CHANNEL_FLAG)
        {
        	if(SONIC2_ocekavano)
        	{
        		aktualniHodnotaCasovaceSonic2 = TPM0_BASEADDR->CONTROLS[SRF05_2_channel].CnV;
				//Zachycena nastupna hrana signalu - echo signal senzoru, delka pulza pak znamena vzdalenost
				if (!risigneEdgeCapturedSonic2)
				{
					//PRINTF("NASTUPNA HRANA SRF05_1\r\n");
					risingEdgeTimeSonic2 = aktualniHodnotaCasovaceSonic2;
					risigneEdgeCapturedSonic2 = true;
					overflowCountSonic2 = 0;
					//Zmena zachycavani na sestupnou hranu
					TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_2_channel, kTPM_FallingEdge);
				}
				else
				{
					//PRINTF("SESTUPNA HRANA SRF05_1\r\n");
					fallingEdgeTimeSonic2 = aktualniHodnotaCasovaceSonic2;
					probehloUspesneMereniSonic2 = true;
					risigneEdgeCapturedSonic2 = false;
					//Zmena zachycavani na nastupnou hranu (bude cekat na prijem signalu pro triggeru)
					TPM_SetupInputCapture(TPM0_BASEADDR, SRF05_2_channel, kTPM_RisingEdge);

					//Na zaklade hodnot ze zaznemanaych casovych hodnoty a za zaklade poctu preteceni se vypocita delka pulzu
					pulseWidthSonic2 = pulseWidthLength(risingEdgeTimeSonic2,fallingEdgeTimeSonic2,overflowCountSonic2);
					distance2 = distanceCountF(pulseWidthSonic2);
					if(distance2 > 5)
					{

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
													//PRINTF("		AVG Distance2 = %u cm\r\n",SRF_distance2_global);
													pocet_mereni2 = 1;
													distance2_sum = 0;
													isObstacle(SRF_distance1_global,SRF_distance2_global);
												}
											}
											//Pokud neni zapnuto prumerovani, kontroluje se kazda zmerena  hodnota
											else
											{
												SRF_distance2_global = distance2;
												//PRINTF("			Distance2 = %u cm \r\n",distance2);
												isObstacle(SRF_distance1_global,SRF_distance2_global);
											}


					}


					TriggerPulse1(); //Vyvolani triggeru a merenim na druhem senzoru, stridani kvuli zajisteni funkcnosti
				}
        	}
            TPM_ClearStatusFlags(TPM0_BASEADDR, SRF05_2_CHANNEL_FLAG);
        }
//-------------------COLORS----------------------------------------------------------------------------------
    if (tmp0_interrupt_status & COLOR_1_CHANNEL_FLAG)
       {
			aktualniHodnotaCasovaceColor1 = TPM0_BASEADDR->CONTROLS[COLOR_1_channel].CnV;
							//Zachycena nastupna hrana signalu - echo signal senzoru, delka pulza pak znamena vzdalenost
			if (!risigneEdgeCapturedColor1)
		   {
			   //PRINTF("COLOR 1 RISING\r\n");
			   risingEdgeTimeColor1 = aktualniHodnotaCasovaceColor1;
			   risigneEdgeCapturedColor1 = true;
			   overflowCountColor1 = 0;
			   //Zmena zachycavani na sestupnou hranu
			   TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_1_channel, kTPM_FallingEdge);
		   }
		   else
		   {
			   fallingEdgeTimeColor1 = aktualniHodnotaCasovaceColor1;
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
			aktualniHodnotaCasovaceColor2 = TPM0_BASEADDR->CONTROLS[COLOR_2_channel].CnV;
			//Zachycena nastupna hrana signalu - echo signal senzoru, delka pulza pak znamena vzdalenost
		   if (!risigneEdgeCapturedColor2)
		   {
			   //PRINTF("COLOR 2 RISING\r\n");
			   risingEdgeTimeColor2 = aktualniHodnotaCasovaceColor2;
			   risigneEdgeCapturedColor2 = true;
			   overflowCountColor2 = 0;
			   //Zmena zachycavani na sestupnou hranu
			   TPM_SetupInputCapture(TPM0_BASEADDR, COLOR_2_channel, kTPM_FallingEdge);
		   }
		   else
		   {

			   fallingEdgeTimeColor2 = aktualniHodnotaCasovaceColor2;
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
	//Hranice pred kterou se vypnout motory (pro soutez NXP je jiz zpomalena rychlost, proto staci jen zastavit a netreba zpomalovat)
	hranice = 35;
	//PRINTF("%d %d\r\n", d1,d2);
	if(driving && dokoncenoKolo)
	{
		if(d1 < hranice | d2 < hranice)
		{
		MotorSetSpeed(0);


		SteerStraight();
		PRINTF("OBSTACLE DETECTED %d %d\r\n", d1,d2);

		UART2_SendTextToHC05("OBST");
		UART2_SendToHC05();
		StopAll();
		led_M();
		}
	}
}
