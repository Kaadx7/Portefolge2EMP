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


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
// RTC
extern uint8_t      seconds;
extern uint8_t      minutes;
extern uint8_t      hours;

// Pump station
extern float        pulses;
extern bool         pump;
extern bool         hook;
extern bool         handle_pressed;
extern bool         shunt;



/*****************************   Semaphores   *******************************/
extern SemaphoreHandle_t RTC_SEM;

/*************************  Queues & Event Groups  *******************************/
QueueHandle_t keypad_queue;
QueueHandle_t lcd_queue;
QueueHandle_t digiSwitch_queue;


/*****************************   Tasks   *******************************/
extern TaskHandle_t keypad_handle;
extern TaskHandle_t RTC_handle;
extern TaskHandle_t pump_handle;
extern TaskHandle_t digiSwitch_handle;
extern TaskHandle_t switch_handle;

extern TaskHandle_t testKeypad_handle;


extern TaskHandle_t lcd_handle;







#endif /* DEFINES_H_ */


