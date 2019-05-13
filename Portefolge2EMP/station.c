
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "stdbool.h"
#include "tm4c123gh6pm.h"


#include "defines.h"
#include "setup.h"
#include "key.h"
#include "digisw.h"
#include "LCD_driver.h"
#include "pulse.h"
#include "station.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
enum state
{
    IDLE,
    PUMP_ACTIVE

};

/*****************************   Functions   *******************************/

extern void station_task(void * pvParameters)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
    EventBits_t xEventGroupValue;
    uint8_t state = 0;
    const EventBits_t uxBitsToWaitFor =  { pump_event | shunt_event | handle_pressed_event |
                                           shunt_event | isPumping | station_event |
                                           price_change_event | pulse_event};
    for( ;; )
    {
        xEventGroupValue = xEventGroupWaitBits(
                station_eventgroup,
                handle_pressed_event,
                pdTRUE,
                pdFALSE,
                portMAX_DELAY);


        if( (xEventGroupValue & handle_pressed_event) != 0)
        {

        }
    }



}
