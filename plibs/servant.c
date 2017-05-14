#include "servant.h"
#include "event.h"
#include "ef.h"
#include "PSPMconfigure.h"

port_semph_t sem[NUMOFSERVANTS];
ps_servant_t servants[NUMOFSERVANTS];


/* create servant and record the time,relation and function information */
ps_servant_t ps_servant_create(   prv_id_t sid,
									prv_stype_t type,
                                    prv_tick_t wcet,
                                    prv_num_t  num,   /* number of elements in src_array */
                                    ps_servant_t src_array[],
                                    void (*runnable)(void *))
{
    int i;
	char ch[10];
	ps_message_t message[num];
	servants[sid] = (ps_servant_t)port_malloc(sizeof(struct servant));
    ps_servant_t s = servants[sid];
	port_semph_create ( sem[sid] );
	prv_hashtable_t * ht = prv_hashtable_new();  // memory allocated in heap

	s->sid  = sid;
    s->type = type;
    s->wcet = wcet;
	s->inbox = ht;

    port_servant_create(runnable, &s->sid, 2);

    for(i = 0; i < num; ++ i){
		prv_ef_add_relation(src_array[i], s);   // construct topological graph
		message[i] = port_malloc(sizeof(struct message));   // init the inbox of each servant
		message[i]->data = 0;
		message[i]->source = src_array[i];
		message[i]->destination = s;
		itoa(src_array[i]->sid, ch);
		//if(-1 == prv_hashtable_add(ht, ch, message[i]))   // the inbox is a hashtable
		//{
		//	port_print("memory allocate failed\n");
		//}
    }

	return s;
}

void ps_servant_wait(prv_id_t sid)
{
	//ps_servant_t s = prv_ef_get_current_servant();
	//prv_id_t  sid = prv_servant_get_id(s);
	port_wait( sem[sid] );
}

void ps_servant_wake( ps_servant_t * s )
{
	prv_id_t  sid = prv_servant_get_id(s);
	port_trigger( sem[sid] );
}

void ps_message_send( ps_servant_t target, ps_message_t message)
{
	char ch[10];
	itoa(target->sid, ch);

	prv_hashtable_add(target->inbox, ch, message);
}

ps_message_t ps_message_receive(ps_servant_t src)
{
	char ch[10];
	itoa(src->sid, ch);
	ps_servant_t s = prv_ef_get_current_servant();
	ps_message_t message = (ps_message_t)prv_hashtable_get(s->inbox, ch);
	return message;
}


