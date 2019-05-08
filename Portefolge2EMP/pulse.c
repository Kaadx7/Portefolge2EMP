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
float        pulses = 0;
bool         handle_pressed = 0;
bool         shunt  = 0;
bool         pump   = 0;

/*****************************   Functions   *******************************/

extern void pump_task(void * pvParameters)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
*   Scheduling: Is scheduled by other tasks by vTaskSupend or vTaskResume
******************************************************************************/
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

     for( ;; )
     {
         if( handle_pressed )
         {
             if( shunt )
             {
                 //Shunt activated 0.02 liter every second
                 vTaskDelayUntil (&xLastWakeTime, 400 ); // With tick Hz set to 9000 this is equal to 22.5 pulses pr. sec
                 pulses++;
             }
             else
             {
                 // Normal flow 0.2 liters every second
                 vTaskDelayUntil (&xLastWakeTime, 40 ); // With tick Hz set to 9000 this is equal to 225 pulses pr. sec
                 pulses++;
             }

         }
         vTaskDelay( pdMS_TO_TICKS(1000) );
     }
}
