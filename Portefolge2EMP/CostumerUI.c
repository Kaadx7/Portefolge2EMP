/*
 * CostumerUI.c
 *
 *  Created on: 13. maj 2019
 *      Author: lars-p
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "LCD_driver.h"
#include "setup.h"
#include "defines.h"


void costumer_task()
{
    TickType_t xLastWakeTime;
      xLastWakeTime = xTaskGetTickCount();

      for( ;; )
      {
          vTaskDelayUntil (&xLastWakeTime, pdMS_TO_TICKS( 5 ) );

          xSemaphoreTake( xLCD_mutex, portMAX_DELAY);

      }
}

