#include "common.h"
#include "lcd.h"

/**
 *
 * Initializer Task
 * Initializes LCD and enables serial receive interrupt.
 * Starts the program after GO command then terminates.
 *
 */

const char init_str[2][16] = {"    CENG 336    ", "   TAMAGOTCHI   "};

TASK(INIT_TSK)
{
	WaitEvent(LCD_DONE);
	ClearEvent(LCD_DONE);
	LcdPrintString(init_str[0], 0, 0);
	PIE1bits.RCIE = 1;
	while (1) {
		if (cmd_in == GO) {
			switchLCD();
			ActivateTask(MAIN_TSK_ID);
			SetEvent(MAIN_TSK_ID, TRANSMISSION_DONE);
			break;
		}
	}
	TerminateTask();
}

/* End of File : tsk_task2.c */
