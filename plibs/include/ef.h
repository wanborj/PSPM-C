#ifndef __EXEC_FLOW_H
#define __EXEC_FLOW_H

#include "PSPMconfigure.h"
#include "PSPMport.h"
#include "event.h"
#include "mode.h"
#include "ptask.h"
#include "servant.h"
#include "list_internal.h"

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



void prv_ef_trigger_first();

/******************************************************
Function: prv_ef_pick_next
Description: kernel function, pick the next servant in the scheduling table
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_ef_pick_next();

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

#endif
