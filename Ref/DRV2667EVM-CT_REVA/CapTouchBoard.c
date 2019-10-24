/******************************************************************************
 * CapTouchBoard.c
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 * Author: a0866685
 *
 * Desc: This file contains common functions for operating the capacitive
 * 		touch EVK.
 *
 ******************************************************************************/
#include "CapTouchBoard.h"
#include "Actuator_Waveforms.h"
#include "Haptics.h"

// public variables
uint8_t CapTouch_mode = MODEOFF;			// Current mode, show on mode LEDs
uint8_t CapTouch_isBinaryModeCounter = 0; 	// If true, count the mode LEDs in binary, otherwise only six modes
uint8_t CapTouch_isModeBtnReleased = 1;		// CapTouch board mode buttons are released/not pressed
uint8_t CapTouch_isEffectBtnReleased = 1;	// CapTouch board effect (B1-B4) buttons are released/not pressed
uint8_t CapTouch_isLEDFrozen = 0; 			// CapTouch board colored LEDs lock current state
uint8_t CapTouch_isModeLEDFrozen = 0;		// CapTouch board white mode LEDs lock current state
uint8_t CapTouch_RandomNumber = 1;			// Random number generated from interrupt

// private variables
static uint8_t 	repeatEffectEnabled = 0;	// Allow the effect to repeat, used in RepeatOnHoldWait()
static uint16_t buttonPressCounter = 0;		// Counts the number of button samples
static uint16_t repeatModeEnabled = 0;		// Allow the mode button to be held down
static uint16_t	modePressCounter = 0;		// Counts the number of mode button samples
static uint8_t 	modeIncrementOk = 1; 	    // Flag if the mode can be incremented
static uint8_t  currentExtraMode = 0;		// Binary mode status

/**
 * CapTouch_Init - Initialization settings for captouch evaluation board
 */
void CapTouch_Init(void)
{
	CapTouch_mode = MODEOFF;					// Set current mode to mode with all LEDs off
	CapTouch_isModeBtnReleased = 1;				// Mode buttons status = mode buttons are released (not pressed)
	CapTouch_isEffectBtnReleased = 1;			// Effect buttons Status (B1-B4) = effect buttons are released (not pressed)

	// Establish Capacitive Touch Baseline
	TI_CAPT_Init_Baseline(&mode_change);		// Measure initial capacitance of mode buttons
	TI_CAPT_Init_Baseline(&buttons);			// Measure initial capacitance of effect buttons (B1-B4)
	TI_CAPT_Update_Baseline(&mode_change,5);	// Measure mode buttons capacitance 5x times and average, this tracks changing capacitance
	TI_CAPT_Update_Baseline(&buttons,5);		// Measure effect buttons capacitance 5x times and average, this tracks changing capacitance
}

/**
 * CapTouch_PowerUpSequence - visual startup sequence for the evaluation board
 */
void CapTouch_PowerUpSequence(void)
{
	CapTouch_ButtonLEDOnSequence();

	CapTouch_FlashModeLEDs(3);

    Haptics_SendWaveform(transition250);		// Trigger actuator

	CapTouch_ButtonLEDOffSequence();
}

/**
 * CapTouch_IncrementMode - increase the board mode (binary or carousel)
 */
void CapTouch_IncrementMode(void)
{
	if(modeIncrementOk)
	{
		if(CapTouch_isBinaryModeCounter)		// If Binary Mode
			CapTouch_IncrementModeBinary();
		else									// Else Carousel Mode
			CapTouch_IncrementModeCarousel();

		Haptics_SendWaveform(click150);			// Trigger actuator
	}
}

/**
 * CapTouch_DecrementMode - decrease the board mode (binary or carousel)
 */
void CapTouch_DecrementMode(void)
{
	if(modeIncrementOk)
	{
		if(CapTouch_isBinaryModeCounter)		// If Binary Mode
			CapTouch_DecrementModeBinary();
		else									// Else Carousel Mode
			CapTouch_DecrementModeCarousel();

		Haptics_SendWaveform(click150);			// Trigger actuator
	}
}

/**
 * CapTouch_SetBinaryMode - set the mode counter to binary format
 */
void CapTouch_SetBinaryMode()
{

	CapTouch_mode = 0;

	if(currentExtraMode == NUM_BINARYMODES)			// Check if reached the end of extra modes
	{
		CapTouch_FlashButtonLEDs(currentExtraMode);	// Flash LED according to extra binary mode
	}
	else
	{
		currentExtraMode++;							// Increment to next extra binary mode
		CapTouch_FlashButtonLEDs(currentExtraMode);	// Flash LED according to extra binary mode
		CapTouch_mode = 0;							// Reset CapTouch Modes
	}

	CapTouch_isBinaryModeCounter = 1;				// Binary modes are enable
	Haptics_OutputEnableSet(1);						// Output enabled
	Haptics_SendWaveform(click150);
	sleep(500);
	Haptics_SendWaveform(click150);
	sleep(500);
	Haptics_SendWaveform(click150);
	sleep(500);
	Haptics_OutputEnableSet(0);						// Output disable
	CapTouch_FlashModeLEDs(2);
}

/**
 * CapTouch_SetBinaryMode - set the mode counter to carousel format
 */
void CapTouch_UnsetBinaryMode()
{
	currentExtraMode = 0;					// Reset to demo mode
	CapTouch_isBinaryModeCounter = 0;		// Stop counting in binary
	CapTouch_mode = 0;						// Reset mode to 0
	Haptics_OutputEnableSet(1);
	Haptics_SendWaveform(click150);
	sleep(500);
	Haptics_SendWaveform(click150);
	sleep(500);
	Haptics_SendWaveform(click150);
	sleep(500);
	Haptics_OutputEnableSet(0);
	CapTouch_FlashModeLEDs(1);
}

/**
 * CapTouch_GetBinaryMode
 * @return - 0 = not in binary mode, 1..7 = binary modes
 */
unsigned char CapTouch_GetBinaryMode()
{
	return currentExtraMode;
}

/**
 * CapTouch_IncrementMode - increase the board mode (6 modes)
 */
void CapTouch_IncrementModeCarousel(void)
{
	if(CapTouch_mode == MODEOFF)
	  CapTouch_mode = MODE4;
	else if(CapTouch_mode == MODE0)
		  CapTouch_mode = MODEOFF;
	else
	  CapTouch_mode = CapTouch_mode >> 1;
}

/**
 * CapTouch_DecrementMode - decrease the board mode (6 modes)
 */
void CapTouch_DecrementModeCarousel(void)
{
	if(CapTouch_mode == MODE4)
	  CapTouch_mode = MODEOFF;
	else if(CapTouch_mode == MODEOFF)
	  CapTouch_mode = MODE0;
	else
	  CapTouch_mode = CapTouch_mode << 1;
}

/**
 * IncrementModeBode - increase the board mode (32 modes)
 */
void CapTouch_IncrementModeBinary(void)
{
	if(CapTouch_mode == MODE_MASK)
		CapTouch_mode = MODEOFF;
	else
		CapTouch_mode = CapTouch_mode + MODE_OFFSET;
}

/**
 * DecrementModeBinary - decrease the board mode (32 modes)
 */
void CapTouch_DecrementModeBinary(void)
{
	if(CapTouch_mode == MODEOFF)
		CapTouch_mode = MODE_MASK;
	else
		CapTouch_mode = CapTouch_mode - MODE_OFFSET;
}

/**
 * CapTouch_SetModeLEDs - set the mode LEDs to the current mode
 */
void CapTouch_SetModeLEDs(void)
{
	P3OUT &= ~MODE_MASK;		// Clear mode LEDs
	P3OUT |= CapTouch_mode;		// Set mode LEDs to current mode
}

/**
 * CapTouch_ModeLEDBinary - display binary on the mode LEDs
 * @param unsigned char number - number to be displayed
 */
void CapTouch_ModeLEDBinary(uint8_t number)
{
	P3OUT &= ~MODE_MASK;		// Clear mode LEDs
	P3OUT |= (number << 3);		// Shift number and then set mode LEDs
}

/**
 * CapTouch_ButtonLEDsOn - turn all button LEDs on
 */
void CapTouch_ButtonLEDsOn(void)
{
	P1OUT |= 0x0F;			// Turn on button LEDs
}

/**
 * CapTouch_ButtonLEDsOff - turn all button LEDs off
 */
void CapTouch_ButtonLEDsOff(void)
{
	P1OUT &= 0xF0;			// Turn off button LEDs
}

/**
 * CapTouch_ModeLEDsOn - turn all mode LEDs on
 */
void CapTouch_ModeLEDsOn(void)
{
	P3OUT |= (MODE_MASK);	// Turn on mode LEDs
}

/**
 * CapTouch_ModeLEDsOff - turn all mode LEDs off
 */
void CapTouch_ModeLEDsOff(void)
{
	P3OUT &= ~MODE_MASK;	// Turn off mode LEDs
}

/**
 * CapTouch_FlashButtonLEDs - flash all button LEDs x times
 * @param unsigned int numberOfBlinks - number of flashes
 */
void CapTouch_FlashButtonLEDs(unsigned int numberOfBlinks)
{
	unsigned int i = 0;

	P1OUT &= 0xF0;							// turn off LEDs
	sleep(LEDBLINKDELAY);

	for(i = 0; i < numberOfBlinks; i++)
	{
		// flash button LEDs
		P1OUT |= 0x0F;						// turn on LEDs
		sleep(LEDBLINKDELAY);
		P1OUT &= 0xF0;						// turn off LEDs
		sleep(LEDBLINKDELAY);
	}
}

/**
 * CapTouch_FlashModeLEDs - flash all mode LEDs x times
 * @param unsigned int numberOfBlinks - number of flashes
 */
void CapTouch_FlashModeLEDs(unsigned int numberOfBlinks)
{
	unsigned int i = 0;

	P3OUT &= ~(MODE_MASK);
	sleep(LEDBLINKDELAY);

	for(i = 0; i < numberOfBlinks; i++)
	{
		// flash mode LEDs
		P3OUT |= (MODE_MASK);				// Turn on mode LEDs
		sleep(LEDBLINKDELAY);
		P3OUT &= ~(MODE_MASK);				// Turn off mode LEDs
		sleep(LEDBLINKDELAY);
	}
}

/**
 * CapTouch_ButtonLEDOnSequence - turn on LEDS from B1->B2->B3->B4
 */
void CapTouch_ButtonLEDOnSequence(void)
{
	P1OUT |= (BUTTON1);								// turn on LED B1
	sleep(LEDBLINKDELAY);
	P1OUT |= (BUTTON2);								// turn on LED B2
	sleep(LEDBLINKDELAY);
	P1OUT |= (BUTTON3);								// turn on LED B3
	sleep(LEDBLINKDELAY);
	P1OUT |= (BUTTON4);								// turn on LED B4
	sleep(LEDBLINKDELAY);
}

/**
 * CapTouch_ButtonLEDOffSequence - turn on LEDS from B1<-B2<-B3<-B4
 */
void CapTouch_ButtonLEDOffSequence(void)
{
	P1OUT &= ~(BUTTON4);							// turn off LED B4
	sleep(LEDBLINKDELAY);
	P1OUT &= ~(BUTTON3);							// turn off LED B3
	sleep(LEDBLINKDELAY);
	P1OUT &= ~(BUTTON2);							// turn off LED B2
	sleep(LEDBLINKDELAY);
	P1OUT &= ~(BUTTON1);							// turn off LED B1
}

/**
 * CapTouch_ModeLEDsScroll - scroll the mode LEDs to the right
 * 		Note: does not affect the current CapTouch_mode variable
 * @param unsigned int numberOfScolls - number of shifts from left to right
 */
void CapTouch_ModeLEDsScroll(unsigned int numberOfScrolls)
{
	unsigned int i;

	CapTouch_ModeLEDsOff();

	P3OUT |= MODE4;
	sleep(LEDBLINKDELAY);
	P3OUT &= ~MODE4;
	P3OUT |= MODE3;
	sleep(LEDBLINKDELAY);
	P3OUT &= ~MODE3;
	P3OUT |= (MODE4+MODE2);
	sleep(LEDBLINKDELAY);
	P3OUT &= ~(MODE4+MODE2);
	P3OUT |= (MODE3+MODE1);
	sleep(LEDBLINKDELAY);


	for(i = 0; i < numberOfScrolls; i++)
	{
		P3OUT &= ~(MODE3+MODE1);
		P3OUT |= (MODE4+MODE2+MODE0);
		sleep(LEDBLINKDELAY);

		P3OUT &= ~(MODE4+MODE2+MODE0);
		P3OUT |= (MODE3+MODE1);
		sleep(LEDBLINKDELAY);
	}

	P3OUT &= ~(MODE3+MODE1);
	P3OUT |= (MODE2+MODE0);
	sleep(LEDBLINKDELAY);

	P3OUT &= ~(MODE2+MODE0);
	P3OUT |= MODE1;
	sleep(LEDBLINKDELAY);

	P3OUT &= ~MODE1;
	P3OUT |= MODE0;
	sleep(LEDBLINKDELAY);

	P3OUT &= ~MODE0;
}


uint8_t CapTouch_IsRepeating()
{
	if(repeatEffectEnabled || repeatModeEnabled)
	{
		return 1;
	}

	return 0;
}

/**
 * CapTouch_RepeatOnHold - repeat the effect when button is held
 */
void CapTouch_RepeatOnHold(uint16_t pause)
{
	CapTouch_isEffectBtnReleased = 1;		// indicate the button has been released
	sleep(pause);
}

/**
 * CapTouch_RepeatOnHoldWait - hold button and wait X ms before playing repeat waveform
 */
void CapTouch_RepeatOnHoldWait()
{
	CapTouch_isEffectBtnReleased = 1;		    // Allow effect repeat

	if(!repeatEffectEnabled)					// If repeat effect is already enabled, skip
	{
		Haptics_OutputEnableSet(0);				// Disable haptics output to prevent repeat
		buttonPressCounter++;					// Set the time to wait

		if(buttonPressCounter > 400)			// Set the time to wait
		{
			repeatEffectEnabled = 1;
			Haptics_OutputEnableSet(1);			// Enable haptics output
		}
	}
}

/**
 * CapTouch_EnableOnHold - hold a mode button to call a function
 * @param action - pointer to function to be called after hold
 */
void CapTouch_EnableOnHold(void (*action)())
{
	CapTouch_isModeBtnReleased = 1;		    // allow effect repeat

	if(!repeatModeEnabled)					// If repeat mode is already enabled, skip
	{
		Haptics_OutputEnableSet(0);			// Disable haptics output to prevent repeat
		modeIncrementOk = 0; 				// Temporarily disable mode increment
		modePressCounter++;					// Counter for timing

		if(modePressCounter > 400)			// Set the time to wait
		{
			modePressCounter = 1;			// Reset counter
			(*action)();					// Run function after wait
			repeatModeEnabled = 1;
		}
	}
}

/**
 * CapTouch_FreezeLEDs - freeze the current state of the LEDs
 */
void CapTouch_FreezeLEDs(void)
{
	CapTouch_isLEDFrozen = 1;
}

/**
 * CapTouch_FreezeModeLEDs - freeze the current state of the mode LEDs
 */
void CapTouch_FreezeModeLEDs()
{
	CapTouch_isModeLEDFrozen = 1;
}

/**
 * CapTouch_UnfreezeLEDs - unfreeze the current state of the LEDs
 */
void CapTouch_UnfreezeLEDs(void)
{
	CapTouch_isLEDFrozen = 0;
	CapTouch_isModeLEDFrozen = 0;
}

/**
 * CapTouch_RepeatReset - to be called in main loop when button is released
 */
void CapTouch_RepeatReset(void)
{
	repeatEffectEnabled = 0;
	buttonPressCounter = 0;
	Haptics_OutputEnableSet(1);
}

/**
 * CapTouch_ModeRepeatReset - to be called in main loop when a mode button is released
 */
void CapTouch_ModeRepeatReset(void)
{
	repeatModeEnabled = 0;
	modePressCounter = 0;
	modeIncrementOk = 1;
}

/**
 * CapTouch_FlashCodeRevision - flash silicon/code revision
 */
void CapTouch_FlashCodeRevision()
{
	CapTouch_ModeLEDsOff();
	sleep(LEDBLINKDELAY);
	CapTouch_ModeLEDBinary(CODE_REVISION);
	sleep(20000);
	CapTouch_ModeLEDsOff();
	sleep(LEDBLINKDELAY);
}

/**
 * CapTouch_FlashDeviceID - flash device ID
 */
void CapTouch_FlashDeviceID(void)
{
	CapTouch_ModeLEDsOff(); 				// Turn off LEDs
	sleep(LEDBLINKDELAY);

	CapTouch_ModeLEDBinary(0x07); 		// Turn on mode LEDs
	sleep(20000);
	CapTouch_ModeLEDsOff();
	sleep(LEDBLINKDELAY);
}

/**
 * CapTouch_FlashDeviceRevision - flash device revision
 */
void CapTouch_FlashDeviceRevision(void)
{
	unsigned char DeviceRev = 0;

	CapTouch_ModeLEDsOff(); 				// Turn off LEDs
	sleep(LEDBLINKDELAY);
	DeviceRev = I2C_ReadSingleByte(0x15); 	// Get Device ID, Reg 0x01, Bits[6:3]

	CapTouch_ModeLEDBinary((DeviceRev >> 4)); 			// Turn on LEDs
	sleep(20000);
	CapTouch_ModeLEDsOff();
	sleep(LEDBLINKDELAY);
}

/**
 * CapTouch_ModeLEDBinarySequence - show a binary number on the mode LEDs (timed sequence)
 * @parm number - unsigned char up to 31
 */
void CapTouch_ModeLEDBinarySequence(unsigned char number)
{
	CapTouch_ModeLEDsOff();
	sleep(LEDBLINKDELAY);
	CapTouch_ModeLEDBinary(number);
	sleep(20000);
	CapTouch_ModeLEDsOff();
	sleep(LEDBLINKDELAY);
}
