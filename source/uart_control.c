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

//Funkce na inicializaci UARTu pro komunikaci
void UART2_Init(void)
{
	 UART_GetDefaultConfig(&config);
	 config.baudRate_Bps = 9600; //HC-05 ma 9600
	 config.enableTx     = true;

	UART_Init(UART, &config, UART_CLK_FREQ);
	UART_TransferCreateHandle(UART, &g_uartHandle, UART_Callback, NULL);

}
//Funkce pro odesliani pres UART na HC05
void UART2_SendToHC05(void)
{

	char buffer[200];

	// Formátování do textu
	snprintf(buffer, sizeof(buffer), "HR%d;HL%d;CR%d;CL%d;S1%d;S1%d;M%d;LEFT%d,RIGHT%d;\r\n",
			otackyRight,otackyLeft,COLOR1_value_global,COLOR2_value_global,SRF_distance1_global, SRF_distance2_global,pctMotory,pctServoL,pctServoR);


	transfer.data = (uint8_t *)buffer;
	transfer.dataSize = strlen(buffer);

	// Poslání dat na UART
	txOnGoing = true;
	UART_TransferSendNonBlocking(UART, &g_uartHandle, &transfer);

	/*
	while (txOnGoing)
	{

	}
	*/
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
        txOnGoing    = false;
    }

}



