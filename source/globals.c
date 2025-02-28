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

//IR TRACKERY
bool ir_trackers[6] = {false,false,false,false,false,false};


//SENZORY BARVY¨
uint16_t COLOR1_value_global = 0;
uint16_t COLOR2_value_global = 0;




