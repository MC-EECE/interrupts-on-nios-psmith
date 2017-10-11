/*
 * buttonISR.c
 *
 *  Created on: 
 *      Author: 
 */

#include "buttonISR.h"
#include "io.h"
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "HexDisplay.h"
#include "alt_types.h"

//Use define statement to give meaningful names to variables used in switch statement

#define KEY1 2
#define KEY2 4
#define KEY3 8

#define timerOn 0x7
#define timerOff 0xB


#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void buttonISR(void* context)
#else
void buttonISR(void* context, alt_u32 id)
#endif
{
    /* Cast context It is important that this be declared 
     * volatile to avoid unwanted compiler optimization.*/
	alt_u32 *pCount = (alt_u32*)context;

	//volatile alt_u32 *keyPrs = (alt_u32*)context;
	alt_u8 timersetting;

	/* Store the value in the PUSHBUTTONS's edge capture register in *context. */
	alt_u8 buttonPress = IORD(PUSHBUTTONS_BASE, 3);

	/* Reset the PUSHBUTTONS's edge capture register. */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE, 0);

    /* Act upon the interrupt */
	switch (buttonPress){

	case KEY1:     //0x2
		timersetting = IORD(INTERVAL_TIMER_BASE, 1);  //assigns current timer state to timersetting
		if (timersetting&0x8) // if timer is off
		{
		timersetting = timerOn; // turn on
		}
		else
		{
		timersetting = timerOff; // if timer is on, turn off
		}
		IOWR(INTERVAL_TIMER_BASE, 1 ,timersetting);     // based on the if statement, will write 0x7 or 0xB and activate/deactivate
		break;

	case KEY2:    //0x4
		*pCount = 0;        //resets counter to 0
		HexDisplay((alt_u32*)HEX3_HEX0_BASE, *pCount); //displays count
		break;

	case KEY3:    //0x8
		toggleBCDHEX(); //toggles hex or BCD display
		break;
	default:  //included a default statement to prevent any errors
		break;
	}


    /*
     * Read the PIO to delay ISR exit. This is done to prevent a spurious
     * interrupt in systems with high processor -> pio latency and fast
     * interrupts.
     */
    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);
}
