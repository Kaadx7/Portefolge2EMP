
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "tm4c123gh6pm.h"


#include "defines.h"
#include "setup.h"
#include "key.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
uint8_t characterReceived = 0;
uint8_t y;
uint8_t received = 0;
uint8_t ch;
uint8_t x = 1;
uint8_t state = 0;

/*****************************   Functions   *******************************/


extern void key_task(void * pvParameters)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  for( ;; )
  {
      switch( state )
      {
        case 0:
          GPIO_PORTA_DATA_R |= 0x04;
          GPIO_PORTF_DATA_R |= 0x0E;
          state = 1;

          break;

        case 1:
          y = GPIO_PORTE_DATA_R & 0x0F;
          if( y )
          {
            GPIO_PORTF_DATA_R &= 0xFD;
            ch = key_catch( x, row(y) );
            xQueueSend( keypad_queue, &ch, 0 );
            state = 2;
          }
          else
          {
            x++;
            if( x > 3 )
              x = 1;
            GPIO_PORTA_DATA_R &= 0xE3;
            GPIO_PORTA_DATA_R |= ( 0x01 << (x+1 ));
            y = GPIO_PORTA_DATA_R;
          }
          break;

        case 2:
          if( !(GPIO_PORTE_DATA_R & 0x0F ))
          {
            x=1;
            GPIO_PORTA_DATA_R |= 0x04;
            state = 1;
          }
          break;
      }
  }


  vTaskDelayUntil (&xLastWakeTime, pdMS_TO_TICKS( 10 ) );
}

extern void testkey_task(void * pvParameters)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
    for( ;; )
    {
        if( xQueueReceive( keypad_queue, &characterReceived, portMAX_DELAY ) == pdPASS )
        {
            received = characterReceived;
        }
    }

}



uint8_t row( uint8_t y )
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
  uint8_t result = 0;

  switch( y )
  {
    case 0x01: result = 1; break;
    case 0x02: result = 2; break;
    case 0x04: result = 3; break;
    case 0x08: result = 4; break;
  }
  return( result );
}


uint8_t key_catch( uint8_t x, uint8_t y )
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
  const uint8_t matrix[3][4] = {{'#','9','6','3'},
                              {'0','8','5','2'},
                              {'*','7','4','1'}};

  return( matrix[x-1][y-1] );

}










