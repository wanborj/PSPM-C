#include "model_time.h"

tick_t xModelTimeStart ;
tick_t xFutureModelTime;
extern struct ps_mode_array_t mod;


static tick_t GCD(tick_t a, tick_t b){
	return b==0?a:GCD(b,a%b);
}

static tick_t  prv_model_time_unit_length(ps_mode_t * pmode)
{
	int i;
	tick_t unit_len = pmode->tasks[0]->period;
	for( i = 1; i < pmode->num; ++i){
		unit_len = GCD(unit_len, pmode->tasks[i]->period);
	}
	return unit_len;
}

static tick_t prv_model_time_period_length(ps_mode_t * pmode)
{
	int i;
	tick_t period_len = pmode->tasks[0]->period;
	tick_t gcd;
	for( i = 1; i < pmode->num; ++i){
		gcd = GCD(period_len, pmode->tasks[i]->period);
		period_len = (period_len * pmode->tasks[i]->period)/gcd;
	}
	return period_len;
}

void prv_model_time_unit_initialize()
{
	int i;
	for( i = 0; i < mod.num; ++i){
		prv_mode_set_mode_unit(i,prv_model_time_unit_length(mod.pmode[i]));
	}

}

void prv_model_time_period_initialize()
{
	int i;
	for( i = 0; i < mod.num; ++i){
		prv_mode_set_mode_period(i,prv_model_time_period_length(mod.pmode[i]));
	}
}

void prv_model_time_initialize()
{
	prv_model_time_unit_initialize();
	prv_model_time_period_initialize();
    prv_model_time_reset();
    prv_model_time_future_reset();  /* when enter new mode period, set the xFutureModelTime as the Input end. */
}

tick_t prv_model_time_input_length()
{
	return INPUT;
}

tick_t prv_model_time_output_length()
{
	return OUTPUT;
}


tick_t prv_model_time_unit_start()
{
	ps_mode_t * pmode = prv_mode_get_current_mode();
	id_t mode_id = pmode->mode_id;
	tick_t current_model_time = port_get_current_time();

    return current_model_time - (current_model_time - xModelTimeStart)%(prv_mode_get_mode_unit(mode_id));  // return absolute time
}

bool prv_model_time_is_unit_start()
{
	ps_mode_t * pmode = prv_mode_get_current_mode();
	id_t mode_id = pmode->mode_id;
	tick_t current_model_time = port_get_current_time();

    if( (current_model_time - xModelTimeStart)%(prv_mode_get_mode_unit(mode_id)) == 0  ){
        return 1;
    }else{
        return 0;
    }
}

bool prv_model_time_is_period_start(ps_task_t * ptask)
{
    if( (port_get_current_time() - xModelTimeStart) % ptask->period == 0){
        return 1;
    }else{
        return 0;
    }
}

void prv_model_time_reset()
{
	xModelTimeStart = port_get_current_time();
}


void prv_model_time_future_reset()
{
	xFutureModelTime = prv_model_time_input_end();
}

tick_t prv_model_time_get_model_time()
{
	return xModelTimeStart;
}

tick_t prv_model_time_get_future_model_time()
{
    return xFutureModelTime;
}

tick_t prv_model_time_input_end()
{
	return prv_model_time_unit_start()+INPUT;
}

tick_t prv_model_time_output_end()
{
	ps_mode_t *pmode = prv_mode_get_current_mode();
	id_t mode_id = pmode->mode_id;
    return prv_model_time_unit_start()+prv_mode_get_mode_unit(mode_id);
}

tick_t prv_model_time_output_start()
{
	return prv_model_time_output_end()-OUTPUT;
}

bool prv_model_time_is_mode_end()
{
	ps_mode_t *pmode = prv_mode_get_current_mode();
	id_t mode_id = pmode->mode_id;
	tick_t current_time = port_get_current_time();

	if( current_time > xModelTimeStart && 0 == ( current_time - xModelTimeStart ) % prv_mode_get_mode_period(mode_id)){
		return 1;
	}else{
		return 0;
	}
}



