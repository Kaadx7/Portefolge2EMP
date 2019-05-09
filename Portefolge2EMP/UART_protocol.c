/*
 * UART_protocol.c
 *
 *  Created on: 6. maj 2019
 *      Author: chris
 */



/***************************** Include files *******************************/

// Standard includes
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

// Hardware includes
#include "tm4c123gh6pm.h"
#include "UART_protocol.h"

// Application includes
#include "define.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "queue.h"

/*****************************    Defines    *******************************/

//States
#define PRINT_LOG_COMMAND       'p'
#define ADJUST_PRICE_COMMAND    'a'

//Waiting for queue
#define UART_WAITING_FOR_QUEUE   0
#define UART_COMMAND_READY       1

/*****************************    Macros     *******************************/
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
bool UITaskCommandReady = UART_COMMAND_READY; //Maybe not bool

struct LogEntry Log[10]; // Array that saves the log entries

/*****************************   Functions   *******************************/

/*****************************     Tasks     *******************************/

void UARTProtocolTask (void * pvParameters)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    uint8_t *first_byte_from_queue = 0;
    uint8_t first_byte_from_queue_nop = 0;

    float temp_new_price = 0;
    struct PriceChange price_change_event;

    for (;;)
    {
        if (UITaskCommandReady)
        {
            if (xQueueReceive( xUARTReceive_queue, &first_byte_from_queue , ( TickType_t ) portMAX_DELAY ) == pdTRUE);
            {
                first_byte_from_queue_nop = *first_byte_from_queue;
            }
        }
        UBaseType_t UARTMessagesWaiting = uxQueueMessagesWaiting(xUARTReceive_queue);
        switch ( first_byte_from_queue_nop )
        {
            case (PRINT_LOG_COMMAND):
                UARTTaskCommandReady = UI_COMMAND_READY;


                // Process all the data in the log and put it to the UART transmit queue
                for (uint8_t i = 0; i < NELEMS(Log); i++)
                {

                }

                break;
            case (ADJUST_PRICE_COMMAND):
                if (UARTMessagesWaiting >= 5)
                {
                    UITaskCommandReady = UI_COMMAND_READY;
                    xQueueReceive( xUARTReceive_queue, &byte_from_UART_queue , ( TickType_t ) 0 );
                    price_change_event.type = byte_from_UART_queue;

                    xQueueReceive( xUARTReceive_queue, &byte_from_UART_queue , ( TickType_t ) 0 );
                    temp_new_price += (byte_from_UART_queue-48)*10;

                    xQueueReceive( xUARTReceive_queue, &byte_from_UART_queue , ( TickType_t ) 0 );
                    temp_new_price += (byte_from_UART_queue-48)*1;

                    xQueueReceive( xUARTReceive_queue, &byte_from_UART_queue , ( TickType_t ) 0 );
                    temp_new_price += (byte_from_UART_queue-48)*0.1;

                    xQueueReceive( xUARTReceive_queue, &byte_from_UART_queue , ( TickType_t ) 0 );
                    temp_new_price += (byte_from_UART_queue-48)*0.01;

                    price_change_event.new_price = temp_new_price;

                    vTaskDelay( pdMS_TO_TICKS( 3000 ) ); // Maybe not necessary if task is scheduled every time something is in the queue
                } else {
                    UITaskCommandReady = UI_WAITING_FOR_COMMAND;
                    vTaskDelay( pdMS_TO_TICKS( 3000 ) ); // Maybe not necessary if task is scheduled every time something is in the queue
                }
                break;
            default:
                break;




        }
    }
}


