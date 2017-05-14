#ifndef __TASK_H
#define __TASK_H

#include "servant.h"


/******************************************************
Function: getter function
description: kernel function, get the values of properties of task
structure:
typedef struct ptask{
    id_t tid;
    tick_t LET;
    tick_t period;
    tick_t deadline;
    int servant_num;      // number of I-servants
    ps_servant_t * servants[NUMOFSERVANTS];  // I-servants in this task
}ps_task_t;
 * *****************************************************/
#define prv_task_get_id(t)          (( (p_task_t)t)->tid)
#define prv_task_get_let(t)         (( (p_task_t)t)->let)
#define prv_task_get_period(t)      (( (p_task_t)t)->period)
#define prv_task_get_deadline(t)    (( (p_task_t)t)->deadline)
#define prv_task_get_servant_num(t) (( (p_task_t)t)->servants->length)

/******************************************************
Function: setter function
Description: Programming API, set the value of properties of tasks
Calls:
Called by: main()
Input: tid; LET(specified LET value of task); servants[](the servants it includes); servant_num( the size of servants array )
Output:
Return: the pointer of a task
Others:
 * *****************************************************/
ps_task_t  ps_task_create(prv_id_t		tid,  /* start from zero for saving memory */
						prv_tick_t		let,
						prv_num_t		num,
						ps_servant_t	servants[]);


#endif

