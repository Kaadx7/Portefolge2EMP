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

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
// RTC
extern uint8_t      seconds;
extern uint8_t      minutes;
extern uint8_t      hours;

// Pump station
extern uint32_t     pulses;
extern bool         handle;
extern bool         shunt;
extern bool         pump;


/*****************************   Semaphores   *******************************/
extern SemaphoreHandle_t RTC_SEM;


/*************************  Queues & Event Groups  *******************************/
QueueHandle_t keypad_queue;

EventGroupHandle_t pump_event;


/*****************************   Tasks   *******************************/
extern TaskHandle_t keypad_handle;
extern TaskHandle_t RTC_handle;



extern TaskHandle_t testKeypad_handle;







#endif /* DEFINES_H_ */

