/*
 * buttonISR.c
 *
 *  Created on: 
 *      Author: 
 */

#include "buttonISR.h"
//#include "io.h"
#include "system.h"
//#include "sys\alt_irq.h"
#include "altera_avalon_pio_regs.h"
//#include "HexDisplay.h"
#include "alt_types.h"

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
	//0x2 is button 1 0x4 is button 2
	switch (buttonPress){
	case 2:
		timersetting = IORD(INTERVAL_TIMER_BASE, 1);
		if (timersetting&0x8)
		{
		timersetting = 0x7;
		}
		else
		{
		timersetting = 0xB;
		}
		IOWR(INTERVAL_TIMER_BASE, 1 ,timersetting);
		break;
	case 4:
		*pCount = 0;
		HexDisplay((alt_u32*)HEX3_HEX0_BASE, *pCount);
		break;
	case 8:
		toggleBCDHEX();
		break;
	default:
		break;
	}


    /*
     * Read the PIO to delay ISR exit. This is done to prevent a spurious
     * interrupt in systems with high processor -> pio latency and fast
     * interrupts.
     */
    //IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);
}
