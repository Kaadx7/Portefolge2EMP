
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
enum
{
    IDLE,
    PUMP_ACTIVE,
    MAX_FUEL_FLOW,
    SHUNT_ACTIVE
};

uint16_t temp_pulse = 0;
uint8_t shunt_state = 0;

/*****************************   Functions   *******************************/

void end_customer()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{

    customer = 0;
    // print out data to log
    //Update LCD
    //Release some semaphore signaling DONE
}


void new_customer()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
    customer = 1;
    pulse_counter = 0;
    // Also calculate amount of cash to amount of gasoline into fuel_amount
}

void price_change()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
//    switch (price_change_event.type)
//    {
//    case FUEL_TYPE_1:
//        octane_92_price = price_change_event.new_price;
//        break;
//    case FUEL_TYPE_2:
//        octane_95_price = price_change_event.new_price;
//        break;
//    case FUEL_TYPE_3:
//        E10_price = price_change_event.new_price;
//    default:
//        break;
//    }
}


extern void station_task(void * pvParameters)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
    EventBits_t xEventGroupValue;
    uint8_t state_station = 0;

    for( ;; )
    {
        switch ( state_station )
        {
        case IDLE:
            xEventGroupValue = xEventGroupWaitBits(station_eventgroup,
                            pump_ON_event | price_change_event, pdTRUE, pdFALSE,
                            portMAX_DELAY);

            if( xEventGroupValue &  price_change_event)
                price_change();

            else if(xEventGroupValue &  pump_ON_event)
                state_station = PUMP_ACTIVE;
            break;



        case PUMP_ACTIVE:
            //If first time fueling
            if(!customer)
                new_customer();

            xEventGroupValue = xEventGroupWaitBits(station_eventgroup,
                    handle_ON_event | pump_OFF_event, pdTRUE, pdFALSE,
                    portMAX_DELAY);

            if(xEventGroupValue &  handle_ON_event)
            {
                //ALSO SET LED
                state_station = SHUNT_ACTIVE;
                xEventGroupSetBits(station_eventgroup, shunt_ON_event);
            }

            //End customer
            else if(xEventGroupValue &  pump_OFF_event)
            {
                end_customer();
                state_station = IDLE;
            }

            //If timeout customer_at_station = 0

                //Do some kind of timing
            break;


        case MAX_FUEL_FLOW:
            xEventGroupValue = xEventGroupWaitBits( station_eventgroup,
                                     handle_OFF_event, pdTRUE, pdFALSE,
                                     portMAX_DELAY);

            if(xEventGroupValue &  handle_OFF_event)
            {
                //SHUT OFF LED
                state_station = PUMP_ACTIVE;
                xEventGroupSetBits(station_eventgroup, maxFlow_OFF_event);
            }

            //Almost done fueling, activate shunt
            if( ( (fuel_amount/1125) - pulse_counter) < 100 )
            {
                state_station = SHUNT_ACTIVE;
                xEventGroupSetBits(station_eventgroup, shunt_ON_event);
                //Do something with leds
            }

            //Do some LCD update
            break;



        case SHUNT_ACTIVE:
            if(!shunt_state)
            {
                temp_pulse = pulse_counter;
                shunt_state = 1;
            }

            xEventGroupValue = xEventGroupWaitBits( station_eventgroup,
                                handle_OFF_event, pdTRUE, pdFALSE,
                                portMAX_DELAY);

            if(xEventGroupValue &  handle_OFF_event)
            {
                //SHUT OFF LED
                state_station = PUMP_ACTIVE;
                xEventGroupSetBits(station_eventgroup, maxFlow_OFF_event);
                shunt_state = 0;
            }

            //Check if more than 0.1 Liter has been given
            if( (pulse_counter - temp_pulse) >= 113 )
            {
                shunt_state = 0;
                state_station = MAX_FUEL_FLOW;
                xEventGroupSetBits(station_eventgroup, maxFlow_ON_event);
                // DO something with led
            }

            if(pulse_counter/1125 >= fuel_amount)
            {
                xEventGroupSetBits(station_eventgroup, maxFlow_OFF_event);
                state_station = IDLE;
                shunt_state = 0;
                end_customer();
                //Turn off some LEDS
            }

            //Put in LCD buffer

            }
            break;


        }

    }

}
