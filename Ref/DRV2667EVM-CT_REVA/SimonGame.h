/******************************************************************************
 * SimonGame.h
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 * Author: a0866685
 *
 * Desc: This file contians functions to operate and play the game of "Simon".
 *
 * To Play:
 * 		1) Press any button (B1-B4) to start the game
 * 		2) The game counts down and then shows a pattern of signals
 * 		3) Once you have seen the pattern, repeat the pattern by touching
 * 			the buttons in the same order
 * 		4) If you repeat the entire pattern correctly, the board will repeat
 * 			the pattern plus one additional signal.  Continue playing until you
 * 			repeat the sequence incorrectly.
 * 		5) If you repeat the pattern incorrectly, the board will buzz and the
 * 			game is over.
 *
 * @TODO - Update this file when changing actuators types (ERM, LRA, Piezo)
 ******************************************************************************/

#ifndef SIMONGAME_H_
#define SIMONGAME_H_

#include "Haptics.h"
#include "CapTouchBoard.h"
#include "Actuator_Waveforms.h"

// Settings
#define MINPATTERNLENGTH 	1				// Minimum pattern length
#define MAXPATTERNLENGTH 	50				// Maximum pattern length
#define PATTERNPAUSE 		3000			// Maximum time to pause before next pattern signal is shown
#define NUMBUTTONS 			NUMBER_BUTTONS	// Number of buttons used in Simon pattern
#define LEDOFFDELAY 		1500			// Time LEDs flash off during sequencing and patterns

// @TODO Update the effects below when changing actuators (ERM, LRA, Piezo)
#define B1EFFECT 			click150		// Effect for button 1
#define B2EFFECT 			click200		// Effect for button 2
#define B3EFFECT 			click250		// Effect for button 3
#define B4EFFECT 			click300		// Effect for button 4
#define COUNTDOWNEFFECT		click300		// Effect for Simon Countdown sequence
#define ERROREFFECT			alert4			// Effect for Simon error sequence
#define SUCCESSEFFECT		transition200	// Effect for Simon success sequence

/**
 * Simon_Init - Initialize Simon status and variables
 */
void Simon_Init(void);

/**
 * Simon_Play - call to begin Simon game
 * @param uint8_t button - the current button being pressed
 */
void Simon_Play(uint8_t button);

/**
 * Simon_Reset - call to cancel current Simon game, call when mode changes
 */
void Simon_Reset(void);

/**
 * Simon_IntroSequence - Simon game intro sequence
 */
void Simon_IntroSequence(void);

/**
 * Simon_CountDownSequence - count down sequence shown on Mode LEDs
 */
void Simon_CountDownSequence(void);

/**
 * Simon_IncorrectSequence - sequence shown when user is incorrect
 */
void Simon_IncorrectSequence(void);

/**
 * Simon_GeneratePattern - call to generate a new pattern
 */
void Simon_GeneratePattern(void);

/**
 * Simon_ShowPattern - call to show pattern to user
 */
void Simon_ShowPattern(void);

/**
 * Simon_VerifyPatter - call to verify the button pressed is correct
 * @param - unsigned int buttonNumber - current button being pressed
 */
void Simon_VerifyPattern(uint16_t buttonNumber);

/**
 * Simon_Success - sequence shown when user enters the correct pattern
 */
void Simon_Success(void);

/**
 * Simon_PlayButton - turn on the LED and play the corresponding effect
 */
void Simon_PlayButton(uint16_t buttonNumber);

/**
 * Simon_EffectButton1 - function when button 1 is pressed
 */
void Simon_EffectButton1(void);

/**
 * Simon_EffectButton2 - function when button 2 is pressed
 */
void Simon_EffectButton2(void);

/**
 * Simon_EffectButton3 - function when button 3 is pressed
 */
void Simon_EffectButton3(void);

/**
 * Simon_EffectButton4 - function when button 4 is pressed
 */
void Simon_EffectButton4(void);

#endif /* SIMONGAME_H_ */
