#ifndef __MODEL_TIME_H
#define __MODEL_TIME_H

#include "PSEFMconfigure.h"
#include "mode.h"

/******************************************************
Function: unit_start( input_start ); input_end; output_start; output_end( unit_end )
Description: Kernel function, get the time information of current unit including input and output duration
structure:
Unit: which is the GCD of periods of all tasks in one mode;
Unit is composed of three time durations including Input, LET_u and Output;
The length of Input and Output is configured in PSEFMconfigure.h as INPUT and OUTPUT
 * *****************************************************/
tick_t prv_model_time_unit_start();
tick_t prv_model_time_output_start();
tick_t prv_model_time_output_end();
tick_t prv_model_time_input_end();

/******************************************************
Function: is_unit_start; is_mode_end; is_period_start;
Description: Kernel function, used to judge wether system reaches a specified time instant
Calls:
Called by: ps_mode_switch
Input:
Output:
Return: 1 for true; 0 for false
Others:
 * *****************************************************/
bool prv_model_time_is_unit_start();
bool prv_model_time_is_mode_end();
bool prv_model_time_is_period_start(ps_task_t * ptask);

/******************************************************
Function: input_length; output_length
Description:Kernel function, return the value configured in PSEFMconfigure.h
Calls:
Called by:
Input:
Output:
Return: the length of input and output
Others:
 * *****************************************************/
tick_t prv_model_time_input_length();
tick_t prv_model_time_output_length();

/******************************************************
Function: unit_initialize; period_initialize; model_time_initialize
Description: Kernel function, initialize the model time of system and set the unit and period properties of mode
Calls:
Called by: system_start()
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_model_time_unit_initialize();
void prv_model_time_period_initialize();
void prv_model_time_initialize();

/******************************************************
Function: model_time_reset; model_time_future_reset
Description: Kernel function, reset the xModelTimeStart to the start time of current mode when system first enter this mode;
reset the xFutureModelTime as input_end of current unit;
Calls:
Called by: ps_mode_switch; ps_model_time_initialize
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_model_time_reset();
void prv_model_time_future_reset();

/******************************************************
Function:  get_model_time; get_future_model_time
Description: Kernel function, get the value of xFutureModelTime and xModelTimeStart
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
tick_t prv_model_time_get_model_time();
tick_t prv_model_time_get_future_model_time();

#endif
