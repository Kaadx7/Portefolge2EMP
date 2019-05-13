#ifndef DEFINES_H_
#define DEFINES_H_


/***************************** Include files *******************************/
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#include "tm4c123gh6pm.h"
#include "gpio.h"
/*****************************    Defines    *******************************/

#define FALSE   0
#define TRUE  !FALSE
#define NULL    ((void *)0)
#define NEGATIVE 0
#define POSITIVE 1
#define LEFT    0
#define RIGHT   1

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
//extern uint8_t seconds = 0;
//extern uint8_t minutes = 0;
//extern uint8_t hours = 0;




/*****************************   Semaphores   *******************************/
extern SemaphoreHandle_t RTC_SEM;


/*****************************    Mutexes    *******************************/
SemaphoreHandle_t xStation_mutex;
SemaphoreHandle_t xLCD_mutex;


/*****************************    Queues    *******************************/
QueueHandle_t keypad_queue;
QueueHandle_t lcd_queue;


QueueHandle_t xUARTTransmit_queue;
QueueHandle_t xUARTReceive_queue;


/*****************************   Tasks   *******************************/
extern TaskHandle_t keypad_handle;
extern TaskHandle_t RTC_handle;



extern TaskHandle_t testKeypad_handle;


extern TaskHandle_t lcd_handle;





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


#endif /* DEFINES_H_ */

