#include <p18f8722.h>

#include "common.h"
#include "lcd.h"
#include "stdlib.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/

command cmd_out_ = -1;
command cmd_out = CHECK;
command cmd_in = -1;

unsigned char trans_ind = 0;

unsigned char recv_buf[11];
unsigned char recv_ind = 0;
unsigned char recv_f = 0;

unsigned int curr_money = 0;

unsigned char alert_string[8];

unsigned char hash[17];
unsigned char hash_done = 0;

unsigned int hunger_meter = 100;
unsigned int happy_meter = 100;
unsigned int thirst_meter = 100;
unsigned char update_lcd = 0;

unsigned char money_opportunity = 0;

LCDdisplayState LCDstate = INIT;

char money_display[17] = "MONEY:";
char money_arr[11];
char hunger_display[17] = "HUNGER";
char happy_display[17] = "HAPPY:";
char thirst_display[17] = "THIRST";

char cmd_list[10][20] = {
	"{GO##}", "{END}",
	"{F}", "{W}", "{P}",
	"{C}", "{S###}",
	"{H################}", "{A########}", "{M##}"
};

void updateLCD()
{
	int i, money_len = 0;
	unsigned int m2use = curr_money;
	unsigned char numBars = 0;

	memset(money_arr, 0, 11);
	itoa(m2use, money_arr);
	money_len = strlen(money_arr);

	money_display[0] = 'M', money_display[1] = 'O',money_display[2] = 'N',
	money_display[3] = 'E', money_display[4] = 'Y', money_display[5] = ':';

	for(i = 0; i < (10 - money_len); i++)
		money_display[i + 6] = ' ';
	for(; i < 10; i++)
		money_display[i + 6] = money_arr[i - (10 - money_len)];
	memset(money_arr, 0, 11);
	
	LcdPrintString(money_display, 0, 0);

	switch (LCDstate) {
	case HUNGER:
		numBars = hunger_meter / 10;

		for (i = 0; i < numBars; i++)
			hunger_display[6 + i] = 0xFF;

		for (; i < 10; i++)
			hunger_display[6 + i] = ' ';

		LcdPrintString(hunger_display, 0, 1);
		break;
	case HAPPY:
		numBars = happy_meter / 10;

		for (i = 0; i < numBars; i++)
			happy_display[6 + i] = 0xFF;

		for (; i < 10; i++)
			happy_display[6 + i] = ' ';

		LcdPrintString(happy_display, 0, 1);
		break;
	case THIRST:
		numBars = thirst_meter / 10;

		for (i = 0; i < numBars; i++)
			thirst_display[6 + i] = 0xFF;

		for (; i < 10; i++)
			thirst_display[6 + i] = ' ';

		LcdPrintString(thirst_display, 0, 1);
		break;
	}
}

void switchLCD()
{
	switch (LCDstate) {
	case HUNGER:
		LCDstate = HAPPY;
		break;
	case HAPPY:
		LCDstate = THIRST;
		break;
	case INIT:
	case THIRST:
		LCDstate = HUNGER;
		break;
	}

	update_lcd = 1;
}

void startTransmission(command cmd)
{
	int i = 2;
	cmd_out_ = cmd;
	if (cmd == HASH_SEND) {
		for (; i < 18; i++)
			cmd_list[cmd][i] = hash[i - 2];
		memset(hash, 0, 16);
	}
	trans_ind = 0;
	TXSTA1bits.TXEN = 1;
	TXREG1 = cmd_list[cmd][trans_ind++];
}

void transmitDataISR()
{
	if (cmd_list[cmd_out_][trans_ind] == '\0') {
		TXSTA1bits.TXEN = 0;
		cmd_out_ = CHECK;
		
		SetEvent(TASK0_ID, TRANSMISSION_DONE);
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
	unsigned int index = 0;
	unsigned char hun_m = 0, hap_m = 0, th_m = 0;
	unsigned int money = 0;

	switch (recv_buf[0]) {
	case 'S':
		// SXYZ  --  Sensor Response (Response to Check)
		cmd_in = SENSOR_RESP;
		hun_m = recv_buf[1];
		hap_m = recv_buf[2];
		th_m = recv_buf[3];
		if (hun_m != hunger_meter
			|| hap_m != happy_meter
			|| th_m != thirst_meter)
			update_lcd = 1;
		hunger_meter = hun_m;
		happy_meter = hap_m;
		thirst_meter = th_m;
		break;
	case 'G':
		// GOXX  --  GO Command
		cmd_in = GO;
		money = recv_buf[2]; // FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
		money <<= 8; // FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
		money += recv_buf[3]; // FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
		curr_money = money;
		break;
	case 'A':
		// ASTR  --  Alert Command
		cmd_in = ALERT_STR;
		for (index = 1; index < 9; index++)
			alert_string[index - 1] = recv_buf[index];
		money_opportunity = 1;
		SetEvent(TASK3_ID, HASH_GO);
		break;
	case 'M':
		// MXX  --  Payment Response
		cmd_in = MONEY;
		money = recv_buf[1];
		money <<= 8;
		money += recv_buf[2];
		curr_money += money;
		update_lcd = 1;
		break;
	case 'E':
		// END  --  END Command
		cmd_in = END;
		break;
	}
}

/* End of File : common.c */
