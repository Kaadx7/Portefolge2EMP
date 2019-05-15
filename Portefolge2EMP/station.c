
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "stdbool.h"
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


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

uint16_t        temp_pulse = 0;
uint8_t         shunt_state = 0;
uint16_t        fuel_amount = 1110;
uint16_t        cash_amount = 0;

float        pulse_counter;
bool         pump;
bool         hook;
bool         handle_pressed;
bool         shunt;
bool         fueling;

bool         customer = 0;

extern SemaphoreHandle_t RTC_SEM;
extern SemaphoreHandle_t PULSE_COUNTER_SEM;
extern SemaphoreHandle_t xStation_mutex;
extern SemaphoreHandle_t xDataLog_mutex;
/*****************************   Functions   *******************************/

void lcd_fuelAmount(float number)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
    uint8_t temp;
    if((number - 10) < 0)
    {
        temp = (uint8_t)number;
        wr_ch_LCD(temp+'0');
    }
    else if((number - 100) < 0)
    {
        temp = (uint8_t)number/10;
        wr_ch_LCD(temp+'0');
        temp = (uint8_t)number % 10;
        wr_ch_LCD(temp+'0');
    }
    else if((number - 1000) < 0)
    {
        temp = (uint8_t)number/100;
        wr_ch_LCD(temp+'0');
        temp = ((uint8_t)number % 100) / 10;
        wr_ch_LCD(temp+'0');
        temp = (uint8_t)number % 10;
        wr_ch_LCD(temp+'0');
    }
    else if((number - 10000) < 0)
    {

    }
    else if((number - 100000) < 0)
    {

    }
    else
    {

    }
}

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
    xSemaphoreTake(PULSE_COUNTER_SEM, portMAX_DELAY);
    pulse_counter = 0;
    xSemaphoreGive(PULSE_COUNTER_SEM);
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
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    for( ;; )
    {
        switch ( state_station )
        {
        case IDLE:
            xEventGroupValue = xEventGroupWaitBits(
                    station_eventgroup,
                    pump_ON_event | price_change_event, pdTRUE, pdFALSE,
                    portMAX_DELAY);

            if( xEventGroupValue &  price_change_event)
                price_change();

            else if(xEventGroupValue &  pump_ON_event)
            {
                state_station = PUMP_ACTIVE;
                xEventGroupSetBits(LEDs_eventgroup, LED_pump_ON_event);
            }
            break;


        case PUMP_ACTIVE:
            //If first time fueling
            if(!customer)
                new_customer();

            xEventGroupValue = xEventGroupWaitBits(station_eventgroup,
                    handle_ON_event | pump_OFF_event, pdTRUE, pdFALSE,
                    0);

            if(xEventGroupValue &  handle_ON_event)
            {
                state_station = SHUNT_ACTIVE;
                xEventGroupSetBits(station_eventgroup, shunt_ON_event);
                xEventGroupSetBits(LEDs_eventgroup, LED_shunt_ON_event);
            }

            //End customer
            else if(xEventGroupValue &  pump_OFF_event)
            {
                end_customer();
                xEventGroupSetBits(LEDs_eventgroup, LED_pump_OFF_event);
                state_station = IDLE;
            }

            //If timeout customer_at_station = 0

                //Do some kind of timing
            break;


        case MAX_FUEL_FLOW:
            xEventGroupValue = xEventGroupWaitBits( station_eventgroup,
                                     handle_OFF_event, pdTRUE, pdFALSE,
                                     0);

            if( xEventGroupValue &  handle_OFF_event )
            {
                //SHUT OFF LED
                state_station = PUMP_ACTIVE;
                xEventGroupSetBits(station_eventgroup, maxFlow_OFF_event);
                xEventGroupSetBits(LEDs_eventgroup, LED_pump_OFF_event);
            }

            //Almost done fueling, activate shunt
            if( ( (fuel_amount/1125) - pulse_counter) < 100 )
            {
                state_station = SHUNT_ACTIVE;
                xEventGroupSetBits(station_eventgroup, shunt_ON_event);
                xEventGroupSetBits(LEDs_eventgroup, LED_shunt_ON_event);
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
                                0);

            if(xEventGroupValue &  handle_OFF_event)
            {
                shunt_state = 0;
                state_station = PUMP_ACTIVE;
                xEventGroupSetBits(station_eventgroup, maxFlow_OFF_event);
                xEventGroupSetBits(LEDs_eventgroup, LED_shunt_OFF_event);
            }

            //Check if more than 0.1 Liter has been given
            if( (pulse_counter - temp_pulse) >= 113 )
            {
                shunt_state = 0;
                state_station = MAX_FUEL_FLOW;
                xEventGroupSetBits(LEDs_eventgroup, LED_shunt_OFF_event);
                xEventGroupSetBits(station_eventgroup, maxFlow_ON_event);
            }

            if(pulse_counter/1125 >= fuel_amount)
            {
                state_station = IDLE;
                shunt_state = 0;
                end_customer();
                xEventGroupSetBits(station_eventgroup, maxFlow_OFF_event);
                xEventGroupSetBits(LEDs_eventgroup, LED_pump_OFF_event);
            }

            xSemaphoreTake(PULSE_COUNTER_SEM, portMAX_DELAY);
            wr_ch_LCD(0xFF);
            lcd_fuelAmount(pulse_counter);
            xSemaphoreGive(PULSE_COUNTER_SEM);
            //Put in LCD buffer

            break;
        }

        vTaskDelayUntil (&xLastWakeTime, pdMS_TO_TICKS(25) );
    }

}
