#include "mode.h"

//ps_mode_t modes[NUMOFMODES];
//ps_mode_condition_t conditions[NUMOFCONDS];

struct mode_array marray;
struct condition_array carray;

static prv_tick_t modestart; // record the real time of mode start
static ps_mode_t current_mode;

ps_mode_t prv_mode_get_current_mode()
{
    return current_mode;
}

prv_tick_t prv_mode_get_modestart()
{
	return modestart;
}

void prv_mode_set_modestart(prv_tick_t t)
{
	// t is the period of current mode
	modestart = t;
}

int prv_mode_is_period_end()
{
	prv_tick_t realtime = port_get_current_time();
	if(realtime == prv_mode_get_period(current_mode) + modestart){
		return 1;
	}else{
		return 0;
	}
}

void ps_mode_create(prv_id_t mid, prv_num_t num, ps_task_t task_array[])
{
    int i;
	int i_gcd = 0 , i_lcm = 1;
	marray.modes[mid] = (ps_mode_t)port_malloc(sizeof(struct mode));
	marray.modes[mid]->tasks = (prv_list_t *)port_malloc(sizeof(struct list));

    marray.modes[mid]->mid = mid;
    for(i=0;i<num;++i){
		prv_list_insert(task_array[i], marray.modes[mid]->tasks);
		i_gcd = gcd(task_array[i]->period, i_gcd);
		i_lcm = lcm(task_array[i]->period, i_lcm);

    }
	marray.modes[mid]->period	= i_lcm;
	marray.modes[mid]->unit	= i_gcd;

	marray.num ++;
}


void ps_mode_switch_create(bool (*condition)(void), prv_id_t target)
{
    int num = carray.num;
	carray.conditions[num] = (ps_mode_condition_t)port_malloc(sizeof(struct condition));
    carray.conditions[num]->mid= target;
    carray.conditions[num]->condition = condition;
    carray.num ++;
}

// being invoked at the end of mode period
void prv_mode_switch()
{
	int i, tmp;
	for( i = 0; i < carray.num; ++i ){
		if( (tmp = carray.conditions[i]->condition()) == 1){
			current_mode = marray.modes[carray.conditions[i]->mid];
			return;
		}
	}
	// no mode switch happens
	current_mode = marray.modes[0];

}

