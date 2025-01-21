/*
 * pixySPI.h
 *
 *  Created on: 17. 1. 2025
 *      Author: xsimav01
 */

#ifndef PIXYSPI_H_
#define PIXYSPI_H_


#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_spi_cmsis.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"


void SPI_MasterSignalEvent_t(uint32_t event);
void PixyInit(void);
void PixySetLamp(uint8_t upper, uint8_t lower);
void PixySetLED(uint8_t r, uint8_t g ,uint8_t b);
void PixyGetResolution(void);
void PixySetServos(uint16_t s1, uint16_t s2);
void PixyGetVectors();


#endif /* PIXYSPI_H_ */
