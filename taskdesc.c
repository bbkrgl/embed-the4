#include "common.h"

/**********************************************************************
 * --------------------- COUNTER & ALARM DEFINITION -------------------
 **********************************************************************/
Counter Counter_list[] ={
	/*******************************************************************
	 * -------------------------- First counter ------------------------
	 *******************************************************************/
	{
		{
			200, /* maxAllowedValue        */
			10, /* ticksPerBase           */
			100 /* minCycle               */
		},
		0, /* CounterValue           */
		0 /* Nbr of Tick for 1 CPT  */
	}
};

Counter Counter_kernel ={
	{
		65535, /* maxAllowedValue        */
		1, /* ticksPerBase           */
		0 /* minCycle               */
	},
	0, /* CounterValue           */
	0 /* Nbr of Tick for 1 CPT  */
};

AlarmObject Alarm_list[] ={
	/*******************************************************************
	 * -------------------------- First task ---------------------------
	 *******************************************************************/
	{
		OFF, /* State                   */
		0, /* AlarmValue              */
		0, /* Cycle                   */
		&Counter_kernel, /* ptrCounter              */
		MAIN_TSK_ID, /* TaskID2Activate         */
		SEND_CMD_EVENT, /* EventToPost             */
		0 /* CallBack                */
	},
	/*******************************************************************
	 * -------------------------- Second task --------------------------
	 *******************************************************************/
	{
		OFF, /* State                   */
		0, /* AlarmValue              */
		0, /* Cycle                   */
		&Counter_kernel, /* ptrCounter              */
		LCD_TSK_ID, /* TaskID2Activate         */
		ALARM_EVENT, /* EventToPost             */
		0 /* CallBack                */
	},
};

#define _ALARMNUMBER_          sizeof(Alarm_list)/sizeof(AlarmObject)
#define _COUNTERNUMBER_        sizeof(Counter_list)/sizeof(Counter)
unsigned char ALARMNUMBER = _ALARMNUMBER_;
unsigned char COUNTERNUMBER = _COUNTERNUMBER_;
unsigned long global_counter = 0;

/**********************************************************************
 * --------------------- COUNTER & ALARM DEFINITION -------------------
 **********************************************************************/
Resource Resource_list[] ={
	{
		10, /* priority           */
		0, /* Task prio          */
		0, /* lock               */
	}
};

#define _RESOURCENUMBER_       sizeof(Resource_list)/sizeof(Resource)
unsigned char RESOURCENUMBER = _RESOURCENUMBER_;

/**********************************************************************
 * ----------------------- TASK & STACK DEFINITION --------------------
 **********************************************************************/
#define DEFAULT_STACK_SIZE      256
DeclareTask(MAIN_TSK);
DeclareTask(INIT_TSK);
DeclareTask(HASH_TSK);
DeclareTask(LCD_TSK);

// to avoid any C18 map error : regroup the stacks into blocks
// of 256 bytes (except the last one).
#pragma		udata      STACK_A   
volatile unsigned char main_stack[DEFAULT_STACK_SIZE];
#pragma		udata      STACK_B   
volatile unsigned char lcd_stack[DEFAULT_STACK_SIZE];
#pragma		udata	   STACK_C
volatile unsigned char init_stack[DEFAULT_STACK_SIZE];
#pragma		udata	   STACK_D
volatile unsigned char hash_stack[DEFAULT_STACK_SIZE];
#pragma		udata

/**********************************************************************
 * ---------------------- TASK DESCRIPTOR SECTION ---------------------
 **********************************************************************/
#pragma		romdata		DESC_ROM
const rom unsigned int descromarea;
/**********************************************************************
 * -----------------------------  task 0 ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_main = {
	MAIN_PRIO, /* prioinit from 0 to 15       */
	main_stack, /* stack address (16 bits)     */
	MAIN_TSK, /* start address (16 bits)     */
	SUSPENDED, /* state at init phase         */
	MAIN_TSK_ID, /* id_tsk from 0 to 15         */
	sizeof(main_stack) /* stack size    (16 bits)     */
};

/**********************************************************************
 * -----------------------------  task 1 ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_lcd = {
	LCD_PRIO, /* prioinit from 0 to 15       */
	lcd_stack, /* stack address (16 bits)     */
	LCD_TSK, /* start address (16 bits)     */
	READY, /* state at init phase         */
	LCD_TSK_ID, /* id_tsk from 0 to 15         */
	sizeof(lcd_stack) /* stack size    (16 bits)     */
};

/**********************************************************************
 * -----------------------------  task 2 ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_init = {
	INIT_PRIO, /* prioinit from 0 to 15       */
	init_stack, /* stack address (16 bits)     */
	INIT_TSK, /* start address (16 bits)     */
	READY, /* state at init phase         */
	INIT_TSK_ID, /* id_tsk from 0 to 15         */
	sizeof(init_stack) /* stack size    (16 bits)     */
};

/**********************************************************************
 * -----------------------------  task 2 ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_hash = {
	HASH_PRIO, /* prioinit from 0 to 15       */
	hash_stack, /* stack address (16 bits)     */
	HASH_TSK, /* start address (16 bits)     */
	READY, /* state at init phase         */
	HASH_TSK_ID, /* id_tsk from 0 to 15         */
	sizeof(hash_stack) /* stack size    (16 bits)     */
};

/**********************************************************************
 * --------------------- END TASK DESCRIPTOR SECTION ------------------
 **********************************************************************/
rom_desc_tsk end = {
	0x00, /* prioinit from 0 to 15       */
	0x0000, /* stack address (16 bits)     */
	0x0000, /* start address (16 bits)     */
	0x00, /* state at init phase         */
	0x00, /* id_tsk from 0 to 15         */
	0x0000 /* stack size    (16 bits)     */
};

volatile rom unsigned int * taskdesc_addr = (&(descromarea) + 1);

/* End of File : taskdesc.c */
