/*****************************************************************************
 * MODULENAME.: filter.c
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
* 190408  JGR    Module created.
*
*****************************************************************************/

#ifndef FILTER_H_
#define FILTER_H_

/***************************** Include files *******************************/

#include <stdint.h>
#include "circular_buffer.h"
/*****************************    Defines    *******************************/
#define MAX_NUMBER_OF_TABS 10	

#define FILTER_1        BUFFER_FILTER_1	//Buffer_Filters are defined in circular_buffer.h
#define FILTER_2        BUFFER_FILTER_2	
#define FILTER_3        BUFFER_FILTER_3
#define FILTER_4        BUFFER_FILTER_4


typedef struct
{
    uint8_t buffer_id;	//must be the same as the id of the filter. One of the IDs' could be removed
    uint8_t id;
    float coefList[10];
    uint8_t tabs;
} filter_descriptor;


/*************************  Function interfaces ****************************/

extern void init_filter(uint8_t id, float coefArray[MAX_NUMBER_OF_TABS], uint8_t numTabs);
/*****************************************************************************
*   Input    : filter id, Array with filter coefficient, number of tabs in the filter.
*   Output   :
*   Function : Initialize filter
******************************************************************************/

extern float run_filter(uint8_t id, float data);
/*****************************************************************************
*   Input    : filter id, sampled data
*   Output   : filtered data
*   Function : filter data
******************************************************************************/


/****************************** End Of Module *******************************/
#endif /* FILTER_H_ */
