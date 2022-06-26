#ifndef __COMMON_H__
#define __COMMON_H__

#include "device.h"
#include <string.h>

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
#define ALARM_EVENT       0x80


/***********************************************************************
 * ----------------------------- Task ID -------------------------------
 **********************************************************************/
/* Info about the tasks:
 * TASK0: USART
 * TASK1: USART
 */
#define TASK0_ID             1
#define TASK1_ID             2

/* Priorities of the tasks */
#define TASK0_PRIO           8
#define TASK1_PRIO           8

#define ALARM_TSK0           0
#define ALARM_TSK1           1

/**********************************************************************
 * ----------------------- GLOBAL DEFINITIONS -------------------------
 **********************************************************************/

extern char recv_buf[11];
extern unsigned char recv_interpret;

typedef enum {
	GO, END,
	FEED, WATER, PLAY,
	CHECK, SENSOR_RESP,
	HASH_SEND, ALERT_STR, MONEY
} command;

/**********************************************************************
 * ----------------------- FUNCTION PROTOTYPES ------------------------
 **********************************************************************/
/* transmits data using serial communication */
void transmitData();
/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceived();

void startTransmission(command cmd, char* cmd_args);

#endif

/* End of File : common.h */
