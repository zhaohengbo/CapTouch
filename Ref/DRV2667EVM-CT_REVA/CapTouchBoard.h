/******************************************************************************
 * CapTouchBoard.h
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 * Author: a0866685
 *
 * Desc: This file contains common functions for operating the capacitive
 * 		touch EVK.
 *
 ******************************************************************************/

#ifndef CAPTOUCHBOARD_H_
#define CAPTOUCHBOARD_H_

#include "CTS/CTS_Layer.h"
#include "msp430.h"
#include "CTS/structure.h"
#include "Timer.h"

#define CODE_REVISION 1		// Set the revision of the main.c program

// Button Definitions
#define BUTTON_MASK   (BUTTON1+BUTTON2+BUTTON3+BUTTON4)		// Button Mask
#define BUTTON1   BIT0										// B1
#define BUTTON2   BIT1										// B2
#define BUTTON3   BIT2										// B3
#define BUTTON4   BIT3										// B4
#define NUMBER_BUTTONS  4									// Number of Buttons

// Mode Definitions
#define MODE_OFFSET BIT3
#define MODE_MASK   (MODE0+MODE1+MODE2+MODE3+MODE4)			// Mode LEDs
#define MODEOFF 	0										// No mode LEDs on
#define MODE0       BIT3									// M0 LED
#define MODE1       BIT4									// M1 LED
#define MODE2       BIT5									// M2 LED
#define MODE3       BIT6									// M3 LED
#define MODE4       BIT7									// M4 LED
#define BUTTONMINUS BIT4									// Mode Button -
#define BUTTONPLUS  BIT5									// Mode Button +

// Enable LED
#define EN			BIT1											// Enable LED (P3.1)
#define LED_EN(x)   ((x == ON) ? (P3OUT |= EN) : (P3OUT &= ~EN))	// Enable/Disable "EN" LED

#define LEDBLINKDELAY 	1500								// LED blink rate
#define NUM_BINARYMODES 2									// Number of binary modes (1-8)

/* Status */
enum { OFF = 0, ON };										// Status Enum

// Status variables
extern uint8_t 	CapTouch_mode; 						// Current mode, show on mode LEDs
extern uint8_t  CapTouch_isBinaryModeCounter; 		// If true, count the mode LEDs in binary, otherwise only six modes
extern uint8_t 	CapTouch_isModeBtnReleased;	   	 	// CapTouch board mode buttons are released/not pressed
extern uint8_t 	CapTouch_isEffectBtnReleased;		// CapTouch board effect (B1-B4) buttons are released/not pressed
extern uint8_t  CapTouch_isLEDFrozen;				// CapTouch board colored LEDs lock current state
extern uint8_t  CapTouch_isModeLEDFrozen;			// CapTouch board white mode LEDs lock current state
extern uint8_t 	CapTouch_RandomNumber; 				// Random number generated from interrupt

/**
 * CapTouch_Init - Initialization settings for captouch evaluation board
 */
void CapTouch_Init(void);

/**
 * CapTouch_PowerUpSequence - visual startup sequence for the evaluation board
 * @TODO - Update this function when changing actuators types (ERM, LRA, Piezo)
 */
void CapTouch_PowerUpSequence(void);

/**
 * CapTouch_IncrementMode - increase the board mode (binary or carousel)
 */
void CapTouch_IncrementMode(void);

/**
 * CapTouch_DecrementMode - decrease the board mode (binary or carousel)
 */
void CapTouch_DecrementMode(void);

/**
 * CapTouch_SetBinaryMode - set the mode counter to binary format
 */
void CapTouch_SetBinaryMode();

/**
 * CapTouch_SetBinaryMode - set the mode counter to carousel format
 */
void CapTouch_UnsetBinaryMode();

/**
 * CapTouch_GetBinaryMode
 * @return - 0 = not in binary mode, 1..7 = binary modes
 */
unsigned char CapTouch_GetBinaryMode();

/**
 * CapTouch_IncrementModeCarousel - increase the board mode (6 modes)
 */
void CapTouch_IncrementModeCarousel(void);

/**
 * CapTouch_DecrementModeCarousel - decrease the board mode (6 modes)
 */
void CapTouch_DecrementModeCarousel(void);

/**
 * CapTouch_IncrementModeBode - increase the board mode (32 modes)
 */
void CapTouch_IncrementModeBinary(void);

/**
 * CapTouch_DecrementModeBinary - decrease the board mode (32 modes)
 */
void CapTouch_DecrementModeBinary(void);

/**
 * CapTouch_SetModeLEDs - set the mode LEDs to the current mode
 */
void CapTouch_SetModeLEDs(void);

/**
 * CapTouch_ModeLEDBinary - display binary on the mode LEDs
 * @param unsigned char number - number to be displayed
 */
void CapTouch_ModeLEDBinary(uint8_t number);

/**
 * CapTouch_ModeLEDsOn - turn all mode LEDs on
 */
void CapTouch_ModeLEDsOn(void);

/**
 * CapTouch_ModeLEDsOff - turn all mode LEDs off
 */
void CapTouch_ModeLEDsOff(void);

/**
 * CapTouch_ButtonLEDsOn - turn all button LEDs on
 */
void CapTouch_ButtonLEDsOn(void);

/**
 * CapTouch_ButtonLEDsOff - turn all button LEDs off
 */
void CapTouch_ButtonLEDsOff(void);

/**
 * CapTouch_FlashButtonLEDs - flash all button LEDs x times
 * @param unsigned int numberOfBlinks - number of flashes
 */
void CapTouch_FlashButtonLEDs(unsigned int numberOfBlinks);

/**
 * CapTouch_FlashModeLEDs - flash all mode LEDs x times
 * @param unsigned int numberOfBlinks - number of flashes
 */
void CapTouch_FlashModeLEDs(unsigned int numberOfBlinks);

/**
 * CapTouch_ButtonLEDOnSequence - turn on LEDS from B1->B2->B3->B4
 */
void CapTouch_ButtonLEDOnSequence(void);

/**
 * CapTouch_ButtonLEDOffSequence - turn on LEDS from B1<-B2<-B3<-B4
 */
void CapTouch_ButtonLEDOffSequence(void);

/**
 * CapTouch_ModeLEDsScroll - scroll the mode LEDs to the right
 * 		(does not affect the current mode)
 * @param unsigned int numberOfScolls - number of shifts from left to right
 */
void CapTouch_ModeLEDsScroll(unsigned int numberOfScrolls);

uint8_t CapTouch_IsRepeating();

/**
 * CapTouch_RepeatOnHold - repeat the effect when button is held
 */
void CapTouch_RepeatOnHold(uint16_t pause);

/**
 * CapTouch_RepeatOnHoldWait - hold button and wait X ms before playing repeat waveform
 */
void CapTouch_RepeatOnHoldWait(void);

/**
 * CapTouch_EnableOnHold - hold a button to call a function
 * @param action - pointer to function to be called after hold
 */
void CapTouch_EnableOnHold(void (*action)());

/**
 * CapTouch_FreezeLEDs - freeze the current state of the LEDs
 */
void CapTouch_FreezeLEDs(void);

/**
 * CapTouch_FreezeModeLEDs - freeze the current state of the mode LEDs
 */
void CapTouch_FreezeModeLEDs();

/**
 * CapTouch_UnfreezeLEDs - unfreeze the current state of the LEDs
 */
void CapTouch_UnfreezeLEDs(void);

/**
 * CapTouch_RepeatReset - to be called in main loop when button is released
 */
void CapTouch_RepeatReset(void);

/**
 * CapTouch_ModeRepeatReset - to be called in main loop when a mode button is released
 */
void CapTouch_ModeRepeatReset(void);

/**
 * CapTouch_FlashCodeRevision - flash silicon/code revision
 */
void CapTouch_FlashCodeRevision();

/**
 * CapTouch_FlashDeviceID - flash device ID
 */
void CapTouch_FlashDeviceID();

/**
 * CapTouch_FlashDeviceRevision - flash device revision
 */
void CapTouch_FlashDeviceRevision(void);

/**
 * CapTouch_ModeLEDBinarySequence - show a binary number on the mode LEDs (timed sequence)
 * @parm number - unsigned char up to 31
 */
void CapTouch_ModeLEDBinarySequence(unsigned char number);

#endif /* CAPTOUCHBOARD_H_ */
