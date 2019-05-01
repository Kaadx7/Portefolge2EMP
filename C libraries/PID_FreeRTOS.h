    
/*****************************************************************************
 * MODULENAME.: PID_FreeRTOS.h
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
* 190423  JGR    Module created.
*
*****************************************************************************/


#ifndef PID_FREERTOS
#define PID_FREERTOS


/***************************** Include files *******************************/
#include <stdint.h>
#include "filter.h"
#include "emp_type.h"

/*****************************    Defines    *******************************/
#define PC_CONTROLLER_1_ID    0
#define VC_CONTROLLER_1_ID    1
#define PC_CONTROLLER_2_ID    2
#define VC_CONTROLLER_2_ID    3

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

extern void PID_PC_task(void *pvParameter );
/*****************************************************************************
*   Function : Task for the PID controller
******************************************************************************/

extern void PID_VC_task(void *pvParameter );
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
#endif /* PID_FREERTOS */
