#ifndef __PSEFMCONFIGURE_H
#define __PSEFMCONFIGURE_H

#include <stdio.h>

typedef int id_t;
typedef long tick_t;
typedef int bool;


#define NUMOFMODES 2  // the maximal number of modes in system, the id of mode start from 0
#define NUMOFCONDS 4 // the maximal number of mode switch condition in system
#define NUMOFTASKS 4 // the maximal number of tasks in one mode
#define NUMOFSERVANTS NUMOFMODES*NUMOFTASKS*4 // the maximal number of servants in one task
#define NUMOFINS 4   // max in degree of every servants
#define NUMOFRELATIONS NUMOFINS*NUMOFSERVANTS

#define INPUT 20
#define OUTPUT 20
/*
 * list type
 * */
typedef struct item{
    void * item;   // the object item, event or servant
    void * owner;   // the list it belongs to
    struct item *next;
    struct item *prev;
} item_t;

typedef struct list{
    int     length;
    tick_t  earliest_time; // the earliest time-stamp of event in a event list
    item_t *first;
    item_t *last;
} list_t;

/*
 * event structure
 * */
typedef struct {
    tick_t deadline;     /*< RM. The smaller the period the task is , the higher priority the event is. >*/
    tick_t timestamp;             /*< the time to be proecessed >*/
    int microstep;            /*< the topology order >*/
    int level;                /*< the depth of current servant in a task >*/
}ps_tag_t;

typedef struct {
    int num;
    /* data type can be changed here. Data type including portCHAR, portFLOAT, portLONG, portSHORT, portBASE_TYPE*/
    double data[NUMOFINS];
} ps_data_t;

/*
 * servant structure
 * */

typedef struct servant{
    id_t servant_id;
    tick_t start_time;
    tick_t LET;
    tick_t LED;
    int servant_type; // 0 for sensor; 1 for controller; 2 for actuator
    int num;   // the src servants of this servant
    int arrive; // arrived number of events from src servants
}ps_servant_t;


typedef struct {
    ps_servant_t * pservant_src;
    ps_servant_t * pservant_dest;
    ps_tag_t  tag;
    ps_data_t data;
    item_t eventItem;
}ps_event_t;


/*
 * task structure
 * */
typedef struct task{
    id_t task_id;
    tick_t LET;
    tick_t period;
    tick_t deadline;
    int servant_num;      // number of I-servants
    ps_servant_t * servants[NUMOFSERVANTS];  // I-servants in this task
}ps_task_t;


/*
 * mode structure
 * */

typedef struct mode{
    id_t mode_id; // equal to the serial number of array
    tick_t period;
	tick_t unit;
    int num;  // number of tasks
    ps_task_t * tasks[NUMOFTASKS];
}ps_mode_t;

typedef struct cond{
    id_t mode_dest;
    bool (*condition)(void);
}ps_mode_cond;




#endif
