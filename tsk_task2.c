#include "common.h"
#include "lcd.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/

/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/
const char init_str[2][16] = {"    CENG 336    ", "   TAMAGOTCHI   "};

TASK(TASK2)
{
	WaitEvent(LCD_DONE);
	ClearEvent(LCD_DONE);
	LcdPrintString(init_str[0], 0, 0);
	PIE1bits.RCIE = 1;
	while (1) {
		// TODO: LCD
		if (cmd_in == GO) {
			switchLCD();
			ActivateTask(TASK0_ID);
			SetEvent(TASK0_ID, TRANSMISSION_DONE);
		}
	}
	TerminateTask();
}

/* End of File : tsk_task2.c */
