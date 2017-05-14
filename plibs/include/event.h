#ifndef __EVENT_H
#define __EVENT_H

#include "PSPMconfigure.h"
#include "PSPMport.h"

#define prv_message_get_data(e)      (( (ps_message_t *)e)->data)
#define prv_message_get_src(e)       (( (ps_message_t *)e)->source)
#define prv_message_get_dest(e)      (( (ps_message_t *)e)->destination)


ps_message_t ps_message_create(int data);




#endif
