/*
 * globals.c
 *
 *  Created on: 20. 2. 2025
 *      Author: xsimav01
 */
#include "globals.h"

bool jedouMotory = false;
uint8_t pctMotory = 0;
int8_t pctServo = 0;
bool isObstacleDetected = false;
bool startMotorsButtonPressed = false;

bool ir_trackers[6] = {false,false,false,false,false,false};

bool driving = false;


