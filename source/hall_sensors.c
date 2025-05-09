/*
 * 	hall_sensors.c
 *
 * Soubor starající se o hallovy senzory
 *
 *  Created on: 14. 3. 2025
 *  Author: Vojtěch Šíma
 *  Diplomová práce  Samořiditelný model autíčka pro NXP Cup
 *  2024/2025
 */
#include "hall_sensors.h"

uint8_t ctvrtkyRight;
float vzdalenostRight;

uint8_t ctvrtkyLeft;
float vzdalenostLeft;


void HallResetValues(void)
{
	ctvrtkyRight = 0;
	ctvrtkyLeft = 0;
	otackyRight =0;
	otackyLeft = 0;
	vzdalenostRight = 0.0;
	vzdalenostLeft = 0.0;
}

void ZmenaHranyHallRight(void)
{
	//ctvrtkyRight++;
	otackyRight++;
	//vzdalenostRight = vzdalenostRight + VZDALENOST_CTVRTKA_OTOCKY;
	/*
	if(ctvrtkyRight == 4)
	{
		ctvrtkyRight = 0;
		otackyRight++;
		//PRINTF("OTOCENI PRAVEHO KOLA %u\r\n", otackyRight);
		//PRINTF("vzdalenost %u\r\n", (int)vzdalenostRight*100);
	}
	*/
}
void ZmenaHranyHallLeft(void)
{
	//ctvrtkyLeft++;
	otackyLeft++;
	//vzdalenostLeft = vzdalenostLeft + VZDALENOST_CTVRTKA_OTOCKY;
	/*
	if(ctvrtkyLeft == 4)
	{
		ctvrtkyLeft = 0;
		otackyLeft++;
		//PRINTF("OTOCENI LEFT %u\r\n", otackyLeft);
		//PRINTF("vzdalenost %u\r\n", (int)vzdalenostLeft*100);
	}
	*/
}

