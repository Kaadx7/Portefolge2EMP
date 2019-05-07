#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "defines.h"
#include "setup.h"
#include "pulse.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
uint32_t     pulses = 0;
bool         handle = 0;
bool         shunt  = 0;
bool         pump   = 0;

/*****************************   Functions   *******************************/

extern void pump_task(void * pvParameters)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
    TickType_t xLastWakeTime;
     xLastWakeTime = xTaskGetTickCount();

     for( ;; )
     {


         vTaskDelayUntil (&xLastWakeTime, 40 ); // With tick Hz set to 9000 this is equal to 225 pulses pr. sek
     }
}
