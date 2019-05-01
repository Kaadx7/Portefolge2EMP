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
#include <stdint.h>
#include "FPGA_comp.h"
/*****************************    Defines    *******************************/
#define MAX_PID_OUT 12
#define PWM_RES     1023

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

extern int16_t voltage_to_duty_cycle(float voltage)
/*****************************************************************************
*   Input    : The voltage which is the output from the PID
*   Output   :
*   Function : Convert the floating point voltage to an integer duty cycle
******************************************************************************/
{
    float result_f = (voltage/MAX_PID_OUT*PWM_RES)+0.5;
    return (int16_t) result_f;

}

/****************************** End Of Module *******************************/