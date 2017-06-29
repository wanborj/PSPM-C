#include "ef.h"

typedef struct relation{
    prv_list_t in;
	prv_list_t out;
} prv_relation_t;  // not a pointer

prv_relation_t relations[NUMOFSERVANTS];

struct node{
	ps_servant_t servant;
	prv_tick_t release;
};
typedef struct node * schedule_node;

prv_list_t	STT[NUMOFMODES];

static ps_servant_t current_servant;
static int servant_flags[NUMOFSERVANTS];
static int indegrees[NUMOFSERVANTS];
static int edges[NUMOFSERVANTS][NUMOFSERVANTS];

extern ps_task_t	tasks[NUMOFTASKS];
extern ps_servant_t servants[NUMOFSERVANTS];
extern port_semph_t sem[NUMOFSERVANTS];
extern struct mode_array marray;

prv_list_t * prv_ef_get_dest_list(ps_servant_t s)
{
	return &relations[s->sid].out;
}

prv_list_t  * prv_ef_get_src_list(ps_servant_t s)
{
	return &relations[s->sid].in;
}

void prv_ef_add_relation(ps_servant_t src, ps_servant_t dest)
{
	prv_list_insert(dest, prv_ef_get_dest_list(src));
	prv_list_insert(src,  prv_ef_get_src_list(dest));
}

void prv_ef_set_current_servant(ps_servant_t s)
{
    current_servant = s;
}

ps_servant_t prv_ef_get_current_servant()
{
    return current_servant;
}

// reset the map at the beginning of each mode period
static void prv_parameter_init()
{
	int  i, j;
	for( i = 0; i < NUMOFSERVANTS; ++i ){
		servant_flags[i] = -1; //1 means in current scheduling list; -1 means not
		indegrees[i] = 0;
		for( j = 0; j < NUMOFSERVANTS; ++j ){
			edges[i][j] = 0; // 1 means exist edges; 0 means not
		}
	}
}

// add edges of the graph by the relation of a servant
static void prv_ef_edge_add(ps_servant_t s)
{
	int i;
	ps_servant_t current;
	prv_id_t sid = s->sid;
	// active the servants
	servant_flags[sid] = 1;

	for( i=0; i<relations[sid].in.length; ++i ){
		// only when two servants of the edge are in scheduling list, the edge is added
		current = (ps_servant_t)prv_list_get_current_entity(prv_ef_get_src_list(s));
		if(servant_flags[current->sid] == 1){
			edges[current->sid][sid] = 1;
		}
	}
	for( i=0; i<relations[sid].out.length; ++i ){
		current = (ps_servant_t)prv_list_get_current_entity(prv_ef_get_dest_list(s));
		if(servant_flags[current->sid] == 1){
			edges[sid][current->sid] = 1;
		}
	}
}

// add edges in the graph of a task
static void prv_ef_topograph_update_one(ps_task_t t)
{
	int i;
	ps_servant_t current;
	for(i = 0; i < t->servants->length; ++i){
		current = (ps_servant_t)prv_list_get_current_entity(t->servants);
		prv_ef_edge_add(current);
	}
}

// being invoked once per unit
static void prv_ef_topograph_update(ps_mode_t m)
{
	int i, j;
	ps_task_t current;
	for(i = 0; i < m->tasks->length; ++i){
		current = (ps_task_t)prv_list_get_current_entity(m->tasks);
		if(prv_modeltime_is_period_start(current) == 1){
			prv_ef_topograph_update_one(current);
		}
	}

	// count the indegrees for each servant at the beginning of each unit
	for( i = 0; i < NUMOFSERVANTS; ++i ){
		indegrees[i] = 0;
	}

	for(i = 0; i < NUMOFSERVANTS; ++i){
		for(j = 0; j < NUMOFSERVANTS; ++j){
			if(edges[i][j]){
				indegrees[j]++;
			}
		}
	}
}


static int compare(void * s1, void *s2)
{
	ps_servant_t a = s1;
	ps_servant_t b = s2;
	ps_task_t	t1 = tasks[a->tid];
	ps_task_t	t2 = tasks[b->tid];
	if(a->type < b->type){
		return 1;
	}else if(a->type == b->type && t1->let < t2->let){
		return 1;
	}else if(a->type == b->type && t1->let == t2->let && t1->tid < t2->tid){
		return 1;
	}else{
		return 0;
	}
}

//being invoked once per mode
static void prv_ef_set_init(prv_list_t *S)
{

	S->length = 0;
	S->first = S->last = S->current = NULL;
}

//being invoked once per unit
static void prv_ef_set_unit_update(prv_list_t *S)
{
	int i;
	for(i = 0; i < NUMOFSERVANTS; ++i){
		// choose the servants with zero indegree
		if(!indegrees[i] && servant_flags[i] == 1){
			servant_flags[i] = -1; // deactive  the servant
			prv_list_insert_sorted(servants[i], S, compare);
		}
	}
}

// being invoked for each sorted servant
// update the set S that contains nodes without edges
static void prv_ef_set_servant_update(ps_servant_t servant, prv_list_t * S)
{
	int i;

	prv_id_t sid = servant->sid;

	for(i = 0; i < NUMOFSERVANTS; ++i){
		if(edges[sid][i]){
			indegrees[i] --;
			edges[sid][i] = 0;
			if(!indegrees[i] && servant_flags[i] == 1){
				servant_flags[i] = -1; // deactive  the servant
				prv_list_insert_sorted(servants[i], S, compare);
			}
		}
	}
}

static ps_servant_t prv_ef_find_servant(prv_stype_t type, prv_list_t * S)
{
	int i, len;
	len = S->length;
	ps_servant_t result;
	prv_item_t * index;

	index = prv_list_get_first_item(S);
	for(i = 0; i < len; ++i){
		result = (ps_servant_t)index->entity;
		if(result->type == type){
			index = index->next;
			prv_list_remove(index->prev);
			return result;
		}else{
			index = index->next;
		}
	}
	return NULL;
}

/*
 * function: prv_STT_create, implement the improvedTSA algorithm
 * call:prv_modeltime_is_mode_end()
 *		prv_ef_topograph_update(m);
 *		prv_ef_set_init()
 *		prv_ef_find_servant();
 *		prv_modeltime_run
 *		prv_ef_set_update
 * */

static void prv_STT_create(ps_mode_t m)
{
	prv_id_t mid = m->mid;
	prv_list_t * table = &STT[mid];
	prv_list_t S; // the set of nodes without in edges
	ps_servant_t tmp_servant;
	prv_tick_t release;
	schedule_node elem;

	prv_ef_set_init(&S);
	do{
		prv_ef_topograph_update(m);
		prv_ef_set_unit_update(&S);
		while((tmp_servant = prv_ef_find_servant(0, &S)) != NULL){
			// construct a schedule node in STT
			if( (release = prv_modeltime_run(tmp_servant)) >= 0 ){
				elem = (schedule_node)port_malloc(sizeof(struct node));
				elem->servant = tmp_servant;
				elem->release = release;
			}else{
				break;
			}
			prv_list_insert(elem, table);
			prv_ef_set_servant_update(tmp_servant, &S);
		}
		// function prv_ef_find_servant() find the servant and remove it from the original list
		while((tmp_servant = prv_ef_find_servant(1, &S)) != NULL){
			if( (release = prv_modeltime_run(tmp_servant)) >= 0 ){
				elem = (schedule_node)port_malloc(sizeof(struct node));
				elem->servant = tmp_servant;
				elem->release = release;
			}else{
				// set the flag into true
				// then the prv_ef_set_unit_update() will insert the servant into S again
				servant_flags[tmp_servant->sid] = 1;
				break;
			}
			prv_list_insert(elem, table);
			prv_ef_set_servant_update(tmp_servant, &S);
		}
		while((tmp_servant = prv_ef_find_servant(2, &S)) != NULL){
			if( (release = prv_modeltime_run(tmp_servant)) >= 0 ){
				elem = (schedule_node)port_malloc(sizeof(struct node));
				elem->servant = tmp_servant;
				elem->release = release;
				prv_list_insert(elem, table);
				prv_ef_set_servant_update(tmp_servant, &S);
			}else{
				// set the flag into true
				// then the prv_ef_set_unit_update() will insert the servant into S again
				servant_flags[tmp_servant->sid] = 1;
			}
		}

		// push forward the model time for simulation
		prv_modeltime_set_new_unit_start();

	}while( !prv_modeltime_is_mode_end() );
}

 void prv_ef_create()
{
	int count = marray.num;
	ps_mode_t m;
	while(count--){
		m = marray.modes[count];
		prv_modeltime_init(m);
		prv_parameter_init();
		prv_STT_create(m);
	}
}

// fixed time slot
#if (configUSE_OPTIMIZATION==0)

int prv_ef_is_time_to_trigger()
{
	ps_mode_t current_mode = prv_mode_get_current_mode();
	prv_item_t * current_item = prv_list_get_current_item(&STT[current_mode->mid]);
	schedule_node n = (schedule_node)current_item->entity;
	prv_tick_t current_time = port_get_current_time();

	if(current_time >= n->release + prv_mode_get_modestart()){
		// set the n->servant as current Servant
		prv_ef_set_current_servant(n->servant);

		// point to the next iterator of the STT
		prv_list_set_current_item(&STT[current_mode->mid]);
		if(current_item == STT[current_mode->mid].last){
			prv_mode_set_modestart(prv_mode_get_modestart() + current_mode->period);
		}
		return 1;
	}else{
		return 0;
	}
}

#else
// advance execution of C-Servants

int prv_ef_is_time_to_trigger()
{
	ps_mode_t current_mode = prv_mode_get_current_mode();
	prv_item_t * current_item = prv_list_get_current_item(&STT[current_mode->mid]);
	schedule_node n = (schedule_node)current_item->entity;
	prv_tick_t current_time = port_get_current_time();

	// cope with I-Servant and O-Servant
	if(prv_servant_get_type(n->servant) != 1){
		if(current_time >= n->release + prv_mode_get_modestart()){
			// set the n->servant as current Servant
			prv_ef_set_current_servant(n->servant);

			// point to the next iterator of the STT
			prv_list_set_current_item(&STT[current_mode->mid]);
			if(current_item == STT[current_mode->mid].last){
				prv_mode_set_modestart(prv_mode_get_modestart() + current_mode->period);
			}
			return 1;
		}else{
			return 0;
		}
	}else{
		// cope with C-Servant: allow next C-Servant to be triggered advance
		// one the previous finish its execution
		if(current_time >= prv_mode_get_modestart() + prv_duration_get_input()){
			prv_ef_set_current_servant(n->servant);
			prv_list_set_current_item(&STT[current_mode->mid]);
			return 1;
		}else{
			return 0;
		}
	}
}

#endif

void prv_ef_triggering()
{
	port_trigger(sem[current_servant->sid]);
}


