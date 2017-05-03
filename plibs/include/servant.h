#ifndef __SERVANT_H
#define __SERVANT_H

#include "PSEFMconfigure.h"
#include "PSEFMport.h"
#include "exec_flow.h"


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
#define prv_servant_get_id(pservant)         (( (ps_servant_t *)pservant)->servant_id)
#define prv_servant_get_start_time(pservant) (( (ps_servant_t *)pservant)->start_time)
#define prv_servant_get_LED(pservant)        (( (ps_servant_t *)pservant)->LED)
#define prv_servant_get_LET(pservant)        (( (ps_servant_t *)pservant)->LET)
#define prv_servant_get_type(pservant)       (( (ps_servant_t *)pservant)->servant_type)
#define prv_servant_get_num(pservant)        (( (ps_servant_t *)pservant)->num)
#define prv_servant_get_arrive(pservant)     (( (ps_servant_t *)pservant)->arrive)


/******************************************************
Function: Setter functions
Description: kernel function, set the values of servants
arrive: which is used to record the arrived number of events of a servant when system runs
s;tart_time: each time when the servant start to run, record the start time which is got from the timestamp of input event
 * *****************************************************/
void    prv_servant_add_arrive     ( ps_servant_t *pservant);
void    prv_servant_set_arrive     ( ps_servant_t * pservant, int n);
void    prv_servant_clean_arrive   ( ps_servant_t * pservant);
void    prv_servant_set_start_time ( ps_servant_t *pservant, tick_t start_time);

/******************************************************
Function: prv_servant_trigger
Description: Kernel function, trigger the servant to run
Calls: port_trigger(ps_event_sem_t e)
Called by: prv_task_start()
Input: pservant(pointer of a servant)
Output:
Return: void
Others:
 * *****************************************************/
void    prv_servant_trigger( ps_servant_t * pservant);


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
ps_servant_t * ps_servant_create(id_t servant_id,
                                    int servant_type,
                                    tick_t LED,
                                    int num,   /* number of elements in src_array */
                                    ps_servant_t *src_array[],
                                    void (*runnable)(void *));

/******************************************************
Function: ps_servant_cooperate
Description: Programming API, for releasing the CPU actively
Calls: port_servant_yield()
Called by: the runnable function of servants in app.c
Input:
Output:
Return:
Others:
 * *****************************************************/
void ps_servant_cooperate();


#endif
