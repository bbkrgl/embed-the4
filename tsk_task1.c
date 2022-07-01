#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/

/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

#define FEED_COST 80
#define FEED_RESTORE 60
#define FEED_CRITICAL 20


#define WATER_COST 30
#define WATER_RESTORE 30
#define WATER_CRITICAL 20


#define PLAY_COST 150
#define PLAY_RESTORE 80
#define PLAY_CRITICAL 20

char feed_crtcl_flag = 0;
char water_crtcl_flag = 0;
char play_crtcl_flag = 0;

TASK(TASK1)
{
	//SetRelAlarm(ALARM_TSK1, 100, 50);

	while (1) {
		WaitEvent(CHECK_CMD_RESPONSE);
		ClearEvent(CHECK_CMD_RESPONSE);

		feed_crtcl_flag = hunger_meter < FEED_CRITICAL;
		water_crtcl_flag = thirst_meter < WATER_CRITICAL;
		play_crtcl_flag = happy_meter < PLAY_CRITICAL;

		if (water_crtcl_flag)
			cmd_out = WATER;
		else if (feed_crtcl_flag)
			cmd_out = FEED;
		else if (play_crtcl_flag)
			cmd_out = PLAY;
		else
			cmd_out = CHECK;

		if (money_opportunity) {
			money_opportunity = 0;
			compute_hash(alert_string, hash);
			cmd_out = HASH_SEND;
		}
	}
	TerminateTask();
}

/* End of File : tsk_task1.c */