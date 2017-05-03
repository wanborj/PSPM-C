#include "event.h"
#include "exec_flow.h"
#include "list_internal.h"
#include "model_time.h"


extern ps_servant_t servants[NUMOFSERVANTS];
extern tick_t xFutureModelTime;

list_t xEventIdleList;
list_t xEventGlobalList;
list_t xEventLocalList;
list_t xEventReadyList;
ps_event_t events[NUMOFRELATIONS];
ps_event_sem_t sem[NUMOFSERVANTS];

static void prv_event_sem_initialize()
{
	int i;
	for( i = 0; i < NUMOFSERVANTS; ++ i){
        port_pure_event_create( sem[i] );
        port_wait( sem[i] );
	}
}

void prv_event_initialize()
{
    int i;
    for( i = 0; i < NUMOFRELATIONS; ++i ){
        events[i].pservant_src = NULL;
        events[i].pservant_dest = NULL;
        prv_item_initialize( (void *)&events[i], &events[i].eventItem);

        prv_list_insert(&events[i].eventItem, &xEventIdleList);
    }

	prv_event_sem_initialize();  // init the semaphores
}

void prv_event_list_initialize()
{
    prv_list_initialize(&xEventIdleList);
    prv_list_initialize(&xEventGlobalList);
    prv_list_initialize(&xEventLocalList);
    prv_list_initialize(&xEventReadyList);

    prv_event_initialize();
}


// send event to xEventGlobalList
void prv_event_send(ps_event_t *pevent)
{
    prv_list_insert(&pevent->eventItem, &xEventGlobalList);
    if( xEventGlobalList.earliest_time >  pevent->tag.timestamp){
        xEventGlobalList.earliest_time = pevent->tag.timestamp;
    }
}

void prv_event_delete(ps_event_t * pevent)
{
    prv_list_insert(&pevent->eventItem, &xEventIdleList);
}

void prv_event_tag_set(ps_event_t * pevent, int microstep)
{
    ps_servant_t * pservant_src = pevent->pservant_src;
	ps_servant_t * pservant_dest = pevent->pservant_dest;

    if(prv_servant_get_type(pservant_dest) == 1){
        if( prv_servant_get_type(pservant_src) == 0){  // sensor communicate with controller
			pevent->tag.timestamp = prv_model_time_input_end();
		}else{ // controller communicate with controller
			pevent->tag.timestamp = prv_servant_get_start_time(pservant_src) + prv_servant_get_LED(pservant_src);
		}
	}
	else if( prv_servant_get_type(pservant_dest) == 2){ // actuator
		pevent->tag.timestamp = prv_model_time_output_start();

	}else{
        port_print("error: unknow event type in prv_event_tag_set()\n\r");
	}
    pevent->tag.microstep = microstep;
    pevent->tag.level = prv_servant_get_id(pservant_dest);
}




// 1 for overlap ; 0 for not overlap
int  prv_event_is_overlap(ps_event_t * pevent)
{
	tick_t led = pevent->pservant_dest->LED;
	if( pevent->tag.timestamp + led > prv_model_time_output_start()){
		return 1;
	}else{
		return 0;
	}
}

int prv_event_tag_update(ps_event_t * pevent)
{
	pevent->tag.timestamp = xFutureModelTime; // bug
	tick_t led = pevent->pservant_dest->LED;
	if( !prv_event_is_overlap(pevent) && xFutureModelTime < prv_model_time_output_start()){
		/* process event pevent at the xFuturemodelTime */
		xFutureModelTime += led;
        return 1; /* event can be processed */
	}else if (prv_event_is_overlap(pevent) && xFutureModelTime < prv_model_time_output_start()){
		xFutureModelTime = prv_model_time_output_end()+INPUT;
		pevent->tag.timestamp = xFutureModelTime;
		xFutureModelTime += led;
		return 0;
	}else{
		xFutureModelTime += led;
		return 0;
	}
}

// 2 for actuator
// 1 for controller
// 0 for no valid
int prv_event_can_process(ps_event_t * pevent)
{
	int servant_type = pevent->pservant_dest->servant_type;
    // servant_type can't be sensor which only triggered by periodic timing events
	if( servant_type == 2){
		return 2;
	}
    if (servant_type == 1){
		if(prv_event_tag_update(pevent)){
			return 1;
		}else{
			return 0;
		}
	}
}

// wait for specific signal
void ps_event_wait( void * para )
{
    id_t servant_id = * (id_t *) para;
    port_wait(sem[servant_id]);
    prv_ef_set_current_servant( &servants[servant_id] );
}


ps_data_t * ps_event_receive()
{
    int len, i, first_event = -1, count ;
    ps_event_t * pevent[NUMOFINS];
    ps_servant_t * pservant;
    item_t * item;
    if( 0 != (len = prv_list_get_length( &xEventReadyList ) ) )
    {

        pservant = prv_ef_get_current_servant();
        /* set the start time of servant as the time-stamp of event when servant start to run */

        for(i = 0, count = 0; i < len && count < pservant->num; ++i){
            item                    = prv_list_receive( &xEventReadyList );
            pevent[i]               = (ps_event_t *)item->item;

            if( first_event == -1 && pevent[i]->pservant_dest == pservant ){
                first_event = i;
                prv_servant_set_start_time( pservant, pevent[i]->tag.timestamp);
                count ++;
                prv_event_delete(pevent[i]);   /* not real delete, keep the memory and data */
                continue;  /* record the data of first input event*/
            }

            if( pevent[i]->pservant_dest == pservant ){
                pevent[first_event]->data.data[count++] = pevent[i]->data.data[0];  /* integrate events' data */
                prv_event_delete(pevent[i]);   /* not real delete, keep the memory and data */
            }
        }

        prv_list_earliest_time_update(&xEventReadyList);
        return &pevent[first_event]->data;
    }
    return NULL;
}


void ps_event_create( ps_data_t * pdata)
{
    int i, num;
    item_t * pitem;
	ps_event_t * pevent_temp;

    ps_servant_t * pservant = prv_ef_get_current_servant();
    num = prv_ef_get_dest_num(pservant);

    for( i = 0; i < num; ++i ){
        pitem = prv_list_receive(&xEventIdleList);

        if(pitem == NULL){
            port_wait("error: no idle event left. in ps_event_create() prv_list_receive() Error!!!\n\r");
            return;
        }
        pevent_temp = (ps_event_t *)pitem->item;

        pevent_temp->pservant_src  = pservant;
        pevent_temp->pservant_dest = prv_ef_get_ith_dest(pservant,i);

        // update the tag of event according to the timing semantics of PSEFM
        prv_event_tag_set(pevent_temp, i);
        pevent_temp->data.data[0]  = pdata->data[0];
        pevent_temp->data.num = 1;

        prv_event_send(pevent_temp);
    }
}


