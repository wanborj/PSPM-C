#ifndef __MODE_H
#define __MODE_H

#include "PSEFMconfigure.h"
#include "ptask.h"
#include "event.h"

struct ps_condition_array_t{
    int num;
    ps_mode_cond conditions[NUMOFCONDS];
};

struct ps_mode_array_t{
    int num;
    ps_mode_t * pmode[NUMOFMODES];
};

/******************************************************
Function: ps_mode_create
Description: Programming API, create mode according to the tasks in parameters
Calls:
Called by: main()
Input: mode_id(mark the mode for mode switch); task_array[](tasks the mode contains); num (number of tasks in the mode)
Output:
Return: void
Others:
 * *****************************************************/
void ps_mode_create(id_t mode_id, ps_task_t * task_array[], int num);


/******************************************************
function: prv_mode_add_task
description: kernel function, add one task into specific mode
calls:
called by: ps_mode_create()
input: mode_id( id of mode ); ptask( the pointer of task )
output:
return: void
others:
 * *****************************************************/
void prv_mode_add_task(id_t mode_id, ps_task_t * ptask);


/******************************************************
function: getter function
description: kernel function, get the values of properties of mode
structure:
typedef struct mode{
    id_t mode_id; // start from zero for saving memory, the id in modes array represents the mode_id as well
    tick_t period;
    tick_t unit;
    int num;  // number of tasks
    ps_task_t * tasks[NUMOFTASKS];
}ps_mode_t;
 * *****************************************************/
tick_t prv_mode_get_mode_period ( id_t mode_id);
tick_t prv_mode_get_mode_unit   ( id_t mode_id);
int    prv_mode_get_task_num    ( id_t mode_id);

/******************************************************
function: setter function
description: kernel function, set the values of properties of mode
structure:
typedef struct mode{
    id_t mode_id; // setted by create function
    tick_t period;
    tick_t unit;
    int num;  // setted by create function
    ps_task_t * tasks[NUMOFTASKS]; // same as above
}ps_mode_t;
 * *****************************************************/
void prv_mode_set_mode_unit   ( id_t mode_id, tick_t unit);
void prv_mode_set_mode_period ( id_t mode_id , tick_t period);

/******************************************************
Function: ps_mode_switch_create
Description: Programming API, create the relationship of the mode switch condition with the destination mode
Calls:
Called by: main()
Input: condition( to judge whether mode switch happended ); mode_dest( the target mode for this mode switch condition )
Output:
Return: void
Others:
 * *****************************************************/
void ps_mode_switch_create(bool (*condition)(void), id_t mode_dest);


/******************************************************
Function: prv_mode_start
Description: kernel function, triggered all the tasks in one mode to run when time reach the beginning of a mode
Calls: prv_task_start()
Called by: ps_mode_switch(); system_start()
Input: mode_id
Output:
Return: void
Others:
 * *****************************************************/
void prv_mode_start(id_t mode_id);

/******************************************************
Function: prv_mode_get_current_mode
Description: Kernel function, get the pointer of current mode which only may be changed when mode switch
Calls:
Called by: ps_mode_switch(); prv_model_time_unit_start(); prv_model_time_is_unit_start(); prv_model_time_output_end(); prv_model_time_is_mode_end();
Input:
Output:
Return: the pointer of current mode
Others:
 * *****************************************************/
ps_mode_t * prv_mode_get_current_mode();


#endif
