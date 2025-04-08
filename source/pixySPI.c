/*
 * pixySPI.c
 *
 *  Created on: 17. 1. 2025
 *      Author: xsimav01
 */


#include "pixySPI.h"

#define SPI_PIXY_DRIVER Driver_SPI0
#define SPI_IRQN SPI0_IRQn

#define TRANSFER_SIZE     16U     /* Transfer dataSize */
#define TRANSFER_BAUDRATE 2000000U /* Transfer baudrate - 500k */

char buffer[256];
uint8_t offsetBuffer = 0;

uint8_t base_value = 20;


uint8_t actual_tranfser_size = 0;
uint8_t masterRxData[TRANSFER_SIZE] = {0};
uint8_t masterTxData[TRANSFER_SIZE] = {0};

uint8_t masterRxDataVECTORS[64] = {0};
uint8_t masterTxDataVECTORS[6] = {174U, 193U, 48U, 2U, 1U, 1U}; // ALL FEATURES
//uint8_t masterTxDataVECTORS[6] = {174U, 193U, 48U, 2U, 0U, 1U}; // MAIN FEATURES
bool pixyInitFinished = false;
volatile bool SPI_Finished = false;
volatile bool SPI_FinishedBlocking = false;

uint8_t vector_index;
uint8_t x_0;
uint8_t x_1;
uint8_t y_0;
uint8_t y_1;
uint8_t pocet_vektoru;
uint8_t pocet_vektoru_i;

uint8_t x_podstatny;
uint8_t y_podstatny;

uint8_t aktualniHodnotaKZatoceni;
bool aktualniHodnotaKZatoceniLEFT = false;
bool IsPrimaryVector = true;
bool bylaZmenenaHodnotaRychlosti = false;

uint8_t x_pocatecni;
uint8_t x_koncove;
uint8_t y_pocatecni;
uint8_t y_koncove;

int8_t delka, smer;
int16_t offset;

int16_t pomer = 0;
uint8_t horizonta_counter = 0;
bool finish_line_detected_vzdalena = false;
bool finish_line_detected_blizka = false;
uint8_t horizontal_line_counter = 0;

bool zaznamenanaKorekce = false;
bool aspon_jedna_kolma_dvojice = false;

typedef struct {
    uint8_t index;
    uint8_t x_0, y_0;
    uint8_t x_1, y_1;
    int16_t pomer;
    uint8_t i;
} VectorStruct;

#define MAX_VECTORS 20
VectorStruct vektory[MAX_VECTORS];

void UlozVektor(uint8_t index, uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1, int16_t pomer) {
    if (pocet_vektoru < MAX_VECTORS) {
        vektory[pocet_vektoru_i].index = index;
        vektory[pocet_vektoru_i].x_0 = x_0;
        vektory[pocet_vektoru_i].y_0 = y_0;
        vektory[pocet_vektoru_i].x_1 = x_1;
        vektory[pocet_vektoru_i].y_1 = y_1;
        vektory[pocet_vektoru_i].pomer = pomer;
        vektory[pocet_vektoru_i].i = pocet_vektoru_i;
    }
}
void SaveImportantVektor(void)
{
	importantVectorIndex = vector_index;
	importantVector[0]=x_pocatecni;
	importantVector[1]=y_pocatecni;
	importantVector[2]=x_koncove;
	importantVector[3]=y_koncove;
}
void KontrolaVektoru(void)
{
	delka = abs(y_0 -y_1); //vzdalenost v ose Y
	smer = abs(x_0 - x_1); //vzdalenost v ose X

	//vyssi y znamena ze je bod blize modelu (kamere), tudiz je pocatecni
	x_pocatecni = (y_0 >= y_1) ? x_0 : x_1;
	x_koncove = (y_0 >= y_1) ? x_1 : x_0;

	y_pocatecni = (y_0 >= y_1) ? y_0 : y_1;
	y_koncove = (y_0 >= y_1) ? y_1 : y_0;

	//Pokud se maji logovat jenom vektory, ukladej je a pozdeji budou naraz posleny
	//Promenna se nastavuje v dp_main.c
	if(logujJenomVektory)
	{
		if(pocet_vektoru_i == 1)
			{
				offsetBuffer += snprintf(&buffer[offsetBuffer], sizeof(buffer) - offsetBuffer,
						                       "%d,%d,%d,%d;%d\r\n", x_pocatecni, y_pocatecni, x_koncove, y_koncove, vector_index);
			}
			else
			{
				offsetBuffer += snprintf(&buffer[offsetBuffer], sizeof(buffer) - offsetBuffer,
						                       "%d,%d,%d,%d,%d;", x_pocatecni, y_pocatecni, x_koncove, y_koncove,vector_index);
			}
	}


	if(IsPrimaryVector)
	{
		aktualniHodnotaKZatoceni = 0;
		aktualniHodnotaKZatoceniLEFT = false;
		primaryVector[0]=x_pocatecni;
		primaryVector[1]=y_pocatecni;
		primaryVector[2]=x_koncove;
		primaryVector[3]=y_koncove;
		primaryVectorIndex = vector_index;
		SaveImportantVektor();
	}
	//UlozVektor(vector_index,x_pocatecni,y_pocatecni,x_koncove,y_koncove,0);

	if(delka == 0) pomer = 0; //rovna horizontalni cara
	else
	{
		//kvuli printu floatu  (hodnota pomeru ja 100x vyssi nez realny pomer)
		if(smer == 0) smer = 1;
		pomer = abs((delka*100 / smer*100)/100);
	}


	if(pomer < 50 && delka < 10) //maly pomer (doky tomu ze je vetsi smer a mala az nulova delka (velikost v ose Y)
	{
		if((y_0+y_1)/2 > 40)
		{
			if(x_0 > 10 && x_1 > 10 && x_0 && x_0 < 60 && x_1 < 60)
						{
						//PRINTF("HORIZONTAL CARA  BLIZKO  ");
						motor_set_speed(0);
						finish_line_detected_blizka = true;
						PIT_StopPixyZpracovavatVektory();
						LPTMR_StopPosilejUART();
						driving = false;
						dokoncenoKolo = true;
						UART2_SendTextToHC05("HOR!");
						SaveImportantVektor();
						}
		}
	}
	//MAME ROVINKU, JDE VIDET POUZE JEDNA CARA, JE DLOUHOA ALE ROVNA
	if(IsPrimaryVector) //if(pocet_vektoru == 1)
	{
		if(smer < 10 && delka > 40)
		{
			if(x_pocatecni > 38)
			{
				if(x_pocatecni < 60)
				{
					PRINTF("PRAVA, HODNE ROVNA, 75 LEFT ");
					aktualniHodnotaKZatoceni = 65;
					aktualniHodnotaKZatoceniLEFT = true;
				}
				else
				{
					PRINTF("PRAVA, HODNE ROVNA, 50 LEFT ");
					aktualniHodnotaKZatoceni = 40;
					aktualniHodnotaKZatoceniLEFT = true;
				}
			}
			else
			{
				if(x_pocatecni > 18)
				{
					PRINTF("LEVA, HODNE ROVNA, 75 RIGHT ");
					aktualniHodnotaKZatoceni = 65;
					aktualniHodnotaKZatoceniLEFT = false;
				}
				else
				{
					PRINTF("LEVA, HODNE ROVNA, 50 RIGHT ");
					aktualniHodnotaKZatoceni = 40;
					aktualniHodnotaKZatoceniLEFT = false;
				}
			}
		}
	}
	//pouze jeden vektor, model je lehce natocen a smeruje ven z drahy pres caru
	if(IsPrimaryVector && delka > 20 && smer>10) //if(pocet_vektoru == 1 && delka > 20 && smer>10)
	{
		//vektor smeruje do leva, je treba udelat korekci
		if(x_pocatecni >= x_koncove)
		{
			if(x_pocatecni < 70)
			{
				if(smer > 40)
				{
					PRINTF("PRAVA, HODNE SIKMA, 75 LEFT");
					aktualniHodnotaKZatoceni = 75;
				}
				else
				{
					PRINTF("PRAVA, SIKMA, 50 LEFT");
					aktualniHodnotaKZatoceni = 40;
				}
				aktualniHodnotaKZatoceniLEFT = true;
			}
		}
		//vektor smeruje doprava treba udelat korekci
		else
		{
			if(x_pocatecni > 10)
			{
				if(smer > 40)
				{
					PRINTF("LEVA, HODNE SIKMA, 75 RIGHT");
					aktualniHodnotaKZatoceni = 75;
			}
			else
			{
					PRINTF("LEVA,  SIKMA, 50 RIGHT");
					aktualniHodnotaKZatoceni = 40;

				}
			}
			aktualniHodnotaKZatoceniLEFT = false;
		}
	}
	//DETEKCE VICEMENE HORIZONTALNI CARY, CO ZNACI VRCHOL ZATACKY, zatacka muze byt kriticka, proto je tady
	//je treba zjistit smer a podle toho zatacet
	if(IsPrimaryVector)
	{
		if(smer > 30 && delka > 10)
			{
				//PRINTF("x_pocatecni %u x_koncove %u\r\n", x_pocatecni, x_koncove);
				if(x_pocatecni > x_koncove)
				{
					PRINTF("VRCHOL ZATACKY DOLEVA, 75\r\n");
					aktualniHodnotaKZatoceni = 60;
					aktualniHodnotaKZatoceniLEFT = true;
				}
				else
				{
					PRINTF("VRCHOL ZATACKY DOPRAVa, 75\r\n");
					aktualniHodnotaKZatoceniLEFT = false;
					aktualniHodnotaKZatoceni = 60;
				}
			}
	}


	PRINTF("[%u,%u] [%u,%u] delka %d smer %d pomer %d index  %u \r\n",x_0,y_0,x_1,y_1, delka, smer,  pomer, vector_index);
}

void PixyZpracujVektory(void)
{
	zaznamenanaKorekce = false;

	pocet_vektoru = masterRxDataVECTORS[20]/6;

	//Na 20te pozici zacinaji data s informaci o prvnim pixelu
	offset = 20;
	PRINTF("POCET VEKTORU %u  \r\n", pocet_vektoru);
	pocet_vektoru_i = pocet_vektoru;
	IsPrimaryVector = true;
	bylaZmenenaHodnotaRychlosti = false;
	offsetBuffer = 0;
	while(pocet_vektoru_i>0)
	{
		//Ziskani hodnot z prijateho bufferu  (zacinaji na offsetu ktery se vzdy posune)
		x_0 = masterRxDataVECTORS[offset+1];
		y_0 = masterRxDataVECTORS[offset+2];
		x_1 = masterRxDataVECTORS[offset+3];
		y_1 = masterRxDataVECTORS[offset+4];
		vector_index = masterRxDataVECTORS[offset+5];
		KontrolaVektoru();
		IsPrimaryVector = false;
		//PRINTF("[x_0,y_0]-[%u,%u]      [x_1,y_1]-[%u,%u]      vector_index  %u \r\n",x_0,y_0,x_1,y_1,vector_index);
		//if(smer < 0) PRINTF("ZAPORNO");
		//PRINTF("DELKA  %i SMER %i \r\n", delka, smer);
		pocet_vektoru_i--;
		offset = offset + 6;
	}

	//VykstujiSeKolmeVektory();
	//if(aspon_jedna_kolma_dvojice) PRINTF("KOLMOOOOOOOOOOOOOOOOOOOOOOOO\r\n");
	//aspon_jedna_kolma_dvojice = false;
	if(pocet_vektoru > 0)
	{
		//odeslani vsech zaznamenanych vektoru
		if(logujJenomVektory) UART2_SendVectorsBuffer(buffer,offsetBuffer);
		if(aktualniHodnotaKZatoceni == 0)
		{
			steer_straight();
			if(driving)
			{
			 if(!bylaZmenenaHodnotaRychlosti && !dokoncenoKolo) motor_set_speed(base_value);

			}
		}
		else
		{
			//ZATOC DOELVA
			if(aktualniHodnotaKZatoceniLEFT)
			{
				steer_left(aktualniHodnotaKZatoceni);
			}
			//ZATOC DOPRAVA
			else
			{
				steer_right(aktualniHodnotaKZatoceni);
			}
			//Prvne kontrola zda jiz nebyla korekce rychlosti kvuli vyssi priorite (zpomalovani horizontal cara)
			if(!bylaZmenenaHodnotaRychlosti)
			{
				if(dokoncenoKolo == false)
				{
					if(driving)
					{
						if(aktualniHodnotaKZatoceni >= 75) motor_set_speed(5);
						else if(aktualniHodnotaKZatoceni >= 50) motor_set_speed(10);
						else if(aktualniHodnotaKZatoceni >= 25) motor_set_speed(15);
					}
				}
			}
		}
	}
	PixyGetVectors();
	PRINTF("------------------------------------\r\n");
}

int16_t JsouVektoryKolme(uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1,
                      uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3)
{
    int16_t Ax = x_1 - x_0;
    int16_t Ay = y_1 - y_0;
    int16_t Bx = x3 - x2;
    int16_t By = y3 - y2;

    int16_t skalarni_soucin = (Ax * Bx + Ay * By);
    // Výpočet délky vektorů
	float delka_A = sqrt(Ax * Ax + Ay * Ay);
	float delka_B = sqrt(Bx * Bx + By * By);

	// Výpočet kosinu úhlu mezi vektory
	float cos_theta = (float)skalarni_soucin / (delka_A * delka_B);
	int16_t cos_theta_D = (float)skalarni_soucin / (delka_A * delka_B)*1000;


	// Výpis hodnot pro kontrolu
	PRINTF("AX %d AY %d BX %d BY %d\r\n", Ax, Ay, Bx, By);
	PRINTF("Skalarni soucin: %d\r\n", skalarni_soucin);
	PRINTF("Kosinus uhlu mezi vektory: %d\r\n", cos_theta_D);

	//priblizne kolme vektory, je ptoreba nejak pracovat s tim prahem
	if (cos_theta > -0.2 && cos_theta < 0.2) {
		PRINTF("Vektory jsou cca kolme.\r\n");
		aspon_jedna_kolma_dvojice = true;
		return 1;  // Vektory jsou přibližně kolmé
	} else {
		PRINTF("Vektory nejsou kolme.\r\n");
		return 0;  // Vektory nejsou kolmé
	}
}

void VykstujiSeKolmeVektory(void)
{
	for(int i = 1; i < pocet_vektoru+1; i++)
	{
		//projdi vsechny vektory
		for(int j = 1; j < pocet_vektoru+1; j++)
		{
			if(j != i)
			{
				int16_t kolmost = JsouVektoryKolme(vektory[i].x_0,vektory[i].y_0, vektory[i].x_1,
						vektory[i].y_1, vektory[j].x_0,vektory[j].y_0, vektory[j].x_1,
						vektory[j].y_1);
				//PRINTF("Kolmost %d  ", kolmost);
			}
		}
	}
}

void PixyStart(void)
{
    PixyInit();
    jedePixy = false;
    SDK_DelayAtLeastUs(100*1000, MHZ48);
    //PixySetLamp(0,0);
    PixySetLamp(1,1);
    PixySetServos(0, 400); //360 //300
    PixySetLED(0,255,255);
    SDK_DelayAtLeastUs(100*1000, MHZ48);
    pixyInitFinished = true;

    PRINTF("Pixy2 Start Finished\r\n");
    //PIT_timer0_start();

    //TADY MAS PROBLEM, tohle kdyz zavolas tak se ti to zacykli
    //je potreba to vyresit aby se to neseklo
	//PixyGetVectors();
}

void PixyInit(void)
{
	 SPI_PIXY_DRIVER.Initialize(SPI_IRQ_HANDLER);
	 NVIC_SetPriority(SPI_IRQN, 3);
	 SPI_PIXY_DRIVER.PowerControl(ARM_POWER_FULL);
	 SPI_PIXY_DRIVER.Control(ARM_SPI_MODE_MASTER | ARM_SPI_SS_MASTER_HW_OUTPUT | ARM_SPI_CPOL1_CPHA1, TRANSFER_BAUDRATE);
}

void SPI_IRQ_HANDLER(uint32_t e)
{
	if (e & ARM_SPI_EVENT_TRANSFER_COMPLETE)
	    {
	        SPI_Finished = true;
	    }
}
void PixyGetVectors(void)
{
	actual_tranfser_size = 10;
	SPI_Finished = false;
	SPI_PIXY_DRIVER.Transfer(masterTxDataVECTORS, masterRxDataVECTORS, 64);
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

		SPI_PIXY_DRIVER.Transfer(masterTxData, masterRxData, actual_tranfser_size);
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

	SPI_PIXY_DRIVER.Transfer(masterTxData, masterRxData, actual_tranfser_size);
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

	SPI_PIXY_DRIVER.Transfer(masterTxData, masterRxData, actual_tranfser_size);
	while (!SPI_Finished)
		{}

}
