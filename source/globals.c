/*
 * globals.c
 *
 *  Created on: 20. 2. 2025
 *      Author: xsimav01
 */
#include "globals.h"


//MOTORY
bool jedouMotory = false;
bool driving = false;
uint8_t pctMotory = 0;
int8_t pctServo = 0;


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

//HALL SENSORY
uint16_t otackyRight =  0;
uint16_t otackyLeft = 0;






