/*
 * Recorder.c
 *
 *  Created on: Jul 25, 2012
 *      Author: a0866685
 */

#include "Recorder.h"
#include "Haptics.h"
#include "Actuator_Waveforms.h"


extern struct Element* modePtr;
extern struct Element* buttonPtr;
extern const struct Element* const buttonList[];


static uint8_t  	IsRecording = 0;
static uint8_t 		IsPlaying = 0;
static uint8_t		IsPressRecording = 0;
static uint16_t 	pressCounter = 0;
static uint16_t 	pauseCounter = 0;
static uint16_t 	recordPattern[100];
static uint8_t		recordPatternCounter = 0;
static uint8_t		playbackPatternCounter = 0;
static uint8_t 		IsWaveformActive = 0;


void Recorder_Init()
{
	uint8_t i = 0;

	IsRecording = 0;

	for(i=0; i < 100; i++)
	{
		recordPattern[i] = 0;
	}
}

uint8_t Recorder_IsRecording()
{
	return IsRecording;
}

void Recorder_RecordToggle()
{
	IsRecording = !IsRecording;
	if(IsRecording)
	{
		recordPatternCounter = 0;
		P3OUT |= 0x02;
	}
	else
		P3OUT &= 0xFD;
}

void Recorder_PlayToggle()
{
	Recorder_Reset();

	IsPlaying = !IsPlaying;
	IsRecording = 0;

	if(IsPlaying)
		P3OUT |= 0x02;
	else
		P3OUT &= 0xFD;
}

void Recorder_Record()
{
	if(IsRecording)
	{
		if(!buttonPtr)
		{
			if(IsPressRecording)							// if previously recording a press
			{
				recordPattern[recordPatternCounter] = pressCounter;		// Store press counter
				IsPressRecording = 0;									// Stop press recording, begin pause recording
				recordPatternCounter++;									// Increment recordPattern[] pointer
				pauseCounter = 0;										// Reset pause counter
				I2C_WriteSingleByte(DRV2667_CONTROL2, STOP); 			// Stop output
			}

			pauseCounter++;									// Count the time during a Pause
		}
		else if(buttonPtr->sequenceNumber == 1)
		{
			if(!IsPressRecording)							// if previously recording a pause
			{
				recordPattern[recordPatternCounter] = pauseCounter;		// Store pause counter
				IsPressRecording = 1;									// Stop pause recording, begin press recording
				recordPatternCounter++;									// Increment recordPattern[] pointer
				pressCounter = 0;										// Reset pause counter
				Haptics_SendWaveform(dynamicWaveform);					// Start Waveform
			}

			pressCounter++;									// Count the time during a Press
		}
	}
}

void Recorder_Reset()
{
	IsPlaying = 0;
	playbackPatternCounter = 0;
	IsRecording = 0;
	IsWaveformActive = 0;
	pauseCounter = 0;
	pressCounter = 0;
	P3OUT &= 0xFD;
	I2C_WriteSingleByte(DRV2667_CONTROL2, STOP); 	// Stop output
}

void Recorder_Playback()
{
	if(IsPlaying)
	{
		if(!IsPressRecording)
		{
			pauseCounter++;

			if(pauseCounter >= recordPattern[playbackPatternCounter])
			{
				pauseCounter = 0;
				playbackPatternCounter++;
				IsPressRecording = 1;
			}
		}
		else
		{
			pressCounter++;

			if(pressCounter >= recordPattern[playbackPatternCounter])
			{
				pressCounter = 0;
				playbackPatternCounter++;
				IsPressRecording = 0;

				// Stop Playback
				I2C_WriteSingleByte(DRV2667_CONTROL2, STOP); 	// Stop output
				IsWaveformActive = 0;

			}
			else if(!IsWaveformActive)
			{
				IsWaveformActive = 1;
				Haptics_SendWaveform(dynamicWaveform);			// Start Waveform
			}

		}

		if(playbackPatternCounter >= recordPatternCounter)
		{
			Recorder_Reset();
		}
	}
	else
	{

	}
}
