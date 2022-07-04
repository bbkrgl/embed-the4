#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#include "common.h"
#include "lcd.h"

/**********************************************************************
 * Definition dedicated to the local functions.
 **********************************************************************/
#define DEFAULT_MODE       0


/**********************************************************************
 * Function prototypes.
 **********************************************************************/
void main(void);
void Init(void);
void StartupHook(void);
void ShutdownHook(StatusType error);
void ErrorHook(StatusType error);
void PreTaskHook(void);
void PostTaskHook(void);

extern union Timers Tmr0;

AppModeType SelectedMode;

/**********************************************************************
 * -------------------------- main function ---------------------------
 *
 * Setup the different alarms and start the kernel.
 *
 **********************************************************************/
void main(void)
{
	STKPTR = 0;
	SelectedMode = DEFAULT_MODE;
	Init();

	while (1) {
		StartOS(SelectedMode);
	}
}

/**********************************************************************
 * Clear all RAM memory and set PORTB to output mode.
 *
 * @return void
 **********************************************************************/
void Init(void)
{
	FSR0H = 0;
	FSR0L = 0;

	/* User setting : actual PIC frequency */
	Tmr0.lt = _40MHZ;

	/* Timer OFF - Enabled by Kernel */
	T0CON = 0x08;
	TMR0H = Tmr0.bt[1];
	TMR0L = Tmr0.bt[0];

	TRISA = 0;
	TRISC = 0;
	TRISE = 0;
	TRISF = 0;
	TRISG = 0;

	TRISH = 0;
	TRISJ = 0;

	PORTA = 0;
	PORTC = 0;
	PORTE = 0;
	PORTF = 0;
	PORTG = 0;

	PORTH = 0;
	PORTJ = 0;

	LATBbits.LATB5 = 0; // E  = 0
	LATBbits.LATB2 = 0; // RS = 0
	LATDbits.LATD4 = 0; // Data bus = 0
	LATDbits.LATD5 = 0; // Data bus = 0
	LATDbits.LATD6 = 0; // Data bus = 0
	LATDbits.LATD7 = 0; // Data bus = 0
	TRISBbits.TRISB5 = 0; // Make Output
	TRISBbits.TRISB2 = 0; // Make Output
	TRISDbits.TRISD4 = 0; // Make Output
	TRISDbits.TRISD5 = 0; // Make Output
	TRISDbits.TRISD6 = 0; // Make Output
	TRISDbits.TRISD7 = 0; // Make Output
	LATBbits.LATB5 = 0; // E  = 0
	LATBbits.LATB2 = 0; // RS = 0
	
	/* configure I/O ports */
	TRISCbits.RC7 = 1; // TX1 and RX1 pin configuration
	TRISCbits.RC6 = 0;

	/* configure USART transmitter/receiver */
	SPBRG1 = 21; // for 40 MHz, to have 115200 baud rate, it should be 21
	TXSTA1 = 0x04; // (= 00000100) 8-bit transmit, transmitter NOT enabled,TXSTA1.TXEN not enabled!
	// asynchronous, high speed mode
	RCSTA1 = 0x90; // (= 10010000) 8-bit receiver, receiver enabled,
	// continuous receive, serial port enabled RCSTA.CREN = 1

	/* configure the interrupts */
	INTCON = 0; // clear interrupt register completely
	PIE1bits.TX1IE = 1; // enable USART transmit interrupt
	PIE1bits.RC1IE = 1; // enable USART receive interrupt
	PIR1 = 0; // clear all peripheral flags
	
	TRISBbits.RB1 = 1;
	INTCON3 = 0;
	INTCON3bits.INT1IE = 1;
	INTCON2bits.INTEDG1 = 1;
	
	INTCONbits.PEIE = 1; // enable peripheral interrupts
	INTCONbits.GIE = 1; // globally enable interrupts
}

/**********************************************************************
 * Hook routine called just before entering in kernel.
 *
 * @param error      IN The new error stored in buffer
 * @return error     Error level
 **********************************************************************/
void StartupHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving the kernel.
 *
 * @param error      IN The last error detected by OS
 * @return void
 **********************************************************************/
void ShutdownHook(StatusType error)
{
}

/**********************************************************************
 * Store a new error in a global buffer keeping a track of the 
 * application history.
 *
 * @param error      IN The new error stored in buffer
 * @return void
 **********************************************************************/
void ErrorHook(StatusType error)
{
}

/**********************************************************************
 * Hook routine called just before entering in a task.
 *
 * @return void
 **********************************************************************/
void PreTaskHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving a task.
 *
 * @return void
 **********************************************************************/
void PostTaskHook(void)
{
}

/* End of File : main.c */
