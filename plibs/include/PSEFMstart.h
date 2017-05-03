#ifndef __PSEFMSTART_H
#define __PSEFMSTART_H
#include "PSEFMconfigure.h"
#include "PSEFMport.h"

/******************************************************
Function: ps_mode_switch
Description: PSEFM kernel function, run every tick interrupt happended.
Implemented the time-triggered semantics, triggered R-Servant and Servants to run according to the information of events
Calls:
Called by: vApplicationTickHook()
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_time_trigger();


/******************************************************
Function: myHardwareInit
Description: RTOS kernel function, init the drivers of stm32
Calls:
Called by: system_start()
Input:
Output:
Return:
Others:
 * *****************************************************/
void myHardwareInit();

/******************************************************
Function: system_start
Description: Programming API, Initialize the model time, event list and execution flow, and start the RTOS scheduler
Calls:
Called by: main()
Input:
Output:
Return:
Others:
 * *****************************************************/
void system_start();

void myTraceCreate();

void myTraceSwitchedIn();

void myTraceSwitchedOut();

void vApplicationTickHook();

#endif
