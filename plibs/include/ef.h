#ifndef __EXEC_FLOW_H
#define __EXEC_FLOW_H

#include "PSPMconfigure.h"
#include "PSPMport.h"
#include "event.h"
#include "mode.h"
#include "ptask.h"
#include "servant.h"
#include "list_internal.h"


prv_list_t * prv_ef_get_dest_list(ps_servant_t s);

prv_list_t * prv_ef_get_src_list(ps_servant_t s);
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
void prv_ef_add_relation(ps_servant_t src, ps_servant_t dest);


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
ps_servant_t prv_ef_get_current_servant();
void         prv_ef_set_current_servant(ps_servant_t s);


/******************************************************
unction: prv_ef_triggering
Description: kernel function, trigger the servant
calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_ef_triggering();

/******************************************************
Function: prv_ef_create
Description: kernel function, create the scheduling table for all the mode
calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_ef_create();


int prv_ef_is_time_to_trigger();
int prv_ef_coroutine_trigger();

#endif
