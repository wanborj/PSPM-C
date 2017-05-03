#ifndef __TASK_H
#define __TASK_H

#include "PSEFMconfigure.h"

/******************************************************
Function: getter function
description: kernel function, get the values of properties of task
structure:
typedef struct task{
    id_t task_id;
    tick_t LET;
    tick_t period;
    tick_t deadline;
    int servant_num;      // number of I-servants
    ps_servant_t * servants[NUMOFSERVANTS];  // I-servants in this task
}ps_task_t;
 * *****************************************************/
#define prv_task_get_id(ptask)          (( (p_task_t *)ptask)->task_id)
#define prv_task_get_LET(ptask)         (( (p_task_t *)ptask)->LET)
#define prv_task_get_period(ptask)      (( (p_task_t *)ptask)->period)
#define prv_task_get_deadline(ptask)    (( (p_task_t *)ptask)->deadline)
#define prv_task_get_servant_num(ptask) (( (p_task_t *)ptask)->num)

ps_servant_t * prv_task_get_sensor   ( ps_task_t *);  /* Assuming that each task has only one sensor */
ps_servant_t * prv_task_get_actuator ( ps_task_t *); /* Assuming that each task has only one actuator */


/******************************************************
Function: prv_task_start
Description: Kernel function, trigger a task to run. In fact, it trigger the sensor of a task to run.
Calls: prv_servant_trigger()
Called by: prv_mode_start()
Input: ptask( pointer of a task )
Output:
Return: void
Others:
 * *****************************************************/
void   prv_task_start(ps_task_t * ptask);




/******************************************************
Function: setter function
Description: Programming API, set the value of properties of tasks
Calls:
Called by: main()
Input: task_id; LET(specified LET value of task); servants[](the servants it includes); servant_num( the size of servants array )
Output:
Return: the pointer of a task
Others:
 * *****************************************************/
ps_task_t *  ps_task_create(id_t task_id,  /* start from zero for saving memory */
                    tick_t LET,
                    ps_servant_t * servants[],
                    int servant_num);


#endif

