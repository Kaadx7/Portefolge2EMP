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
#include "tm4c123gh6pm.h"
#include "gpio.h"

#include "defines.h"
#include "setup.h"
#include "key.h"

#include "lcd_driver.h"

/*****************************    Defines    *******************************/
//#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

int main(void)
{
   init_gpio();

    // Taskhandles - made extern in defines.h
    // ----------------
    TaskHandle_t keypad_handle = NULL;
    TaskHandle_t testKeypad_handle = NULL;
    TaskHandle_t lcd_handle = NULL;


    // Create queues
    // ----------------
    keypad_queue = xQueueCreate(15, 8);
    lcd_queue = xQueueCreate(15, 8);


    // Start the tasks.
    // ----------------
    xTaskCreate(key_task, "Keypad task", 100, NULL, 1, &keypad_handle);
    xTaskCreate(testkey_task, "Test keypad task", 100, NULL, 1, &testKeypad_handle);
    xTaskCreate(lcd_task, "lcd task", 100, NULL, 1, &lcd_handle);


    // Start the scheduler.
    // --------------------
    vTaskStartScheduler();


    // Will only get here, if there was insufficient memory.
    // -----------------------------------------------------
    return 0;
}

