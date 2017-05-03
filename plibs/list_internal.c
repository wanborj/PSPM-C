#include "list_internal.h"
#include "event.h"

void prv_list_initialize(list_t * pEventList)
{
    pEventList->length = 0;
    pEventList->earliest_time = 100000000; // set to the max time
    pEventList->first = pEventList->last = NULL;
}

void prv_item_initialize(void * item, item_t * pEventItem)
{
    pEventItem->item = item;
    pEventItem->owner = NULL;
    pEventItem->next = NULL;
    pEventItem->prev = NULL;
}

void prv_list_insert( item_t * pEventItem, list_t * pEventList)
{
    ps_event_t * pevent = (ps_event_t *)prv_item_get_entity(pEventItem);

    if(pEventList->length == 0){
        pEventList->first = pEventList->last = pEventItem;
        pEventItem->prev = NULL;
        pEventItem->next = NULL;
    }else{
        pEventList->last->next = pEventItem;
        pEventList->first->prev = pEventItem;

        pEventItem->prev = pEventList->last;
        pEventItem->next = pEventList->first;
        pEventList->last = pEventItem;
    }

    pEventList->length ++;
    pEventItem->owner = (void *)pEventList;

    if( pEventList->earliest_time >  pevent->tag.timestamp){
        pEventList->earliest_time = pevent->tag.timestamp;
    }
}

static int tag_compare(ps_event_t * pe1, ps_event_t *pe2)
{
    if( pe1->tag.timestamp < pe2->tag.timestamp ){
        return 1;
    }else if( pe1->tag.timestamp == pe2->tag.timestamp ){
        if( pe1->tag.microstep < pe2->tag.microstep ){
            return 1;
        }else if( pe1->tag.microstep == pe2->tag.microstep ){
            if( pe1->tag.level < pe2->tag.level ){
                return 1;
            }
        }
    }

    return 0;
}

void prv_list_insert_sorted(item_t * pEventItem, list_t * pEventList)
{
    ps_event_t * pevent = (ps_event_t *)prv_item_get_entity(pEventItem);
    volatile  item_t * pIndex;
    int i, len;

    if(pEventList->length == 0){
        pEventList->first = pEventList->last = pEventItem;
        pEventItem->prev = pEventItem;
        pEventItem->next = pEventItem;
    }else{
        len = pEventList->length;
        pIndex = prv_list_get_first_item(pEventList);
        for( i = 0; i < len; ++i){
            if(1 == tag_compare(pevent, prv_item_get_entity(pIndex))){
                if( i == 0 ){
                    pEventList->first = pEventItem;
                }
                break;
            }
            pIndex = prv_item_get_next(pIndex);
        }
        if( i == len ){
            pEventList->last = pEventItem;
        }
        pEventItem->next = (item_t *)pIndex;
        pEventItem->prev = pIndex->prev;
        pIndex->prev->next = pEventItem;
        pIndex->prev = pEventItem;
    }

    pEventList->length ++;
    pEventItem->owner = (void *)pEventList;
    if( pEventList->earliest_time >  pevent->tag.timestamp){
        pEventList->earliest_time = pevent->tag.timestamp;
    }
}

void prv_list_remove(item_t * pEventItem)
{
    list_t * pList = (list_t *)pEventItem->owner;

    if(pList->length == 0){
        port_print("error: no event could be remove in this event list; in prv_list_remove()\n\r");
    }

    if(pList->length == 1){
        pList->first = NULL;
        pList->last = NULL;
    }else{
        if(pEventItem == prv_list_get_first_item(pList)){
            pList->first = pEventItem->next;
        }
        if(pEventItem == prv_list_get_last_item(pList)){
            pList->last = pEventItem->prev;
        }
        pEventItem->prev->next = pEventItem->next;
        pEventItem->next->prev = pEventItem->prev;
    }
    pEventItem->owner = NULL;
    pList->length --;
}


void prv_list_earliest_time_update(list_t * pEventList)
{
    int i, len;
    if(pEventList->length == 0)
    {
        pEventList->earliest_time = 100000000;
    }else{
        len = prv_list_get_length(pEventList);
        volatile  item_t * pIndex = prv_list_get_first_item(pEventList);
        pEventList->earliest_time = ((ps_event_t *)pIndex->item)->tag.timestamp;

        for( i = 0; i < len; i++ , pIndex = prv_item_get_next(pIndex)){
            if(pEventList->earliest_time > ((ps_event_t *)pIndex->item)->tag.timestamp)
            {
                pEventList->earliest_time = ((ps_event_t *)pIndex->item)->tag.timestamp;
            }
        }
    }
}


item_t * prv_list_receive(list_t * pEventList)
{
    if(pEventList->length == 0){
        return NULL;
    }

    item_t * pitem = pEventList->first;
    prv_list_remove(pitem);

    return pitem;
}


