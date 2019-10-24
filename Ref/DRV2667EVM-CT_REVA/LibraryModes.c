/******************************************************************************
 * LibraryModes.h
 *
 * Created on: February 17, 2013
 * Board: DRV2667EVM-CT
 * Author: a0866685
 *
 * Desc: This file contains the button functions to access the DRV2667 RAM
 * 		waveforms.  Include this file to compile with the DRV2667 RAM library
 * 		modes.
 *
 * 		Mode 0 - 31 are the library effects.
 *
 ******************************************************************************/

/* *****************************************************************************
 * Button Descriptions
 * *****************************************************************************
 * To Access:  Hold the "+" button for approximately 3 seconds.  The
 * 		mode LEDs will flash.
 *
 * The mode LEDs will count in binary.
 *
 * Mode 0 (All LEDs Off):
 * B1: Library Effect 1 - StrongClick_100
 * B2: Library Effect 2 - StrongClick_60
 * B3: Library Effect 3 - StrongClick_30
 * B4: Library Effect 4 - SharpClick_100
 *
 * Mode 1 (M0 On):
 * B1: Library Effect 5
 * B2: Library Effect 6
 * B3: Library Effect 7
 * B4: Library Effect 8
 *
 * Mode 2 (M1 On):
 * ...
 *
 * Mode 3 (M1 and M0 On):
 * ...
 *
 *
 *******************************************************************************/

#ifndef LIBRARYMODES_C_
#define LIBRARYMODES_C_

#include "LibraryModes.h"
#include "DRV2667.h"
#include "Actuator_Waveforms.h"
#include "CapTouchBoard.h"
#include "I2C.h"

#define  WAVEFORMLIBRARY_SIZE 	20 		// Size including "0"

extern struct Element* modePtr;
extern struct Element* buttonPtr;


static const Waveform* const waveformLibrary[] = {
		&click150,
		&click200,
		&click250,
		&click300,
		&bounce100,
		&bounce150,
		&bounce200,
		&bounce250,
		&transition100,
		&transition150,
		&transition200,
		&transition250,
		&clickBounce,
		&transitionClick,
		&flyBy,
		&longClickBounce,
		&alert1,
		&alert2,
		&alert3,
		&alert4
};



/**
 * LibraryModes - function containing the DRV2667 RAM library where BIT0 = 1 (ie = xxxxx001'b)
 */
void LibraryModes(void)
{
	uint8_t libraryMode;	// Current set of buttons

	// To access the DRV2667 RAM library hold the "+" button
	if(CapTouch_GetBinaryMode() == LIBRARYMODESLEVEL)
	{
		libraryMode = (CapTouch_mode >> 1);				// CapTouch_Mode[7:3] >> 3 * 4

		if(libraryMode < (WAVEFORMLIBRARY_SIZE))
		{
			switch(buttonPtr->referenceNumber)
			{
				case BUTTON1:
				{
					Haptics_SendWaveform(*(waveformLibrary[(libraryMode)]));
					break;
				}
				case BUTTON2:
				{
					Haptics_SendWaveform(*(waveformLibrary[(libraryMode + 1)]));
					break;
				}
				case BUTTON3:
				{
					Haptics_SendWaveform(*(waveformLibrary[(libraryMode + 2)]));
					break;
				}
				case BUTTON4:
				{
					Haptics_SendWaveform(*(waveformLibrary[(libraryMode + 3)]));
					break;
				}
			}
		}
	}
}


#endif /* LIBRARYMODES_C_ */
