#include "ptask.h"
#include "PSEFMconfigure.h"
#include "PSEFMport.h"

struct ps_task_array_t{
    int num;
    ps_task_t tasks[NUMOFTASKS*NUMOFMODES];
};

struct ps_task_array_t tsk = {0};


ps_servant_t * prv_task_get_sensor(ps_task_t * ptask)
{
    int i;
    for(i = 0; i < ptask->servant_num; ++i){
        if(ptask->servants[i]->servant_type == 0){
            return ptask->servants[i];
        }
    }
    return NULL;
}

ps_servant_t * prv_task_get_actuator(ps_task_t * ptask)
{
    int i;
    for(i = 0; i < ptask->servant_num; ++i){
        if(ptask->servants[i]->servant_type == 2){
            return ptask->servants[i];
        }
    }
    return NULL;
}

void   prv_task_start(ps_task_t * ptask)
{
    ps_servant_t * sensor = prv_task_get_sensor(ptask);
    prv_servant_trigger( sensor );
}

ps_task_t * ps_task_create(id_t task_id, tick_t LET,
                       ps_servant_t * servants[],
                       int servant_num)
{
    int i;
    int pos = tsk.num;
    tsk.tasks[pos].task_id = task_id;
    tsk.tasks[pos].LET = tsk.tasks[pos].period = tsk.tasks[pos].deadline = LET;
    tsk.tasks[pos].servant_num = servant_num;

    for(i=0;i<servant_num;++i){
        tsk.tasks[pos].servants[i] = servants[i];
        tsk.tasks[pos].servants[i]->LET = LET;
    }
    tsk.num ++;

    return &tsk.tasks[pos];
}
