/***************************** Include files *******************************/

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Hardware includes. */
#include "tm4c123gh6pm.h"
#include "gpio.h"

#include "event_groups.h"
#include "defines.h"
#include "setup.h"
#include "key.h"
#include "RTC.h"
#include "pulse.h"
#include "digisw.h"
#include "switch.h"
#include "station.h"
#include "LED_driver.h"
#include "UART_driver.h"
#include "UART_protocol.h"
#include "CustomerUI.h"


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
    init_sem();
    uart0_init(9600, 8, 1, 'e');

    // Taskhandles - made extern in defines.h
    // ----------------
    TaskHandle_t keypad_handle = NULL;
    TaskHandle_t RTC_handle = NULL;
    TaskHandle_t lcd_handle = NULL;
    TaskHandle_t pump_handle = NULL;
    TaskHandle_t digiSwitch_handle = NULL;
    TaskHandle_t switch_handle = NULL;
    TaskHandle_t station_task_handle = NULL;
    TaskHandle_t testKeypad_handle = NULL;
    TaskHandle_t UARTProtocol_handle = NULL;
    TaskHandle_t UARTTransmitDriver_handle = NULL;
    TaskHandle_t UARTReceiveDriver_handle = NULL;
    TaskHandle_t LEDDriver_handle = NULL;
    TaskHandle_t UICustomer_handle = NULL;

    station_eventgroup = xEventGroupCreate();
    LEDs_eventgroup = xEventGroupCreate();

    // Create queues
    // ----------------
    keypad_queue = xQueueCreate(15, 8);
    digiSwitch_queue = xQueueCreate(10, 8);
    lcd_queue = xQueueCreate(15, 8);
    xUARTTransmit_queue = xQueueCreate ( 10, 8 );
    xUARTReceive_queue = xQueueCreate ( 10, 8 );


    // Start the tasks.
    // ----------------
    xTaskCreate(key_task, "Keypad task", 100, NULL, 2, &keypad_handle);
    xTaskCreate(testkey_task, "Test keypad task", 100, NULL, 1, &testKeypad_handle);
    xTaskCreate(RTC_task, "Real time clock task", 100, NULL, 1, &RTC_handle);
    xTaskCreate(pump_task, "Pump task", 100, NULL, 3, &pump_handle);
    xTaskCreate(digiSwitch_task, "Digiswitch task", 50, NULL, 1, &digiSwitch_handle);
    xTaskCreate(switch_task, "SW1 and SW2 task", 50, NULL, 3, &switch_handle); //HIGH PRIORITY
    xTaskCreate(lcd_task, "lcd task", 100, NULL, 1, &lcd_handle);
    xTaskCreate(station_task, "station logic task", 200, NULL, 1, &station_task_handle);

    xTaskCreate(UARTProtocolTask, "UART Protocol", 600, NULL, 2, &UARTProtocol_handle );
    xTaskCreate(UARTTransmitDriverTask, "UART Transmit Driver", 100, NULL, 1, &UARTTransmitDriver_handle );
    xTaskCreate(UARTReceiveDriverTask, "UART Receive Driver", 100, NULL, 1, &UARTReceiveDriver_handle );
    xTaskCreate(LEDDriverTask, "LED Driver", 100, NULL, 1, &LEDDriver_handle);
    //xTaskCreate(UICustomerTask, "UI Customer", 100, NULL, 1, &UICustomer_handle);
    // Does not compile with this task active



    // Start the scheduler.
    // --------------------
    vTaskStartScheduler();


    // Will only get here, if there was insufficient memory.
    // -----------------------------------------------------
    return 0;
}

