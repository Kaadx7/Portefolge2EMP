/*****************************************************************************
 * MODULENAME.: PID_FreeRTOS.c
 *
 * PROJECT....: 4. semester project
 *
 * Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 190423  JGR    Module created.
*
*****************************************************************************/
#include "PID_FreeRTOS.h"
#include "filter.h"
#include "tmodel.h"
#include "FPGA_comp.h"
#include "SPI.h"
#include "FreeRTOS.h"
#include "defines.h"
#include "setup.h"



/*****************************    Defines    *******************************/
#define NOF_PIDS     4

/*****************************   Variables   *******************************/
PID_controller PID_pool[NOF_PIDS];


extern void PID_PC_task(void* pvParameters)
{

    struct PID_parameter controller_parameter = *((struct PID_parameter *) pvParameters);
    struct SPI_queue_element data_request;
    TickType_t xLastWakeTime;
    float result_PID = 0;
    float temp_feedback = 0;
    float temp_reference = 0;
    int16_t temp_output = 0;


    data_request.id = controller_parameter.slave_id;
    data_request.data = 0xFFFF;

    for (;;)
    {
        xLastWakeTime = xTaskGetTickCount();

        if(xSemaphoreTake(*controller_parameter.queue_semaphore, portMAX_DELAY)==pdTRUE)
        {
            xQueueSend( SPI_queue, (void * ) &data_request, 0);
            xSemaphoreGive(*controller_parameter.queue_semaphore);
        }

        if(xSemaphoreTake(*controller_parameter.feedback_semaphore, portMAX_DELAY) == pdTRUE)
        {
            if(xSemaphoreTake(*controller_parameter.reference_semaphore, portMAX_DELAY) == pdTRUE)
            {

                temp_feedback = *controller_parameter.feedback_signal;
                temp_reference = *controller_parameter.reference_signal;
                xSemaphoreGive(*controller_parameter.reference_semaphore);
            }

            result_PID = run_PID(temp_feedback, temp_reference, controller_parameter.id);

        }

        //temp_output = voltage_to_duty_cycle(result_PID);

        if(xSemaphoreTake(*controller_parameter.output_semaphore, portMAX_DELAY)==pdTRUE)
        {
            *controller_parameter.place_to_store_output = result_PID;
            xSemaphoreGive(*controller_parameter.output_semaphore);
        }

        vTaskDelayUntil (&xLastWakeTime, pdMS_TO_TICKS(controller_parameter.delayTime) );
    }

}


extern void PID_VC_task(void* pvParameters)
{

    struct PID_parameter controller_parameter = *((struct PID_parameter *) pvParameters);
    struct SPI_queue_element data_request;
    struct SPI_queue_element data_to_send;
    TickType_t xLastWakeTime;
    float result_PID = 0;
    float temp_feedback = 0;
    float temp_reference = 0;

    data_request.id = controller_parameter.slave_id;
    data_request.data = 0xFFFF;
    data_to_send.id = controller_parameter.output_id;

    xLastWakeTime = xTaskGetTickCount(); // Is automatically updated by vTaskDelayUntil()
    for (;;)
    {
        GPIO_PORTA_DATA_R &= ~(0x04);


        if(xSemaphoreTake(*controller_parameter.queue_semaphore, portMAX_DELAY)==pdTRUE)
        {
            GPIO_PORTA_DATA_R &= ~(0x10);
            xQueueSend( SPI_queue, (void * ) &data_request, 0);
            xSemaphoreGive(*controller_parameter.queue_semaphore);
        }

        if(xSemaphoreTake(*controller_parameter.feedback_semaphore, portMAX_DELAY) == pdTRUE)
        {
            if(xSemaphoreTake(*controller_parameter.reference_semaphore, portMAX_DELAY) == pdTRUE)
            {

                temp_feedback = *controller_parameter.feedback_signal;
                temp_reference = *controller_parameter.reference_signal;
                xSemaphoreGive(*controller_parameter.reference_semaphore);
                GPIO_PORTA_DATA_R |= 0x10;
            }

            result_PID = run_PID(temp_feedback, temp_reference, controller_parameter.id);

        }

            data_to_send.data = (result_PID / 12) * 1023 + 0.5;//voltage_to_duty_cycle(result_PID);


        if(xSemaphoreTake(*controller_parameter.queue_semaphore, portMAX_DELAY)==pdTRUE)
        {
            xQueueSendToFront( SPI_queue, (void * ) &data_to_send, 0);
            xSemaphoreGive(*controller_parameter.queue_semaphore);
        }

        GPIO_PORTA_DATA_R |= 0x04;

        vTaskDelayUntil (&xLastWakeTime, pdMS_TO_TICKS(controller_parameter.delayTime) );
        //GPIO_PORTA_DATA_R ^= (0x04);
        //GPIO_PORTA_DATA_R |= 0x04;
        //vTaskDelay(pdMS_TO_TICKS(10));

    }

}



extern void init_PIDs()
/*****************************************************************************
*   Function : Initialize all the PID controllers
******************************************************************************/
{
    //Setup of position controller 1:

    PID_pool[PC_CONTROLLER_1_ID].Kp = 1;
    PID_pool[PC_CONTROLLER_1_ID].Kd = 0.01;
    PID_pool[PC_CONTROLLER_1_ID].Ki = 5;
    PID_pool[PC_CONTROLLER_1_ID].dt = 0.01;
    PID_pool[PC_CONTROLLER_1_ID].integral = 0;
    PID_pool[PC_CONTROLLER_1_ID].previous_error = 0;
    PID_pool[PC_CONTROLLER_1_ID].upper_sat = 12;
    PID_pool[PC_CONTROLLER_1_ID].lower_sat = -12;
    PID_pool[PC_CONTROLLER_1_ID].filter_id = PC_CONTROLLER_1_ID;
    PID_pool[PC_CONTROLLER_1_ID].pastError = 0;
    PID_pool[PC_CONTROLLER_1_ID].Ud = 0;
    PID_pool[PC_CONTROLLER_1_ID].sat_flag = 0;
    float PC1_Filter_Coef[MAX_NUMBER_OF_TABS] = {0.0249, 0.9502, 0.0249, 1, 1, 1, 1, 1, 1, 1};
    init_filter(PC_CONTROLLER_1_ID, PC1_Filter_Coef, 2);

    //Setup of position controller 2:

    PID_pool[PC_CONTROLLER_2_ID].Kp = 1;
    PID_pool[PC_CONTROLLER_2_ID].Kd = 0.01;
    PID_pool[PC_CONTROLLER_2_ID].Ki = 5;
    PID_pool[PC_CONTROLLER_2_ID].dt = 0.01;
    PID_pool[PC_CONTROLLER_2_ID].integral = 0;
    PID_pool[PC_CONTROLLER_2_ID].previous_error = 0;
    PID_pool[PC_CONTROLLER_2_ID].upper_sat = 12;
    PID_pool[PC_CONTROLLER_2_ID].lower_sat = -12;
    PID_pool[PC_CONTROLLER_2_ID].filter_id = PC_CONTROLLER_2_ID;
    PID_pool[PC_CONTROLLER_2_ID].pastError = 0;
    PID_pool[PC_CONTROLLER_2_ID].Ud = 0;
    PID_pool[PC_CONTROLLER_2_ID].sat_flag = 0;
    float PC2_Filter_Coef[MAX_NUMBER_OF_TABS] = {0.0249, 0.9502, 0.0249, 1, 1, 1, 1, 1, 1, 1};
    init_filter(PC_CONTROLLER_2_ID, PC2_Filter_Coef, 2);


    
    //Setup of velocity controller 1:

     PID_pool[VC_CONTROLLER_1_ID].Kp = 1;
     PID_pool[VC_CONTROLLER_1_ID].Kd = 0.02;
     PID_pool[VC_CONTROLLER_1_ID].Ki = 2;
     PID_pool[VC_CONTROLLER_1_ID].dt = 0.001;
     PID_pool[VC_CONTROLLER_1_ID].integral = 0;
     PID_pool[VC_CONTROLLER_1_ID].previous_error = 0;
     PID_pool[VC_CONTROLLER_1_ID].upper_sat = 12;
     PID_pool[VC_CONTROLLER_1_ID].lower_sat = -12;
     PID_pool[VC_CONTROLLER_1_ID].filter_id = VC_CONTROLLER_1_ID;
     PID_pool[VC_CONTROLLER_1_ID].pastError = 0;
     PID_pool[VC_CONTROLLER_1_ID].Ud = 0;
     PID_pool[VC_CONTROLLER_1_ID].sat_flag = 0;
     float VC1_Filter_Coef[MAX_NUMBER_OF_TABS] = {0.0249, 0.9502, 0.0249, 1, 1, 1, 1, 1, 1, 1};
     init_filter(VC_CONTROLLER_1_ID, VC1_Filter_Coef, 2);

     //Setup of velocity controller 2:

     PID_pool[VC_CONTROLLER_2_ID].Kp = 1;
     PID_pool[VC_CONTROLLER_2_ID].Kd = 1;
     PID_pool[VC_CONTROLLER_2_ID].Ki = 1;
     PID_pool[VC_CONTROLLER_2_ID].dt = 0.001;
     PID_pool[VC_CONTROLLER_2_ID].integral = 0;
     PID_pool[VC_CONTROLLER_2_ID].previous_error = 0;
     PID_pool[VC_CONTROLLER_2_ID].upper_sat = 12;
     PID_pool[VC_CONTROLLER_2_ID].lower_sat = -12;
     PID_pool[VC_CONTROLLER_2_ID].filter_id = VC_CONTROLLER_1_ID;
     PID_pool[VC_CONTROLLER_2_ID].pastError = 0;
     PID_pool[VC_CONTROLLER_2_ID].Ud = 0;
     PID_pool[VC_CONTROLLER_2_ID].sat_flag = 0;
     float VC2_Filter_Coef[MAX_NUMBER_OF_TABS] = {0.0249, 0.9502, 0.0249, 1, 1, 1, 1, 1, 1, 1};
     init_filter(VC_CONTROLLER_2_ID, VC2_Filter_Coef, 2);


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
   float T = PID_pool[id].dt;
   float static integral_term;

   error = setpoint - feedback;

   error = run_filter(PID_pool[id].filter_id, error);

    // calculate the proportional and derivative terms
   float proportional_term  = PID_pool[id].Kp*error;
   float derivative_term = PID_pool[id].Kd*2/T*(error - PID_pool[id].previous_error);// - PID_pool[id].Ud;


   // integral is only given a value if the controller is not in saturation
   if (PID_pool[id].sat_flag)
   {
       integral_term = 0;
   }
   else
   {
       integral_term = PID_pool[id].integral + PID_pool[id].Ki*T/2*(error + PID_pool[id].previous_error);
   }


   output = proportional_term + integral_term + derivative_term;

   PID_pool[id].integral = integral_term;
   PID_pool[id].Ud = derivative_term;
   PID_pool[id].previous_error = error;

    // check for saturation for next run through and set flag
   if (output > PID_pool[id].upper_sat)
   {
       output = PID_pool[id].upper_sat;
       PID_pool[id].sat_flag = 1;
   }
   else if (output < PID_pool[id].lower_sat)
   {
       output = PID_pool[id].lower_sat;
       PID_pool[id].sat_flag = 1;
   }
   else
   {
       PID_pool[id].sat_flag = 0;
   }

   return output;

}
