#include "common.h"

/**
 * Critical values indicate that the corresponding command must be sent to the simulator.
 * They are low enough so that no extra money is spent
 */

#define FEED_COST 80
#define FEED_RESTORE 60
#define FEED_CRITICAL 20

#define WATER_COST 30
#define WATER_RESTORE 30
#define WATER_CRITICAL 20

#define PLAY_COST 150
#define PLAY_RESTORE 80
#define PLAY_CRITICAL 20

TASK(MAIN_TSK)
{
	SetRelAlarm(ALARM_TSK0, 50, 50);
	while (1) {
		WaitEvent(SEND_CMD_EVENT); // Wait for timing
		ClearEvent(SEND_CMD_EVENT);

		if (cmd_in == END)
			break;

		WaitEvent(TRANSMISSION_DONE); // Wait for the ongoing transmission to be done
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
				cmd_out = HASH_SEND;
				hash_done = 0;
			} else {
				cmd_out = CHECK;
			}
		}

		if (update_lcd) {
			updateLCD();
			update_lcd = 0;
		}

		startTransmission(cmd_out);
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */