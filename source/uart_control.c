/*
 * uart_control.c
 *
 *  Created on: 17. 3. 2025
 *      Author: xsimav01
 */
#include "uart_control.h"
volatile bool txOnGoing                = false;
uart_handle_t g_uartHandle;
uart_config_t config;
uart_transfer_t transfer;

void UART2_Init(void)
{
	 UART_GetDefaultConfig(&config);
	 config.baudRate_Bps = 9600; //HC-05 ma 9600
	 config.enableTx     = true;

	UART_Init(UART, &config, UART_CLK_FREQ);
	UART_TransferCreateHandle(UART, &g_uartHandle, UART_Callback, NULL);

}

void UART2_SendToHC05(void)
{
	char buffer[20];
	 sprintf(buffer, "HR%d; HL%d\r\n", otackyRight,otackyLeft); // Převod čísla na řetězec

	transfer.data     = buffer;
	transfer.dataSize = sizeof(buffer);
	txOnGoing     = true;
	UART_TransferSendNonBlocking(UART, &g_uartHandle, &transfer);

	/* Wait send finished */
	while (txOnGoing)
	{
	}


}


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



