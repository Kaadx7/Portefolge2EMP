
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
#include "event_groups.h"

/* Hardware includes. */
#include "tm4c123gh6pm.h"
#include "gpio.h"
#include "UART_driver.h"
#include "LED_driver.h"

/* Application includes */
#include "UART_protocol.h"
#include "define.h"

/*****************************    Defines    *******************************/
//#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

/*****************************   Constants   *******************************/


//struct Log[0] entry1;



/*****************************   Functions   *******************************/

int main(void)
{
    init_gpio();
    init_sem();
    uart0_init(9600, 8, 1, 'e');


    // Create queues
    xUARTTransmit_queue = xQueueCreate ( 10, 8 );
    xUARTReceive_queue = xQueueCreate ( 10, 8 );
    keypad_queue = xQueueCreate( 15, 8 );

    // event groups
    station_eventgroup = xEventGroupCreate();
    LEDs_eventgroup = xEventGroupCreate();

//    for (uint32_t i = 0; i < 16000000; i++)
//    {
//        ;
//    }


    //xSemaphoreTake();
    // Start the tasks.
    // ----------------

    //Construct a struct for transmission:



    //xTaskCreate(UARTProtocolTask, "UART Protocol", 600, NULL, 2, NULL );
    //xTaskCreate(UARTTransmitDriverTask, "UART Transmit Driver", 100, NULL, 1, NULL );
    xTaskCreate(UARTReceiveDriverTask, "UART Receive Driver", 100, NULL, 1, NULL );
    xTaskCreate(LEDDriverTask, "LED Driver", 100, NULL, 1, NULL);
    xTaskCreate(UICustomerTask, "UI Customer", 100, NULL, 1, NULL);



    // Start the scheduler.
    // --------------------
    vTaskStartScheduler();
    //GPIO_PORTF_DATA_R = 0x02;


    // Will only get here, if there was insufficient memory.
    // -----------------------------------------------------
    return 0;
}
