/*
 * setup.c
 *
 *  Created on: 6. maj 2019
 *      Author: jmkaa
 */






#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "gpio.h"
#include "define.h"

#include "setup.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/


/****************************   Semaphores   *******************************/
SemaphoreHandle_t RTC_SEM;


/*****************************   Functions   *******************************/

extern void init_sem()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
    RTC_SEM = xSemaphoreCreateCounting(1, 1);
    //HANDLE_PRESSED_SEM = xSemaphoreCreateCounting(1,0);

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

