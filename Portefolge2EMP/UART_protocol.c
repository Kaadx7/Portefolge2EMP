/*
 * UART_protocol.c
 *
 *  Created on: 6. maj 2019
 *      Author: chris
 */


/***************************** Include files *******************************/

// Standard includes
#include <stdint.h>
#include <stdbool.h>

// Hardware includes
#include "tm4c123gh6pm.h"
#include "UART_protocol.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

/*****************************     Tasks     *******************************/

void UARTProtocol (void * pvParameters)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    uint8_t *byte_from_queue = 0;
    uint8_t byte_from_queue_nop = 0;

    for (;;)
    {
        if (xQueueReceive( xUARTTransmit_queue, &byte_from_queue , ( TickType_t ) portMAX_DELAY ) == pdTRUE);
        {
            byte_from_queue_nop = *byte_from_queue;
        }
    }
}


