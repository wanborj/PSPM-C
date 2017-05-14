#ifndef __MODEL_TIME_H
#define __MODEL_TIME_H

#include "PSPMconfigure.h"
#include "mode.h"

void prv_modeltime_init(ps_mode_t m);


prv_tick_t prv_modeltime_unit_start();
prv_tick_t prv_modeltime_compute_start();
prv_tick_t prv_modeltime_output_start();
prv_tick_t prv_modeltime_unit_end();

// simulation the temporal behaviors of servants
prv_tick_t prv_modeltime_run(ps_servant_t s);
// only be invoked by prv_STT_create
void prv_modeltime_set_new_unit_start();

bool prv_modeltime_is_mode_end();
bool prv_modeltime_is_period_start(ps_task_t t);

#endif
