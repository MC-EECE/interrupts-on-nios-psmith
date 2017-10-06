/*
 * HexDisplay.c
 *
 *  Created on: Apr 21, 2015
 *      Author: brent.horine
 */
#include "seven_seg_hex.h"
#include "seven_seg_bcd.h"
#include "alt_types.h"
#include "io.h"
#include "system.h"

extern volatile alt_u8 leading_zeros;
extern volatile alt_u8 display_as_bcd;

void toggleBCDHEX(){
	display_as_bcd = !display_as_bcd;
}

void HexDisplay(alt_u32 * base, alt_u16 num){

	alt_u8 leading_zeros = 0x01&IORD(SLIDER_SWITCHES_BASE, 0);
	if (display_as_bcd){
		seven_seg_bcd(base, num, leading_zeros);
	}
	else
	{
		seven_seg_hex(base, num, leading_zeros); // leading_zeros);
	}
}
