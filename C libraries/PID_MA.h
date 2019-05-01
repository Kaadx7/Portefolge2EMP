/*****************************************************************************
 * MODULENAME.: PID_MA.h
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
* 190415  JGR    Module created.
*
*****************************************************************************/


#ifndef PID_MA
#define PID_MA


/***************************** Include files *******************************/
#include <stdint.h>
//#include "Filter.h"
#include "emp_type.h"

/*****************************    Defines    *******************************/
#define CC_CONTROLLER_ID    FILTER_1
#define VC_CONTROLLER_ID    FILTER_2
#define PC_CONTROLLER_ID    FILTER_3

typedef struct
{
  float Kp;
  float Ki;
  float Kd;
  float integral;
  float previous_error;
  float dt;         //stepsize for integral
  float upper_sat;
  float lower_sat;
  float pastError;
  float Ud;
  uint8_t filter_id;
  uint8_t sat_flag;
} PID_controller;


/*************************  Function interfaces ****************************/

extern void PID_task(uint8_t id, uint8_t state, uint8_t event, uint8_t data);
/*****************************************************************************
*   Function : Task for the PID controller
******************************************************************************/

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
#endif /* PID_MA */
