#ifndef __SERVANT_H
#define __SERVANT_H

#include "PSPMconfigure.h"
#include "PSPMport.h"
#include "list_internal.h"
#include "hashtable.h"
#include "ef.h"



/******************************************************
Function: Getter functions
description: kernel function, get the values of properties of servants
structure:
typedef struct servant{
    id_t servant_id;
    tick_t start_time;
    tick_t LET;
    tick_t LED;
    int servant_type; // 0 for sensor; 1 for controller; 2 for actuator
    int num;   // the src servants of this servant
    int arrive; // arrived number of events from src servants
}ps_servant_t;
 * *****************************************************/
#define prv_servant_get_id(s)			 (((ps_servant_t)s)->sid)
#define prv_servant_get_wcet(s)			 (((ps_servant_t)s)->wcet)
#define prv_servant_get_type(s)			 (((ps_servant_t)s)->type)
#define prv_servant_get_task(s)			 (((ps_servant_t)s)->tid)
#define prv_servant_get_inbox(s)		 (((ps_servant_t)s)->inbox)



/******************************************************
Function: ps_servant_create
Description: Programming API, create a entity of a servant which is implemented based on non-preempted task in RTOS
Calls: port_servant_create(); prv_ef_add_relation()
Called by: main()
Input: servant_id; servant_type(include 0(sensor), 1(controller), 2(actuator)); LED(logical execution time duration);
src_array(the source servants that communicate to this servant); runnable( a function invoked when the servant runs )
Output:
Return: pointer of the servant
Others:
 * *****************************************************/
/*
 * Function:
 * 1) record the information of a servant
 * 2) create the platform dependent thread
 * 3) build the communication relation
 * */
ps_servant_t ps_servant_create(   prv_id_t sid,
									prv_stype_t type,
                                    prv_tick_t wcet,
                                    prv_num_t  num,   /* number of elements in src_array */
                                    ps_servant_t src_array[],
                                    void (*runnable)(void *));

/******************************************************
Function: prv_servant_trigger
Description: Kernel function, trigger the servant to run
Calls: port_trigger(ps_event_sem_t e)
Called by: prv_task_start()
Input: s(pointer of a servant)
Output:
Return: void
Others:
 * *****************************************************/
void    ps_servant_wake( ps_servant_t * s);


/******************************************************
Function: ps_servant_wait
Description: Programming API, for releasing the CPU actively
Calls: port_servant_yield()
Called by: the runnable function of servants in app.c
Input:
Output:
Return:
Others:
 * *****************************************************/
void ps_servant_wait(prv_id_t sid);


/******************************************************
Function: ps_servant_send
Description: Programming API, send the message to target servants
Calls:
Called by: the runnable function of servants in app.c
Input:
Output:
Return:
Others:
 * *****************************************************/
void ps_message_send( ps_servant_t target, ps_message_t message );

/******************************************************
Function: ps_servant_receive
Description: Programming API, return the list of receiving messages
Calls:
Called by: the runnable function of servants in app.c
Input:
Output:
Return:
Others:
 * *****************************************************/
ps_message_t ps_message_receive(ps_servant_t src);


#endif
