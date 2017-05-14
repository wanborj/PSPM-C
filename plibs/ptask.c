#include "ptask.h"
#include "ef.h"
#include "PSPMconfigure.h"
#include "PSPMport.h"

ps_task_t tasks[NUMOFTASKS];

ps_task_t  ps_task_create(prv_id_t		tid,
							prv_tick_t		let,
							prv_num_t		num,
							ps_servant_t	servants[])
{
    int i;
	tasks[tid] = (ps_task_t)port_malloc(sizeof(struct ptask));
	tasks[tid]->servants = (prv_list_t *)port_malloc(sizeof(prv_list_t));
	tasks[tid]->tid = tid;
	tasks[tid]->let = tasks[tid]->period = tasks[tid]->deadline = let;

    for(i=0;i<num;++i){
		prv_list_insert(servants[i], tasks[tid]->servants);
		servants[i]->tid = tid;
    }

    return tasks[tid];
}
