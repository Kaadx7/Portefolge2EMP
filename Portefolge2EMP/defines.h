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

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
extern uint8_t seconds;
extern uint8_t minutes;
extern uint8_t hours;


/*****************************   Semaphores   *******************************/
extern SemaphoreHandle_t RTC_SEM;


/*****************************    Queues    *******************************/
QueueHandle_t keypad_queue;


/*****************************   Tasks   *******************************/
extern TaskHandle_t keypad_handle;
extern TaskHandle_t RTC_handle;



extern TaskHandle_t testKeypad_handle;







#endif /* DEFINES_H_ */

