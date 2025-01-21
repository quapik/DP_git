//Pixy2 communication functions using CMSIS I2C for FRDM-MCXN947 board.
/*

#include "pixy.h"
#include <stdio.h>
#include "fsl_debug_console.h"
//#include "fsl_lpi2c_cmsis.h"



#define EXAMPLE_I2C_MASTER Driver_I2C2
#define I2C_DATA_LENGTH 32
#define LPI2C_CLOCK_FREQUENCY CLOCK_GetLPFlexCommClkFreq(2u)


static volatile Pixy2 Pixy2Instance = {0};
static volatile DetectedVectors Pixy2DetectedVectors = {0};
static uint8_t PixyReceivedLinesBuffer[100U];
static uint8_t PixyLinesRequestCommand[6U] = {174U, 193U, 48U, 2U, 1U, 1U};
static uint8_t PixyLedSetCommand[7U] = {174U, 193U, 20U, 3U, 0U, 0U, 0U};
static uint8_t PixyLedSetReceiveBuffer[10U];



static void lpi2c_master_callback(uint32_t event) {
    if (event == ARM_I2C_EVENT_TRANSFER_DONE) {
        g_MasterCompletionFlag = true;
    }
}


void Pixy2Init(uint8_t PixyI2cAddress, uint8_t PixyI2cChannel){
    EXAMPLE_I2C_MASTER.Initialize(lpi2c_master_callback);
    EXAMPLE_I2C_MASTER.PowerControl(ARM_POWER_FULL);
    EXAMPLE_I2C_MASTER.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);

    Pixy2Instance.I2cAddress = PixyI2cAddress;
    Pixy2Instance.I2cChannel = PixyI2cChannel;
    Pixy2Instance.State = Ready;
}


Pixy2State GetPixy2State(void) {
    return Pixy2Instance.State;
}


void PixySetLed(uint8_t Red, uint8_t Green, uint8_t Blue) {
	PRINTF("Setting Pixy LED to Red: %d, Green: %d, Blue: %d\n", Red, Green, Blue);

    g_MasterCompletionFlag = false;

    PixyLedSetCommand[4] = Red;
    PixyLedSetCommand[5] = Green;
    PixyLedSetCommand[6] = Blue;

    EXAMPLE_I2C_MASTER.MasterTransmit(Pixy2Instance.I2cAddress, PixyLedSetCommand, 7, true);

    while (!g_MasterCompletionFlag) {}
    g_MasterCompletionFlag = false;

    PRINTF("LED command sent. Waiting for response...\n");

    EXAMPLE_I2C_MASTER.MasterReceive(Pixy2Instance.I2cAddress, PixyLedSetReceiveBuffer, 10, true);


    while (!g_MasterCompletionFlag) {}
    g_MasterCompletionFlag = false;

    PRINTF("Received response from Pixy: ");
    for (int i = 0; i < 10; i++) {
    	PRINTF("0x%02X ", PixyLedSetReceiveBuffer[i]);
    }
    PRINTF("\n");

    PRINTF("Pixy LED color change complete.\n");
}


DetectedVectors PixyGetVectors() {
    uint16_t Index = 6U, PacketLength, FeatureLength, VectorsNumber, CurrentVector = 0U, Offset;
    uint8_t* FeatureDataPointer;

    g_MasterCompletionFlag = false;

    EXAMPLE_I2C_MASTER.MasterTransmit(Pixy2Instance.I2cAddress, PixyLinesRequestCommand, 6, false);

    while (!g_MasterCompletionFlag) {}
    g_MasterCompletionFlag = false;

    EXAMPLE_I2C_MASTER.MasterReceive(Pixy2Instance.I2cAddress, PixyReceivedLinesBuffer, 100, false);

    while (!g_MasterCompletionFlag) {}
    g_MasterCompletionFlag = false;

    PacketLength = PixyReceivedLinesBuffer[3U] + 4U;
    while (Index < PacketLength) {
        FeatureLength = PixyReceivedLinesBuffer[Index + 1U];
        VectorsNumber = FeatureLength / 6U;
        FeatureDataPointer = &PixyReceivedLinesBuffer[Index + 2U];
        for (uint8_t FeatureDataIndex = 0U; FeatureDataIndex < VectorsNumber; FeatureDataIndex++) {
            Offset = FeatureDataIndex * 6U;
            Pixy2DetectedVectors.Vectors[CurrentVector].x0 = FeatureDataPointer[Offset];
            Pixy2DetectedVectors.Vectors[CurrentVector].y0 = FeatureDataPointer[Offset + 1U];
            Pixy2DetectedVectors.Vectors[CurrentVector].x1 = FeatureDataPointer[Offset + 2U];
            Pixy2DetectedVectors.Vectors[CurrentVector].y1 = FeatureDataPointer[Offset + 3U];
            Pixy2DetectedVectors.Vectors[CurrentVector].VectorIndex = FeatureDataPointer[Offset + 4U];
            CurrentVector++;
        }
        Index += 2U + FeatureLength;
    }
    Pixy2DetectedVectors.NumberOfVectors = CurrentVector;
    return Pixy2DetectedVectors;
}
*/
