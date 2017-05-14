#include "modeltime.h"

static prv_tick_t modestart;
static prv_tick_t modeltime;
static ps_mode_t current_mode;
extern ps_task_t tasks[NUMOFTASKS];

void prv_modeltime_init(ps_mode_t m)
{
	current_mode = m; // set the m as the current mode
	modestart = 0;    // simulation, each mode start at 0
	modeltime = 0;
}

// duration of input phase
static prv_tick_t prv_modeltime_di()
{
	return INPUT;
}

// duration of output phase
static prv_tick_t prv_modeltime_do()
{
	return OUTPUT;
}

prv_tick_t prv_modeltime_unit_start()
{
	// I'am a genius, to implement the duration like (x,y]
	// for example, unit = 500, then the time point 500 is the end of the first unit not the start of the second unit
	return modeltime - (modeltime - modestart - 1)%(prv_mode_get_unit(current_mode)) - 1;
}

prv_tick_t prv_modeltime_compute_start()
{
	return prv_modeltime_unit_start() +  prv_modeltime_di();
}

prv_tick_t prv_modeltime_output_start()
{
	return prv_modeltime_unit_start() + prv_mode_get_unit(current_mode) - prv_modeltime_do();
}

prv_tick_t prv_modeltime_unit_end()
{
	return prv_modeltime_output_start() + prv_modeltime_do();
}

// simulate the execution of a servant and return its release time
prv_tick_t prv_modeltime_run(ps_servant_t s )
{
	prv_tick_t release;
	if(s->type == 0){
		// I-Servants are executing with synchrony hypothesis
		release = modeltime; // modeltime is set to unit start time point each time enter a new unit
		modeltime += s->wcet;
	}else if(s->type == 1){
		if(modeltime < prv_modeltime_compute_start()){
			modeltime = prv_modeltime_compute_start();
			release = modeltime;
		}
		if(modeltime + s->wcet <= prv_modeltime_output_start()){
			release = modeltime;
			modeltime += s->wcet; // simulate the execution
		}else{
			return -1; // no enough time for this servants, the system enters idle state
		}
	}else{
		if(modeltime < prv_modeltime_output_start()){
			modeltime = prv_modeltime_output_start();
		}
		if(0 == (prv_modeltime_output_start() + OUTPUT) % tasks[s->tid]->period){
			release = modeltime;
			modeltime += s->wcet;
		}else{
			release = -1;
		}
	}
	return release;
}

// start a new unit, must be called in function improvedTSA
// when all O-Servant are added into STT
void prv_modeltime_set_new_unit_start()
{
	modeltime = prv_modeltime_unit_end();
}

bool prv_modeltime_is_mode_end()
{
	if(modeltime == modestart + prv_mode_get_period(current_mode)){
		return 1;
	}else{
		return 0;
	}
}

bool prv_modeltime_is_period_start(ps_task_t t)
{
    if( (modeltime - modestart) % t->period == 0){
        return 1;
    }else{
        return 0;
    }
}

