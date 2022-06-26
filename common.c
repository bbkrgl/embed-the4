#include <p18f8722.h>

#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
char* trans_buf = 0;
unsigned char trans_ind = 0;
char recv_buf[11];
unsigned char recv_ind = 0;
unsigned char recv_f = 0;
unsigned char recv_interpret = 0;

char* cmd_list[] = {
	"{GO##}", "{END}",
	"{F}", "{W}", "{P}",
	"{C}", "{S###}",
	"{H################}", "{A########}", "{M##}"
};

/**********************************************************************
 * ----------------------- GLOBAL FUNCTIONS ---------------------------
 **********************************************************************/

void startTransmission(command cmd, char* cmd_args)
{
	int i = 2;
	trans_buf = cmd_list[cmd];
	if (cmd == HASH_SEND)
		for (; i < 18; i++)
			trans_buf[i] = cmd_args[i];
	
	TXREG1 = trans_buf[trans_ind++];
	TXSTA1bits.TXEN = 1;
}

void transmitData()
{
	if (!trans_buf || !trans_buf[trans_ind]) {
		TXSTA1bits.TXEN = 0;
		trans_buf = 0;
		return;
	}
	TXREG1 = trans_buf[trans_ind++];
}

/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceived()
{
	char tmp = RCREG1;

	if (tmp == '{') {
		recv_f = 1;
		return;
	}

	if (!recv_f)
		return;

	if (tmp == '}') {
		recv_f = 0;
		recv_interpret = 1;
	}

	recv_buf[recv_ind++] = tmp;
}

/* End of File : common.c */
