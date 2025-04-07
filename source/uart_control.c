/*
 * uart_control.c
 *
 *  Created on: 17. 3. 2025
 *      Author: xsimav01
 */
#include "uart_control.h"
volatile bool txOnGoing = false;
uart_handle_t g_uartHandle;
uart_config_t config;
uart_transfer_t transfer;

static char txBuffer[20];
static char txLONGBuffer[130]; //tohle neni idealni, ale jinak se nepodarilo to udelat aby to fungovalo

//Funkce na inicializaci UARTu pro komunikaci
void UART2_Init(void)
{
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = 115200; //Musí být nastaveno i v HC05
	config.enableTx     = true;

	UART_Init(UART, &config, UART_CLK_FREQ);
	UART_TransferCreateHandle(UART, &g_uartHandle, UART_Callback, NULL);

}

//Funkce pro odesliani pres UART na HC05
void UART2_SendToHC05(void)
{
	//Kontrola aby se pripadne neposilalo pokud jeste nebyl dokoncen predchhozi prenos (radsi se vynecha)
	while(!txOnGoing)
	{
		/*
		snprintf(txLONGBuffer, sizeof(txLONGBuffer), "HR%d;HL%d;CR%d;CL%d;S1%d;S2%d;M%d;LEFT%d;RIGHT%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;\r\n",
		otackyRight,otackyLeft,COLOR1_value_global,COLOR2_value_global,SRF_distance1_global, SRF_distance2_global,pctMotory,pctServoL,pctServoR,
		primaryVector[0],primaryVector[1],primaryVector[2],primaryVector[3],primaryVectorIndex, importantVector[0],importantVector[1],importantVector[2],importantVector[3],importantVectorIndex);
		*/

		snprintf(txLONGBuffer, sizeof(txLONGBuffer), "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;\r\n",
		otackyRight,otackyLeft,COLOR1_value_global,COLOR2_value_global,SRF_distance1_global, SRF_distance2_global,pctMotory,pctServoL,pctServoR, primaryVector[0],primaryVector[1],primaryVector[2],primaryVector[3],primaryVectorIndex,
		importantVector[0],importantVector[1],importantVector[2],importantVector[3],importantVectorIndex);

		//Jen natoceni kol
		//snprintf(txLONGBuffer, sizeof(txLONGBuffer), "LEFT %d RIGHT %d\r\n", pctServoL,pctServoR);

		/*
		//Jen otacky
		snprintf(txLONGBuffer, sizeof(txLONGBuffer), "%d;%d;\r\n", otackyRight,otackyLeft);
		*/
		transfer.data = (uint8_t *)txLONGBuffer;
		transfer.dataSize = strlen(txLONGBuffer);
		//PRINTF("delka %d\r\n",strlen(txLONGBuffer));
		txOnGoing = true;
		UART_TransferSendNonBlocking(UART, &g_uartHandle, &transfer);
	}
	/*
	else
	{
		PRINTF("NEPOSLALO SE ------------------------------------------------------------------------\r\n");
	}
	*/
}

void UART2_SendTextToHC05(const char *text)
{
	//Radsi cekame, muzeme si to dovolit ale potrebuje aby se tato hodnota zapsala
    while(!txOnGoing)
    {
        size_t len = strlen(text);

        memcpy(txBuffer, text, len);
        txBuffer[len+1] = '\r';
        txBuffer[len+2] = '\n';
        txBuffer[len+3] = '\0';
        len = len + 3;

        transfer.data = (uint8_t *)txBuffer;
        transfer.dataSize = len;

        txOnGoing = true;
        UART_TransferSendNonBlocking(UART, &g_uartHandle, &transfer);
    }
}

//Funkce preruseni pro neblokujici posilani pres UART
void UART2_FLEXIO_IRQHandler(void)
{
    UART_TransferHandleIRQ(UART2, &g_uartHandle);
}
void UART_Callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_UART_TxIdle == status)
    {
        txOnGoing = false; //Znaci ukonceni prenosu pres uart
    }

}



