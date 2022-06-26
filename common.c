#include <p18f8722.h>

#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/

command cmd_i;
unsigned char trans_ind = 0;
char recv_buf[11];
unsigned char recv_ind = 0;
unsigned char recv_f = 0;
unsigned char recv_interpret = 0;

rom char cmd_list[][20] = {
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
	cmd_i = cmd;
	if (cmd == HASH_SEND)
		for (; i < 18; i++)
			cmd_list[cmd][i] = cmd_args[i];
	trans_ind = 0;
	TXSTA1bits.TXEN = 1;
	TXREG1 = cmd_list[cmd][trans_ind++];
}

void transmitData()
{
	if (cmd_list[cmd_i][trans_ind] == '\0') {
		TXSTA1bits.TXEN = 0;
		cmd_i = 0;
		return;
	}
	TXREG1 = cmd_list[cmd_i][trans_ind++];
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
