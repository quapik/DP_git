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

//Funkce co pocita ctvrtotacky praveho kola
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
	//Prevence kvůli opakované detekci jednoho magnetu
	if(g_systickCounter - otackyRightTicks > 25)
	{
		otackyRight++;
		otackyRightTicks = g_systickCounter;
	}

}
//Funkce co pocita ctvrtotacky leveho kola
void ZmenaHranyHallLeft(void)
{
	//Prevence kvůli opakované detekci jednoho magnetu
	if(g_systickCounter - otackyLeftTicks > 25)
	{
	otackyLeft++;
	otackyLeftTicks = g_systickCounter;
	}
}

