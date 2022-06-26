#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/


/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

TASK(TASK0)
{
	SetRelAlarm(ALARM_TSK0, 100, 5000);
	while (1) {
		WaitEvent(ALARM_EVENT);
		ClearEvent(ALARM_EVENT);
		
		startTransmission(FEED, 0);
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */