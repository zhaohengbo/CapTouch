/******************************************************************************
 * Actuator_Waveforms.h
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 * Author: a0866685
 *
 * Desc: This file contains the waveform data arrays.  See instructions below
 * 			to create new waveforms.
 *
 *
 ******************************************************************************
 * INSTRUCTIONS FOR CREATING A HAPTIC WAVEFORM
 * ****************************************************************************
 * 1. Create and array of data point pairs and label it
 * 		"effect_data". Where "effect" is the name of the
 * 		waveform. 
 *
 *		In Mode2, Mode3 and analog mode, the first value in the 
 *		pair is the register and the second is the register value.
 * 		waveform[] = {
 * 			register,value,
 * 			register,value,
 * 			...
 * 		};
 *		
 *		In Mode1, the first value is the frequency and the second is 
 *		the duration.
 *		waveform[] = {
 * 			frequency, duration
 * 		};
 *
 * 2. Create a Waveform struct below and call it "effect".
 *
 * 3. The Waveform structure should be set to:
 * 		Waveform effect = {
 * 			mode, 	//
 * 			length, // size in bytes of "effect_data"
 * 			data	// name of the data array "effect_data"
 * 		};
 *
 *	PIEZO_MODE1 - Streaming mode
 *	PIEZO_MODE2 - RAM mode
 *	PIEZO_MODE3 - Waveform generator mode
 *  PIEZO_ANALOGMODE50 	- analog mode with 50Vpp output
 *  PIEZO_ANALOGMODE100 - analog mode with 100Vpp output
 *	PIEZO_ANALOGMODE150 - analog mode with 150Vpp output
 *  PIEZO_ANALOGMODE200 - analog mode with 200Vpp output
 ******************************************************************************/

#include "Actuator_Waveforms.h"

//--------------------------------------------------------//
// MODE1 - Streaming Waveform Effects
//--------------------------------------------------------//
static const unsigned char mode1_bump_data[] = {
		MODE1_FREQ100, 20		// frequency, duration
};
const Waveform mode1_bump = {PIEZO_MODE1,4,mode1_bump_data};

static const unsigned char mode1_click_data[] = {
		MODE1_FREQ166, 6
};
const Waveform mode1_click = {PIEZO_MODE1,4,mode1_click_data};

static const unsigned char mode1_tick_data[] = {
		MODE1_FREQ266, 8
};
const Waveform mode1_tick = {PIEZO_MODE1,4,mode1_tick_data};

static const unsigned char mode1_alert_data[] = {
		MODE1_FREQ166, 150
};
const Waveform mode1_alert = {PIEZO_MODE1,4,mode1_alert_data};

//--------------------------------------------------------//
// MODE2 - RAM Waveform Effects
//--------------------------------------------------------//
static const unsigned char mode2_twotone_data[] = {
		DRV2667_WAVSEQ0, 0x15,   // Sequence Register 0 = Waveform 21
		DRV2667_WAVSEQ1, 0x00    // Sequence Register 1 = Waveform 0
};
const Waveform mode2_twotone = {PIEZO_MODE2,4,mode2_twotone_data};


//--------------------------------------------------------//
// Mode 3 - Waveform Generator Effects
//--------------------------------------------------------//

// Dynamic Waveform - changes based on button input
static const unsigned char dynamicWaveform_data[] = {
		DRV2667_WAVSEQ0, 0x25,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform dynamicWaveform = {PIEZO_MODE3,4,dynamicWaveform_data};

// Dynamic Waveform (no repeat) - changes based on button input
static const unsigned char dynamicWaveformNoRepeat_data[] = {
		DRV2667_WAVSEQ0, 0x26,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform dynamicWaveformNoRepeat = {PIEZO_MODE3,4,dynamicWaveformNoRepeat_data};

// Dynamic Waveform (no repeat) - changes based on button input
static const unsigned char dynamicWaveformNoRepeatWait1s_data[] = {
		DRV2667_WAVSEQ0, 0x27,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform dynamicWaveformNoRepeatWait1s = {PIEZO_MODE3,4,dynamicWaveformNoRepeatWait1s_data};

/**
 * Audio
 */
static const unsigned char ding_data[] = {
		DRV2667_WAVSEQ0, 0x16, 	// Sequence Register 0 = Waveform 16
		DRV2667_WAVSEQ1, 0x00	    // Sequence Register 1 = Waveform 0
};
const Waveform ding1k = {PIEZO_MODE3,4,ding_data};

static const unsigned char pianoF5_data[] = {
		DRV2667_WAVSEQ0, 0x17,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoF5 = {PIEZO_MODE3,4,pianoF5_data};

static const unsigned char pianoG5_data[] = {
		DRV2667_WAVSEQ0, 0x18,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoG5 = {PIEZO_MODE3,4,pianoG5_data};

static const unsigned char pianoA5_data[] = {
		DRV2667_WAVSEQ0, 0x19,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoA5 = {PIEZO_MODE3,4,pianoA5_data};

static const unsigned char pianoAsharp5_data[] = {
		DRV2667_WAVSEQ0, 0x1A,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoAsharp5 = {PIEZO_MODE3,4,pianoAsharp5_data};

static const unsigned char pianoB5_data[] = {
		DRV2667_WAVSEQ0, 0x1B,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoB5 = {PIEZO_MODE3,4,pianoB5_data};

static const unsigned char pianoC6_data[] = {
		DRV2667_WAVSEQ0, 0x1C,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoC6 = {PIEZO_MODE3,4,pianoC6_data};

static const unsigned char pianoD6_data[] = {
		DRV2667_WAVSEQ0, 0x1D,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoD6 = {PIEZO_MODE3,4,pianoD6_data};

static const unsigned char pianoE6_data[] = {
		DRV2667_WAVSEQ0, 0x1E,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoE6 = {PIEZO_MODE3,4,pianoE6_data};

static const unsigned char pianoF6_data[] = {
		DRV2667_WAVSEQ0, 0x1F,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoF6 = {PIEZO_MODE3,4,pianoF6_data};

static const unsigned char pianoG6_data[] = {
		DRV2667_WAVSEQ0, 0x20,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoG6 = {PIEZO_MODE3,4,pianoG6_data};


static const unsigned char pianoA6_data[] = {
		DRV2667_WAVSEQ0, 0x21,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoA6 = {PIEZO_MODE3,4,pianoA6_data};

static const unsigned char pianoAsharp6_data[] = {
		DRV2667_WAVSEQ0, 0x22,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoAsharp6 = {PIEZO_MODE3,4,pianoAsharp6_data};

static const unsigned char pianoB6_data[] = {
		DRV2667_WAVSEQ0, 0x23,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform pianoB6 = {PIEZO_MODE3,4,pianoB6_data};


/**
 * Clicks
 */
static const unsigned char click150_data[] = {
		DRV2667_WAVSEQ0, 0x01, 	// Sequence Register 0 = Waveform 1
		DRV2667_WAVSEQ1, 0x00	// Sequence Register 1 = Waveform 0
};
const Waveform click150 = {PIEZO_MODE3,4,click150_data};

static const unsigned char click200_data[] = {
		DRV2667_WAVSEQ0, 0x02,  // Sequence Register 0 = Waveform 2
		DRV2667_WAVSEQ1, 0x00   // Sequence Register 1 = Waveform 0
};
const Waveform click200 = {PIEZO_MODE3,4,click200_data};

static const unsigned char click250_data[] = {
		DRV2667_WAVSEQ0, 0x03,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform click250 = {PIEZO_MODE3,4,click250_data};

static const unsigned char click300_data[] = {
		DRV2667_WAVSEQ0, 0x04,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform click300 = {PIEZO_MODE3,4,click300_data};


/**
 * Bounces (soft clicks)
 */
static const unsigned char bounce100_data[] = {
    DRV2667_WAVSEQ0, 0x05,
    DRV2667_WAVSEQ1, 0x00
};
const Waveform bounce100 = {PIEZO_MODE3,4,bounce100_data};

static const unsigned char bounce150_data[] = {
		DRV2667_WAVSEQ0, 0x06,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform bounce150 = {PIEZO_MODE3,4,bounce150_data};

static const unsigned char bounce200_data[] = {
		DRV2667_WAVSEQ0, 0x07,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform bounce200 = {PIEZO_MODE3,4,bounce200_data};

static const unsigned char bounce250_data[] = {
		DRV2667_WAVSEQ0, 0x08,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform bounce250 = {PIEZO_MODE3,4,bounce250_data};


/**
 * Transitions
 */
static const unsigned char transition100_data[] = {
		DRV2667_WAVSEQ0, 0x09,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform transition100 = {PIEZO_MODE3,4,transition100_data};

static const unsigned char transition150_data[] = {
		DRV2667_WAVSEQ0, 0x0A,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform transition150 = {PIEZO_MODE3,4,transition150_data};

static const unsigned char transition200_data[] = {
		DRV2667_WAVSEQ0, 0x0B,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform transition200 = {PIEZO_MODE3,4,transition200_data};

static const unsigned char transition250_data[] = {
		DRV2667_WAVSEQ0, 0x0C,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform transition250 = {PIEZO_MODE3,4,transition250_data};


/**
 * Alerts
 */
static const unsigned char alert1_data[] = {
		DRV2667_WAVSEQ0, 0x011,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform alert1 = {PIEZO_MODE3,4,alert1_data};

static const unsigned char alert2_data[] = {
		DRV2667_WAVSEQ0, 0x012,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform alert2 = {PIEZO_MODE3,4,alert2_data};

static const unsigned char alert3_data[] = {
		DRV2667_WAVSEQ0, 0x013,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform alert3 = {PIEZO_MODE3,4,alert3_data};

static const unsigned char alert4_data[] = {
		DRV2667_WAVSEQ0, 0x014,
		DRV2667_WAVSEQ1, 0x00
};
const Waveform alert4 = {PIEZO_MODE3,4,alert4_data};


/**
 * Combination Effects
 */
static const unsigned char clickBounce_data[] = {
		DRV2667_WAVSEQ0, 0x03,
		DRV2667_WAVSEQ1, 0x05,
		DRV2667_WAVSEQ2, 0x00,
};
const Waveform clickBounce = {PIEZO_MODE3,6,clickBounce_data};

static const unsigned char transitionClick_data[] = {
		DRV2667_WAVSEQ0, 0x0A,
		DRV2667_WAVSEQ1, 0x02,
		DRV2667_WAVSEQ2, 0x00,
};
const Waveform transitionClick = {PIEZO_MODE3,6,transitionClick_data};

static const unsigned char flyBy_data[] = {
		DRV2667_WAVSEQ0, 0x0A,
		//DRV2667_WAVSEQ1, 0x02,
		DRV2667_WAVSEQ1, 0x06,
		DRV2667_WAVSEQ2, 0x00
};
const Waveform flyBy = {PIEZO_MODE3,6,flyBy_data};

static const unsigned char longClickBounce_data[] = {
		DRV2667_WAVSEQ0, 0x04,
		DRV2667_WAVSEQ1, 0x02,
		DRV2667_WAVSEQ2, 0x05,
		DRV2667_WAVSEQ3, 0x00
};
const Waveform longClickBounce = {PIEZO_MODE3,8,longClickBounce_data};



//--------------------------------------------------------//
// Analog Mode Settings
//--------------------------------------------------------//
static const unsigned char analogMode50_data[] = {		// Analog Mode 50Vpp
		DRV2667_CONTROL, (INPUT_MUX + GAIN_50),			// switch to analog mode
		DRV2667_CONTROL2, EN_OVERRIDE					// permanently enable DRV2667
};
const Waveform analogMode50 = {PIEZO_ANALOGMODE50,4,analogMode50_data};

static const unsigned char analogMode100_data[] = {		// Analog Mode 100Vpp
		DRV2667_CONTROL, (INPUT_MUX + GAIN_100),
		DRV2667_CONTROL2, EN_OVERRIDE
};
const Waveform analogMode100 = {PIEZO_ANALOGMODE100,4,analogMode100_data};

static const unsigned char analogMode150_data[] = {		// Analog Mode 150Vpp
		DRV2667_CONTROL, (INPUT_MUX + GAIN_150),
		DRV2667_CONTROL2, EN_OVERRIDE
};
const Waveform analogMode150 = {PIEZO_ANALOGMODE150,4,analogMode150_data};

static const unsigned char analogMode200_data[] = {		// Analog Mode 200Vpp
		DRV2667_CONTROL, (INPUT_MUX + GAIN_200),
		DRV2667_CONTROL2, EN_OVERRIDE
};
const Waveform analogMode200 = {PIEZO_ANALOGMODE200,4,analogMode200_data};

static const unsigned char analogModeExit_data[] = {	// Exit Analog Mode
		DRV2667_CONTROL2, 0x00,		// Disable Enable Override
		DRV2667_CONTROL, GAIN		// Switch input mux to digital input and reset gain
};
const Waveform analogModeExit = {PIEZO_ANALOGOFF,4,analogModeExit_data};

