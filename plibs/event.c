#include "event.h"
#include "PSPMport.h"
#include "ef.h"
#include "list_internal.h"
#include "modeltime.h"



ps_message_t ps_message_create( int data )
{
	ps_message_t  message = port_malloc(sizeof(struct message));

	ps_servant_t s =  prv_ef_get_current_servant();
	message->source = s;
	message->data = data;

	return message;
}

