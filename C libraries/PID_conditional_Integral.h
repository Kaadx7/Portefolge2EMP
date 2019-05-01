/*****************************************************************************
 * MODULENAME.: PID_controller_Integral.h
 *
 * PROJECT....: 4. semester project
 *
 * DESCRIPTION: implementation of PID controller
 *
 * Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 190408  JGR    Module created.
* 190409  PC	 Revised, comments added
*
*****************************************************************************/


#ifndef PID_H_
#define PID_H_


/***************************** Include files *******************************/
#include <stdint.h>
#include "filter.h"

/*****************************    Defines    *******************************/
#define CC_CONTROLLER_ID    FILTER_1
#define VC_CONTROLLER_ID    FILTER_2
#define PC_CONTROLLER_ID    FILTER_3

typedef struct
{
  float Td;
  float Kp;
  float Ti;
  float integral;
  float previous_error;
  float dt;         //stepsize for integral
  float upper_sat;
  float lower_sat;
  float pastError;
  float Ud;
  uint8_t filter_id;
} PID_controller;


/*************************  Function interfaces ****************************/

extern void init_PIDs();
/*****************************************************************************
*   Function : Initialize all the PID controllers
******************************************************************************/


extern float run_PID(float feedback, float setpoint, uint8_t id);
/*****************************************************************************
*   Input    : Sampled data, setpoint, controller id
*   Output   : Regulated signal
*   Function : PID controller
******************************************************************************/

/****************************** End Of Module *******************************/
#endif /* PID_H_ */
