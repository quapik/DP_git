/*
 * 	globals.c
 *
 * Soubor s globálními proměnnými a startovací funkcí
 *
 *
 *  Created on: 14. 1. 2025
 *  Author: Vojtěch Šíma
 *  Diplomová práce  Samořiditelný model autíčka pro NXP Cup
 *  2024/2025
 */
#include "globals.h"


//MOTORY
bool jedouMotory = false;
bool driving = false;
 bool logujPeriodicky = false;
uint8_t pctMotory = 0;
int8_t pctServoR = 0;
int8_t pctServoL = 0;


bool isObstacleDetected = false;
bool startMotorsButtonPressed = false;

//IR TRACKERY a SENZOR
bool ir_trackers[6] = {false,false,false,false,false,false};
uint16_t IR_sensor_cm = 0;
uint32_t IR_sensor_raw = 0;
bool IR_mesure_done = false;

//SENZORY BARVY
uint16_t COLOR1_value_global = 0;
uint16_t COLOR2_value_global = 0;

uint16_t SRF_distance1_global = 0;
uint16_t SRF_distance2_global = 0;

bool probihaZmena = false;
uint8_t actualTrigger = 1;
bool isTriggerTriggering = false;

bool jedePixy = false;
uint8_t importantVector[4] = {0,0,0,0};
uint8_t importantVectorIndex = 0;
uint8_t primaryVector[4] = {0,0,0,0};
uint8_t primaryVectorIndex = 0;
uint8_t secondaryVector[4] = {0,0,0,0};
uint8_t secondaryVectorIndex = 0;
uint8_t pocetVektoruGlobal = 0;

//HALL SENSORY
uint16_t otackyRight =  0;
uint16_t otackyLeft = 0;
uint32_t otackyRightTicks = 0;
uint32_t otackyLeftTicks = 0;

bool dokoncenoKolo = false;

volatile uint32_t g_systickCounter = 0;

void SysTick_Handler(void) {
    g_systickCounter++;
}

void SysTick_Init(void)
{
	if (SysTick_Config(SystemCoreClock/1000U)) //uS
		{
			while (1)
			{
			}
		}
}

void StopAll(void)
{
	led_R();
	startMotorsButtonPressed = false;
	PRINTF("VSECHNO ZASTAVENO \r\n");
	MotorSetSpeed(0);
	SteerStraight();
	PIT_StopPixyZpracovavatVektory();
	if(logujPeriodicky)LPTMR_StopPosilejUART();
	PIT_StopZpracujBarvuIRSensor();
	driving = false;
	jedePixy = false;
}

void StartAll(void)
{
	g_systickCounter = 0;
	led_G();
	startMotorsButtonPressed = true;
	PRINTF("VSECHNO SPUSTENO \r\n");
	HallResetValues();
	SteerStraight();
	if(logujPeriodicky)LPTMR_StartPosilejUART();
	PIT_StartPixyZpracovavatVektory();
	PIT_StartZpracujBarvuIRSensor();
	PixyGetVectors();
	MotorSetSpeed(15);
	driving = true;

	jedePixy = true;
	dokoncenoKolo = false;
}





