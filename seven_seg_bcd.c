/*
 * seven_seg_bcd.c
 *
 *  Created on: Apr 21, 2015
 *      Author: brent.horine
 */
//#include "system.h"
//#include "io.h"
#include "alt_types.h"

void seven_seg_bcd(alt_u32* base, alt_u16 num, alt_u8 leading_zeros) {
	alt_u8 bcd[4];// = {0, 0, 0, 0};
	int i, j;

	alt_u8 BCD_CODES[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

	bcd[3] = 0;
	bcd[2] = 0;
	bcd[1] = 0;
	bcd[0] = 0;

	for (i=0; i<8; i++) {
		// check all columns for >= 5
		bcd[3] += (bcd[3] >= 5)?3:0;
		bcd[2] += (bcd[2] >= 5)?3:0;
		bcd[1] += (bcd[1] >= 5)?3:0;
		bcd[0] += (bcd[0] >= 5)?3:0;
		bcd[3] = ((bcd[3] << 1) & 0xE) + ((bcd[2] & 0x8) >> 3);
		bcd[2] = ((bcd[2] << 1) & 0xE) + ((bcd[1] & 0x8) >> 3);
		bcd[1] = ((bcd[1] << 1) & 0xE) + ((bcd[0] & 0x8) >> 3);
		bcd[0] = ((bcd[0] << 1) & 0xE) + ((num & 0x80) >> 7);
		num <<= 1;
	}
	alt_u8 trailing_zeros = 0;
	int bcd_total = 0;
	int k = 24;
	for (j=3; j>=0; j--) {
		if (leading_zeros || bcd[j] > 0 || trailing_zeros)
		{
			bcd_total += BCD_CODES[bcd[j]] << k;
			trailing_zeros = 1;
		}
		k -= 8;
	}
	// Output the value by dereferencing the base address pointer
	*(base) = bcd_total;
}
