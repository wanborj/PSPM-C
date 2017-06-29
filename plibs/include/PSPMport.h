#ifndef __PSEFMPORT_H
#define __PSEFMPORT_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
// based on FreeRTOS API
// for configuring the mode of prv_ef_is_time_to_trigger in ef.c file
// 0, complete deterministic mode
// otherwise, support C-Servants execute advance for power saving
#define configUSE_OPTIMIZATION  1
// servant operation
#define port_servant_create(runnable, para, prio) xTaskCreate(runnable, NULL, 128, para, prio, NULL)

// trigger R_Servant to run
#define port_servant_yield() taskYIELD()


// event operation
#define port_wait(e)   xSemaphoreTake( e, portMAX_DELAY)
#define port_trigger(e) xSemaphoreGive( e )
// create semaphore and take the token away
#define port_semph_create(e)  vSemaphoreCreateBinary(e);xSemaphoreTake(e, portMAX_DELAY)
#define port_semph_t xQueueHandle


// get current information
#define port_get_current_time() xTaskGetTickCount()

// prv_ef_get_current_servant()

#define port_malloc(size) pvPortMalloc(size)

#define port_free(pointer) vPortFree(pointer)

//#define port_print(string)  { int i = 0; while( (char)string[i]!='\0') { send_byte((char)string[i]); i++;}}
#define port_print(string) vPrintString((char *) string)

#define port_scheduler_start() vTaskStartScheduler()

#endif
