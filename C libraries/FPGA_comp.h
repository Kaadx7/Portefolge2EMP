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

/*****************************    Defines    *******************************/



/*************************  Function interfaces ****************************/
extern int16_t voltage_to_duty_cycle(float voltage);
/*****************************************************************************
*   Input    : The voltage which is the output from the PID
*   Output   :
*   Function : Convert the floating point voltage to an integer duty cycle
******************************************************************************/
/****************************** End Of Module *******************************/
#endif /* FILTER_H_ */