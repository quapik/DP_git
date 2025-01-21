/*
 * pixySPI.c
 *
 *  Created on: 17. 1. 2025
 *      Author: xsimav01
 */
#include "pixySPI.h"

#define DRIVER_MASTER_SPI Driver_SPI0

#define TRANSFER_SIZE     32U     /* Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /* Transfer baudrate - 500k */

uint8_t actual_tranfser_size = 0;
uint8_t masterRxData[TRANSFER_SIZE] = {0};
uint8_t masterTxData[TRANSFER_SIZE] = {0};

volatile bool isTransferCompleted = false;
volatile bool isMasterOnTransmit  = false;
volatile bool isMasterOnReceive   = false;

void PixyInit(void)
{
	 DRIVER_MASTER_SPI.Initialize(SPI_MasterSignalEvent_t);
	 DRIVER_MASTER_SPI.PowerControl(ARM_POWER_FULL);
	 DRIVER_MASTER_SPI.Control(ARM_SPI_MODE_MASTER | ARM_SPI_SS_MASTER_HW_OUTPUT, TRANSFER_BAUDRATE);}

void SPI_MasterSignalEvent_t(uint32_t event)
{
    if (true == isMasterOnReceive)
    {
        PRINTF("This is SPI_MasterSignalEvent_t\r\n");
        PRINTF("Master receive data from slave has completed!\r\n");
        isMasterOnReceive = false;
    }
    if (true == isMasterOnTransmit)
    {
        PRINTF("This is SPI_MasterSignalEvent_t\r\n");
        PRINTF("Master transmit data to slave has completed!\r\n");
        isMasterOnTransmit = false;
    }
    isTransferCompleted = true;
}
void PixyGetVectors()
{

	actual_tranfser_size = 10;
	masterTxData[0] = 174U;
	masterTxData[1] = 193U;
	masterTxData[2] = 48U;
	masterTxData[3] = 2U;
	masterTxData[4] = 1U;
	masterTxData[5] = 1U;


	isTransferCompleted = false;

		DRIVER_MASTER_SPI.Transfer(masterTxData, masterRxData, 64);
		while (!isTransferCompleted)
			{}

	uint8_t pocet_vektoru = masterRxData[20]/6;
	uint16_t offset = 20;
	PRINTF("POCET VEKTORU %u  \r\n", pocet_vektoru);
	int8_t x0, y0, x1, y1, index = 0;
	int8_t delka, smer = 0;
	while(pocet_vektoru>0)
	{
		x0 = masterRxData[offset+1];
		y0 = masterRxData[offset+2];
		x1 = masterRxData[offset+3];
		y1 = masterRxData[offset+4];
		index = masterRxData[offset+5];
		PRINTF("[x0,y0]-[%u,%u]      [x1,y1]-[%u,%u]      index  %u \r\n",x0,y0,x1,y1,index);
		delka = y0 -y1;
		smer = x1 - x0;
		if(smer < 0) PRINTF("ZAPORNO");
		PRINTF("DELKA  %i SMER %i \r\n", delka, smer);
		pocet_vektoru--;
		offset = offset + 6;
	}


	/*
	for (int i = 0; i < 64; i++)
	{
		PRINTF("%u . %u\r\n", i, masterRxData[i]);
	}
	*/
	PRINTF("------------------------------------\r\n");
}


void PixySetServos(uint16_t s1, uint16_t s2)
{
	/*
	SetServos(s0, s1)
	Request:
	Byte	Description	Value(s)
	0 - 1	16-bit sync	174, 193 (0xc1ae)
	2	Type of packet	18
	3	Length of payload	4
	4 - 5	16-bit s0 - pan servo value	0 - 511
	6 - 7	16-bit s1 - tilt servo value	0 - 511
	 */
	if(s1 > 511) s1 = 511;
	if(s1 < 0) s1 = 0;
	if(s2 > 511) s2 = 511;
	if(s2 < 0) s2 = 0;
	actual_tranfser_size = 10;
	masterTxData[0] = 174U;
	masterTxData[1] = 193U;
	masterTxData[2] = 18U;
	masterTxData[3] = 4U;
	masterTxData[4] = s1 & 0xFF;
	masterTxData[5] = (s1 >> 8) & 0xFF;
	masterTxData[6] = s2 & 0xFF;
	masterTxData[7] = (s2 >> 8) & 0xFF;

	isTransferCompleted = false;

		DRIVER_MASTER_SPI.Transfer(masterTxData, masterRxData, actual_tranfser_size);
		while (!isTransferCompleted)
			{}

}

void PixySetLED(uint8_t r, uint8_t g ,uint8_t b)
{
	/*
	 SetLED(r, g, b)
	Request:
	Byte	Description	Value(s)
	0 - 1	16-bit sync	174, 193 (0xc1ae)
	2	Type of packet	20
	3	Length of payload	3
	4	r - Red value	0 - 255
	5	g - Green value	0 - 255
	6	b - Blue value	0 - 255
	 */
	actual_tranfser_size = 10;
	masterTxData[0] = 174U;
	masterTxData[1] = 193U;
	masterTxData[2] = 20U;
	masterTxData[3] = 3U;
	masterTxData[4] = r;
	masterTxData[5] = g;
	masterTxData[6] = b;

	isTransferCompleted = false;

	DRIVER_MASTER_SPI.Transfer(masterTxData, masterRxData, actual_tranfser_size);
	while (!isTransferCompleted)
		{}

}

void PixyGetResolution(void)
{
}

void PixySetLamp(uint8_t upper, uint8_t lower)
{
	/*Byte	Description	Value(s) setLamp(upper, lower)
	0 - 1	16-bit sync	174, 193 (0xc1ae)
	2	Type of packet	22
	3	Length of payload	2
	4	Upper - turn on the two white LEDs along Pixy2 top edge	0 (off) or 1 (on)
	5	Lower - turn on all channels of lower RGB LED	0 (off) or 1 (on)
	*/
	actual_tranfser_size = 10;
	masterTxData[0] = 174U;
	masterTxData[1] = 193U;
	masterTxData[2] = 22U;
	masterTxData[3] = 2U;
	masterTxData[4] = upper;
	masterTxData[5] = lower;

	isTransferCompleted = false;

	DRIVER_MASTER_SPI.Transfer(masterTxData, masterRxData, actual_tranfser_size);
	while (!isTransferCompleted)
		{}

}
