/*****************************************************************************
 * MODULENAME.: circular_buffer.c
 *
 * PROJECT....: 4. semester project
 *
 * Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 190204  JGR    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "circular_buffer.h"

/*****************************    Defines    *******************************/
#define NOF_BUFFERS     8  //change to required number when deploying

typedef struct
{
    uint8_t     firstElement;
    uint8_t     nextToReplace;
    float       buf[MAX_BUFFER_LENGTH];
} buffer_descriptor;

/*****************************   Variables   *******************************/
buffer_descriptor       buffer_pool[NOF_BUFFERS];

/*****************************   Functions   *******************************/

extern void initialize_buffer(uint8_t id)
/*****************************************************************************
*   Input    : id
*   Function : initialize a circular buffer
******************************************************************************/
{
    if(id < NOF_BUFFERS)
    {
        buffer_pool[id].firstElement = 0;
        buffer_pool[id].nextToReplace = 0;
    }
    return;
}

extern void update_buffer(uint8_t id, float data)
/*****************************************************************************
*   Input    : id, data
*   Function : store data element in buffer
******************************************************************************/
{
       if(id < NOF_BUFFERS)
       {
           buffer_pool[id].buf[buffer_pool[id].nextToReplace] = data;
           buffer_pool[id].firstElement = buffer_pool[id].nextToReplace;
           buffer_pool[id].nextToReplace = (buffer_pool[id].nextToReplace + 1) % MAX_BUFFER_LENGTH;

       }
       return;
}

extern float peek_buffer(uint8_t id, uint8_t element)
/*****************************************************************************
*   Input    : id, element number (most recent data at index 1)
*   Output   : data element
*   Function : get data element from the buffer
******************************************************************************/

{
    float result = 0;
    if(id < NOF_BUFFERS)
    {
        int8_t bufAddress = buffer_pool[id].firstElement - (element - 1);

            if(bufAddress < 0)
            {
                bufAddress = MAX_BUFFER_LENGTH + bufAddress;
            }

        result = buffer_pool[id].buf[bufAddress];
    }
    return result;
}

/****************************** End Of Module *******************************/
