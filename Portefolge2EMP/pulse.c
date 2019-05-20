
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

SemaphoreHandle_t PULSE_COUNTER_SEM;

enum
{
    IDLE,
    SHUNT,
    MAX_FLOW
};
/*****************************   Functions   *******************************/

extern void pump_task(void * pvParameters)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
*   Scheduling: Is scheduled by other tasks by vTaskSupend or vTaskResume
******************************************************************************/
{
    EventBits_t xEventGroupValue;
    TickType_t xLastWakeTime;
    uint8_t state_pump= IDLE;
    xLastWakeTime = xTaskGetTickCount();


     for( ;; )
     {
         switch( state_pump )
         {
         case IDLE:
             xEventGroupValue = xEventGroupWaitBits(
                     station_eventgroup,
                     shunt_ON_event| maxFlow_ON_event, pdTRUE, pdFALSE,
                     portMAX_DELAY);

             if( xEventGroupValue &  maxFlow_ON_event)
                 state_pump = MAX_FLOW;


             else if (xEventGroupValue &  shunt_ON_event)
                 state_pump = SHUNT;
             break;



         case SHUNT:
             xEventGroupValue = xEventGroupWaitBits(
                     station_eventgroup,
                     maxFlow_OFF_event| maxFlow_ON_event, pdTRUE, pdFALSE,
                     0);

             if( xEventGroupValue &  maxFlow_ON_event)
                 state_pump = MAX_FLOW;

             if( xEventGroupValue &  maxFlow_OFF_event)
                 state_pump = IDLE;


             //Shunt activated 0.02 liter every second
             vTaskDelayUntil (&xLastWakeTime, 400 ); // With tick Hz set to 9000 this is equal to 22.5 pulses pr. sec
             xSemaphoreTake(PULSE_COUNTER_SEM, portMAX_DELAY);
             pulse_counter++;
             xSemaphoreGive(PULSE_COUNTER_SEM);
             break;



         case MAX_FLOW:
             xEventGroupValue = xEventGroupWaitBits(
                      station_eventgroup,
                      shunt_ON_event| maxFlow_OFF_event, pdTRUE, pdFALSE,
                      0);

              if( xEventGroupValue &  maxFlow_OFF_event)
                  state_pump = IDLE;

              if( xEventGroupValue &  shunt_ON_event)
                  state_pump = SHUNT;


             // Normal flow 0.2 liters every second
             vTaskDelayUntil (&xLastWakeTime, 40 ); // With tick Hz set to 9000 this is equal to 225 pulses pr. sec
             xSemaphoreTake(PULSE_COUNTER_SEM, portMAX_DELAY);
             pulse_counter++;
             xSemaphoreGive(PULSE_COUNTER_SEM);
             break;

         }
     }
}
