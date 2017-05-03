#include "exec_flow.h"

typedef struct relation{
    int num;
    ps_servant_t * pservants[NUMOFINS];
} ps_relation_t;

ps_relation_t srcs[NUMOFSERVANTS];
ps_relation_t dests[NUMOFSERVANTS];

static ps_servant_t * pcurrent_servant;
extern ps_servant_t * pservants[NUMOFSERVANTS];
extern list_t xEventGlobalList;
extern list_t xEventReadyList;
extern list_t xEventLocalList;
extern ps_event_sem_t sem[NUMOFSERVANTS];

int prv_ef_get_dest_num(ps_servant_t * pservant)
{
    return dests[pservant->servant_id].num;
}

ps_servant_t * prv_ef_get_ith_dest(ps_servant_t * pservant, int i)
{
    return dests[pservant->servant_id].pservants[i];
}

void prv_ef_set_current_servant(ps_servant_t * pservant)
{
    pcurrent_servant = pservant;
}

ps_servant_t * prv_ef_get_current_servant()
{
    return pcurrent_servant;
}

void prv_ef_add_relation(ps_servant_t * pservant_src, ps_servant_t * pservant_dest)
{
    int num = dests[pservant_src->servant_id].num;
    dests[pservant_src->servant_id].pservants[num] = pservant_dest;
    dests[pservant_src->servant_id].num ++;

    num = srcs[pservant_dest->servant_id].num;
    srcs[pservant_dest->servant_id].pservants[num] = pservant_src;
    srcs[pservant_dest->servant_id].num ++;
}

// R-Servant
void prv_ef_sorting()
{
    int i,len;
    item_t * pevent_item, *pevent_temp;
    tick_t pcurrent_time = port_get_current_time();


    // sort the executable events
    pevent_item = prv_list_get_first_item( &xEventGlobalList );
    len = prv_list_get_length( &xEventGlobalList );
    for( i = 0; i < len; ++i){

        if(prv_event_get_timestamp((ps_event_t *)prv_item_get_entity(pevent_item)) <= pcurrent_time){
            pevent_temp = pevent_item;
            pevent_item = prv_item_get_next( pevent_item);
            prv_list_remove(pevent_temp);
            prv_list_insert_sorted( pevent_temp, &xEventLocalList);

            // add the arrive record in dest servant
            prv_servant_add_arrive( prv_event_get_dest((ps_event_t *)prv_item_get_entity(pevent_temp)));
        }else{
            pevent_item = prv_item_get_next( pevent_item );
        }
    }
    prv_list_earliest_time_update( &xEventGlobalList);
}


void prv_ef_triggering()
{
    item_t * pevent_item, *pevent_temp, *pevent_iterator;
    ps_servant_t * pservant;
    int src_num, i, j, len, flag;
    tick_t timestamp;

    len = prv_list_get_length(&xEventLocalList);
    pevent_item = prv_list_get_first_item(&xEventLocalList);


    for( i = 0 ; i < len;  i++){
        //port_print("I'm in prv_ef_triggering()\n\r");

        pservant = prv_event_get_dest((ps_event_t *)prv_item_get_entity(pevent_item));
        src_num = prv_servant_get_num(pservant);

        if(prv_servant_get_arrive(pservant) == src_num){
            prv_servant_clean_arrive(pservant);  /* set the arrive of pservant to 0 */
            flag = prv_event_can_process((ps_event_t *)prv_item_get_entity(pevent_item));

            if(2 == flag || 1 == flag){
                timestamp = prv_event_get_timestamp(prv_item_get_entity(pevent_item));
                pevent_iterator = pevent_item;
                for(j = 0; j < src_num && i < len;){
                    if(pservant == prv_event_get_dest((ps_event_t *)pevent_iterator->item)){
                        /* send events to ready list */
                        pevent_temp = pevent_iterator;
                        pevent_iterator = prv_item_get_next(pevent_iterator);

                        /* Set all the events for one dest servant to the same time-stamp*/
                        ((ps_event_t *)pevent_temp->item)->tag.timestamp = timestamp;
                        prv_list_remove(pevent_temp);
                        prv_list_insert(pevent_temp, &xEventReadyList);
                        j ++;
                    }else{
                        pevent_iterator = prv_item_get_next(pevent_iterator);
                    }
                    i ++;
                } // end for
                //prv_servant_trigger(pservant);
                break;
            } else{
                // transit all the left event in local list into the global event list
                pevent_temp = pevent_item;
                pevent_item = prv_item_get_next(pevent_item);
                prv_list_remove(pevent_temp);
                prv_list_insert(pevent_temp,&xEventGlobalList);
            }
        } else{
            pevent_item = prv_item_get_next(pevent_item);
        }
    }// end for
    prv_list_earliest_time_update( &xEventLocalList);
}


static void R_Servant()
{
    while(1){
        port_wait(sem[NUMOFSERVANTS-1]);

        //port_print("i'm in R-Servant\n\r");
        prv_ef_sorting();
        prv_ef_triggering();
    }
}

void prv_ef_create()
{
    port_servant_create(R_Servant,NULL, 1);
}
