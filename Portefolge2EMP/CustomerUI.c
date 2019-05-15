/*
 * CostrumerUI.c
 *
 *  Created on: 13. maj 2019
 *      Author: chris
 */


#include "LED_driver.h"

// Hardware includes
#include "tm4c123gh6pm.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "queue.h"

//
#include "CustomerUI.h"

//
#include "defines.h"
SemaphoreHandle_t RTC_SEM;
SemaphoreHandle_t HANDLE_PRESSED_SEM;
SemaphoreHandle_t xDataLog_mutex;
SemaphoreHandle_t xStation_mutex;

void UICustomerTask (void * pvParameters)
{
    char customer_mtxkey_input;
    // keypad_queue

    for(;;)
    {
        //xQueueReceive()
        if (xQueueReceive( xUARTReceive_queue, customer_mtxkey_input, portMAX_DELAY ) == pdTRUE);
        {

        }
    }
}



