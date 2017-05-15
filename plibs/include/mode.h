#ifndef __MODE_H
#define __MODE_H

#include "PSPMconfigure.h"
#include "ptask.h"
#include "event.h"

#define prv_mode_get_task_num(mode)		(((ps_mode_t)mode)->tasks.legnth)
#define prv_mode_get_period(mode)		(((ps_mode_t)mode)->period)
#define prv_mode_get_unit(mode)			(((ps_mode_t)mode)->unit)
#define prv_mode_get_tasks(mode)		(((ps_mode_t)mode)->tasks)
/******************************************************
Function: ps_mode_create
Description: Programming API, create mode according to the tasks in parameters
Calls:
Called by: main()
Input: mid(mark the mode for mode switch); task_array[](tasks the mode contains); num (number of tasks in the mode)
Output:
Return: void
Others:
 * *****************************************************/
void ps_mode_create(prv_id_t mid, prv_num_t num, ps_task_t task_array[]);


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
void ps_mode_switch_create(bool (*condition)(void), prv_id_t mode_dest);


/******************************************************
Function: prv_mode_start
Description: kernel function, triggered all the tasks in one mode to run when time reach the beginning of a mode
Calls: prv_task_start()
Called by: ps_mode_switch(); system_start()
Input: mid
Output:
Return: void
Others:
 * *****************************************************/
void prv_mode_switch();

/******************************************************
Function: prv_mode_get_current_mode
Description: Kernel function, get the pointer of current mode which only may be changed when mode switch
Calls:
Called by: ef
Input:
Output:
Return: the pointer of current mode
Others:
 * *****************************************************/
ps_mode_t prv_mode_get_current_mode();

// called by ef
prv_tick_t prv_mode_get_modestart();

// called by prv_ef_is_time_to_trigger()
void prv_mode_set_modestart(prv_tick_t t);

int prv_mode_is_period_end();

#endif
