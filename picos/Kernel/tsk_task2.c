#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/

/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


TASK(TASK2)
{
	SetRelAlarm(ALARM_TSK1, 100, 50);
	while (1) {
		//WaitEvent(ALARM_EVENT);
		//ClearEvent(ALARM_EVENT);
	}
	TerminateTask();
}

/* End of File : tsk_task2.c */