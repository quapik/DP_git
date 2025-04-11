/*
 * pixySPI.c
 *
 *  Created on: 17. 1. 2025
 *      Author: xsimav01
 */

#include "pixySPI.h"
char buffer[256];
uint8_t offsetBuffer = 0;

uint8_t base_speed_value = 20;

uint8_t actual_tranfser_size = 0;
uint8_t masterRxData[TRANSFER_SIZE] = {0};
uint8_t masterTxData[TRANSFER_SIZE] = {0};

uint8_t masterRxDataVECTORS[64] = {0};
uint8_t masterTxDataVECTORS[6] = {174U, 193U, 48U, 2U, 1U, 1U}; // ALL FEATURES
//uint8_t masterTxDataVECTORS[6] = {174U, 193U, 48U, 2U, 0U, 1U}; // MAIN FEATURES
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


//struktura pro mozne ulozeni vektoru
typedef struct {
    uint8_t index;
    uint8_t x_0, y_0;
    uint8_t x_1, y_1;
    int16_t pomer;
    uint8_t i;
} VectorStruct;

#define MAX_VECTORS 20
VectorStruct vektory[MAX_VECTORS];
//Funkce pro ulozeni vektoru do struktury
void SaveVectorToStruct(uint8_t index, uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1, int16_t pomer) {
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
//Ulozeni do pole vektor pro pozdejsi vypis, jedna se o vektor podle ktereho bylo neco vyhodnoceo
void SaveImportantVector(void)
{
	importantVectorIndex = vector_index;
	importantVector[0]=x_pocatecni;
	importantVector[1]=y_pocatecni;
	importantVector[2]=x_koncove;
	importantVector[3]=y_koncove;
}
void CheckVector(void)
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
		SaveImportantVector();
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
		if((y_0+y_1)/2 > 25)// if((y_0+y_1)/2 > 40)
		{
			if(x_0 > 10 && x_1 > 10 && x_0 && x_0 < 60 && x_1 < 60)
			{
					horizontal_line_counter++;
					if(horizontal_line_counter == 2)
					{
						PRINTF("HORIZONTAL CARA  BLIZKO DRUHA  ");
						led_B();
						MotorSetSpeed(5);
						finish_line_detected_blizka = true;
						dokoncenoKolo = true;
						UART2_SendTextToHC05("HOR-B!");
						SaveImportantVector();
						/*
						PIT_StopPixyZpracovavatVektory();
						LPTMR_StopPosilejUART();
						driving = false;
						*/

					}
			}
		}
		else
		{
			if(x_0 > 10 && x_1 > 10 && x_0 && x_0 < 60 && x_1 < 60)
			{
					horizontal_line_counter++;
					if(horizontal_line_counter == 2)
					{
						PRINTF("HORIZONTAL CARA  DALEKO DRUHA  ");
						led_Y();
						MotorSetSpeed(10);
						finish_line_detected_blizka = true;
						dokoncenoKolo = true;
						UART2_SendTextToHC05("HOR-D!");
						SaveImportantVector();
						/*
						PIT_StopPixyZpracovavatVektory();
						LPTMR_StopPosilejUART();
						driving = false;
						*/

					}
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
	PRINTF("[%u,%u] [%u,%u] delka %d smer %d pomer %d index  %u \r\n",x_pocatecni,y_pocatecni,x_koncove,y_koncove, delka, smer,  pomer, vector_index);
}

void ProccessVectors(void)
{
	zaznamenanaKorekce = false;
	offset = 20;
	//Na dvacate pozici se vyskytuje informacfe o poctu zaznamenanych vektoru a pak pokracuji data
	pocet_vektoru = masterRxDataVECTORS[offset]/6;
	pocet_vektoru_i = pocet_vektoru;
	//PRINTF("POCET VEKTORU %u  \r\n", pocet_vektoru);

	IsPrimaryVector = true;
	bylaZmenenaHodnotaRychlosti = false;
	horizontal_line_counter = 0;
	offsetBuffer = 0;

	//Postupne se procahzi vsechny vektory co byly poslany
	while(pocet_vektoru_i>0)
	{
		//Ziskani hodnot z prijateho bufferu  (zacinaji na offsetu ktery se vzdy posune)
		x_0 = masterRxDataVECTORS[offset+1];
		y_0 = masterRxDataVECTORS[offset+2];
		x_1 = masterRxDataVECTORS[offset+3];
		y_1 = masterRxDataVECTORS[offset+4];
		vector_index = masterRxDataVECTORS[offset+5];
		CheckVector();
		IsPrimaryVector = false;
		pocet_vektoru_i--;
		offset = offset + 6;
	}

	//Obcas pixy neposle zadne vektory, v tom pripade se nic endela
	if(pocet_vektoru > 0)
	{
		//odeslani vsech zaznamenanych vektoru pokud se zaznamenavaji jenom ty
		if(logujJenomVektory) UART2_SendVectorsBuffer(buffer,offsetBuffer);

		if(aktualniHodnotaKZatoceni == 0)
		{
			//pokud byla nebylo vyhodnoceno ze je traba zatacet, kola rovne
			if(driving)
			{
			 SteerStraight();
			 //Pokud je treba zmeni rychlost a jeste nebylo dokonceno kolo (tam je rychlost konstatni)
			 if(!bylaZmenenaHodnotaRychlosti && !dokoncenoKolo) MotorSetSpeed(base_speed_value);

			}
		}
		else
		{
			//ZATOC DOELVA
			if(aktualniHodnotaKZatoceniLEFT)
			{
				SteerLeft(aktualniHodnotaKZatoceni);
			}
			//ZATOC DOPRAVA
			else
			{
				SteerRight(aktualniHodnotaKZatoceni);
			}
			//Prvne kontrola zda jiz nebyla korekce rychlosti kvuli vyssi priorite (zpomalovani horizontal cara)
			if(!bylaZmenenaHodnotaRychlosti)
			{
				if(dokoncenoKolo == false)
				{
					if(driving)
					{
						if(aktualniHodnotaKZatoceni >= 75) MotorSetSpeed(5);
						else if(aktualniHodnotaKZatoceni >= 50) MotorSetSpeed(10);
						else if(aktualniHodnotaKZatoceni >= 25) MotorSetSpeed(15);
					}
				}
			}
		}
	}
	PixyGetVectors();
	PRINTF("------------------------------------\r\n");
}

//Funkce která nastartuje Pixy2 - provede se inicializace, nastavi servo a zapnou se ledky
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
    PRINTF("Pixy2 Start Finished\r\n");
}
//Nastavi se SPI pro komunikaci s Pixy2
void PixyInit(void)
{
	 SPI_PIXY_DRIVER.Initialize(SPI_IRQ_HANDLER);
	 NVIC_SetPriority(SPI_IRQN, 3);
	 SPI_PIXY_DRIVER.PowerControl(ARM_POWER_FULL);
	 SPI_PIXY_DRIVER.Control(ARM_SPI_MODE_MASTER | ARM_SPI_SS_MASTER_HW_OUTPUT | ARM_SPI_CPOL1_CPHA1, TRANSFER_BAUDRATE);
}
//Preruseni volnao kdyz je dokoncen pixy2 prenos
void SPI_IRQ_HANDLER(uint32_t e)
{
	if (e & ARM_SPI_EVENT_TRANSFER_COMPLETE)
	    {
	        SPI_Finished = true;
	    }
}

//Odeslani registru pres pixy a prijmuti vektoru
void PixyGetVectors(void)
{
	actual_tranfser_size = 10;
	SPI_Finished = false;
	SPI_PIXY_DRIVER.Transfer(masterTxDataVECTORS, masterRxDataVECTORS, 64);
}

//Funkce pro nastaveni pixy serva do polohy
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
	//Blokujici protoze se provede jen jednou na zacatku
	SPI_PIXY_DRIVER.Transfer(masterTxData, masterRxData, actual_tranfser_size);
	while (!SPI_Finished)
		{}

}
//Funkce pro nastaveni pixy ledek (RGB barvy)
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
//Funkce pro nastaveni pixy ledek (prisvetlovaci diody)
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
