/******************************************************************************
 * WaveformData.c
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 * Author: a0866685
 *
 * Desc: This file contains waveform data for all three DRV2667 modes.
 *
 * Last Updated: 2/17/2013
 ******************************************************************************/

#include "WaveformData.h"
#include "DRV2667.h"

/******************************************************************************
 * RAM Header
 * 1) First Byte - Register to begin loading data
 * 		(Used for I2C function, not sent to or stored in DRV2667)
 * 2) Second Byte - Pointer to last byte in header ([Header size] - 1),
 * 3) Remaining Bytes - Waveform Header Format
 * 		- Byte 1: Start address of waveform, Upper Byte
 * 		- Byte 2: Start address of waveform, Lower Byte
 * 		- Byte 3: Stop address of waveform, Upper Byte
 * 		- Byte 4: Stop address of waveform, Lower Byte
 * 		- Byte 5: Repeat Count
 ******************************************************************************/
const unsigned char DRV2667_HEADER[] = {
REG_0,				  	  // Begin loading data at DRV2667 register zero
0xC2,                     // Point to last byte in header (Header size - 1)

// Begin Waveforms
0x81,0x00,0x1,0x03,0x1,   // Waveform #1 Click150
0x81,0x04,0x1,0x07,0x1,   // Waveform #2 Click200
0x81,0x08,0x1,0x0B,0x1,   // Waveform #3 Click250
0x81,0x0C,0x1,0x0F,0x1,   // Waveform #4 Click300
0x81,0x10,0x1,0x13,0x1,   // Waveform #5 Bounce100
0x81,0x14,0x1,0x17,0x1,   // Waveform #6 Bounce150
0x81,0x18,0x1,0x1B,0x1,   // Waveform #7 Bounce200
0x81,0x1C,0x1,0x1F,0x1,   // Waveform #8 Bounce250
0x81,0x20,0x1,0x23,0x1,   // Waveform #9 Trans100
0x81,0x24,0x1,0x27,0x1,   // Waveform #A Trans150
0x81,0x28,0x1,0x2B,0x1,   // Waveform #B Trans200
0x81,0x2C,0x1,0x2F,0x1,   // Waveform #C Trans250
0x81,0x30,0x1,0x33,0x1,   // Waveform #D Delay50ms
0x81,0x34,0x1,0x37,0x1,   // Waveform #E Delay100ms
0x81,0x38,0x1,0x3B,0x1,   // Waveform #F Delay250ms
0x81,0x3C,0x1,0x3F,0x1,   // Waveform #10 Delay500ms
0x81,0x40,0x1,0x47,0x1,   // Waveform #11 Alert1
0x81,0x48,0x1,0x57,0x1,   // Waveform #12 Alert2
0x81,0x58,0x1,0x7F,0x1,   // Waveform #13 Alert3
0x81,0x80,0x1,0x83,0x1,   // Waveform #14 Alert4
0x02,0x00,0x3,0x00,0x2,   // Waveform #15 Two-Tone
0x81,0x84,0x1,0x87,0x1,	  // Waveform #16 1kHz Fade
0x81,0x88,0x1,0x8B,0x1,	  // Waveform #17 Piano F5
0x81,0x8C,0x1,0x8F,0x1,	  // Waveform #18 Piano G5
0x81,0x90,0x1,0x93,0x1,   // Waveform #19 Piano A5
0x81,0x94,0x1,0x97,0x1,	  // Waveform #1A Piano A#5
0x81,0x98,0x1,0x9B,0x1,	  // Waveform #1B Piano B5
0x81,0x9C,0x1,0x9F,0x1,	  // Waveform #1C Piano C6
0x81,0xA0,0x1,0xA3,0x1,   // Waveform #1D Piano D6
0x81,0xA4,0x1,0xA7,0x1,	  // Waveform #1E Piano	E6
0x81,0xA8,0x1,0xAB,0x1,	  // Waveform #1F Piano F6
0x81,0xAC,0x1,0xAF,0x1,	  // Waveform #20 Piano G6
0x81,0xB0,0x1,0xB3,0x1,   // Waveform #21 Piano A6
0x81,0xB4,0x1,0xB7,0x1,	  // Waveform #22 Piano A#6
0x81,0xB8,0x1,0xBB,0x1,	  // Waveform #23 Piano B6
0x81,0xBC,0x1,0xBF,0x1,	  // Waveform #24 2000kHz
0x81,0xC0,0x1,0xC3,0x0,	  // Waveform #25 Dynamic Waveform (Infinite)
0x81,0xC4,0x1,0xC7,0x1,	  // Waveform #26 Dynamic Waveform (Single)	(Header Byte = 0xBD)
0x81,0xC4,0x1,0xCB,0x0	  // Waveform #27 Dynamic Waveform Wait 1s (Header Byte = 0xC2)
};

const unsigned char temp = 0xFF * VPEAK / 100;	// Calculation used for non-standard voltages

/******************************************************************************
 * Waveform Data
 * 1) First Byte - Register to begin loading data
 * 		(Used for I2C function, not sent to or stored in DRV2667)
 * 2) Remaining Bytes - Waveform Data Format (See datasheet for more details)
 * 		Byte 1: Amplitude
 * 		Byte 2: Frequency
 * 		Byte 3: Duration
 * 		Byte 4: Rise/Fall Time
 ******************************************************************************/
const unsigned char DRV2667_DATA[] = {
REG_0,		// Begin loading data at DRV2667 register zero

// Begin Waveforms
(0xFF * VPEAK / 100),0x13,0x6,0x0,      // Click150
(0x80 * VPEAK / 100),0x1A,0x6,0x0,      // Click200
(0xC0 * VPEAK / 100),0x20,0x6,0x0,      // Click250
(0xFF * VPEAK / 100),0x26,0x4,0x0,      // Click300
(0xFF * VPEAK / 100),0xD,0x1,0xB,       // Bounce100
(0xFF * VPEAK / 100),0x13,0x1,0xA,      // Bounce150
(0x80 * VPEAK / 100),0x1A,0x1,0x9,      // Bounce200
(0xC0 * VPEAK / 100),0x20,0x1,0x9,      // Bounce250
(0xFF * VPEAK / 100),0xD,0x3C,0x90,     // Trans100
(0xFF * VPEAK / 100),0x13,0x55,0x90,    // Trans150
(0x80 * VPEAK / 100),0x1A,0x3C,0x80,    // Trans200
(0xFF * VPEAK / 100),0x20,0x4B,0x80,    // Trans250
0x00,0x0D,0x05,0x00,    				// Delay50ms
0x00,0x0D,0x0A,0x00,    				// Delay100ms
0x00,0x0D,0x19,0x00,    				// Delay250ms
0x00,0x0D,0x32,0x00,    				// Delay500ms
// Alert1
(0x80 * VPEAK / 100),0x1A,0x0B,0x00,    // Buzz200
0x00,0x0D,0x0A,0x00,    				// Delay100ms
// Alert2
(0x70 * VPEAK / 100),0x1A,0x3C,0x80,    // Trans200
0x00,0x0D,0x05,0x00,    				// Delay50ms
(0x80 * VPEAK / 100),0x1A,0x0B,0x00,    // Buzz200
0x00,0x0D,0x0A,0x00,					// Delay100ms
// Alert3
(0xFF * VPEAK / 100),0x13,0x6,0x0,      // Click150
0x00,0x0D,0x05,0x00,					// Delay50ms
(0x80 * VPEAK / 100),0x1A,0x6,0x0,     	// Click200
0x00,0x0D,0x05,0x00,    				// Delay50ms
(0xC0 * VPEAK / 100),0x20,0x6,0x0,      // Click250
0x00,0x0D,0x05,0x00,    				// Delay50ms
(0xFF * VPEAK / 100),0x26,0x4,0x0,      // Click300
0x00,0x0D,0x05,0x00,    				// Delay50ms
(0xC0 * VPEAK / 100),0x20,0x1,0x8,      // Bounce250
0x00,0x0D,0x0A,0x00,    				// Delay100ms
// Alert4
(0xC0 * VPEAK / 100),0x16,0x32,0x00,	// Buzz175	 (Array Size = 133)
// Ding (Audio)
(0xFF * VPEAK / 100),0x80,0x32,0x09,	// Audio 1K
(0xFF * VPEAK / 100),0x59,0x32,0x08,	// Audio 698  Hz (F5)
(0xFF * VPEAK / 100),0x64,0x32,0x08,	// Audio 784  Hz (G5)
(0xFF * VPEAK / 100),0x71,0x32,0x08,	// Audio 880  Hz (A5)
(0xFF * VPEAK / 100),0x77,0x32,0x08,	// Audio 932  Hz (A#5)
(0xFF * VPEAK / 100),0x7F,0x32,0x08,	// Audio 987  Hz (B5)
(0xFF * VPEAK / 100),0x86,0x32,0x08,	// Audio 1046 Hz (C6)
(0xFF * VPEAK / 100),0x96,0x32,0x09,	// Audio 1174 Hz (D6)
(0xFF * VPEAK / 100),0xA9,0x32,0x08,	// Audio 1318 Hz (E6)
(0xFF * VPEAK / 100),0xB3,0x32,0x08,	// Audio 1396 Hz (F6)
(0xFF * VPEAK / 100),0xC9,0x32,0x08,    // Audio 1567 Hz (G6)
(0xFF * VPEAK / 100),0xE1,0x32,0x08,    // Audio 1760 Hz (A6)
(0xFF * VPEAK / 100),0xEF,0x32,0x08,    // Audio 1864 Hz (A#6)
(0xFF * VPEAK / 100),0xFD,0x32,0x08,    // Audio 1975 Hz (B6)
(0xFF * VPEAK / 100),0xFF,0x32,0x09,    // Audio 2000 Hz
(DYNAMIC_AMP * VPEAK / 100),DYNAMIC_FREQ,DYNAMIC_DURATION,0x00,	// Dynamic Waveform Infinite (Array Size = 197)
(DYNAMIC_AMP * VPEAK / 100),DYNAMIC_FREQ,DYNAMIC_DURATION,0x00,	// Dynamic Waveform	Finite (Array Size = 201)
0x00,0x01,0x08,0x00	    				// Delay1s (Array Size = 205)
};


// Sinusoidal Data for Mode 1 - 150V (100%)
const signed char sinewaveform[] = {
0x00,0x03,0x07,0x0A,0x0D,0x11,0x14,0x17,0x1B,0x1E,0x21,0x24,
0x27,0x2B,0x2E,0x31,0x34,0x37,0x3A,0x3D,0x40,0x43,0x45,0x48,
0x4B,0x4E,0x50,0x53,0x55,0x58,0x5A,0x5C,0x5F,0x61,0x63,0x65,
0x67,0x69,0x6B,0x6D,0x6E,0x70,0x72,0x73,0x74,0x76,0x77,0x78,
0x79,0x7A,0x7B,0x7C,0x7D,0x7D,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,
0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7D,0x7D,0x7C,0x7B,0x7A,
0x79,0x78,0x77,0x76,0x74,0x73,0x72,0x70,0x6E,0x6D,0x6B,0x69,
0x67,0x65,0x63,0x61,0x5F,0x5C,0x5A,0x58,0x55,0x53,0x50,0x4E,
0x4B,0x48,0x45,0x43,0x40,0x3D,0x3A,0x37,0x34,0x31,0x2E,0x2B,
0x27,0x24,0x21,0x1E,0x1B,0x17,0x14,0x11,0x0D,0x0A,0x07,0x03,
0x00,0xFD,0xF9,0xF6,0xF3,0xEF,0xEC,0xE9,0xE5,0xE2,0xDF,0xDC,
0xD9,0xD5,0xD2,0xCF,0xCC,0xC9,0xC6,0xC3,0xC0,0xBD,0xBB,0xB8,
0xB5,0xB2,0xB0,0xAD,0xAB,0xA8,0xA6,0xA4,0xA1,0x9F,0x9D,0x9B,
0x99,0x97,0x95,0x93,0x92,0x90,0x8E,0x8D,0x8C,0x8A,0x89,0x88,
0x87,0x86,0x85,0x84,0x83,0x83,0x82,0x82,0x81,0x81,0x81,0x81,
0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x83,0x83,0x84,0x85,0x86,
0x87,0x88,0x89,0x8A,0x8C,0x8D,0x8E,0x90,0x92,0x93,0x95,0x97,
0x99,0x9B,0x9D,0x9F,0xA1,0xA4,0xA6,0xA8,0xAB,0xAD,0xB0,0xB2,
0xB5,0xB8,0xBB,0xBD,0xC0,0xC3,0xC6,0xC9,0xCC,0xCF,0xD2,0xD5,
0xD9,0xDC,0xDF,0xE2,0xE5,0xE9,0xEC,0xEF,0xF3,0xF6,0xF9,0xFD};

// Two-Tone Waveform for Mode 2
const unsigned char TWOTONE[] = {
REG_0,
0x00,0xEC,0xD8,0xC5,0xB4,0xA5,0x99,0x90,0x8A,0x87,
0x88,0x8C,0x92,0x9C,0xA8,0xB5,0xC5,0xD5,0xE5,0xF5,
0x05,0x13,0x20,0x2B,0x34,0x3A,0x3F,0x41,0x40,0x3E,
0x3A,0x34,0x2E,0x26,0x1F,0x17,0x10,0x0A,0x05,0x01,
0xFF,0xFE,0xFF,0x01,0x04,0x09,0x0E,0x13,0x19,0x1E,
0x22,0x25,0x27,0x27,0x26,0x22,0x1D,0x15,0x0C,0x01,
0xF5,0xE8,0xDA,0xCD,0xC0,0xB4,0xA9,0xA0,0x9A,0x96,
0x95,0x97,0x9C,0xA4,0xAF,0xBC,0xCC,0xDE,0xF1,0x05,
0x19,0x2C,0x3F,0x50,0x5F,0x6C,0x76,0x7C,0x7F,0x7F,
0x7C,0x75,0x6B,0x5F,0x50,0x3F,0x2E,0x1B,0x08,0xF6,
0xE5,0xD5,0xC8,0xBC,0xB3,0xAD,0xA9,0xA8,0xAA,0xAE,
0xB4,0xBB,0xC5,0xCF,0xD9,0xE3,0xED,0xF7,0xFF,0x06,
0x0B,0x0E,0x10,0x10,0x0F,0x0D,0x09,0x05,0x00,0xFB,
0xF7,0xF3,0xF1,0xF0,0xF0,0xF2,0xF5,0xFA,0x01,0x09,
0x13,0x1D,0x27,0x31,0x3B,0x45,0x4C,0x52,0x56,0x58,
0x57,0x53,0x4D,0x44,0x38,0x2B,0x1B,0x0A,0xF8,0xE5,
0xD2,0xC1,0xB0,0xA1,0x95,0x8B,0x84,0x81,0x80,0x84,
0x8A,0x94,0xA1,0xB0,0xC1,0xD4,0xE7,0xFB,0x0F,0x22,
0x34,0x44,0x51,0x5C,0x64,0x69,0x6B,0x6A,0x66,0x60,
0x57,0x4C,0x40,0x33,0x26,0x18,0x0B,0xFF,0xF4,0xEB,
0xE3,0xDE,0xDA,0xD9,0xD9,0xDB,0xDE,0xE2,0xE7,0xED,
0xF2,0xF7,0xFC,0xFF,0x01,0x02,0x01,0xFF,0xFB,0xF6,
0xF0,0xE9,0xE1,0xDA,0xD2,0xCC,0xC6,0xC2,0xC0,0xBF,
0xC1,0xC6,0xCC,0xD5,0xE0,0xED,0xFB,0x0B,0x1B,0x2B,
0x3B,0x4B,0x58,0x64,0x6E,0x74,0x78,0x79,0x76,0x70,
0x67,0x5B,0x4C,0x3B,0x28,0x14,0x00};