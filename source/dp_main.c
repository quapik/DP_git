/*
 * 	dp_main.c
 *
 * Soubor starající se o základní inicializační funkce a běh programu
 *
 *
 *  Created on: 14. 1. 2025
 *  Author: Vojtěch Šíma
 *  Diplomová práce  Samořiditelný model autíčka pro NXP Cup
 *  2024/2025
 */

#include "globals.h"
volatile uint8_t getCharValue = 0U;

uint16_t dc = 0;
uint16_t pct = 0;

bool pixyMainFeatures = false;


int main(void)
{
 	BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("NXP CUP & VOJTECH SIMA MASTER DEGREE THESIS APP, WELCOME \r\n");

    enableInterruptsOnPorts();
    MotorsInit();
    SysTick_Init();

    //Inicializace TMP0 (ultrazvukove a barevne senzory)
    TMP0Init();
    //Inicializace prenosu dat pres uart
    UART2_Init();
    //Inicializace timeru
    PIT_Timer_Init();
    LPTMR_Timer_Init();
    PIT_StartZpracujBarvuIRSensor();

    irSensorInit();

    //Start pro Pixy kameru -  inicalizace,  rozsviti se svetla, nastavi do spravne polohy, nespusti se sledovani vektoru
    PixyStart();


    PRINTF("Vsechny init funkce dokonceny\r\n");
    led_R();

	//Urcuje, zda budou logovany pouze vektory nebo vse pri pohybu a prenosu pre uart
    logujPeriodicky = false;
 	while (1)
 	{

    }
}
