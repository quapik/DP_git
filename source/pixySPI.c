/*
 * pixySPI.c
 *
 *  Created on: 17. 1. 2025
 *      Author: xsimav01
 */
#include "pixySPI.h"

#define DRIVER_MASTER_SPI Driver_SPI0

#define TRANSFER_SIZE     16U     /* Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /* Transfer baudrate - 500k */

uint8_t actual_tranfser_size = 0;
uint8_t masterRxData[TRANSFER_SIZE] = {0};
uint8_t masterTxData[TRANSFER_SIZE] = {0};

uint8_t masterRxDataVECTORS[TRANSFER_SIZE] = {0};
uint8_t masterTxDataVECTORS[TRANSFER_SIZE] = {174U, 193U, 48U, 2U, 1U, 1U};
bool pixyInitFinished = false;
volatile bool SPI_Finished = false;


void PixyZpracujVektory(void)
{
	//TODO deklarace pryc z fce
	uint8_t pocet_vektoru = masterRxDataVECTORS[20]/6;
	uint16_t offset = 20;
	PRINTF("POCET VEKTORU %u  \r\n", pocet_vektoru);
	int8_t x0, y0, x1, y1, index = 0;
	int8_t delka, smer = 0;
	while(pocet_vektoru>0)
	{
		x0 = masterRxDataVECTORS[offset+1];
		y0 = masterRxDataVECTORS[offset+2];
		x1 = masterRxDataVECTORS[offset+3];
		y1 = masterRxDataVECTORS[offset+4];
		index = masterRxDataVECTORS[offset+5];
		PRINTF("[x0,y0]-[%u,%u]      [x1,y1]-[%u,%u]      index  %u \r\n",x0,y0,x1,y1,index);
		delka = y0 -y1;
		smer = x1 - x0;
		if(smer < 0) PRINTF("ZAPORNO");
		PRINTF("DELKA  %i SMER %i \r\n", delka, smer);
		pocet_vektoru--;
		offset = offset + 6;
	}

	PRINTF("------------------------------------\r\n");


}
/*
void SetTxForVectors()
{
	masterTxDataVECTORS[0] = 174U;
	masterTxDataVECTORS[1] = 193U;
	masterTxDataVECTORS[2] = 48U;
	masterTxDataVECTORS[3] = 2U;
	masterTxDataVECTORS[4] = 1U;
	masterTxDataVECTORS[5] = 1U;
}
*/
void PixyStart(void)
{
    PixyInit();
    SDK_DelayAtLeastUs(100*1000, MHZ48);
    PixySetLamp(1,1);
    PixySetServos(0, 400);
    PixySetLED(0,255,255);
    SDK_DelayAtLeastUs(100*1000, MHZ48);
    pixyInitFinished = true;
    PIT_timer0_start();

    //TADY MAS PROBLEM, tohle kdyz zavolas tak se ti to zacykli
    //je potreba to vyresit aby se to neseklo
	//PixyGetVectors();
}

void PixyInit(void)
{
	 DRIVER_MASTER_SPI.Initialize(SPI_IRQ_HANDLER);
	 DRIVER_MASTER_SPI.PowerControl(ARM_POWER_FULL);
	 DRIVER_MASTER_SPI.Control(ARM_SPI_MODE_MASTER | ARM_SPI_SS_MASTER_HW_OUTPUT, TRANSFER_BAUDRATE);}

void SPI_IRQ_HANDLER(uint32_t e)
{
	//PRINTF("SPI IRQ\r\n");
	if (e & ARM_SPI_EVENT_TRANSFER_COMPLETE)
	    {
	        SPI_Finished = true;
	    }
	if(pixyInitFinished) PixyGetVectors();
}
void PixyGetVectors(void)
{

	actual_tranfser_size = 10;
	SPI_Finished = false;

	//CO TU TECH 32, mozna potreba vic pro vic vektoru?
	DRIVER_MASTER_SPI.Transfer(masterTxDataVECTORS, masterRxDataVECTORS, 64);
	/*
	while (!SPI_Finished)
		{}
	 */
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

	SPI_Finished = false;

		DRIVER_MASTER_SPI.Transfer(masterTxData, masterRxData, actual_tranfser_size);
		while (!SPI_Finished)
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

	SPI_Finished = false;

	DRIVER_MASTER_SPI.Transfer(masterTxData, masterRxData, actual_tranfser_size);
	while (!SPI_Finished)
		{}

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

	SPI_Finished = false;

	DRIVER_MASTER_SPI.Transfer(masterTxData, masterRxData, actual_tranfser_size);
	while (!SPI_Finished)
		{}

}
