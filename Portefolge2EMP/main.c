

/**
 * main.c
 */

/***************************** Include files *******************************/

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Hardware includes. */
//#include "hardware_tm4c123g.h" // Requires TivaWare library
//#include "systick_frt.h"    // From Mortens Lessons
#include "tm4c123gh6pm.h"
#include "gpio.h"


/*****************************    Defines    *******************************/
//#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

#define GREEN_LED_TASK 2
#define YELLOW_LED_TASK 3


/*****************************   Constants   *******************************/
SemaphoreHandle_t taskSignalSem;

TaskHandle_t semTaskHandle = NULL;
/*****************************   Variables   *******************************/

//uint32_t SystemCoreClock;

/*****************************   Functions   *******************************/

void ledTaskFlash( void * pvParameters)
{
    TickType_t xLastWakeTime;
    //uint32_t xLastWakeTime;
    //parameter = (uint8_t *) pvParameters;
    xLastWakeTime = xTaskGetTickCount();
    //uint16_t *parameter = (uint16_t * ) pvParameters;

    uint16_t time = 200;
    for (;;)
    {
        uint8_t Parameter = *((uint8_t *) pvParameters);

        if (pvParameters == GREEN_LED_TASK)
        {
                GPIO_PORTF_DATA_R ^= 0x04;
                time = 2000;

        }
        if (pvParameters == YELLOW_LED_TASK)
        {
            GPIO_PORTF_DATA_R ^= 0x02;
            time = 3000;
            //xSemaphoreGive( taskSignalSem );
            xTaskNotifyGive( semTaskHandle );
        }


        vTaskDelayUntil (&xLastWakeTime, pdMS_TO_TICKS( time ) );

    }
}

void semTask( void * pvParameters)
{
    static uint32_t thread_notification;
    for (;;)
    {

        //ulTaskNotifyTake(pdTRUE, portMAX_DELAY)
        if(ulTaskNotifyTake(pdTRUE, portMAX_DELAY) == pdTRUE)
        {

        GPIO_PORTF_DATA_R ^= 0x08;
        //xTaskNotifyWait(0x00, 0, &notifyValue, portMAX_DELAY);

        }
       // xTaskNotifyWait( 0x00,      /* Don't clear any notification bits on entry. */
//                         0xffffffff , /* Reset the notification value to 0 on exit. */
//                          &thread_notification, /* Notified value pass out in
//                                               ulNotifiedValue. */
//                          portMAX_DELAY );  /* Block indefinitely. */
        //vTaskDelay(portMAX_DELAY);
    }
}



int main(void)
{
    //SystemCoreClock;
    //init_systick();
    init_gpio();

    //Create semaphores
    taskSignalSem = xSemaphoreCreateCounting(10, 0);

    //xSemaphoreTake();
    // Start the tasks.
    // ----------------
    //TaskHandle_t semTaskHandle;
    xTaskCreate(ledTaskFlash, "Yellow LED task", 100, 2, 1, NULL );
    xTaskCreate(ledTaskFlash, "Green LED task", 100, 3, 1, NULL );
    xTaskCreate(semTask, "semTask", 100, 4, 1, &semTaskHandle);

    //GPIO_PORTF_DATA_R = 0x02;

    // Start the scheduler.
    // --------------------
    vTaskStartScheduler();

    GPIO_PORTF_DATA_R = 0x02;

    // Will only get here, if there was insufficient memory.
    // -----------------------------------------------------
    return 0;
}
