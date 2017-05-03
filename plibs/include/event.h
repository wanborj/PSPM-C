#ifndef __EVENT_H
#define __EVENT_H

#include "PSEFMconfigure.h"
#include "PSEFMport.h"

/******************************************************
Function: Getter functions
description: kernel function, get the values of properties of events
structure:
typedef struct {
    ps_servant_t * pservant_src;
    ps_servant_t * pservant_dest;
    ps_tag_t  tag;
    ps_data_t data;
    item_t eventItem;
}ps_event_t;

typedef struct {
    tick_t deadline;  // RM. The smaller the period the task is , the higher priority the event is. Not used in version V1.*
    tick_t timestamp; // the time to be proecessed
    int microstep;    // the topology order
    int level;        // the depth of current servant in a task
}ps_tag_t;

typedef struct {
    int num;
    // data type can be changed here. Data type including portCHAR, portFLOAT, portLONG, portSHORT, portBASE_TYPE
    double data[NUMOFINS];
} ps_data_t;
 * *****************************************************/
#define prv_event_get_timestamp(pevent) (( (ps_event_t *)pevent)->tag.timestamp)
#define prv_event_get_microstep(pevent) (( (ps_event_t *)pevent)->tag.microstep)
#define prv_event_get_level(pevent)     (( (ps_event_t *)pevent)->tag.level)
#define prv_event_get_deadline(pevent)  (( (ps_event_t *)pevent)->tag.deadline)
#define prv_event_get_data(pevent)      (( (ps_event_t *)pevent)->data.data)
#define prv_event_get_data_num(pevent)  (( (ps_event_t *)pevent)->data.num)
#define prv_event_get_src(pevent)       (( (ps_event_t *)pevent)->pservant_src)
#define prv_event_get_dest(pevent)      (( (ps_event_t *)pevent)->pservant_dest)

/******************************************************
Function: prv_event_send
Description: Kernel function, send event to xEventGlobalList
Calls: prv_list_insert
Called by: prv_event_create
Input: pevent(pointer of a event)
Output:
Return: void
Others:
 * *****************************************************/
void prv_event_send(ps_event_t * pevent);

/******************************************************
Function: prv_event_del
Description: Kernel function, send event to xEventIdleList
Calls: prv_list_insert
Called by: ps_event_receive
Input: pevent(pointer of a event)
Output:
Return: void
Others:
 * *****************************************************/
void prv_event_delete(ps_event_t * pevent);

/******************************************************
Function: prv_event_list_initialize
Description: Kernel function, initialize all the structures about events including xEvent****List and events array
Calls: prv_list_initialize(); prv_event_initialize()
Called by: system_start()
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_event_list_initialize();


// invoked in prv_event_list_initialize()
/******************************************************
Function: prv_event_initialize
Description: kernel function, initialize the event entities and the semaphore entities
Calls: prv_item_initialize() ;  prv_event_sem_initialize()
Called by: prv_event_list_initialize()
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_event_initialize();

/******************************************************
Function: prv_event_tag_set
Description: Kernel function, set the tag of events
Calls: prv_servant_get_type()
Called by: ps_event_create()
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_event_tag_set(ps_event_t * pevent, int microstep);


/******************************************************
Function: prv_event_is_overlap
Description: Kernel function, judge wether the destinate servant of event violate the LET semantics
Calls: prv_model_time_output_start()
Called by: prv_event_tag_update()
Input: pevent( pointer of a event )
Output:
Return: 1 for overlap; 0 for not overlap
Others:
 * *****************************************************/
int  prv_event_is_overlap(ps_event_t * pevent);


/******************************************************
Function: prv_event_tag_update
Description: kernel function, update the timestamp of event
Calls: prv_event_is_overlap()
Called by: prv_event_can_process()
Input: pevent( the pointer of a event )
Output:
Return: 1 for can be proceed; 0 for can not be proceed
Others:
 * *****************************************************/
// update the timestamp of event
int prv_event_tag_update(ps_event_t * pevent);




/******************************************************
Function:
Description:
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
//  judge wether event can be processed
int prv_event_can_process(ps_event_t * pevent);



/******************************************************
Function: ps_event_wait
Description: Programming API, blocking to wait for a event
Calls: port_wait()
Called by: runnable function in servants
Input: para( the id of triggered servant )
Output:
Return:
Others:
 * *****************************************************/
/* API */
// waiting for periodic timing event or message-arrival event
void ps_event_wait( void * para );

/******************************************************
Function: ps_event_receive
Description: Programming API, receive events for current servant from xEventReadyList and integrate their data into one event
Calls:
Called by: runnable function in servants
Input:
Output:
Return:
Others:
 * *****************************************************/
// receive event from ready event list and return the data set of these events
ps_data_t * ps_event_receive();



/******************************************************
Function: ps_event_create
Description: Programming API, create a event entity with the data and send it into xEventGlobalList
Calls: prv_event_send
Called by: runnable function in servants
Input: pdata( pointer of data )
Output:
Return:
Others:
 * *****************************************************/
// event create for several destinate servants
// initialize the flag in event
void ps_event_create(ps_data_t *pdata);



#endif
