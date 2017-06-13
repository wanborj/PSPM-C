#define USE_STDPERIPH_DRIVER
#include "app.h"
#include "PSPMconfigure.h"


int main()
{
	ps_servant_t ps_servants[8];
	ps_task_t ps_tasks[2];

	// the duration of input phase and output phase of a Unit
	// which is the max jitter tolerance of system
	ps_duration_set_input(200);
	ps_duration_set_output(200);

	/* parameters : servant_id , servant_type, LED, num of src, src_array[], runnable */
	ps_servants[0] = ps_servant_create(0, 0, 100, 0, NULL, sensor1);
	ps_servants[1] = ps_servant_create(1, 1, 1000, 1, ps_servants, controller11);
	ps_servants[2] = ps_servant_create(2, 1, 500,  1, ps_servants, controller12);
	ps_servants[3] = ps_servant_create(3, 2, 100, 2, ps_servants+1, actuator1);


	ps_servants[4] = ps_servant_create(4, 0, 100, 0, NULL, sensor2);
	ps_servants[5] = ps_servant_create(5, 1, 2000, 1, ps_servants+4, controller21);
	ps_servants[6] = ps_servant_create(6, 1, 1500, 1, ps_servants+5, controller22);
	ps_servants[7] = ps_servant_create(7, 2, 100, 1, ps_servants+6, actuator2);


	/* parameters: task_id, LET, servant_num, servant_array[]*/
	ps_tasks[0] = ps_task_create(0, 5000, 4, ps_servants);
	ps_tasks[1] = ps_task_create(1, 10000, 4, ps_servants+4);

	/* parameters: mode_id, task_num, task_array[]*/
	ps_mode_create(0, 2, ps_tasks);

	/* parameters: condition, mode_dest */
	ps_mode_switch_create( mode_switch1, 0);

	/* PSEFM kernel initialize and RTOS scheduler start */
	system_start();

}


