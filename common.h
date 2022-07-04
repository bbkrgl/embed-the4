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
#define SEND_CMD_EVENT      0x80 // Posted every 50-60 ms to send command
#define HASH_GO             0x01 // Starts hash computation
#define TRANSMISSION_DONE   0x02 // Signals that the transmission is done and a new command can be sent
#define ALARM_EVENT         0x04 // LCD alarm, used for its timing
#define LCD_EVENT           0x08 // LCD is ready for new commands
#define LCD_DONE            0x10 // LCD initialization done

/***********************************************************************
 * ----------------------------- Task ID -------------------------------
 **********************************************************************/
/* Info about the tasks:
 * MAIN_TSK: Main task, which decides which command to send and starts the transmission
 * LCD_TSK: LCD task
 * INIT_TSK: Initializes the program
 * HASH_TSK: The task that concurrently computes the hash
 */
#define MAIN_TSK_ID          1
#define LCD_TSK_ID           2
#define INIT_TSK_ID          4
#define HASH_TSK_ID          8

/** 
 * Priorities of the tasks
 * MAIN and HASH has the same priority since they must be run concurrently
 * LCD has higher priority so that it can be updated during hash computation
 */
#define MAIN_PRIO            5
#define LCD_PRIO             6
#define INIT_PRIO            2
#define HASH_PRIO            5

#define ALARM_TSK0           0
#define LCD_ALARM_ID         1

/***********************************************************************
 * --------------------- Command and State Enums -----------------------
 **********************************************************************/
typedef enum {
    INIT, HUNGER, HAPPY, THIRST
} LCDdisplayState;

typedef enum {
    GO, END,
    FEED, WATER, PLAY,
    CHECK, SENSOR_RESP,
    HASH_SEND, ALERT_STR, MONEY
} command;

/***********************************************************************
 * ---------------------------- Global vars ----------------------------
 **********************************************************************/

extern command cmd_out; // Command to be sent, it is globally exposed to be able to check its value from the main task
extern command cmd_in; // Command that is read from the simulator (serial)

extern LCDdisplayState LCDstate; // Current value that is shown in the LCD

extern unsigned int curr_money; // Self descriptive
extern unsigned int hunger_meter; // Self descriptive
extern unsigned int happy_meter; // Self descriptive
extern unsigned int thirst_meter; // Self descriptive

extern unsigned char update_lcd; // Set when a change in the values of LCD is occurred

extern unsigned char alert_string[8]; // String that is read from serial
extern unsigned char hash[17]; // Hash to be sent
extern unsigned char hash_done; // Indicates that the hash generation is complete and must be sent

/* transmits data using serial communication */
void transmitDataISR();
/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceiveISR();

// Parse string that is read from serial, updates cmd_in, meters or curr_money depending on the command
void parseBuffer();
// Starts the transmission of a command
void startTransmission(command cmd);

// Switch the value shown in the LCD
void switchLCD();
// Update the values shown in the LCD
void updateLCD();

#endif

/* End of File : common.h */
