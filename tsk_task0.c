#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
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

/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

// send all commands periodically (T = 50 ms))

TASK(TASK0)
{
	SetRelAlarm(ALARM_TSK0, 50, 60);
	while (1) {
		WaitEvent(SEND_CMD_EVENT);
		ClearEvent(SEND_CMD_EVENT);

		if (cmd_in == END) // TODO: Use events??? No just suspend TASK0 in the 
			break;

		WaitEvent(TRANSMISSION_DONE);
		ClearEvent(TRANSMISSION_DONE);

		if (hunger_meter < FEED_CRITICAL && curr_money >= FEED_COST) {
			cmd_out = FEED;
			curr_money -= FEED_COST;
			hunger_meter += FEED_RESTORE;
		} else if (happy_meter < PLAY_CRITICAL && curr_money >= PLAY_COST) {
			cmd_out = PLAY;
			curr_money -= PLAY_COST;
			happy_meter += PLAY_RESTORE;
		} else if (thirst_meter < WATER_CRITICAL && curr_money >= WATER_COST) {
			cmd_out = WATER;
			curr_money -= WATER_COST;
			thirst_meter += WATER_RESTORE;
		} else {
			if (hash_done) {
				hash_done = 0;
				cmd_out = HASH_SEND;
			} else {
				cmd_out = CHECK;
			}
		}

		startTransmission(cmd_out);
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */