/*****************************************************************************
 * MODULENAME.: PID_conditional_Integral.c
 *
 * PROJECT....: 4. semester project
 *
 * Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 190408  JGR    Module created.
* 190409  PC	 Revised.
*
*****************************************************************************/
#include "PID_conditional_Integral.h"
#include "filter.h"


/*****************************    Defines    *******************************/
#define NOF_PIDS     1


/*****************************   Variables   *******************************/
PID_controller PID_pool[NOF_PIDS];




extern void init_PIDs()
/*****************************************************************************
*   Function : Initialize all the PID controllers
******************************************************************************/
{
    //Setup of the Current Controller
    PID_pool[CC_CONTROLLER_ID].Kp = 1;
    PID_pool[CC_CONTROLLER_ID].Td = 1;
    PID_pool[CC_CONTROLLER_ID].Ti = 1;
    PID_pool[CC_CONTROLLER_ID].dt = 0.1;
    PID_pool[CC_CONTROLLER_ID].integral = 0;
    PID_pool[CC_CONTROLLER_ID].previous_error = 0;
    PID_pool[CC_CONTROLLER_ID].upper_sat = 10;
    PID_pool[CC_CONTROLLER_ID].lower_sat = -10;
    PID_pool[CC_CONTROLLER_ID].filter_id = CC_CONTROLLER_ID;
	PID_pool[CC_CONTROLLER_ID].pastError = 0;
	PID_pool[CC_CONTROLLER_ID].Ud = 0;
    float CC_Filter_Coef[MAX_NUMBER_OF_TABS] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    init_filter(CC_CONTROLLER_ID, CC_Filter_Coef, 3);
    //SETUP FILTER

    //Setup of the Velocity Controller


    //Setup of the Position controller

}







extern float run_PID(float feedback, float setpoint, uint8_t id) // CHANGE TO PID_ID instead of PID_controller
/*****************************************************************************
*   Input    : Sampled data, setpoint, controller id
*   Output   : Regulated signal
*   Function : PID controller
******************************************************************************/
{
    // see powerpoint, Control engineering course: lesson 8, for implementation of discrete PID.

   float error;
   float output;
   float TempIntegral = 0;
   /*static float integral = 0;
   static float pastError = 0;
   static float Ud = 0;*/



   error = setpoint - feedback;

   error = run_filter(PID_pool[id].filter_id, error);

   
   TempIntegral = PID_pool[id].integral + (error + PID_pool[id].pastError)*(((PID_pool[id].Kp/PID_pool[id].Ti)*PID_pool[id].dt)/2);
   
   output = PID_pool[id].Kp * error + TempIntegral + (PID_pool[id].Kp*PID_pool[id].Td*2)/PID_pool[id].dt * (PID_pool[id].pastError-error) - PID_pool[id].Ud;
												   

   // Implementation of conditional integration (Maybe change to back-calculation)
   if ((PID_pool[id].lower_sat > output) || (output > PID_pool[id].upper_sat)) //forkert
   {
       output = PID_pool[id].Kp * error + (PID_pool[id].Kp*PID_pool[id].Td*2)/PID_pool[id].dt * (PID_pool[id].pastError-error) - PID_pool[id].Ud;
							   
   }
   else
   {
       PID_pool[id].integral = TempIntegral;
   }

   PID_pool[id].Ud = (PID_pool[id].Td*PID_pool[id].Kp*2)/PID_pool[id].dt * (PID_pool[id].pastError-error) - PID_pool[id].Ud;
   PID_pool[id].pastError = error;

   return output;
}


