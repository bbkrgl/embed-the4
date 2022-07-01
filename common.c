#include <p18f8722.h>

#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/

command cmd_out_;
command cmd_out = CHECK;
command cmd_in = -1;
unsigned char trans_ind = 0;
char recv_buf[11];
unsigned char recv_ind = 0;
unsigned char recv_f = 0;

unsigned int curr_money = 0;
unsigned char alert_string[8];
unsigned char hash[16];
unsigned int hunger_meter = 0;
unsigned int happy_meter = 0;
unsigned int thirst_meter = 0;
unsigned char money_opportunity = 0;


char cmd_list[][20] = {
	"{GO##}", "{END}",
	"{F}", "{W}", "{P}",
	"{C}", "{S###}",
	"{H################}", "{A########}", "{M##}"
};

/**********************************************************************
 * ----------------------- GLOBAL FUNCTIONS ---------------------------
 **********************************************************************/

void startTransmission(command cmd)
{
	int i = 2;
	cmd_out_ = cmd;
	if (cmd == HASH_SEND)
		for (; i < 18; i++)
			cmd_list[cmd][i] = hash[i - 2];
	trans_ind = 0;
	TXSTA1bits.TXEN = 1;
	TXREG1 = cmd_list[cmd][trans_ind++];
}

void transmitDataISR()
{
	if (cmd_list[cmd_out_][trans_ind] == '\0') {
		TXSTA1bits.TXEN = 0;
		cmd_out_ = 0;
		return;
	}
	TXREG1 = cmd_list[cmd_out_][trans_ind++];
}

/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceiveISR()
{
	char tmp = RCREG1;

	if (tmp == '{') {
		recv_ind = 0;
		recv_f = 1;
		return;
	}

	if (!recv_f)
		return;

	if (tmp == '}') {
		recv_ind = 0;
		recv_f = 0;
		parseBuffer();
		return;
	}

	recv_buf[recv_ind++] = tmp;
}

// Command receiver from serial communication
void parseBuffer()
{
	int index = 0;
	unsigned int money = 0;

	switch (recv_buf[0]) {
	case 'S':
		// SXYZ  --  Sensor Response (Response to Check)
		cmd_in = SENSOR_RESP;
		hunger_meter = recv_buf[1];
		happy_meter = recv_buf[2];
		thirst_meter = recv_buf[3];
		break;
	case 'G':
		// GOXX  --  GO Command
		cmd_in = GO;
		money = recv_buf[2] << 8;
		money += recv_buf[3];
		curr_money = money;
		break;
	case 'A':
		// ASTR  --  Alert Command
		cmd_in = ALERT_STR;
		for (index = 1; index < 9; index++)
			alert_string[index - 1] = recv_buf[index];
		money_opportunity = 1;
		break;
	case 'M':
		// MXX  --  Payment Response
		cmd_in = MONEY;
		money = recv_buf[1] << 8;
		money += recv_buf[2];
		curr_money += money;
		break;
	case 'E':
		// END  --  END Command
		cmd_in = END;
		break;
	}
}

/* End of File : common.c */
