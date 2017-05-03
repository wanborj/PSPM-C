#include "PSEFMstart.h"
#include "PSEFMconfigure.h"
#include "PSEFMport.h"
#include "mode.h"

extern struct ps_condition_array_t cond;
extern struct ps_mode_array_t mod;
extern ps_event_sem_t sem[NUMOFSERVANTS];
extern list_t xEventGlobalList;
extern list_t xEventLocalList;
extern list_t xEventReadyList;

void prv_time_trigger()
{
    int i;
    ps_mode_t * current_mode;
    ps_task_t  * ptask;

	if( xEventReadyList.earliest_time <= port_get_current_time() ){ /* trigger the ready servant to run */

        xEventReadyList.earliest_time = 100000000;
        ps_servant_t * pservant = ((ps_event_t *)xEventReadyList.first->item)->pservant_dest;
        prv_servant_trigger( pservant );

    } else if( prv_model_time_is_mode_end() == 1){  /* start a new mode, and trigger all the tasks in this mode*/

        current_mode = prv_mode_get_current_mode();
        for(i=0;i<cond.num;++i){
			if(cond.conditions[i].condition() == 1){  /* one of the mode switch condition satisfied */
				if(cond.conditions[i].mode_dest != current_mode->mode_id){ /* mode switch to another mode*/
                    current_mode = mod.pmode[cond.conditions[i].mode_dest];  /* set the mode as current mode*/
                    prv_model_time_reset();  /* reset the xModeTimeStart only when mode switches */

                }
                prv_mode_start(cond.conditions[i].mode_dest);
                break;
            }
        }
        if(i == cond.num){
			prv_mode_start(current_mode->mode_id);  /* no mode switch happened, continue running in the current mode*/
        }
        prv_model_time_future_reset();  /* when enter new mode period, set the xFutureModelTime as the Input end. */

	}else if( prv_model_time_is_unit_start() == 1 ){ /* start the task at the start time of a new periods */

        current_mode = prv_mode_get_current_mode();
        for(i = 0; i < mod.pmode[current_mode->mode_id]->num; ++i){
            ptask = mod.pmode[current_mode->mode_id]->tasks[i];
            if(1 == prv_model_time_is_period_start(ptask)){
                prv_task_start( ptask);
            }
        }
        prv_model_time_future_reset();  /* when enter new unit, set the xFutureModelTime as the Input end. */

    }else if( xEventReadyList.length == 0 &&
            ( xEventGlobalList.earliest_time <= port_get_current_time() ||
                xEventLocalList.earliest_time <= port_get_current_time())){ /* trigger R-Servant to run to process the events in list */

        port_trigger( sem[NUMOFSERVANTS-1] );

    }else{

    }
}


void myHardwareInit(){
	init_led();
	init_rs232();
	enable_rs232_interrupts();
	enable_rs232();
}

void system_start()
{
    myHardwareInit();
    prv_model_time_initialize();
    prv_event_list_initialize();
    prv_ef_create();
    prv_mode_start(0);
    port_scheduler_start();
}

void myTraceCreate      (){
}

void myTraceSwitchedIn  (){
}

void myTraceSwitchedOut	(){
}


void vApplicationTickHook( void )
{
	prv_time_trigger();
}
