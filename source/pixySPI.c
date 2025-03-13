/*
 * pixySPI.c
 *
 *  Created on: 17. 1. 2025
 *      Author: xsimav01
 */
#include "pixySPI.h"

#define DRIVER_MASTER_SPI Driver_SPI0
#define SPI_IRQN SPI0_IRQn

#define TRANSFER_SIZE     16U     /* Transfer dataSize */
#define TRANSFER_BAUDRATE 2000000U /* Transfer baudrate - 500k */

uint8_t actual_tranfser_size = 0;
uint8_t masterRxData[TRANSFER_SIZE] = {0};
uint8_t masterTxData[TRANSFER_SIZE] = {0};

uint8_t masterRxDataVECTORS[64] = {0};
uint8_t masterTxDataVECTORS[6] = {174U, 193U, 48U, 2U, 1U, 1U};
bool pixyInitFinished = false;
volatile bool SPI_Finished = false;

uint8_t vector_index;
uint8_t x0;
uint8_t x1;
uint8_t y0;
uint8_t y1;
uint8_t pocet_vektoru;
uint8_t pocet_vektoru_i;

uint8_t x_podstatny;
uint8_t y_podstatny;


uint8_t x_pocatecni;
uint8_t x_koncove;

int8_t delka, smer;
int16_t offset;

int16_t pomer = 0;
bool finish_line_detected_vzdalena = false;
bool finish_line_detected_blizka = false;
uint8_t horizontal_line_counter = 0;

void KontrolaVektoru(void)
{
	delka = abs(y0 -y1); //vzdalenost v ose Y
	smer = abs(x0 - x1); //vzdalenost v ose X

	//vyssi y znamena ze je bod blize modelu (kamere), tudiz je pocatecni
	x_pocatecni = (y0 >= y1) ? x0 : x1;
	x_koncove = (y0 >= y1) ? x1 : x0;


	if(delka == 0) pomer = 0; //rovna horizontalni cara
	else
	{
		//kvuli printu floatu  (hodnota pomeru ja 100x vyssi nez realny pomer)
		if(smer == 0) smer = 1;
		pomer = abs((delka*100 / smer*100)/100);
	}
	if(pomer < 50 && delka < 10) //maly pomer (doky tomu ze je vetsi smer a mala az nulova delka (velikost v ose Y)
	{
		if((y0+y1)/2 > 40)
		{
			PRINTF("HORIZONTAL CARA  BLIZKO  ");
			finish_line_detected_blizka = true;
		}
		else
		{
			PRINTF("HORIZONTAL CARA  DALEKO  ");
			finish_line_detected_vzdalena = true;
		}
	}
	//FULL STRAIGHT a jdou videt obe cary (model smeruje cca rovne a je mezi carama)
	if (delka > 10 && pomer > 100 && pomer < 1000 && pocet_vektoru > 1)
	{
		if((x0+x1)/2 < 39) //LEFT
		{
			if(pomer > 300)
			{
				PRINTF("LEFT STRAIGHT correction %u ");
			}
			else
			{
				x_podstatny = (x0 <= x1) ? x0 : x1; //leva cast, chci levejsi index (je na pocatku a pozdeji se presouva doprava)
				PRINTF("LEFT STRAIGHT %u ", x_podstatny);
			}


		}
		else //RIGHT
		{
			if(pomer > 300)
			{
				PRINTF("RIGHT STRAIGHT correction %u ");
			}
			else
			{
				x_podstatny = (x0 >= x1) ? x0 : x1;
							PRINTF("RIGHT STRAIGHT %u ", x_podstatny);
			}

		}
	}

	//pouze jeden vektor, model je lehce natocen a smeruje ven z drahy pres caru
	//bude treba udelat vetsi korekci
	if(pocet_vektoru == 1 && delka > 20 && smer>10)
	{
		//vektor smeruje do leva, je treba udelat korekci
		if(x_pocatecni >= x_koncove)
		{
			PRINTF("RIGHT STRAIGHT, ONE line, bigger correction");
		}
		//vektor smeruje doprava treba udelat korekci
		else
		{
			PRINTF("LEFT STRAIGHT, ONE line, bigger correction");
		}


	}



	PRINTF("[%u,%u] [%u,%u] delka %d smer %d pomer %d index  %u \r\n",x0,y0,x1,y1, delka, smer,  pomer, vector_index);
}

void PixyZpracujVektory(void)
{
	//TODO deklarace pryc z fce
	/*
	for(int i = 0; i < 64; i++)
	{
		PRINTF("%u . value %u\r\n", i, masterRxDataVECTORS[i]);
	}
	*/
	pocet_vektoru = masterRxDataVECTORS[20]/6;

	//Na 20te pozici zacinaji data s informaci o prvnim pixelu
	offset = 20;
	PRINTF("POCET VEKTORU %u  \r\n", pocet_vektoru);
	pocet_vektoru_i = pocet_vektoru;

	while(pocet_vektoru_i>0)
	{
		x0 = masterRxDataVECTORS[offset+1];
		y0 = masterRxDataVECTORS[offset+2];
		x1 = masterRxDataVECTORS[offset+3];
		y1 = masterRxDataVECTORS[offset+4];
		vector_index = masterRxDataVECTORS[offset+5];
		KontrolaVektoru();
		//PRINTF("[x0,y0]-[%u,%u]      [x1,y1]-[%u,%u]      vector_index  %u \r\n",x0,y0,x1,y1,vector_index);
		//if(smer < 0) PRINTF("ZAPORNO");
		//PRINTF("DELKA  %i SMER %i \r\n", delka, smer);
		pocet_vektoru_i--;
		offset = offset + 6;
	}
	PixyGetVectors();

	PRINTF("------------------------------------\r\n");


}

void PixyStart(void)
{
    PixyInit();
    SDK_DelayAtLeastUs(100*1000, MHZ48);
    PixySetLamp(0,0);
    PixySetLamp(1,1);
    PixySetServos(0, 400);
    PixySetLED(0,255,255);
    SDK_DelayAtLeastUs(100*1000, MHZ48);
    pixyInitFinished = true;
    PIT_timer0_start();

    //TADY MAS PROBLEM, tohle kdyz zavolas tak se ti to zacykli
    //je potreba to vyresit aby se to neseklo
	PixyGetVectors();
}

void PixyInit(void)
{
	 DRIVER_MASTER_SPI.Initialize(SPI_IRQ_HANDLER);
	 NVIC_SetPriority(SPI_IRQN, 3);
	 DRIVER_MASTER_SPI.PowerControl(ARM_POWER_FULL);
	 DRIVER_MASTER_SPI.Control(ARM_SPI_MODE_MASTER | ARM_SPI_SS_MASTER_HW_OUTPUT | ARM_SPI_CPOL1_CPHA1, TRANSFER_BAUDRATE);
}

void SPI_IRQ_HANDLER(uint32_t e)
{
	//PRINTF("SPI IRQ\r\n");
	if (e & ARM_SPI_EVENT_TRANSFER_COMPLETE)
	    {
	        SPI_Finished = true;
	    }
	/*
	if(pixyInitFinished)
		{
		uint8_t pocet_vektoruSS = masterRxDataVECTORS[20]/6;
		PRINTF("POCET VEKTORUSS %u  \r\n", pocet_vektoruSS);
		PixyGetVectors();
		}
	*/
}
void PixyGetVectors(void)
{
	//PRINTF("SPI VECTORS SEND \r\n");
	actual_tranfser_size = 10;
	SPI_Finished = false;

	//CO TU TECH 32, mozna potreba vic pro vic vektoru?
	DRIVER_MASTER_SPI.Transfer(masterTxDataVECTORS, masterRxDataVECTORS, 64);

	/*
	DRIVER_MASTER_SPI.Send(masterTxDataVECTORS,6);

	while (!SPI_Finished)
		{}
	SPI_Finished = false;

	DRIVER_MASTER_SPI.Receive(masterRxDataVECTORS,64);
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
