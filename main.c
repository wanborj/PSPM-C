#define USE_STDPERIPH_DRIVER
#include "app.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "stm32f10x.h"
#include "PSEFMstart.h"


int main()
{
	ps_servant_t * ps_servants[NUMOFSERVANTS];
	ps_task_t * ps_tasks[NUMOFTASKS];

	/* parameters : servant_id , servant_type, LED, num of src, src_array[], runnable */
	ps_servants[0] = ps_servant_create(0, 0, 10, 0, NULL, sensor1);
	ps_servants[1] = ps_servant_create(1, 1, 100, 1, ps_servants, controller11);
	ps_servants[2] = ps_servant_create(2, 1, 50,  1, ps_servants, controller12);
	ps_servants[3] = ps_servant_create(3, 2, 10, 2, ps_servants+1, actuator1);


	ps_servants[4] = ps_servant_create(4, 0, 10, 0, NULL, sensor2);
	ps_servants[5] = ps_servant_create(5, 1, 200, 1, ps_servants+4, controller21);
	ps_servants[6] = ps_servant_create(6, 1, 150, 1, ps_servants+5, controller22);
	ps_servants[7] = ps_servant_create(7, 2, 10, 1, ps_servants+6, actuator2);


	/* parameters: task_id, LET, servant_array[], servant_num */
	ps_tasks[0] = ps_task_create(0, 500, ps_servants, 4);
	ps_tasks[1] = ps_task_create(1, 1000,  ps_servants+4, 4);

	/* parameters: mode_id, task_array[], task_num */
	ps_mode_create(0, ps_tasks, 2);

	/* parameters: condition, mode_dest */
	ps_mode_switch_create( mode_switch1, 0);

	/* PSEFM kernel initialize and RTOS scheduler start */
	system_start();

}


