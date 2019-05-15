#ifndef DEFINES_H_
#define DEFINES_H_


/***************************** Include files *******************************/
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

#include "tm4c123gh6pm.h"
#include "gpio.h"
/*****************************    Defines    *******************************/
// LCD
#define FALSE   0
#define TRUE  !FALSE
#define NULL    ((void *)0)
#define NEGATIVE 0
#define POSITIVE 1
#define LEFT    0
#define RIGHT   1

// Digiswitch Events
#define DSE_CW      '1'
#define DSE_CCW     '2'
#define DSE_PRESS   '3'

// Eventgroup bits
#define pump_ON_event          (1 << 0)
#define pump_OFF_event         (1 << 1)
#define handle_ON_event        (1 << 2)
#define handle_OFF_event       (1 << 3)

#define shunt_ON_event         (1 << 4)
#define shunt_OFF_event        (1 << 5)
#define maxFlow_ON_event       (1 << 6)
#define maxFlow_OFF_event      (1 << 7)
#define price_change_event     (1 << 8)

// LED event group
#define LED_pump_ON_event              (1 << 1)
#define LED_shunt_ON_event             (1 << 2)
#define LED_pump_OFF_event             (1 << 3)
#define LED_shunt_OFF_event            (1 << 4)

// Uart protocol and station
#define FUEL_TYPE_1             1 //Octane 92
#define FUEL_TYPE_2             2 //Octane 95
#define FUEL_TYPE_3             3 //E10


/*****************************   Constants   *******************************/

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
/*****************************   Variables   *******************************/
// RTC
extern uint8_t      seconds;
extern uint8_t      minutes;
extern uint8_t      hours;

// Pump station
extern float        pulse_counter;
extern bool         pump;
extern bool         hook;
extern bool         handle_pressed;
extern bool         shunt;
extern bool         fueling;

extern bool         customer;

extern float        octane_92_price;
extern float        octane_95_price;
extern float        E10_price;

extern uint16_t     fuel_amount;
extern uint16_t     cash_amount;


/*****************************   Semaphores   *******************************/
extern SemaphoreHandle_t RTC_SEM;
extern SemaphoreHandle_t PULSE_COUNTER_SEM;
extern SemaphoreHandle_t xStation_mutex;
extern SemaphoreHandle_t xDataLog_mutex; //Not extern in christians file

/*************************  Queues & Event Groups  *******************************/
QueueHandle_t keypad_queue;
QueueHandle_t digiSwitch_queue;
QueueHandle_t lcd_queue;
QueueHandle_t xUARTTransmit_queue;
QueueHandle_t xUARTReceive_queue;

EventGroupHandle_t station_eventgroup;
EventGroupHandle_t LEDs_eventgroup;
/*****************************   Tasks   *******************************/
extern TaskHandle_t keypad_handle;
extern TaskHandle_t RTC_handle;
extern TaskHandle_t pump_handle;
extern TaskHandle_t digiSwitch_handle;
extern TaskHandle_t switch_handle;
extern TaskHandle_t station_task_handle;

extern TaskHandle_t UARTProtocol_handle;
extern TaskHandle_t UARTTransmitDriver_handle;
extern TaskHandle_t UARTReceiveDriver_handle;
extern TaskHandle_t LEDDriver_handle;
extern TaskHandle_t UICustomer_handle;

extern TaskHandle_t testKeypad_handle;







#endif /* DEFINES_H_ */

