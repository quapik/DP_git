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
#include "globals.h"
#include "math.h"

#define SPI_PIXY_DRIVER Driver_SPI0
#define SPI_IRQN SPI0_IRQn
#define TRANSFER_SIZE     16U
#define TRANSFER_BAUDRATE 2000000U

void SPI_IRQ_HANDLER(uint32_t e);
void PixyInit(void);
void PixySetLamp(uint8_t upper, uint8_t lower);
void PixySetLED(uint8_t r, uint8_t g ,uint8_t b);
void PixyGetResolution(void);
void PixySetServos(uint16_t s1, uint16_t s2);
void PixyGetVectors(void);
void ProccessVectors(void);
void PixyStart(void);
void CheckVector(void);
void SaveImportantVector(void);
void SaveSecondaryVector(void);
void SaveVectorToStruct(uint8_t index, uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1, int16_t pomer);
void KontrolaVektoru(void);


#endif /* PIXYSPI_H_ */
