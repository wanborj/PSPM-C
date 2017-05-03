#ifndef __EXEC_FLOW_H
#define __EXEC_FLOW_H

#include "PSEFMconfigure.h"
#include "PSEFMport.h"
#include "event.h"
#include "mode.h"
#include "ptask.h"
#include "servant.h"
#include "list_internal.h"



/******************************************************
Function: get_dest_num; get_ith_dest
Description: Kernel function, get the number of or ith destinate servants of a servant

typedef struct relation{
    int num;
    ps_servant_t * pservants[NUMOFINS];
} ps_relation_t;

ps_relation_t srcs[NUMOFSERVANTS];
ps_relation_t dests[NUMOFSERVANTS];
 * *****************************************************/
int            prv_ef_get_dest_num(ps_servant_t * pservant);
ps_servant_t * prv_ef_get_ith_dest(ps_servant_t * pservant, int i);


/******************************************************
Function: get_current_servant; set_current_servant
Description: Kernel function, get and set the current servant
Calls:
Called by: ps_event_wait()
Input:
Output:
Return:
Others:
 * *****************************************************/
ps_servant_t * prv_ef_get_current_servant();
void           prv_ef_set_current_servant(ps_servant_t * pservant);



/******************************************************
Function: prv_ef_add_relation
Description: Kernel function, build the communication relationship among servants
Calls:
Called by: ps_servant_create
Input: pservant_src( the pointer of source servant ); pservant_dest( the pointer of destination servant )
Output:
Return:
Others:
 * *****************************************************/
// build the communication relation, invoked in ps_servant_create()
void prv_ef_add_relation(ps_servant_t * pservant_src, ps_servant_t * pservant_dest);


/******************************************************
Function: prv_ef_sorting
Description: kernel function, sorting the events in xeventgloballist and send them into xeventlocallist;
increasing the "arrive" properties of servant by the way.
Calls:
Called by: ps_servant_create
Input:
Output:
Return:
Others:
 * *****************************************************/
// transit events from local list to ready list for triggering next servant to run
void prv_ef_sorting();

/******************************************************
Function: prv_ef_triggering
Description: kernel function, if the events for the same destination servant can be proceed( the time-stamp will be updated in this process ),
then these events are transited to xEventReadyList. The "arrive" property is cleaned at the same time.
Increasing the "arrive" properties of servant by the way.
calls:
Called by: ps_servant_create
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_ef_triggering();

/******************************************************
Function: prv_ef_create
Description: kernel function,  implementing the execution flow in PSEFM as a R-Servant
calls: port_servant_create()
Called by: system_start()
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_ef_create();

#endif
