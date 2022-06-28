#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/


/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

// send all commands periodically (T = 50 ms))
TASK(TASK0)
{
	
	SetRelAlarm(ALARM_TSK0, 0, 50);
	while (1) {
		WaitEvent(ALARM_TSK0);
		ClearEvent(ALARM_TSK0);

		startTransmission(cmd_out);
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */