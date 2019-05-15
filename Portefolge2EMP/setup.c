
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "gpio.h"
#include "defines.h"

#include "setup.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/


/****************************   Semaphores   *******************************/
SemaphoreHandle_t RTC_SEM;
SemaphoreHandle_t PULSE_COUNTER_SEM;
SemaphoreHandle_t xStation_mutex;
SemaphoreHandle_t xDataLog_mutex;

/*****************************   Functions   *******************************/

extern void init_sem()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
    RTC_SEM = xSemaphoreCreateCounting(1, 1);
    PULSE_COUNTER_SEM = xSemaphoreCreateCounting(1,1);

    xStation_mutex = xSemaphoreCreateMutex();
    if( xStation_mutex != NULL )
    {
        /* The semaphore was created successfully and
        can be used. */
    }

    xDataLog_mutex = xSemaphoreCreateMutex();
    if( xDataLog_mutex != NULL )
    {
        /* The semaphore was created successfully and
        can be used. */
    }

}








