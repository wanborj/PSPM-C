#ifndef __LIST_INTERNAL_H
#define __LIST_INTERNAL_H

#include "PSEFMport.h"
#include "PSEFMconfigure.h"



/******************************************************
Function: list and item get function
Description: get the properties of a list or a item

typedef struct item{
    void * item;   // the object item, event or servant
    void * owner;   // the list it belongs to
    struct item *next;
    struct item *prev;
} item_t;

typedef struct list{
    int     length;
    tick_t  earliest_time; // the earliest time-stamp of event in a event list
    item_t *first;
    item_t *last;
} list_t;
 * *****************************************************/
#define prv_list_get_length(pevent_list)         (((list_t *)pevent_list)->length)
#define prv_list_get_earliest_time(pevent_list)  (((list_t *)pevent_list)->earliest_time)
#define prv_list_get_first_item(pevent_list)     (((list_t *)pevent_list)->first)
#define prv_list_get_last_item(pevent_list)      (((list_t *)pevent_list)->last)

#define prv_item_get_entity(pevent_item)   (((item_t *)pevent_item)->item)
#define prv_item_get_owner(pevent_item)    (((item_t *)pevent_item)->owner)
#define prv_item_get_next(pevent_item)     (((item_t *)pevent_item)->next)
#define prv_item_get_prev(pevent_item)     (((item_t *)pevent_item)->prev)


/******************************************************
Function: list and item initialize
Description: Kernel function, initialize the properties of list or item
Calls:
Called by: prv_event_list_initialize; prv_event_initialize
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_list_initialize(list_t * pEventList);
void prv_item_initialize(void * item, item_t * pEventItem);

/******************************************************
Function: insert( insert a item into the tail of a list ); insert_sorted( insert a item into specific position with its tag )
 * *****************************************************/
void prv_list_insert( item_t * pEventItem, list_t * pEventList);
void prv_list_insert_sorted(item_t * pEventItem, list_t * pEventList);


/******************************************************
Function: earliest_time_update
Description: kernel function, update the earliest_time property of lists including xEventReadyList, xEventGlobalList, xEventLocalList.
Calls:
Called by:  ps_event_receive; prv_ef_sorting; prv_ef_triggering
Input: the pointer of list
Output:
Return:
Others:
 * *****************************************************/
void prv_list_earliest_time_update(list_t * pEventList);

/******************************************************
Function: prv_list_remove
Description: kernel function, remove a specific event from its list
Calls:
Called by:
Input: the pointer of item in a event
Output:
Return:
Others:
 * *****************************************************/
void prv_list_remove(item_t * pEventItem);


/******************************************************
Function: prv_list_receive
Description: kernel function, remove the first event in a list
Calls: prv_list_remove
Called by: ps_event_receive
Input: the pointer of a list
Output: the item of first event in a list
Return:
Others:
 * *****************************************************/
item_t * prv_list_receive(list_t * pEventList);

#endif
