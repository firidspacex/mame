// license:BSD-3-Clause
// copyright-holders:David Haywood, ElSemi
/***********************************************************************
 PGM 022 + 025 PGM protection emulation

 this file contains the game / pgm specifc hookups for the IGS022/IGS025
 protection chips, actual simulation is in igs025_igs022.c

 sed by

 The Killing Blade
 Dragon World 3
 Dragon World 3 EX

 ----



 ***********************************************************************/

#include "emu.h"
#include "includes/pgm.h"
#include "machine/pgmprot_igs025_igs022.h"

/* The IGS022 is an MCU which performs encrypted DMA used by
 - The Killing Blade
 - Dragon World 3
 - Dragon World 3 Ex

 There is also an automatic transfer which happens on startup using params stored in the data ROM.
 This has been verified on real hardware running without any 68k game program.
*/


/* NON-device stuff, game specific, keep here */

void pgm_022_025_state::pgm_dw3_decrypt()
{
	int i;
	uint16_t *src = (uint16_t *) (memregion("maincpu")->base()+0x100000);

	int rom_size = 0x100000;

	for(i=0; i<rom_size/2; i++) {
		uint16_t x = src[i];

		if((i & 0x005460) == 0x001400 || (i & 0x005450) == 0x001040)
			x ^= 0x0100;

		if((i & 0x005e00) == 0x001c00 || (i & 0x005580) == 0x001100)
			x ^= 0x0040;

		src[i] = x;
	}
}

void pgm_022_025_state::pgm_killbld_decrypt()
{
	int i;
	uint16_t *src = (uint16_t *) (memregion("maincpu")->base()+0x100000);

	int rom_size = 0x200000;

	for(i=0; i<rom_size/2; i++) {
		uint16_t x = src[i];

		if((i & 0x006d00) == 0x000400 || (i & 0x006c80) == 0x000880)
			x ^= 0x0008;

		if((i & 0x007500) == 0x002400 || (i & 0x007600) == 0x003200)
			x ^= 0x1000;

		src[i] = x;
	}
}

// these were all xored by a table at $178B2A
static const uint8_t killbld_source_data[0x0c][0xec] =  // offsets to these tables stored at $155ed0
{
	{ // region 16, $178772
		0x5e, 0x09, 0xb3, 0x39, 0x60, 0x71, 0x71, 0x53, 0x11, 0xe5, 0x26, 0x34, 0x4c, 0x8c, 0x90, 0xee,
		0xed, 0xb5, 0x05, 0x95, 0x9e, 0x6b, 0xdd, 0x87, 0x0e, 0x7b, 0xed, 0x33, 0xaf, 0xc2, 0x62, 0x98,
		0xec, 0xc8, 0x2c, 0x2b, 0x57, 0x3d, 0x00, 0xbd, 0x12, 0xac, 0xba, 0x64, 0x81, 0x99, 0x16, 0x29,
		0xb4, 0x63, 0xa8, 0xd9, 0xc9, 0x5f, 0xfe, 0x21, 0xbb, 0xbf, 0x9b, 0xd1, 0x7b, 0x93, 0xc4, 0x82,
		0xef, 0x2b, 0xe8, 0xa6, 0xdc, 0x68, 0x3a, 0xd9, 0xc9, 0x23, 0xc7, 0x7b, 0x98, 0x5b, 0xe1, 0xc7,
		0xa3, 0xd4, 0x51, 0x0a, 0x86, 0x30, 0x20, 0x51, 0x6e, 0x04, 0x1c, 0xd4, 0xfb, 0xf5, 0x22, 0x8f,
		0x16, 0x6f, 0xb9, 0x59, 0x30, 0xcf, 0xab, 0x32, 0x1d, 0x6c, 0x84, 0xab, 0x23, 0x90, 0x94, 0xb1,
		0xe7, 0x4b, 0x6d, 0xc1, 0x84, 0xba, 0x32, 0x68, 0xa3, 0xf2, 0x47, 0x28, 0xe5, 0xcb, 0xbb, 0x47,
		0x14, 0x2c, 0xad, 0x4d, 0xa1, 0xd7, 0x18, 0x53, 0xf7, 0x6f, 0x05, 0x81, 0x8f, 0xbb, 0x29, 0xdc,
		0xbd, 0x17, 0x61, 0x92, 0x9b, 0x1d, 0x4e, 0x7a, 0x83, 0x14, 0x9f, 0x7b, 0x7a, 0x6a, 0xe1, 0x27,
		0x62, 0x52, 0x7e, 0x82, 0x45, 0xda, 0xed, 0xf1, 0x0a, 0x3b, 0x6c, 0x02, 0x5b, 0x6e, 0x45, 0x4e,
		0xf2, 0x65, 0x87, 0x1d, 0x80, 0xed, 0x6a, 0xc3, 0x77, 0xcb, 0xe8, 0x8d, 0x5a, 0xb8, 0xda, 0x89,
		0x88, 0x4b, 0x27, 0xd5, 0x57, 0x29, 0x91, 0x86, 0x12, 0xbb, 0xd3, 0x8c, 0xc7, 0x49, 0x84, 0x9c,
		0x96, 0x59, 0x30, 0x93, 0x92, 0xeb, 0x59, 0x2b, 0x93, 0x5b, 0x5f, 0xf9, 0x67, 0xac, 0x97, 0x8c,
		0x04, 0xda, 0x1b, 0x65, 0xd7, 0xef, 0x44, 0xca, 0xc4, 0x87, 0x18, 0x2b
	},
	{ // region 17, $178a36
		0xd7, 0x49, 0xb3, 0x39, 0x60, 0x71, 0x70, 0x53, 0x11, 0x00, 0x27, 0xb2, 0x61, 0xd3, 0x8c, 0x8b,
		0xb2, 0xde, 0x6a, 0x78, 0x40, 0x5d, 0x4d, 0x88, 0xeb, 0x81, 0xd0, 0x2a, 0xbf, 0x8c, 0x22, 0x0d,
		0x89, 0x83, 0xc8, 0xef, 0x0d, 0x7a, 0xf6, 0xf0, 0x1d, 0x49, 0xa2, 0xd3, 0x1e, 0xef, 0x1c, 0xa2,
		0xce, 0x00, 0x5e, 0xa8, 0x7f, 0x4c, 0x41, 0x27, 0xa8, 0x6b, 0x92, 0x0a, 0xb8, 0x03, 0x2f, 0x7e,
		0xaf, 0x4a, 0xd0, 0x5c, 0xce, 0xeb, 0x0e, 0x8a, 0x4d, 0x0b, 0x73, 0xb3, 0xf3, 0x0c, 0x83, 0xaa,
		0xe5, 0xe4, 0x84, 0x06, 0xd7, 0xcc, 0xcb, 0x52, 0x8d, 0xbe, 0xa4, 0xdf, 0xd9, 0xab, 0x50, 0x59,
		0x53, 0x61, 0xa1, 0xc8, 0x6d, 0xbc, 0xde, 0xab, 0xaa, 0x5e, 0xc6, 0xf7, 0x83, 0xdc, 0x40, 0xcb,
		0x1b, 0xdd, 0x28, 0x3b, 0xee, 0xb1, 0x1f, 0x37, 0xdb, 0xe9, 0xbb, 0x74, 0x4b, 0xc2, 0x8a, 0xe8,
		0xec, 0x6e, 0x0e, 0x35, 0xe3, 0x2e, 0xbe, 0xef, 0xfd, 0x07, 0xbf, 0x8c, 0xfe, 0xf3, 0x5c, 0xbf,
		0x87, 0xe5, 0xbc, 0xcf, 0x60, 0xdc, 0x18, 0xf8, 0xfc, 0x51, 0x50, 0x86, 0xc6, 0x48, 0x3d, 0xb9,
		0x1d, 0x26, 0xf7, 0x7e, 0x87, 0x90, 0x12, 0xe8, 0x06, 0x0a, 0x45, 0xe9, 0xd9, 0xd8, 0x41, 0x68,
		0x21, 0x52, 0x92, 0x0f, 0xd6, 0xda, 0xa2, 0x97, 0xeb, 0x68, 0xd0, 0xb1, 0x15, 0x19, 0x8b, 0xd0,
		0x48, 0x1a, 0xeb, 0x90, 0x3f, 0x2a, 0x33, 0x1e, 0x5e, 0x30, 0x66, 0x01, 0x64, 0xef, 0x99, 0x52,
		0xba, 0x23, 0xbd, 0x53, 0xc0, 0x60, 0x87, 0x09, 0xcb, 0x4d, 0xd3, 0x87, 0x0e, 0x3a, 0x5c, 0x8d,
		0xc8, 0xb8, 0xb7, 0x34, 0x01, 0xeb, 0x72, 0x0d, 0xb1, 0x1f, 0x0f, 0xea
	},
	{ // region 18, $17dac4
		0x6a, 0x13, 0xb3, 0x09, 0x60, 0x79, 0x61, 0x53, 0x11, 0x33, 0x41, 0x31, 0x76, 0x34, 0x88, 0x0f,
		0x77, 0x08, 0xb6, 0x74, 0xc8, 0x36, 0xbc, 0x70, 0xe2, 0x87, 0x9a, 0x21, 0xe8, 0x56, 0xe1, 0x9a,
		0x26, 0x57, 0x7e, 0x9b, 0xdb, 0xb7, 0xd4, 0x3d, 0x0f, 0xfe, 0x8a, 0x2a, 0xba, 0x2d, 0x22, 0x03,
		0xcf, 0x9c, 0xfa, 0x77, 0x35, 0x39, 0x6a, 0x14, 0xae, 0x30, 0x89, 0x42, 0xdc, 0x59, 0xb2, 0x93,
		0x6f, 0x82, 0xd1, 0x12, 0xd9, 0x88, 0xfa, 0x3b, 0xb7, 0x0c, 0x1f, 0x05, 0x68, 0xa3, 0x0c, 0xa6,
		0x0f, 0xf4, 0x9e, 0x1b, 0x29, 0x82, 0x77, 0x3a, 0xac, 0x92, 0x2d, 0x04, 0xd0, 0x61, 0x65, 0x0a,
		0x77, 0x6c, 0x89, 0x38, 0xaa, 0xa9, 0xf8, 0x0c, 0x1f, 0x37, 0x09, 0x2b, 0xca, 0x29, 0x05, 0xe5,
		0x4e, 0x57, 0xfb, 0xcd, 0x40, 0xa8, 0x0c, 0x06, 0x2d, 0xe0, 0x30, 0xd9, 0x97, 0xb9, 0x59, 0x8a,
		0xde, 0xc9, 0x87, 0x1d, 0x3f, 0x84, 0x4c, 0x73, 0x04, 0x85, 0x61, 0xb0, 0x6e, 0x2c, 0x8f, 0xa2,
		0x6a, 0xcd, 0x31, 0xf3, 0x25, 0x83, 0xe1, 0x5e, 0x5d, 0xa7, 0xe7, 0xaa, 0x13, 0x26, 0xb1, 0x33,
		0xf0, 0x13, 0x58, 0x7a, 0xb0, 0x46, 0x1d, 0xdf, 0x02, 0xbf, 0x1e, 0xd1, 0x71, 0x43, 0x56, 0x82,
		0x4f, 0x58, 0x9d, 0x01, 0x2d, 0xc7, 0xda, 0x6b, 0x47, 0x05, 0xd1, 0xd5, 0xe8, 0x92, 0x3c, 0x18,
		0x21, 0xcf, 0xc9, 0x32, 0x0e, 0x12, 0xed, 0xb5, 0xaa, 0xa4, 0x12, 0x75, 0x01, 0x7d, 0xc7, 0x21,
		0xde, 0xec, 0x32, 0x13, 0xee, 0xd4, 0x9c, 0xe6, 0x04, 0x3f, 0x48, 0xfb, 0xb4, 0xc7, 0x21, 0x8e,
		0x8d, 0x7d, 0x54, 0x03, 0x11, 0xe7, 0xb9, 0x4f, 0x85, 0xb6, 0x1f, 0xaa
	},
	{ // region 19, $178eee
		0xe3, 0x53, 0xb3, 0x09, 0x60, 0x79, 0x60, 0x53, 0x11, 0x66, 0x5b, 0xc8, 0x8b, 0x94, 0x84, 0xab,
		0x3c, 0x18, 0x03, 0x57, 0x6a, 0x0f, 0x45, 0x58, 0xc0, 0x74, 0x64, 0x18, 0xf8, 0x39, 0xa1, 0x0f,
		0xc2, 0x2b, 0x1b, 0x60, 0xaa, 0x0e, 0xb2, 0x89, 0x01, 0x9b, 0x72, 0x80, 0x57, 0x83, 0x28, 0x63,
		0xe9, 0x39, 0x97, 0x46, 0xea, 0x3f, 0x93, 0x01, 0x9b, 0xf4, 0x80, 0x93, 0x01, 0xaf, 0x1d, 0x8f,
		0x16, 0xa1, 0xb9, 0xc7, 0xe4, 0x0c, 0xe7, 0xd2, 0x3b, 0xf3, 0xca, 0x3d, 0xc3, 0x54, 0xad, 0x89,
		0x51, 0x1e, 0xd1, 0x17, 0x7a, 0x1f, 0x23, 0x22, 0xcb, 0x4d, 0xce, 0x0f, 0xae, 0x30, 0x93, 0xd3,
		0x9b, 0x77, 0x71, 0xa7, 0xe7, 0x96, 0x2c, 0x85, 0xac, 0x29, 0x4b, 0x5e, 0x2b, 0x75, 0xb0, 0x00,
		0x81, 0xe9, 0xb6, 0x47, 0xaa, 0x9f, 0xdf, 0xd4, 0x7e, 0xd7, 0xa4, 0x3f, 0xe3, 0xb0, 0x41, 0x2c,
		0xb7, 0x0c, 0xe7, 0xeb, 0x9a, 0xda, 0xd9, 0x10, 0x23, 0x1d, 0x1c, 0xd4, 0xdd, 0x7d, 0xc2, 0x6c,
		0x4d, 0x9c, 0xa5, 0x18, 0xd0, 0x43, 0xab, 0xdc, 0xbd, 0xe4, 0x7f, 0xb5, 0x5f, 0x04, 0x0d, 0xac,
		0xab, 0xe6, 0xb8, 0x76, 0xf2, 0x15, 0x41, 0xef, 0x17, 0x8e, 0xf6, 0xb9, 0xef, 0x94, 0x52, 0x83,
		0x96, 0x45, 0x8f, 0xf2, 0x9c, 0xb4, 0x13, 0x3f, 0xbb, 0xa1, 0xd2, 0xf9, 0xa3, 0xf2, 0x06, 0x78,
		0xe0, 0x9e, 0xa7, 0xd3, 0xdc, 0x13, 0x8f, 0x4d, 0xf6, 0x19, 0xbd, 0x03, 0x9d, 0x24, 0xdc, 0xd6,
		0xe9, 0xcf, 0xa6, 0xd2, 0x1d, 0x49, 0xca, 0xc4, 0x55, 0x18, 0xbc, 0x70, 0x5b, 0x55, 0xfe, 0x8f,
		0x6b, 0x42, 0xf0, 0xd1, 0x21, 0xe3, 0xe7, 0x91, 0x59, 0x4e, 0x16, 0x83
	},
	{ 0, }, // unused region 1a
	{ 0, }, // unused region 1b
	{ 0, }, // unused region 1c
	{ 0, }, // unused region 1d
	{ 0, }, // unused region 1e
	{ 0, }, // unused region 1f
	{ // region 20, $17a322
		0xb3, 0x10, 0xf3, 0x0b, 0xe0, 0x71, 0x60, 0x53, 0x11, 0x9a, 0x12, 0x70, 0x1f, 0x1e, 0x81, 0xda,
		0x9d, 0x1f, 0x4b, 0xd6, 0x71, 0x48, 0x83, 0xe1, 0x04, 0x6c, 0x1b, 0xf1, 0xcd, 0x09, 0xdf, 0x3e,
		0x0b, 0xaa, 0x95, 0xc1, 0x07, 0xec, 0x0f, 0x54, 0xd0, 0x16, 0xb0, 0xdc, 0x86, 0x7b, 0x52, 0x38,
		0x3c, 0x68, 0x2b, 0xed, 0xe2, 0xeb, 0xb3, 0xc6, 0x48, 0x24, 0x41, 0x36, 0x17, 0x25, 0x1f, 0xa5,
		0x22, 0xc6, 0x5c, 0xa6, 0x19, 0xef, 0x17, 0x5c, 0x56, 0x4b, 0x4a, 0x2b, 0x75, 0xab, 0xe6, 0x22,
		0xd5, 0xc0, 0xd3, 0x46, 0xcc, 0xe4, 0xd4, 0xc4, 0x8c, 0x9a, 0x8a, 0x75, 0x24, 0x73, 0xa4, 0x26,
		0xca, 0x79, 0xaf, 0xb3, 0x94, 0x2a, 0x15, 0xbe, 0x40, 0x7b, 0x4d, 0xf6, 0xb4, 0xa4, 0x7b, 0xcf,
		0xce, 0xa0, 0x1d, 0xcb, 0x2f, 0x60, 0x28, 0x63, 0x85, 0x98, 0xd3, 0xd2, 0x45, 0x3f, 0x02, 0x65,
		0xd7, 0xf4, 0xbc, 0x2a, 0xe7, 0x50, 0xd1, 0x3f, 0x7f, 0xf6, 0x05, 0xb8, 0xe9, 0x39, 0x10, 0x6e,
		0x68, 0xa8, 0x89, 0x60, 0x00, 0x68, 0xfd, 0x20, 0xc4, 0xdc, 0xef, 0x67, 0x75, 0xfb, 0xbe, 0xfe,
		0x2b, 0x16, 0xa6, 0x5a, 0x77, 0x0d, 0x0c, 0xe2, 0x2d, 0xd1, 0xe4, 0x11, 0xc9, 0x4b, 0x81, 0x3a,
		0x0c, 0x24, 0xaa, 0x77, 0x2b, 0x2f, 0x83, 0x23, 0xd1, 0xe9, 0xa7, 0x29, 0x0a, 0xf9, 0x26, 0x9d,
		0x51, 0xc8, 0x6d, 0x71, 0x9d, 0xce, 0x46, 0x72, 0x26, 0x48, 0x3d, 0x64, 0xe5, 0x67, 0xbb, 0x1a,
		0xb4, 0x6d, 0x21, 0x11, 0x79, 0x78, 0xc2, 0xd5, 0x11, 0x6a, 0xd2, 0xea, 0x03, 0x4d, 0x92, 0xaf,
		0x18, 0xd5, 0x07, 0x79, 0xaa, 0xf9, 0x44, 0x93, 0x6f, 0x41, 0x22, 0x0d
	},
	{ // region 21, $17b3b4
		0x2d, 0x50, 0xf3, 0x0b, 0xe0, 0x71, 0x61, 0x53, 0x11, 0xb4, 0x2c, 0xee, 0x34, 0x7e, 0x7d, 0x5e,
		0x62, 0x48, 0x97, 0xd2, 0xf9, 0x3a, 0xf2, 0xc9, 0xfa, 0x59, 0xe4, 0xe8, 0xf6, 0xd2, 0x9f, 0xb2,
		0xa7, 0x7e, 0x32, 0x86, 0xbc, 0x43, 0xec, 0xa0, 0xc2, 0xcb, 0x98, 0x33, 0x23, 0xd1, 0x58, 0x98,
		0x56, 0x05, 0xc7, 0xbc, 0x98, 0xd8, 0xdc, 0xb3, 0x35, 0xe8, 0x51, 0x6e, 0x3b, 0x7b, 0x89, 0xba,
		0xe1, 0xe5, 0x44, 0x5c, 0x24, 0x73, 0x04, 0x0d, 0xd9, 0x33, 0xf5, 0x63, 0xe9, 0x5c, 0x88, 0x05,
		0x18, 0xd0, 0x07, 0x5b, 0x1e, 0x81, 0x80, 0xac, 0x92, 0x6e, 0x13, 0x80, 0x1b, 0x29, 0xd2, 0xef,
		0x08, 0x84, 0x97, 0x23, 0xd1, 0x17, 0x2f, 0x38, 0xb4, 0x6d, 0x8f, 0x2a, 0x15, 0xf0, 0x40, 0xe9,
		0x02, 0x33, 0xd7, 0x5e, 0x99, 0x57, 0x15, 0x32, 0xbd, 0x8f, 0x48, 0x38, 0x91, 0x36, 0xe9, 0x07,
		0xc9, 0x37, 0x1d, 0x12, 0x2a, 0xbf, 0x5f, 0xdb, 0x85, 0x75, 0xbf, 0xdc, 0x59, 0x8a, 0x43, 0x51,
		0x4b, 0x77, 0xfd, 0x84, 0xc4, 0x28, 0xc7, 0x85, 0x25, 0x1a, 0x87, 0x8b, 0xc1, 0xd9, 0x1a, 0x78,
		0xe5, 0x03, 0x20, 0x56, 0xa0, 0xc2, 0x17, 0xf2, 0x29, 0xa0, 0xbd, 0xf8, 0x61, 0x9c, 0x7d, 0x54,
		0x3a, 0x11, 0xb5, 0x69, 0x9a, 0x1c, 0xbb, 0xf6, 0x2d, 0x86, 0xa8, 0x4d, 0xdd, 0x5a, 0xd6, 0xe4,
		0x11, 0x7e, 0x4b, 0x13, 0x6c, 0xb6, 0x01, 0x0a, 0x72, 0xbc, 0xe8, 0xf1, 0x82, 0x0e, 0xd0, 0xcf,
		0xbf, 0x50, 0x95, 0xb7, 0xa7, 0xec, 0xd7, 0xb3, 0x49, 0x5c, 0x47, 0x5f, 0xa9, 0xda, 0x70, 0xb0,
		0xdc, 0x9a, 0xa3, 0x48, 0xd3, 0xf5, 0x72, 0xd5, 0x43, 0xd8, 0x19, 0xcc
	}
};

// all tables xored with data from $149c4c
// tables are the same as olds and drgw2
static const uint8_t dw3_source_data[0x08][0xec] =
{
	{ 0, },
	{ // region 1, $14c21a
		0x67, 0x51, 0xf3, 0x19, 0xa0, 0x09, 0xb1, 0x21, 0xb0, 0xee, 0xe3, 0xf6, 0xbe, 0x81, 0x35, 0xe3,
		0xfb, 0xe6, 0xef, 0xdf, 0x61, 0x01, 0xfa, 0x22, 0x5d, 0x43, 0x01, 0xa5, 0x3b, 0x17, 0xd4, 0x74,
		0xf0, 0xf4, 0xf3, 0x43, 0xb5, 0x19, 0x04, 0xd5, 0x84, 0xce, 0x87, 0xfe, 0x35, 0x3e, 0xc4, 0x3c,
		0xc7, 0x85, 0x2a, 0x33, 0x00, 0x86, 0xd0, 0x4d, 0x65, 0x4b, 0xf9, 0xe9, 0xc0, 0xba, 0xaa, 0x77,
		0x9e, 0x66, 0xf6, 0x0f, 0x4f, 0x3a, 0xb6, 0xf1, 0x64, 0x9a, 0xe9, 0x25, 0x1a, 0x5f, 0x22, 0xa3,
		0xa2, 0xbf, 0x4b, 0x77, 0x3f, 0x34, 0xc9, 0x6e, 0xdb, 0x12, 0x5c, 0x33, 0xa5, 0x8b, 0x6c, 0xb1,
		0x74, 0xc8, 0x40, 0x4e, 0x2f, 0xe7, 0x46, 0xae, 0x99, 0xfc, 0xb0, 0x55, 0x54, 0xdf, 0xa7, 0xa1,
		0x0f, 0x5e, 0x49, 0xcf, 0x56, 0x3c, 0x90, 0x2b, 0xac, 0x65, 0x6e, 0xdb, 0x58, 0x3e, 0xc9, 0x00,
		0xae, 0x53, 0x4d, 0x92, 0xfa, 0x40, 0xb2, 0x6b, 0x65, 0x4b, 0x90, 0x8a, 0x0c, 0xe2, 0xa5, 0x9a,
		0xd0, 0x20, 0x29, 0x55, 0xa4, 0x44, 0xac, 0x51, 0x87, 0x54, 0x53, 0x34, 0x24, 0x4b, 0x81, 0x67,
		0x34, 0x4c, 0x5f, 0x31, 0x4e, 0xf2, 0xf1, 0x19, 0x18, 0x1c, 0x34, 0x38, 0xe1, 0x81, 0x17, 0xcf,
		0x24, 0xb9, 0x9a, 0xcb, 0x34, 0x51, 0x50, 0x59, 0x44, 0xb1, 0x0b, 0x50, 0x95, 0x6c, 0x48, 0x7e,
		0x14, 0xa4, 0xc6, 0xd9, 0xd3, 0xa5, 0xd6, 0xd0, 0xc5, 0x97, 0xf0, 0x45, 0xd0, 0x98, 0x51, 0x91,
		0x9f, 0xa3, 0x43, 0x51, 0x05, 0x90, 0xee, 0xca, 0x7e, 0x5f, 0x72, 0x53, 0xb1, 0xd3, 0xaf, 0x36,
		0x08, 0x75, 0xb0, 0x9b, 0xe0, 0x0d, 0x43, 0x88, 0xaa, 0x27, 0x44, 0x11
	},
	{ // region 2, $14c126
		0xf9, 0x19, 0xf3, 0x09, 0xa0, 0x09, 0xb0, 0x21, 0xb0, 0x22, 0xfd, 0x8e, 0xd3, 0xc8, 0x31, 0x67,
		0xc0, 0x10, 0x3c, 0xc2, 0x03, 0xf2, 0x6a, 0x0a, 0x54, 0x49, 0xca, 0xb5, 0x4b, 0xe0, 0x94, 0xe8,
		0x8d, 0xc8, 0x90, 0xee, 0x6b, 0x6f, 0xfa, 0x09, 0x76, 0x84, 0x6f, 0x55, 0xd1, 0x94, 0xca, 0x9c,
		0xe1, 0x22, 0xc6, 0x02, 0xb5, 0x8c, 0xf9, 0x3a, 0x52, 0x10, 0xf0, 0x22, 0xe4, 0x11, 0x15, 0x73,
		0x5e, 0x9e, 0xde, 0xc4, 0x5a, 0xbd, 0xa3, 0x89, 0xe7, 0x9b, 0x95, 0x5d, 0x75, 0xf6, 0xc3, 0x9f,
		0xe4, 0xcf, 0x65, 0x73, 0x90, 0xd0, 0x75, 0x56, 0xfa, 0xcc, 0xe4, 0x3e, 0x9c, 0x41, 0x81, 0x62,
		0xb1, 0xd3, 0x28, 0xbd, 0x6c, 0xed, 0x60, 0x28, 0x27, 0xee, 0xf2, 0xa1, 0xb4, 0x2c, 0x6c, 0xbb,
		0x42, 0xd7, 0x1d, 0x62, 0xc0, 0x33, 0x7d, 0xf9, 0xe4, 0x5c, 0xe2, 0x41, 0xa4, 0x1c, 0x98, 0xa1,
		0x87, 0x95, 0xad, 0x61, 0x56, 0x96, 0x40, 0x08, 0x6b, 0xe2, 0x4b, 0x95, 0x7b, 0x1b, 0xd8, 0x64,
		0xb3, 0xee, 0x9d, 0x79, 0x69, 0xea, 0x5d, 0xcf, 0x01, 0x91, 0xea, 0x3f, 0x70, 0x29, 0xdc, 0xe0,
		0x08, 0x20, 0xbf, 0x46, 0x90, 0xa8, 0xfc, 0x29, 0x14, 0xd1, 0x0d, 0x20, 0x79, 0xd2, 0x2c, 0xe9,
		0x52, 0xa6, 0x8c, 0xbd, 0xa3, 0x3e, 0x88, 0x2d, 0xb8, 0x4e, 0xf2, 0x74, 0x50, 0xcc, 0x12, 0xde,
		0xd3, 0x5a, 0xa4, 0x7b, 0xa2, 0x8d, 0x91, 0x68, 0x12, 0x0c, 0x9c, 0xb9, 0x6d, 0x26, 0x66, 0x60,
		0xc3, 0x6d, 0xd0, 0x11, 0x33, 0x05, 0x1d, 0xa8, 0xb6, 0x51, 0xe6, 0xe0, 0x58, 0x61, 0x74, 0x37,
		0xcc, 0x3a, 0x4d, 0x6a, 0x0a, 0x09, 0x71, 0xe3, 0x7e, 0xa5, 0x3b, 0xe9
	},
	{ // region 3, $14E5BE
		0x73, 0x59, 0xf3, 0x09, 0xa0, 0x09, 0xb1, 0x21, 0xb0, 0x55, 0x18, 0x0d, 0xe8, 0x29, 0x2d, 0x04,
		0x85, 0x39, 0x88, 0xbe, 0x8b, 0xcb, 0xd9, 0x0b, 0x32, 0x36, 0x94, 0xac, 0x74, 0xc3, 0x3b, 0x5d,
		0x2a, 0x83, 0x46, 0xb3, 0x3a, 0xac, 0xd8, 0x55, 0x68, 0x21, 0x57, 0xab, 0x6e, 0xd1, 0xd0, 0xfc,
		0xe2, 0xbe, 0x63, 0xd0, 0x6b, 0x79, 0x23, 0x40, 0x58, 0xd4, 0xe7, 0x73, 0x22, 0x67, 0x7f, 0x88,
		0x05, 0xbd, 0xdf, 0x7a, 0x65, 0x41, 0x90, 0x3a, 0x52, 0x83, 0x28, 0xae, 0xe9, 0x8e, 0x65, 0x82,
		0x0e, 0xdf, 0x98, 0x88, 0xe1, 0x86, 0x21, 0x3e, 0x1a, 0x87, 0x6d, 0x62, 0x7a, 0xf6, 0xaf, 0x2c,
		0xd5, 0xc5, 0x10, 0x2d, 0xa9, 0xda, 0x93, 0xa1, 0x9b, 0xc7, 0x35, 0xd4, 0x15, 0x78, 0x18, 0xd5,
		0x75, 0x6a, 0xd7, 0xdb, 0x12, 0x2a, 0x6a, 0xc8, 0x36, 0x53, 0x57, 0xa6, 0xf0, 0x13, 0x67, 0x43,
		0x79, 0xf0, 0x0e, 0x49, 0xb1, 0xec, 0xcd, 0xa4, 0x8a, 0x61, 0x06, 0xb9, 0xea, 0x53, 0xf2, 0x47,
		0x7d, 0xd6, 0xf8, 0x9d, 0x2e, 0xaa, 0x27, 0x35, 0x61, 0xce, 0x9b, 0x63, 0xbc, 0x07, 0x51, 0x5a,
		0xc2, 0x0d, 0x39, 0x42, 0xd2, 0x5e, 0x21, 0x20, 0x10, 0xa0, 0xe5, 0x08, 0xf7, 0x3d, 0x28, 0x04,
		0x99, 0x93, 0x97, 0xaf, 0xf9, 0x12, 0xc0, 0x01, 0x2d, 0xea, 0xf3, 0x98, 0x0b, 0x46, 0xc2, 0x26,
		0x93, 0x10, 0x69, 0x1d, 0x71, 0x8e, 0x33, 0x00, 0x5e, 0x80, 0x2f, 0x47, 0x0a, 0xcc, 0x94, 0x16,
		0xe7, 0x37, 0x45, 0xd0, 0x61, 0x79, 0x32, 0x86, 0x08, 0x2a, 0x5b, 0x55, 0xfe, 0xee, 0x52, 0x38,
		0xaa, 0x18, 0xe9, 0x39, 0x1a, 0x1e, 0xb8, 0x26, 0x6b, 0x3d, 0x4b, 0xa9
	},
	{ // region 4, $14d500
		0x06, 0x01, 0xf3, 0x39, 0xa0, 0x09, 0xa0, 0x21, 0xb0, 0x6f, 0x32, 0x8b, 0xfd, 0x89, 0x29, 0xa0,
		0x4a, 0x62, 0xed, 0xa1, 0x2d, 0xa4, 0x49, 0xf2, 0x10, 0x3c, 0x77, 0xa3, 0x84, 0x8d, 0xfa, 0xd1,
		0xc6, 0x57, 0xe2, 0x78, 0xef, 0xe9, 0xb6, 0xa1, 0x5a, 0xbd, 0x3f, 0x02, 0x0b, 0x28, 0xd6, 0x76,
		0xfc, 0x5b, 0x19, 0x9f, 0x21, 0x66, 0x4c, 0x2d, 0x45, 0x99, 0xde, 0xab, 0x46, 0xbd, 0xe9, 0x84,
		0xc4, 0xdc, 0xc7, 0x30, 0x70, 0xdd, 0x64, 0xea, 0xbc, 0x6b, 0xd3, 0xe6, 0x45, 0x3f, 0x07, 0x7e,
		0x50, 0xef, 0xb2, 0x84, 0x33, 0x3c, 0xcc, 0x3f, 0x39, 0x5b, 0xf5, 0x6d, 0x71, 0xc5, 0xdd, 0xf5,
		0xf9, 0xd0, 0xf7, 0x9c, 0xe6, 0xc7, 0xad, 0x1b, 0x29, 0xb9, 0x90, 0x08, 0x75, 0xc4, 0xc3, 0xef,
		0xa8, 0xfc, 0xab, 0x55, 0x7c, 0x21, 0x57, 0x97, 0x87, 0x4a, 0xcb, 0x0c, 0x56, 0x0a, 0x4f, 0xcb,
		0x52, 0x33, 0x87, 0x31, 0xf3, 0x43, 0x5b, 0x41, 0x90, 0xf8, 0xc0, 0xdd, 0x5a, 0xa4, 0x26, 0x2a,
		0x60, 0xa5, 0x6d, 0xda, 0xf2, 0x6a, 0xf0, 0xb3, 0xda, 0x25, 0x33, 0x87, 0x22, 0xe4, 0xac, 0xd3,
		0x96, 0xe0, 0x99, 0x3e, 0xfb, 0x14, 0x45, 0x17, 0x25, 0x56, 0xbe, 0xef, 0x8f, 0x8e, 0x3d, 0x1e,
		0xc7, 0x99, 0xa2, 0xa1, 0x50, 0xfe, 0xdf, 0xd4, 0xa1, 0x87, 0xf4, 0xd5, 0xde, 0xa6, 0x8c, 0x6d,
		0x6c, 0xde, 0x47, 0xbe, 0x59, 0x8f, 0xd4, 0x97, 0xc3, 0xf4, 0xda, 0xbb, 0xa6, 0x73, 0xa9, 0xcb,
		0xf2, 0x01, 0xb9, 0x90, 0x8f, 0xed, 0x60, 0x64, 0x40, 0x1c, 0xb6, 0xc9, 0xa5, 0x7c, 0x17, 0x52,
		0x6f, 0xdc, 0x6d, 0x08, 0x2a, 0x1a, 0xe6, 0x68, 0x3f, 0xd4, 0x42, 0x69
	},
	{ // region 5, $14bfb2
		0x7f, 0x41, 0xf3, 0x39, 0xa0, 0x09, 0xa1, 0x21, 0xb0, 0xa2, 0x4c, 0x23, 0x13, 0xe9, 0x25, 0x3d,
		0x0f, 0x72, 0x3a, 0x9d, 0xb5, 0x96, 0xd1, 0xda, 0x07, 0x29, 0x41, 0x9a, 0xad, 0x70, 0xba, 0x46,
		0x63, 0x2b, 0x7f, 0x3d, 0xbe, 0x40, 0xad, 0xd4, 0x4c, 0x73, 0x27, 0x58, 0xa7, 0x65, 0xdc, 0xd6,
		0xfd, 0xde, 0xb5, 0x6e, 0xd6, 0x6c, 0x75, 0x1a, 0x32, 0x45, 0xd5, 0xe3, 0x6a, 0x14, 0x6d, 0x80,
		0x84, 0x15, 0xaf, 0xcc, 0x7b, 0x61, 0x51, 0x82, 0x40, 0x53, 0x7f, 0x38, 0xa0, 0xd6, 0x8f, 0x61,
		0x79, 0x19, 0xe5, 0x99, 0x84, 0xd8, 0x78, 0x27, 0x3f, 0x16, 0x97, 0x78, 0x4f, 0x7b, 0x0c, 0xa6,
		0x37, 0xdb, 0xc6, 0x0c, 0x24, 0xb4, 0xc7, 0x94, 0x9d, 0x92, 0xd2, 0x3b, 0xd5, 0x11, 0x6f, 0x0a,
		0xdb, 0x76, 0x66, 0xe7, 0xcd, 0x18, 0x2b, 0x66, 0xd8, 0x41, 0x40, 0x58, 0xa2, 0x01, 0x1e, 0x6d,
		0x44, 0x75, 0xe7, 0x19, 0x4f, 0xb2, 0xe8, 0xc4, 0x96, 0x77, 0x62, 0x02, 0xc9, 0xdc, 0x59, 0xf3,
		0x43, 0x8d, 0xc8, 0xfe, 0x9e, 0x2a, 0xba, 0x32, 0x3b, 0x62, 0xe3, 0x92, 0x6e, 0xc2, 0x08, 0x4d,
		0x51, 0xcd, 0xf9, 0x3a, 0x3e, 0xc9, 0x50, 0x27, 0x21, 0x25, 0x97, 0xd7, 0x0e, 0xf8, 0x39, 0x38,
		0xf5, 0x86, 0x94, 0x93, 0xbf, 0xeb, 0x18, 0xa8, 0xfc, 0x24, 0xf5, 0xf9, 0x99, 0x20, 0x3d, 0xcd,
		0x2c, 0x94, 0x25, 0x79, 0x28, 0x77, 0x8f, 0x2f, 0x10, 0x69, 0x86, 0x30, 0x43, 0x01, 0xd7, 0x9a,
		0x17, 0xe3, 0x47, 0x37, 0xbd, 0x62, 0x75, 0x42, 0x78, 0xf4, 0x2b, 0x57, 0x4c, 0x0a, 0xdb, 0x53,
		0x4d, 0xa1, 0x0a, 0xd6, 0x3a, 0x16, 0x15, 0xaa, 0x2c, 0x6c, 0x39, 0x42
	},
	{ // region 6, $14cd82
		0x12, 0x09, 0xf3, 0x29, 0xa0, 0x09, 0xa0, 0x21, 0xb0, 0xd5, 0x66, 0xa1, 0x28, 0x4a, 0x21, 0xc0,
		0xd3, 0x9b, 0x86, 0x80, 0x57, 0x6f, 0x41, 0xc2, 0xe4, 0x2f, 0x0b, 0x91, 0xbd, 0x3a, 0x7a, 0xba,
		0x00, 0xe5, 0x35, 0x02, 0x74, 0x7d, 0x8b, 0x21, 0x57, 0x10, 0x0f, 0xae, 0x44, 0xbb, 0xe2, 0x37,
		0x18, 0x7b, 0x52, 0x3d, 0x8c, 0x59, 0x9e, 0x20, 0x1f, 0x0a, 0xcc, 0x1c, 0x8e, 0x6a, 0xd7, 0x95,
		0x2b, 0x34, 0xb0, 0x82, 0x6d, 0xfd, 0x25, 0x33, 0xaa, 0x3b, 0x2b, 0x70, 0x15, 0x87, 0x31, 0x5d,
		0xbb, 0x29, 0x19, 0x95, 0xd5, 0x8e, 0x24, 0x28, 0x5e, 0xd0, 0x20, 0x83, 0x46, 0x4a, 0x21, 0x70,
		0x5b, 0xcd, 0xae, 0x7b, 0x61, 0xa1, 0xfa, 0xf4, 0x2b, 0x84, 0x15, 0x6e, 0x36, 0x5d, 0x1b, 0x24,
		0x0f, 0x09, 0x3a, 0x61, 0x38, 0x0f, 0x18, 0x35, 0x11, 0x38, 0xb4, 0xbd, 0xee, 0xf7, 0xec, 0x0f,
		0x1d, 0xb7, 0x48, 0x01, 0xaa, 0x09, 0x8f, 0x61, 0xb5, 0x0f, 0x1d, 0x26, 0x39, 0x2e, 0x8c, 0xd6,
		0x26, 0x5c, 0x3d, 0x23, 0x63, 0xe9, 0x6b, 0x97, 0xb4, 0x9f, 0x7b, 0xb6, 0xba, 0xa0, 0x7c, 0xc6,
		0x25, 0xa1, 0x73, 0x36, 0x67, 0x7f, 0x74, 0x1e, 0x1d, 0xda, 0x70, 0xbf, 0xa5, 0x63, 0x35, 0x39,
		0x24, 0x8c, 0x9f, 0x85, 0x16, 0xd8, 0x50, 0x95, 0x71, 0xc0, 0xf6, 0x1e, 0x6d, 0x80, 0xed, 0x15,
		0xeb, 0x63, 0xe9, 0x1b, 0xf6, 0x78, 0x31, 0xc6, 0x5c, 0xdd, 0x19, 0xbd, 0xdf, 0xa7, 0xec, 0x50,
		0x22, 0xad, 0xbb, 0xf6, 0xeb, 0xd6, 0xa3, 0x20, 0xc9, 0xe6, 0x9f, 0xcb, 0xf2, 0x97, 0xb9, 0x54,
		0x12, 0x66, 0xa6, 0xbe, 0x4a, 0x12, 0x43, 0xec, 0x00, 0xea, 0x49, 0x02
	},
	{ // region 7, $14ce76
		0xa4, 0x49, 0xf3, 0x29, 0xa0, 0x09, 0xa1, 0x21, 0xb0, 0xef, 0x80, 0x20, 0x3d, 0xaa, 0x36, 0x5d,
		0x98, 0xc4, 0xd2, 0x63, 0xdf, 0x61, 0xb0, 0xc3, 0xc2, 0x35, 0xd4, 0x88, 0xe6, 0x1d, 0x3a, 0x2f,
		0x9c, 0xb9, 0xd1, 0xc6, 0x43, 0xba, 0x69, 0x6d, 0x49, 0xac, 0xdd, 0x05, 0xe0, 0xf8, 0xe8, 0x97,
		0x19, 0x18, 0x08, 0x0c, 0x42, 0x46, 0xc7, 0x0d, 0x25, 0xce, 0xc3, 0x54, 0xb2, 0xd9, 0x42, 0x91,
		0xea, 0x53, 0x98, 0x38, 0x78, 0x81, 0x12, 0xca, 0x15, 0x23, 0xbd, 0xc1, 0x70, 0x1f, 0xd2, 0x40,
		0xfd, 0x39, 0x33, 0xaa, 0x27, 0x2b, 0xe8, 0x10, 0x7d, 0xa4, 0xa8, 0x8e, 0x3d, 0x00, 0x4f, 0x3a,
		0x7f, 0xd8, 0x96, 0xea, 0x9e, 0x8e, 0x15, 0x6e, 0x9f, 0x76, 0x57, 0xba, 0x7d, 0xc2, 0xdf, 0x57,
		0x42, 0x82, 0xf4, 0xda, 0x89, 0x06, 0x05, 0x04, 0x62, 0x2f, 0x29, 0x23, 0x54, 0xd5, 0xbb, 0x97,
		0xf5, 0xf9, 0xc1, 0xcf, 0xec, 0x5f, 0x1d, 0xfd, 0xbb, 0xa6, 0xd7, 0x4a, 0xa8, 0x66, 0xbf, 0xb9,
		0x09, 0x44, 0xb1, 0x60, 0x28, 0xa9, 0x35, 0x16, 0x15, 0xf5, 0x13, 0xc1, 0x07, 0x7e, 0xd7, 0x40,
		0xdf, 0x8e, 0xd3, 0x32, 0xa9, 0x35, 0x98, 0x15, 0x32, 0xa9, 0x49, 0xc0, 0x24, 0xb4, 0x4a, 0x53,
		0x6b, 0x79, 0xaa, 0x77, 0x6c, 0xc5, 0x88, 0x69, 0xe5, 0x5d, 0xde, 0x42, 0x28, 0xf9, 0xb7, 0x5c,
		0xab, 0x19, 0xc7, 0xbc, 0xc5, 0x60, 0xeb, 0x5e, 0xa8, 0x52, 0xc4, 0x32, 0x7c, 0x35, 0x02, 0x06,
		0x46, 0x77, 0x30, 0xb6, 0x33, 0x4b, 0xb8, 0xfd, 0x02, 0xd8, 0x14, 0x40, 0x99, 0x25, 0x7e, 0x55,
		0xd6, 0x44, 0x43, 0x8d, 0x73, 0x0e, 0x71, 0x48, 0xd3, 0x82, 0x40, 0xda
	}
};

void pgm_022_025_state::machine_reset_killbld()
{
	int region = (ioport(":Region")->read()) & 0xff;

	m_igs025->m_kb_region = region - 0x16;
	m_igs025->m_kb_game_id = 0x89911400 | region;

	machine_reset_pgm();
}

void pgm_022_025_state::machine_reset_dw3()
{
	int region = (ioport(":Region")->read()) & 0xff;

	m_igs025->m_kb_region = region;
	m_igs025->m_kb_game_id = 0x00060000 | region;

	machine_reset_pgm();
}




void pgm_022_025_state::igs025_to_igs022_callback( void )
{
//  printf("igs025_to_igs022_callback\n");
	m_igs022->IGS022_handle_command();
}



void pgm_022_025_state::init_killbld()
{
	pgm_basic_init();
	pgm_killbld_decrypt();

	// install and configure protection device(s)
	m_maincpu->space(AS_PROGRAM).install_readwrite_handler(0xd40000, 0xd40003, read16_delegate(FUNC(igs025_device::killbld_igs025_prot_r), (igs025_device*)m_igs025), write16_delegate(FUNC(igs025_device::killbld_igs025_prot_w), (igs025_device*)m_igs025));
	m_igs022->m_sharedprotram = m_sharedprotram;
	m_igs025->m_kb_source_data = killbld_source_data;
}

void pgm_022_025_state::init_drgw3()
{
	pgm_basic_init();
	pgm_dw3_decrypt();

	// install and configure protection device(s)
	m_maincpu->space(AS_PROGRAM).install_readwrite_handler(0xda5610, 0xda5613, read16_delegate(FUNC(igs025_device::killbld_igs025_prot_r), (igs025_device*)m_igs025), write16_delegate(FUNC(igs025_device::killbld_igs025_prot_w), (igs025_device*)m_igs025));
	m_igs022->m_sharedprotram = m_sharedprotram;
	m_igs025->m_kb_source_data = dw3_source_data;
}


static ADDRESS_MAP_START( killbld_mem, AS_PROGRAM, 16, pgm_022_025_state )
	AM_IMPORT_FROM(pgm_mem)
	AM_RANGE(0x100000, 0x2fffff) AM_ROMBANK("bank1") /* Game ROM */
	AM_RANGE(0x300000, 0x303fff) AM_RAM AM_SHARE("sharedprotram") // Shared with protection device
ADDRESS_MAP_END


MACHINE_CONFIG_START( pgm_022_025, pgm_022_025_state )
	MCFG_FRAGMENT_ADD(pgmbase)

	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(killbld_mem)

	MCFG_DEVICE_ADD("igs025", IGS025, 0)
	MCFG_IGS025_SET_EXTERNAL_EXECUTE( pgm_022_025_state, igs025_to_igs022_callback )

	MCFG_DEVICE_ADD("igs022", IGS022, 0)

MACHINE_CONFIG_END

MACHINE_CONFIG_DERIVED(pgm_022_025_dw3, pgm_022_025)
	MCFG_MACHINE_RESET_OVERRIDE(pgm_022_025_state, dw3)
MACHINE_CONFIG_END

MACHINE_CONFIG_DERIVED(pgm_022_025_killbld, pgm_022_025)
	MCFG_MACHINE_RESET_OVERRIDE(pgm_022_025_state, killbld)
MACHINE_CONFIG_END


INPUT_PORTS_START( killbld )
	PORT_INCLUDE ( pgm )

	PORT_MODIFY("Region")   /* Region - supplied by protection device */
	PORT_DIPNAME( 0x00ff, 0x0021, "Region" )
	PORT_DIPSETTING(      0x0016, DEF_STR( Taiwan ) )
	PORT_DIPSETTING(      0x0017, DEF_STR( China ) )
	PORT_DIPSETTING(      0x0018, DEF_STR( Hong_Kong ) )
	PORT_DIPSETTING(      0x0019, DEF_STR( Japan ) )
//  PORT_DIPSETTING(      0x001a, "1a" ) // invalid
//  PORT_DIPSETTING(      0x001b, "1b" ) // invalid
//  PORT_DIPSETTING(      0x001c, "1c" ) // invalid
//  PORT_DIPSETTING(      0x001d, "1d" ) // invalid
//  PORT_DIPSETTING(      0x001e, "1e" ) // invalid
//  PORT_DIPSETTING(      0x001f, "1f" ) // invalid
	PORT_DIPSETTING(      0x0020, DEF_STR( Korea ) )
	PORT_DIPSETTING(      0x0021, DEF_STR( World ) )
INPUT_PORTS_END

INPUT_PORTS_START( dw3 )
	PORT_INCLUDE ( pgm )

	PORT_MODIFY("Region")   /* Region - supplied by protection device */
	PORT_CONFNAME( 0x000f, 0x0006, DEF_STR( Region ) )
//  PORT_CONFSETTING(      0x0000, "0" )
	PORT_CONFSETTING(      0x0001, DEF_STR( Japan ) )
	PORT_CONFSETTING(      0x0002, DEF_STR( Korea ))
	PORT_CONFSETTING(      0x0003, DEF_STR( Taiwan ) )
	PORT_CONFSETTING(      0x0004, DEF_STR( Hong_Kong ) ) // typo Hokg Kong
	PORT_CONFSETTING(      0x0005, DEF_STR( China ) )
	PORT_CONFSETTING(      0x0006, DEF_STR( World ) )
	PORT_CONFSETTING(      0x0007, "Singapore" )

INPUT_PORTS_END


INPUT_PORTS_START( dw3j ) // for dw3100 set
	PORT_INCLUDE ( pgm )

	PORT_MODIFY("Region")   /* Region - supplied by protection device */
	PORT_CONFNAME( 0x000f, 0x0001, DEF_STR( Region ) )
//  PORT_CONFSETTING(      0x0000, "0" )
	PORT_CONFSETTING(      0x0001, DEF_STR( Japan ) )
//  PORT_CONFSETTING(      0x0002, DEF_STR( Korea ))
//  PORT_CONFSETTING(      0x0003, DEF_STR( Taiwan ) )
//  PORT_CONFSETTING(      0x0004, DEF_STR( Hong_Kong ) ) // typo Hokg Kong
//  PORT_CONFSETTING(      0x0005, DEF_STR( China ) )
//  PORT_CONFSETTING(      0x0006, DEF_STR( World ) )
//  PORT_CONFSETTING(      0x0007, "Singapore" )

INPUT_PORTS_END
