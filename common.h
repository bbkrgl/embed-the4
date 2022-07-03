#ifndef __COMMON_H__
#define __COMMON_H__

#include "device.h"
#include <string.h>

#define _XTAL_FREQ   40000000

/***********************************************************************
 * ------------------------ Timer settings -----------------------------
 **********************************************************************/
#define _10MHZ	63320
#define _16MHZ	61768
#define _20MHZ	60768
#define _32MHZ	57768
#define _40MHZ 	55768

/***********************************************************************
 * ----------------------------- Events --------------------------------
 **********************************************************************/
#define SEND_CMD_EVENT      0x80
#define HASH_GO             0x01
#define TRANSMISSION_DONE   0x02
#define ALARM_EVENT         0x04
#define LCD_INIT            0x10
#define LCD_EVENT           0x08


/***********************************************************************
 * ----------------------------- Task ID -------------------------------
 **********************************************************************/
/* Info about the tasks:
 * TASK0: USART
 * TASK1: USART
 */
#define TASK0_ID             1
#define LCD_ID               2
#define TASK2_ID             4
#define TASK3_ID             8

/* Priorities of the tasks */
#define TASK0_PRIO           5
#define LCD_PRIO             4
#define TASK2_PRIO           2
#define TASK3_PRIO           5

#define ALARM_TSK0           0
#define LCD_ALARM_ID         1

/**********************************************************************
 * ----------------------- GLOBAL DEFINITIONS -------------------------
 **********************************************************************/

typedef enum {
    GO, END,
	FEED, WATER, PLAY,
	CHECK, SENSOR_RESP,
	HASH_SEND, ALERT_STR, MONEY
} command;

extern command cmd_out;
extern command cmd_in;

extern unsigned int curr_money;
extern unsigned int hunger_meter;
extern unsigned int happy_meter;
extern unsigned int thirst_meter;
extern unsigned char money_opportunity;

extern unsigned char alert_string[8];
extern unsigned char hash[16];
extern unsigned char hash_done;

/**********************************************************************
 * ----------------------- FUNCTION PROTOTYPES ------------------------
 **********************************************************************/
/* transmits data using serial communication */
void transmitDataISR();
/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceiveISR();

void parseBuffer();
void startTransmission(command cmd);

#endif

/* End of File : common.h */
