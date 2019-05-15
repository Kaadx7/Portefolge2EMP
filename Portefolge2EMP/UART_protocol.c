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
#include <stdlib.h>
#include <string.h>

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

#define FUEL_TYPE_1             1 //Octane 92
#define FUEL_TYPE_2             2 //Octane 95
#define FUEL_TYPE_3             3 //E10

/*****************************    Macros     *******************************/
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

/*****************************   Constants   *******************************/

//Descriptors
const char account_number_des[] = "Account number:";
const char fuel_type_des[] = "Fuel type:";
const char payment_type_des[] = "Payment type:";
const char fuel_amount_des[] = "Fuel amount:";
const char total_price_des[] = "Total price:";
const char notes_des[] = "Amount in notes:";
const char timestamp_des[] = "Timestamp:";
//char stamp_seconds_des[] = ""

// Fuel types
const char fuel_type_1[] = "92 Octane";
const char fuel_type_2[] = "95 Octane";
const char fuel_type_3[] = "E10";

// Payment types
const char payment_type_1[] = "Account";
const char payment_type_2[] = "Notes";




/*****************************   Variables   *******************************/
bool UARTTaskCommandReady = UART_COMMAND_READY; //Maybe not bool
char first_byte_from_queue_nop = 0;
char *first_byte_from_queue = 0;




/*****************************   Functions   *******************************/

void putStringToQueue(char str[])
{
    //some_size = NELEMS(str);
    uint8_t *byte_to_transmit = 0;
    for (uint8_t i = 0; i < strlen(str); i++)
    {
        byte_to_transmit = str[i];
        xQueueSend(xUARTTransmit_queue,  &byte_to_transmit, (TickType_t) portMAX_DELAY);
    }
    byte_to_transmit = 32; // Space
    xQueueSend(xUARTTransmit_queue,  &byte_to_transmit, (TickType_t) portMAX_DELAY);
}


/*****************************     Tasks     *******************************/

void UARTProtocolTask (void * pvParameters)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();


    uint8_t *byte_to_transmit = 0;
    uint8_t *byte_from_UART_queue = 0;

    //Log[0].account_number = "123456";
//    strcpy(Log[0].account_number, "123456" );
//    Log[0].fuel_type = 1;
//    Log[0].payment_type = 0;
//    Log[0].fuel_amount = 9.94;
//    Log[0].total_price = 111.35;
//    Log[0].notes = 0;
//    Log[0].stamp_seconds = 35;
//    Log[0].stamp_minutes = 59;
//    Log[0].stamp_hours = 13;
//
//    strcpy(Log[1].account_number, "654312" );
//    Log[1].fuel_type = 2;
//    Log[1].payment_type = 1;
//    Log[1].fuel_amount = 5.54;
//    Log[1].total_price = 200.0;
//    Log[1].notes = 200;
//    Log[1].stamp_seconds = 59;
//    Log[1].stamp_minutes = 9;
    Log[1].stamp_hours = 11;

    float temp_new_price = 0;
    struct PriceChange price_change_event;

    for (;;)
    {

        if (UARTTaskCommandReady)
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
                UARTTaskCommandReady = UART_COMMAND_READY;

                if ( xSemaphoreTake(xDataLog_mutex, portMAX_DELAY) == pdTRUE )
                {
                    byte_to_transmit = 0x0A;
                    xQueueSend(xUARTTransmit_queue,  &byte_to_transmit, (TickType_t) portMAX_DELAY);
                    byte_to_transmit = 0x0D;
                    xQueueSend(xUARTTransmit_queue,  &byte_to_transmit, (TickType_t) portMAX_DELAY);
                    putStringToQueue("Log");

                    for (uint8_t i = 0; i < NELEMS(Log); i++)//NELEMS(Log)
                    {

                        byte_to_transmit = 0x0A;
                        xQueueSend(xUARTTransmit_queue,  &byte_to_transmit, (TickType_t) portMAX_DELAY);
                        byte_to_transmit = 0x0D;
                        xQueueSend(xUARTTransmit_queue,  &byte_to_transmit, (TickType_t) portMAX_DELAY);
                        putStringToQueue(account_number_des);
                        putStringToQueue(Log[i].account_number);
                        putStringToQueue(fuel_type_des);
                        switch(Log[i].fuel_type)
                        {
                        case (FUEL_TYPE_1):
                            putStringToQueue(fuel_type_1);
                            break;
                        case (FUEL_TYPE_2):
                            putStringToQueue(fuel_type_2);
                            break;
                        case (FUEL_TYPE_3):
                            putStringToQueue(fuel_type_3);
                            break;
                        default:
                            break;
                        }
                        putStringToQueue(payment_type_des);
                        switch(Log[i].payment_type)
                        {
                        case (0):
                            putStringToQueue(payment_type_1);
                            break;
                        case (1):
                            putStringToQueue(payment_type_2);
                            break;
                        default:
                            break;
                        }
                        //fuel amount
                        putStringToQueue(fuel_amount_des);
                        char temp_string[10];

                        //GPIO_PORTF_DATA_R ^= 0x02;
                        sprintf(temp_string, "%.2f", Log[i].fuel_amount);

                        putStringToQueue(temp_string);
                        //total price
                        putStringToQueue(total_price_des);
                            //char temp_string[10];
                        sprintf(temp_string, "%.2f", Log[i].total_price);
                        putStringToQueue(temp_string);
                        //notes
                        putStringToQueue(notes_des);
                            //char temp_string[10];
                        sprintf(temp_string, "%u", Log[i].notes);
                        putStringToQueue(temp_string);
                        // timestamp
                        putStringToQueue(timestamp_des);
                        sprintf(temp_string, "%u", Log[i].stamp_hours);
                        putStringToQueue(temp_string);
                        putStringToQueue(": ");
                        sprintf(temp_string, "%u", Log[i].stamp_minutes);
                        putStringToQueue(temp_string);
                        putStringToQueue(": ");
                        sprintf(temp_string, "%u", Log[i].stamp_seconds);
                        putStringToQueue(temp_string);
    //                    byte_to_transmit = 0x0A;
    //                    xQueueSend(xUARTTransmit_queue,  &byte_to_transmit, (TickType_t) portMAX_DELAY);
    //                    byte_to_transmit = 0x0D;
    //                    xQueueSend(xUARTTransmit_queue,  &byte_to_transmit, (TickType_t) portMAX_DELAY);
                    }

                    xSemaphoreGive(xDataLog_mutex);
                }

                break;
            case (ADJUST_PRICE_COMMAND):
                if (UARTMessagesWaiting >= 5)
                {
                    //Must wait for station to be available in order to update the price
                    if ( xSemaphoreTake(xStation_mutex, portMAX_DELAY) == pdTRUE )
                    {

                        UARTTaskCommandReady = UART_COMMAND_READY;
                        xQueueReceive( xUARTReceive_queue, &byte_from_UART_queue , ( TickType_t ) 0 );
                        price_change_event.type = byte_from_UART_queue;

                        xQueueReceive( xUARTReceive_queue, &byte_from_UART_queue , ( TickType_t ) 0 );
                        temp_new_price += (*byte_from_UART_queue-48)*10;

                        xQueueReceive( xUARTReceive_queue, &byte_from_UART_queue , ( TickType_t ) 0 );
                        temp_new_price += (*byte_from_UART_queue-48)*1;

                        xQueueReceive( xUARTReceive_queue, &byte_from_UART_queue , ( TickType_t ) 0 );
                        temp_new_price += (*byte_from_UART_queue-48)*0.1;

                        xQueueReceive( xUARTReceive_queue, &byte_from_UART_queue , ( TickType_t ) 0 );
                        temp_new_price += (*byte_from_UART_queue-48)*0.01;

                        //price_change_event.new_price = temp_new_price;

                        //xEventGroupSetBits(station_eventgroup, price_change_event);
                        xSemaphoreGive(xStation_mutex);
                    }

                    vTaskDelay( pdMS_TO_TICKS( 3000 ) ); // Maybe not necessary if task is scheduled every time something is in the queue
                } else {
                    UARTTaskCommandReady = UART_WAITING_FOR_QUEUE;
                    vTaskDelay( pdMS_TO_TICKS( 3000 ) ); // Maybe not necessary if task is scheduled every time something is in the queue
                }
                break;
            default:
                break;




        }
    }
}


