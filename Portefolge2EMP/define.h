/*
 * define.h
 *
 *  Created on: 6. maj 2019
 *      Author: jmkaa
 */

#ifndef DEFINE_H_
#define DEFINE_H_

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


QueueHandle_t xUARTTransmit_queue;
QueueHandle_t xUARTReceive_queue;

#endif /* DEFINE_H_ */



