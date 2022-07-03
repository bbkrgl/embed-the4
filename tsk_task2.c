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
	WaitEvent(LCD_INIT);
	ClearEvent(LCD_INIT);
	LcdPrintString(init_str[0], 0, 0);
	PIE1bits.RCIE = 1;
	while (1) {
		// TODO: LCD
		if (cmd_in == GO) {
			ActivateTask(TASK0_ID);
			SetEvent(TASK0_ID, TRANSMISSION_DONE);
			break;
		}
	}
	TerminateTask();
}

/* End of File : tsk_task2.c */
