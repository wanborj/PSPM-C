#include "app.h"

void sensor1( void * para )
{
	prv_id_t id = *((prv_id_t *)para);

	while(1){
        // blocking for message-arrival event arriving
		ps_servant_wait(id);
		// process pevent including tag and data
		port_print("sensor 1\n\r");
	}
}

void controller11(void * para)
{
	prv_id_t id = *((prv_id_t *)para);

	while(1){
        // blocking for message-arrival event arriving
		ps_servant_wait(id);

		// process pevent including tag and data
		port_print("controller 1-1\n\r");
	}
}

void controller12(void * para)
{
	prv_id_t id = *((prv_id_t *)para);

	while(1){
        // blocking for message-arrival event arriving
		ps_servant_wait(id);

		// process pevent including tag and data
		port_print("controller 1-2\n\r");
	}
}

void actuator1(void * para)
{
	prv_id_t id = *((prv_id_t *)para);

	while(1){
        // blocking for message-arrival event arriving
		ps_servant_wait(id);
		// process pevent including tag and data
		port_print("actuator 1\n\r");

	}
}

void sensor2(void * para)
{
	prv_id_t id = *((prv_id_t *)para);

	while(1){
        // blocking for message-arrival event arriving
		ps_servant_wait(id);

		// process pevent including tag and data
		port_print("sensor 2\n\r");
	}

}

void controller21(void * para)
{
	prv_id_t id = *((prv_id_t *)para);

	while(1){
        // blocking for message-arrival event arriving
		ps_servant_wait(id);
		// process pevent including tag and data
		port_print("controller 2-1\n\r");
	}

}

void controller22(void * para)
{
	prv_id_t id = *((prv_id_t *)para);

	while(1){
        // blocking for message-arrival event arriving
		ps_servant_wait(id);

		// process pevent including tag and data
		port_print("controller 2-2\n\r");

	}

}

void actuator2(void * para)
{
	prv_id_t id = *((prv_id_t *)para);

	while(1){
        // blocking for message-arrival event arriving
		ps_servant_wait(id);

		// process pevent including tag and data
		port_print("actuator 2\n\r");

	}
}


bool mode_switch1()
{
	return 1;
}


