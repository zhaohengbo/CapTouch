/******************************************************************************
 * WaveformData.h
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 * Author: a0866685
 *
 * Desc: This file contains the data for use with the Actuator_Waveforms.h
 *
 * Last Updated: 2/17/2013
 ******************************************************************************/

#include "Haptics.h"

/* Array Size Constants */
#define SINEWAVEFORM_SIZE 	240					// Number of points in the sinewaveform
#define DRV2667_HEADERSIZE 	197					// Number of bytes in "DRV2667_HEADER[]"
#define DRV2667_DATASIZE 	205					// Number of bytes in "DRV2667_DATA[]"
#define TWOTONE_SIZE 		258					// Number of bytes in TWOTONE[]

#define DYNAMIC_FREQ		0x20				// Dynamic waveform default frequency
#define DYNAMIC_AMP			0xC0				// Dynamic waveform default amplitude
#define DYNAMIC_DURATION	0x01				// Dynamic waveform default duration
#define DYNAMIC_RAMPUP		0x00				// Dynamic waveform default rampup
#define DYNAMIC_RAMPDOWN	0x00				// Dynamic waveform default rampdown

/* Voltage Calculation Constants */
#define VPEAK_CALC			(100 / VPEAK)		// Constant for calculating the maximum output voltage


/* Header Info */
extern const unsigned char DRV2667_HEADER[];	// DRV2667 RAM Header Information

/* Mode 1 - Real-time Playback */
extern const signed char sinewaveform[];		// Sine wave look-up table for MODE1 generation

/* Mode 2 - RAM Mode */
extern const unsigned char TWOTONE[];			// Two-tone look-up table for MODE2

/* Mode 3 - Waveform Synthesizer / Generator */
extern const unsigned char DRV2667_DATA[];		// DRV2667 RAM Waveform Data




