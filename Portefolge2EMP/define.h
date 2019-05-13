/*
 * define.h
 *
 *  Created on: 6. maj 2019
 *      Author: jmkaa
 */

#ifndef DEFINE_H_
#define DEFINE_H_

/***************************** Include files *******************************/
/* Kernel includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define pump_event              (1 << 0)
#define shunt_event             (1 << 1)
#define handle_pressed_event    (1 << 2)
#define isPumping               (1 << 3)
//#define price_change_event      (1 << 4)
#define pulse_event             (1 << 5)

// LED event group
#define pump_ON_event              (1 << 0)
#define shunt_ON_event             (1 << 1)
#define pump_OFF_event             (1 << 2)
#define shunt_OFF_event            (1 << 3)




/*****************************    Structs    *******************************/

struct PriceChange {
    uint8_t type; // 1 for "92", 2 for "95", 3 for "E10"
    float   new_price;
};

struct LogEntry {
    char account_number[6];
    uint8_t fuel_type;
    uint8_t payment_type;
    float fuel_amount;
    float total_price;
    uint16_t notes;
    uint8_t stamp_seconds;
    uint8_t stamp_minutes;
    uint8_t stamp_hours;
};

/*****************************    Queues     *******************************/

QueueHandle_t xUARTTransmit_queue;
QueueHandle_t xUARTReceive_queue;

/*****************************    Mutexes    *******************************/
SemaphoreHandle_t xStation_mutex;
SemaphoreHandle_t xDataLog_mutex;

//
EventGroupHandle_t station_eventgroup;
EventGroupHandle_t LEDs_eventgroup;

#endif /* DEFINE_H_ */



