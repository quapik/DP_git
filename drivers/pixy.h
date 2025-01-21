#ifndef PIXY
#define PIXY

#include <stdint.h>
#include <stdbool.h>
//#include "fsl_lpi2c_cmsis.h"


typedef struct {
    uint16_t x0;
    uint16_t y0;
    uint16_t x1;
    uint16_t y1;
    uint16_t VectorIndex;
} DetectedVector;

typedef struct {
    DetectedVector Vectors[100];
    uint16_t NumberOfVectors;
} DetectedVectors;

typedef enum {
    Uninitialized,
    Ready
} Pixy2State;

typedef struct {
	uint8_t I2cAddress;
    uint8_t I2cChannel;
    Pixy2State State;
} Pixy2;

extern volatile bool g_MasterCompletionFlag;

void Pixy2Init(uint8_t PixyI2cAddress, uint8_t PixyI2cChannel);
Pixy2State GetPixy2State(void);
void PixySetLed(uint8_t Red, uint8_t Green, uint8_t Blue);
DetectedVectors PixyGetVectors(void);

#endif
