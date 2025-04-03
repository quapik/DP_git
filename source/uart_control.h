/*
 * uart_control.h
 *
 *  Created on: 17. 3. 2025
 *      Author: xsimav01
 */

#include "globals.h"


#ifndef UART_CONTROL_H_
#define UART_CONTROL_H_



#define UART            UART2
#define UART_CLKSRC     UART2_CLK_SRC
#define UART_CLK_FREQ   CLOCK_GetFreq(UART2_CLK_SRC)


void UART2_Init(void);
void UART_Callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);
void UART2_SendToHC05(void);



#endif /* UART_CONTROL_H_ */
