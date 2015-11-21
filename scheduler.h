/*==================================================================
  File Name    : scheduler.h
  Function name: -
  File name    : $Id$
  Author       : E. van de Logt
  ------------------------------------------------------------------
  Purpose : This is the header-file for scheduler.c
  ------------------------------------------------------------------
  $Log$
  Revision 1.1  2015/03/21 11:12:51  Emile
  - Bugfix W command (PWM output), W0 was always sent
  - V command enabled (Valves output)
  - scheduler.h and scheduler.c added to CVS library

  ==================================================================
*/ 
#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "misc.h"

#define MAX_TASKS        (15)
#define MAX_MSEC      (60000)
#define TICKS_PER_SEC   (10L)
#define NAME_LEN         (30)

#define TASK_READY    (0x01)
#define TASK_ENABLED  (0x02)

#define NO_ERR        (0x00)
#define ERR_CMD	      (0x01)
#define ERR_NUM	      (0x02)
#define ERR_I2C	      (0x03)
#define ERR_NAME      (0x04)
#define ERR_EMPTY     (0x05)
#define ERR_MAX_TASKS (0x06)

typedef unsigned int  uint16_t;
typedef unsigned char uint8_t;

typedef struct _task_struct
{
	void     (* pFunction)(); // Function pointer
	char     Name[NAME_LEN];  // Task name
	uint16_t Period;          // Period between 2 calls in msec.
	uint16_t Delay;           // Initial delay before Counter starts in msec.
	uint16_t Counter;         // Running counter, is init. from Period
	uint8_t	 Status;          // bit 1: 1=enabled ; bit 0: 1=ready to run
        long     Duration;        // Measured task-duration in usec.
	long     Duration_Max;    // Max. measured task-duration
} task_struct;

void    scheduler_isr(void);  // run-time function for scheduler
void    dispatch_tasks(void); // run all tasks that are ready
uint8_t add_task(void (*task_ptr), char *Name, uint16_t delay, uint16_t period);
uint8_t set_task_time_period(uint16_t Period, char *Name);
uint8_t enable_task(char *Name);
uint8_t disable_task(char *Name);
void    list_all_tasks(FILE *fd);

#ifdef __cplusplus
};
#endif
#endif
