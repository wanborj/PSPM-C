#ifndef __LIST_INTERNAL_H
#define __LIST_INTERNAL_H

/*
 * list type
 * */

typedef struct item{
    void		* entity;   // the object item, event or servant
    void		* owner;   // the list it belongs to
    struct item *next;
    struct item *prev;
} prv_item_t;

typedef struct list{
    int				length;
    prv_item_t		*first;
	prv_item_t		*last;
	prv_item_t		*current;
} prv_list_t;



#define prv_list_get_length(L)			(((prv_list_t *)L)->length)
#define prv_list_get_first_item(L)		(((prv_list_t *)L)->first)
#define prv_list_get_last_item(L)		(((prv_list_t *)L)->last)
#define prv_list_get_current_item(L)	(((prv_list_t *)L)->current)


#define prv_item_get_entity(item)		(((prv_item_t *)item)->entity)
#define prv_item_get_owner(item)		(((prv_item_t *)item)->owner)
#define prv_item_get_next(item)			(((prv_item_t *)item)->next)
#define prv_item_get_prev(item)			(((prv_item_t *)item)->prev)

void prv_list_set_current_item(prv_list_t *);
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
void prv_list_initialize(prv_list_t * L);
void prv_item_initialize(void * entity, prv_item_t * item);

void * prv_list_get_current_entity(prv_list_t * L);
/******************************************************
Function: insert( insert a item into the tail of a list ); insert_sorted( insert a item into specific position with its tag )
 * *****************************************************/
void prv_list_insert( void * entity, prv_list_t * L);

void prv_list_insert_sorted(void * entity, prv_list_t * L, int (*compare)(void *, void *));


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
void prv_list_remove(prv_item_t * item);

void * prv_list_pop(prv_list_t * L);


#endif
