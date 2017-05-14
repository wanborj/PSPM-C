#include "list_internal.h"
#include "PSPMconfigure.h"
#include "PSPMport.h"


void prv_list_initialize(prv_list_t * L)
{
    L->length = 0;
    L->first = L->last = L->current = NULL;
}

void prv_item_initialize(void * entity, prv_item_t * item)
{
    item->entity= entity;
    item->owner = NULL;
    item->next = NULL;
    item->prev = NULL;
}

void prv_list_set_current_item(prv_list_t * l)
{
	l->current = l->current->next;
}

void * prv_list_get_current_entity(prv_list_t * L)
{
	void * s = ((prv_list_t *)L)->current->entity;
	((prv_list_t *)L)->current = ((prv_list_t *)L)->current->next;

	return s;
}

void prv_list_insert( void * entity, prv_list_t * L)
{
	prv_item_t * item = (prv_item_t*)port_malloc(sizeof(prv_item_t));
	prv_item_initialize(entity, item);

    if(L->length == 0){
        L->first = L->last = L->current= item;
        item->prev = item;
		item->next = item;
    }else{
        L->last->next = item;
        L->first->prev = item;

        item->prev = L->last;
        item->next = L->first;
		L->last = item; // insert in the end of list
		//L->fist = item; // insert in the start of list
    }

    L->length ++;
    item->owner = (void *)L;
}


void prv_list_insert_sorted(void * entity, prv_list_t * L, int (*compare)(void *, void *))
{
    volatile  prv_item_t * pIndex;
    int i;
	int len = prv_list_get_length(L);

	prv_item_t * item = (prv_item_t*)port_malloc(sizeof(prv_item_t));
	prv_item_initialize(entity, item);

    if(len == 0){
        L->first = L->last = L->current = item;
        item->prev = item;
        item->next = item;
    }else{
        pIndex = prv_list_get_first_item(L);
        for( i = 0; i < len; ++i, pIndex = prv_item_get_next(pIndex)){
            if(1 == compare(entity, prv_item_get_entity(pIndex))){
                if( i == 0 ){
					L->first = item;  // the entity should be the first one
                }
				break; // find the right position
            }
        }
        if( i == len ){
			L->last = item; // the entity should be the last one
        }
        item->next = (prv_item_t *)pIndex;
        item->prev = pIndex->prev;
        pIndex->prev->next = item;
        pIndex->prev = item;
    }

    L->length ++;
    item->owner = (void *)L;
}


void prv_list_remove(prv_item_t * item)
{
    prv_list_t * L = (prv_list_t *)item->owner;

    if(L->length == 0){
        //port_print("error: no event could be remove in this event list; in prv_list_remove()\n\r");
		return;
    }

    if(L->length == 1){
        L->first = NULL;
        L->last = NULL;
		L->current = NULL;
    }else{
        if(item == prv_list_get_first_item(L)){
            L->first = item->next;
        }
        if(item == prv_list_get_last_item(L)){
            L->last = item->prev;
        }
        item->prev->next = item->next;
        item->next->prev = item->prev;
    }

	port_free(item); // free the memory of item but not the entity
	item = NULL;
    L->length --;
}


// remove and return the first item of a list
void * prv_list_pop(prv_list_t * L)
{
    if(L->length == 0){
        return NULL;
    }

	void * entity = L->first->entity;
    prv_list_remove(L->first);

    return entity;
}



