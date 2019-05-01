/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: emp.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

extern void SPI_init();
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : SPI initation
******************************************************************************/

extern void SPI_task(void *);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/

extern void update_values_task(void *);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/

extern void data_transmit(uint16_t data);
/*****************************************************************************
*   Input    :Byte that needs to be send
*   Output   :
*   Function :Sends the data given and waits until the transmission is complete.
******************************************************************************/

extern void send_data(uint16_t, uint8_t);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : SPI initation
******************************************************************************/

extern int16_t receive_data();


#endif /* SPI_H_ */
