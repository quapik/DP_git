/*
 * uart_control.c
 *
 *  Created on: 17. 3. 2025
 *      Author: xsimav01
 */
#include "globals.h"


void uart_comm_init(void)
{

	uart_config_t config;
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE; //115200
	config.enableTx     = true;
	config.enableRx     = false; //neprijimam zadna data

	UART_Init(UART, &config, UART_CLK_FREQ);

}



