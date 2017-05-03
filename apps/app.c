#include "app.h"

void sensor1( void * para )
{
	while(1){
        // blocking for periodic timing event arriving
		ps_event_wait(para);

		// process pevent including tag and data
		port_print("sensor 1\t");

        // parameter :pointer of struct data
        ps_event_create(NULL);

        // release CPU and trigger R-Sevant to execute
		ps_servant_cooperate();
	}
}

void controller11(void * para)
{
	ps_data_t * data;

	while(1){
        // blocking for message-arrival event arriving
		ps_event_wait(para);
        // receive ready event from xEventReadyList
		data = ps_event_receive();

		// process pevent including tag and data
		port_print("controller 1-1\t");

        // parameter one is not NULL mean that sysmem will upate the existing event
        ps_event_create(data);
		ps_servant_cooperate();
	}
}

void controller12(void * para)
{
	ps_data_t * data;

	while(1){
        // blocking for message-arrival event arriving
		ps_event_wait(para);
        // receive ready event from xEventReadyList
		data = ps_event_receive();

		// process pevent including tag and data
		port_print("controller 1-2\t");

        // parameter one is not NULL mean that sysmem will upate the existing event
        ps_event_create(data);
		ps_servant_cooperate();
	}
}

void actuator1(void * para)
{
	ps_data_t * data;

	while(1){
		ps_event_wait(para);
		data = ps_event_receive();

		// process pevent including tag and data
		port_print("actuator 1\t");

        // output the event's data to physical device
		ps_servant_cooperate();
	}
}

void sensor2(void * para)
{
	while(1){
		ps_event_wait(para);

		// process pevent including tag and data
		port_print("sensor 2\t");

		ps_event_create(NULL);
		ps_servant_cooperate();
	}

}

void controller21(void * para)
{
    ps_data_t * data;

	while(1){
		ps_event_wait(para);
		data = ps_event_receive();

		// process pevent including tag and data
		port_print("controller 2-1\t");

		ps_event_create(data);
		ps_servant_cooperate();
	}

}

void controller22(void * para)
{
    ps_data_t * data;

	while(1){
		ps_event_wait(para);
		data = ps_event_receive();

		// process pevent including tag and data
		port_print("controller 2-2\t");

		ps_event_create(data);
		ps_servant_cooperate();
	}

}

void actuator2(void * para)
{
    ps_data_t * data;

	while(1){
		ps_event_wait(para);
		data = ps_event_receive();

		// process pevent including tag and data
		port_print("actuator 2\t");
        // output the event's data to physical device

		ps_servant_cooperate();
	}
}


bool mode_switch1()
{
	return 1;
}


