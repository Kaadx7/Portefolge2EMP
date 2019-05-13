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
#include "digisw.h"

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
uint8_t digisw_state = DSS_IDLE;
uint8_t event = 1;
uint8_t temp1 = 0;
bool temp2 = 0;

/*****************************   Functions   *******************************/

void digisw_init(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  int8_t dummy;
  // Enable the GPIO port A.
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA | SYSCTL_RCGC2_GPIOF;

  // Do a dummy read to insert a few cycles after enabling the peripheral.
  dummy = SYSCTL_RCGC2_R;

  GPIO_PORTA_DEN_R |= 0xE0;

  GPIO_PORTF_DIR_R |= 0x04;
  GPIO_PORTF_DEN_R |= 0x04;

}


void digiSwitch_task(void *pvParameters)
{
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  digisw_init();
  for( ;; )
  {

    if( GPIO_PORTA_DATA_R & 0x20 ){
      if( digisw_state != DSS_A_ON ){
        if( GPIO_PORTA_DATA_R & 0x40 ){
          event = DSE_CCW;
          xQueueSend( digiSwitch_queue,  &event, portMAX_DELAY  );
          GPIO_PORTF_DATA_R ^= 0x04;
        }
        else{
          event = DSE_CW;
          xQueueSend( digiSwitch_queue,  &event, portMAX_DELAY  );
          GPIO_PORTF_DATA_R ^= 0x02;
        }
        digisw_state = DSS_A_ON;
        GPIO_PORTF_DATA_R |= 0x04;
      }
      if(GPIO_PORTA_DATA_R & 0x80){
          event = DSE_PRESS;
          xQueueSend(digiSwitch_queue, &event, portMAX_DELAY);
          GPIO_PORTF_DATA_R ^= 0x08;
      }
    }
    else{
      if( digisw_state != DSS_A_OFF ){
        if( GPIO_PORTA_DATA_R & 0x40 ){
          event = DSE_CW;
          xQueueSend( digiSwitch_queue,  &event, portMAX_DELAY  );
        }
        else{
          event = DSE_CCW;
          xQueueSend( digiSwitch_queue,  &event, portMAX_DELAY  );
        }
        digisw_state = DSS_A_OFF;
        GPIO_PORTF_DATA_R &= 0xFB;
      }
      if(GPIO_PORTA_DATA_R & 0x80){
          event = DSE_PRESS;
          xQueueSend(digiSwitch_queue, &event,portMAX_DELAY);
          GPIO_PORTF_DATA_R ^= 0x08;
      }
    }
    GPIO_PORTF_DATA_R ^= 0x04;
    vTaskDelayUntil (&xLastWakeTime, 5 );
  }
}


/****************************** End Of Module *******************************/
