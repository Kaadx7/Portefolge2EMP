/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: emp.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
 ****************************************************************************
 Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "test.h"
#include "EMP_type.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "tmodel.h"
#include "rtcs.h"
#include "events.h"
#include "SPI.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void test_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
    INT16U POS_data;

    if( wait_sem( SEM_POS_UPDATE, WAIT_FOREVER ) )
    {
        if( get_queue( Q_SPI_POS, &POS_data, WAIT_FOREVER ) )
        {
            INT8U data_LOW = POS_data & 0xFF;
            INT8U data_HIGH = (POS_data >> 8);

            while( !uart0_tx_rdy() )
            {}
            uart0_putc(data_LOW);
            while( !uart0_tx_rdy() )
            {}
            uart0_putc(data_HIGH);
        }
    }


}


/****************************** End Of Module *******************************/
