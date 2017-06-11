#ifndef __PSEFMCONFIGURE_H
#define __PSEFMCONFIGURE_H

#include "list_internal.h"
#include "hashtable.h"

#define NUMOFMODES 2  // the maximal number of modes in system, the id of mode start from 0
#define NUMOFCONDS 4 // the maximal number of mode switch condition in system
#define NUMOFTASKS 4 // the maximal number of tasks in one mode
#define NUMOFSERVANTS NUMOFMODES*NUMOFTASKS*4 // the maximal number of servants in one task
#define NUMOFINS 4   // max in degree of every servants
#define NUMOFRELATIONS NUMOFINS*NUMOFSERVANTS


typedef int		prv_id_t;
typedef int		prv_stype_t;
typedef long	prv_tick_t;
typedef int		prv_num_t;
typedef int		bool;

// set the INPUT and OUTPUT
// in the main function of PSPM program
void ps_duration_set_input(prv_tick_t t);
void ps_duration_set_output(prv_tick_t t);
prv_tick_t prv_duration_get_input();
prv_tick_t prv_duration_get_output();



struct servant;
struct message;
struct ptask;
struct mode;
struct condition;
struct condition_array;
struct mode_array;

typedef struct message *  ps_message_t;
typedef struct servant * ps_servant_t;
typedef struct ptask * ps_task_t;
typedef struct mode * ps_mode_t;
typedef struct condition * ps_mode_condition_t;
typedef struct condition_array * ps_condition_array_t;
typedef struct mode_array * ps_mode_array_t;

struct servant{
	prv_id_t		sid;
	prv_stype_t		type; // 0 for sensor; 1 for controller; 2 for actuator
    prv_tick_t		wcet;
	prv_id_t		tid;
	prv_hashtable_t *inbox;
};

struct message{
	ps_servant_t	source;
	ps_servant_t	destination;
	// there is only one type of data in a application for simplifying system design.
//	union{
//		char			c_data;
//		long long int	i_data;
//		double			d_data;
//	}
	int data;
};

struct ptask{
	prv_id_t		tid;
    prv_tick_t		let;
    prv_tick_t		period;
    prv_tick_t		deadline;
	prv_list_t		*servants;
};

struct mode{
    prv_id_t	mid; // equal to the serial number of array
    prv_tick_t	period;
	prv_tick_t	unit;
	prv_list_t  *tasks;
};

struct condition{
    prv_id_t	mid;
    bool		(*condition)(void);
};

struct condition_array{
    prv_num_t num;
    ps_mode_condition_t conditions[NUMOFCONDS];
};

struct mode_array{
    prv_num_t num;
    ps_mode_t modes[NUMOFMODES];
};


void prv_time_trigger();

void myHardwareInit();

void system_start();


void myTraceCreate();

void myTraceSwitchedIn();

void myTraceSwitchedOut();


void vApplicationTickHook( void );

void vApplicationIdleHook(void );

//#define offsetof(s, m)   (char *)&(((s *)0)->m)
/**
 *get struct pointer from member pointer
 */
#define m2s(m, s) ((s*)((char *)m - offsetof(s,m)))

int gcd(int a, int b);

int lcm(int a, int b);

// a simple itoa implementation

void itoa(int n , char *s);


#endif
