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
#include "switch.h"

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/
#define     TAKEN         1
#define     HOOKED        2

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
uint8_t hook_state = HOOKED;
bool hook = 0;

/*****************************   Functions   *******************************/

void switch_task(void *pvParameters)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    bool sw1 = 0;
    bool sw1_pressed = 0;
    bool sw2 = 0;
    for( ;; )
    {

        // SW1 toggles hook for the trunk
        if( GPIO_PORTF_DATA_R & 0x10 )
            sw1 = 0;
        else
            sw1 = 1;

        switch( hook_state )
        {
        case HOOKED:
            if( sw1 == 1 && sw1_pressed == 0)
                sw1_pressed = 1;
            else if( sw1 == 0 && sw1_pressed == 1)
            {
                hook_state = TAKEN;
                hook = 1;
            }
            break;

        case TAKEN:
            if( sw1 == 1 && sw1_pressed == 1)
            {
                sw1_pressed = 0;
            }
            else if( sw1 == 0 && sw1_pressed == 0)
            {
                hook = 0;
                hook_state = HOOKED;
            }

            break;
        }


        if( hook == 1 )
            GPIO_PORTF_DATA_R |= 0x02;
        else
            GPIO_PORTF_DATA_R &= ~(0x02);


        // SW2 is for valve in handle handle
        //if ( GPIO_PORTF_DATA_R & 0x01 )


        vTaskDelayUntil (&xLastWakeTime, pdMS_TO_TICKS(20) );
    }


}