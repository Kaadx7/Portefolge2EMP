
#include "LED_driver.h"

// Hardware includes
#include "tm4c123gh6pm.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

//
#include "defines.h"

#define BIT_PUMP 1      // RED
#define BIT_SHUNT 2     // YELLOW

/*****************************     Tasks      *******************************/
void LEDDriverTask (void * pvParameters)
{
    GPIO_PORTF_DATA_R = 0xFE;
    //EventBits_t
    uint8_t uxLEDBits;
    //uxLEDBits = 2;

    for (;;)
    {
        uxLEDBits = xEventGroupWaitBits(
                LEDs_eventgroup,
                LED_pump_ON_event  | LED_pump_OFF_event | LED_shunt_ON_event | LED_shunt_OFF_event,
                pdTRUE,
                pdFALSE,
                portMAX_DELAY );

        //GPIO_PORTF_DATA_R = 0x02;
        if (uxLEDBits & LED_pump_ON_event)
        {
            GPIO_PORTF_DATA_R &= 0x0C; // Turn on RED LED
        }
        if (uxLEDBits & LED_pump_OFF_event)
        {
            GPIO_PORTF_DATA_R |= 0x02; // Turn off RED LED
        }

        if (uxLEDBits & LED_shunt_ON_event)
        {
            GPIO_PORTF_DATA_R &= 0x0A; // Turn on YELLOW LED
        }
        if (uxLEDBits & LED_shunt_OFF_event)
        {
            GPIO_PORTF_DATA_R |= 0x04; // Turn off YELLOW LED
        }

//#define FUEL_TYPE_1             1 //Octane 92
//#define FUEL_TYPE_2             2 //Octane 95
//#define FUEL_TYPE_3             3 //E10
//        if (xEventGroupValue & price_change_event)
//        {
//            switch (price_change_event.type)
//            {
//            case FUEL_TYPE_1:
//                octane_92_price = price_change_event.new_price;
//                break;
//            case FUEL_TYPE_2:
//                octane_95_price = price_change_event.new_price;
//                break;
//            case FUEL_TYPE_3:
//                E10_price = price_change_event.new_price;
//            default:
//                break;
//            }
//        }

        //vTaskDelay(pdMS_TO_TICKS( 50 ));

    }
}
