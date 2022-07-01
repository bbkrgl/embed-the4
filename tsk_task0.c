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
	SetRelAlarm(ALARM_TSK0, 50, 50);
	while (1) {
		WaitEvent(SEND_CMD_EVENT);
		ClearEvent(SEND_CMD_EVENT);

		if (cmd_in == END) // TODO: Use events???
			break;

		startTransmission(cmd_out);

		SetEvent(TASK1_ID, CHECK_CMD_RESPONSE);
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */