/*****************************************************************************
 * MODULENAME.: filter.c
 *
 * PROJECT....: 4. semester project
 *
 * Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 190408  JGR    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "filter.h"
#include "circular_buffer.h"
#include <stdint.h>
/*****************************    Defines    *******************************/
#define NOF_FILTERS     3

/*****************************   Variables   *******************************/
filter_descriptor filter_pool[NOF_FILTERS]; //Pool of filters

/*****************************   Functions   *******************************/

extern void init_filter(uint8_t id, float coefArray[MAX_NUMBER_OF_TABS], uint8_t tabs)
/*****************************************************************************
*   Input    : filter id, Array with filter coefficient, number of tabs in the filter.
*   Output   :
*   Function : Initialize filter
******************************************************************************/
{
    filter_pool[id].id = id; // Filter must have same ID, as the used circular_buffer.
    filter_pool[id].tabs = tabs;

    for(uint8_t i = 0; i < MAX_NUMBER_OF_TABS; i++)
    {
        filter_pool[id].coefList[i] = coefArray[i];
    }

    initialize_buffer(id); //initializes buffer to filter with same ID as filter
}

float run_filter(uint8_t id, float data)
/*****************************************************************************
*   Input    : filter id, sampled data
*   Output   : filtered data
*   Function : filter data
******************************************************************************/
{
    float result = 0;
    float immediateResult; //Used as intermediate step in calculations
    update_buffer(id, data); //Puts data into buffer

    for(uint8_t i = 0; i < filter_pool[id].tabs; i++)
    {
        immediateResult = filter_pool[id].coefList[i] * peek_buffer(id, i);//Calculates the n'th step of the FIR filter
        result = result + immediateResult;
    }

    return result;
}

/****************************** End Of Module *******************************/
