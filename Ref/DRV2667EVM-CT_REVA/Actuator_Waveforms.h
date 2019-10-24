/******************************************************************************
 * Actuator_Waveforms.h
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 * Author: a0866685
 *
 * Desc: This file contains the waveform data arrays.  See instructions in
 * 			Actuator_Waveforms.c to create new waveforms.
 *
 ******************************************************************************/

#include "Haptics.h"


//--------------------------------------------------------//
// MODE1 - Streaming Waveform Effects
//--------------------------------------------------------//
extern const Waveform mode1_bump;	// 100 Hz Bump
extern const Waveform mode1_click;	// 166 Hz Click
extern const Waveform mode1_tick;	// 266 Hz Tick
extern const Waveform mode1_alert;	// 166 Hz Alert

//--------------------------------------------------------//
// MODE2 - RAM Mode Effects
//--------------------------------------------------------//
extern const Waveform mode2_twotone; 	// Two tone click

//--------------------------------------------------------//
// MODE3 - Waveform Generator Effects
//--------------------------------------------------------//

// Dynamic
extern const Waveform dynamicWaveform;
extern const Waveform dynamicWaveformNoRepeat;
extern const Waveform dynamicWaveformNoRepeatWait1s;

// Audio
extern const Waveform ding1k;
extern const Waveform pianoF5;
extern const Waveform pianoG5;
extern const Waveform pianoA5;
extern const Waveform pianoAsharp5;
extern const Waveform pianoB5;
extern const Waveform pianoC6;
extern const Waveform pianoD6;
extern const Waveform pianoE6;
extern const Waveform pianoF6;
extern const Waveform pianoG6;
extern const Waveform pianoA6;
extern const Waveform pianoAsharp6;
extern const Waveform pianoB6;

// Clicks
extern const Waveform click150;
extern const Waveform click200;
extern const Waveform click250;
extern const Waveform click300;

// Bounces
extern const Waveform bounce100;
extern const Waveform bounce150;
extern const Waveform bounce200;
extern const Waveform bounce250;

// Transitions
extern const Waveform transition100;
extern const Waveform transition150;
extern const Waveform transition200;
extern const Waveform transition250;

// Alerts
extern const Waveform alert1;
extern const Waveform alert2;
extern const Waveform alert3;
extern const Waveform alert4;

// Combination Effects
extern const Waveform clickBounce;
extern const Waveform transitionClick;
extern const Waveform flyBy;
extern const Waveform longClickBounce;

//--------------------------------------------------------//
// Analog Mode Settings
//--------------------------------------------------------//
extern const Waveform analogMode50;		// Analog input mode 50Vpp
extern const Waveform analogMode100;	// Analog input mode 100Vpp
extern const Waveform analogMode150;	// Analog input mode 150Vpp
extern const Waveform analogMode200;	// Analog input mode 200Vpp
extern const Waveform analogModeExit; 	// Exit analog mode




