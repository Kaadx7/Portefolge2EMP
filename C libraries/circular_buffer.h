/*****************************************************************************
 * MODULENAME.: circular_buffer.c
 *
 * PROJECT....: 4. semester project
 *
 * DESCRIPTION: implementation of circular buffer
 *
 * Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 190204  JGR    Module created.
*
*****************************************************************************/


#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

/***************************** Include files *******************************/
#include <stdint.h>

/*****************************    Defines    *******************************/
#define MAX_BUFFER_LENGTH   64 //Max size = 127 - change to required length when deploying

#define BUFFER_FILTER_1    0
#define BUFFER_FILTER_2    1
#define BUFFER_FILTER_3    2
#define BUFFER_FILTER_4    3
#define BUFFER_5           4
#define BUFFER_6           5
#define BUFFER_7           6
#define BUFFER_8           7
/*************************  Function interfaces ****************************/

extern void initialize_buffer(uint8_t id);
/*****************************************************************************
*   Input    : id
*   Function : initialize a circular buffer
******************************************************************************/

extern void update_buffer(uint8_t id, float data);
/*****************************************************************************
*   Input    : id, data
*   Function : store data element in buffer
******************************************************************************/

extern float peek_buffer(uint8_t id, uint8_t element);
/*****************************************************************************
*   Input    : id (indexed from 0), element number (most recent data is stored at index 1)
*   Output   : data element
*   Function : get data element from the buffer
******************************************************************************/


/****************************** End Of Module *******************************/
#endif /* CIRCULAR_BUFFER_H_ */
