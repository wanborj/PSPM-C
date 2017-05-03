#include "servant.h"
#include "exec_flow.h"
#include "PSEFMconfigure.h"

extern ps_event_sem_t sem[NUMOFSERVANTS];

ps_servant_t servants[NUMOFSERVANTS];


void prv_servant_set_arrive( ps_servant_t * pservant, int n)
{
    pservant->arrive = n;
}

void prv_servant_clean_arrive( ps_servant_t * pservant)
{
    prv_servant_set_arrive(pservant, 0);
}

void prv_servant_add_arrive(ps_servant_t *pservant)
{
    pservant->arrive ++;
}

void prv_servant_trigger( ps_servant_t * pservant)
{
    id_t servant_id = prv_servant_get_id( pservant );
    port_trigger(sem[servant_id]);  // trigger the sem of the dest servant
}

void prv_servant_set_start_time ( ps_servant_t *pservant, tick_t start_time)
{
    pservant->start_time = start_time;
}

/* create servant and record the time,relation and function information */
ps_servant_t * ps_servant_create(id_t servant_id, int servant_type, tick_t LED,
                                int num,
                                ps_servant_t *src_array[],
                                void (*runnable)(void *))
{
    int i;
    ps_servant_t * pservant = &servants[servant_id];

    pservant->servant_id = servant_id;
    pservant->servant_type = servant_type;
    pservant->start_time = 0;
    pservant->LED = LED;
    pservant->num = num;
    pservant->arrive = 0;

    port_servant_create(runnable, &pservant->servant_id , 2);

    for(i = 0; i < num; ++ i){
        prv_ef_add_relation(src_array[i], pservant);
    }

	return pservant;
}

void ps_servant_cooperate()
{
    // print the start time of current servant
    ps_servant_t * pservant = prv_ef_get_current_servant();
    vPrintNumber(pservant->start_time);
    port_servant_yield();
}

//void ps_servant_wait()
//{
//
//}


