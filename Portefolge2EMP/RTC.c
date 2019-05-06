#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "tm4c123gh6pm.h"


#include "defines.h"
#include "setup.h"
#include "RTC.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;


/*****************************   Functions   *******************************/

extern void RTC_task(void * pvParameters)
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


        if( xSemaphoreTake(RTC_SEM, 0) == pdTRUE )
        {
            seconds++;

            if ( seconds > 59 )
            {
                seconds = 0;
                minutes++;
                if( minutes > 59 )
                {
                    minutes = 0;
                    hours++;
                    if ( hours > 23 )
                    {
                        hours = 0;
                    }
                }
            }

            xSemaphoreGive( RTC_SEM );
        }

        vTaskDelayUntil (&xLastWakeTime, pdMS_TO_TICKS( 1000 ) );
    }
}




