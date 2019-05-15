/*
 * UART_protocol.h
 *
 *  Created on: 6. maj 2019
 *      Author: chris
 */

#ifndef UART_PROTOCOL_H_
#define UART_PROTOCOL_H_

/***************************** Include files *******************************/

// Application includes
#include "defines.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

struct LogEntry Log[10]; // Array that saves the log entries

/*****************************   Functions   *******************************/

/*****************************     Tasks     *******************************/

void UARTProtocolTask (void * pvParameters);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Decodes UART messages
******************************************************************************/

#endif /* UART_PROTOCOL_H_ */
