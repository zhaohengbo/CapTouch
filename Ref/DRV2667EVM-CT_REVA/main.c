/*******************************************************************************
 * main.c
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 *
 * Description: This program controls the DRV2667EVK-CT evaluation board.
 *      The program demonstrates waveforms using a Piezo actuator.
 * 
 * I/O
 * 		   -------------
 * 		  |			P1.0|--> Red LED
 * 		  |			P1.1|--> Green LED
 * 		  |			P1.2|--> Yellow LED
 *	M0 <--|P3.3		P1.3|--> Blue LED
 * 	M1 <--|P3.4			|
 * 	M2 <--|P3.5		P2.0|<-- B1 Button
 * 	M3 <--|P3.6		P2.1|<-- B2 Button
 * 	M4 <--|P3.7  	P2.2|<-- B3 Button
 * 		  |			P2.3|<-- B4 Button
 * 		  |			P2.4|<-- Minus Button
 * SCL <->|P1.6		P2.5|<-- Plus button
 * SDA <->|P1.7		P2.6|--> GAIN0 (Resistor Select)
 * 		  |			P2.7|--> GAIN1 (Resistor Select)
 * 		  |			P3.1|--> Enable LED
 *		   -------------
 *
 * Last Update: 3/12/2013
 *
 * *****************************************************************************
 * Button Descriptions 
 * *****************************************************************************
 * Mode Off (LEDs Off) - Alerts - Hold the button to repeat effect
 * B1: Alert1
 * B2: Alert2
 * B3: Alert3
 * B4: Alert4
 *
 * Mode 4 (M4 On) - Clicks
 * B1: Light, Short Click
 * B2: Heavy Click, Click-and-Release
 * B3: Popple, Click-and-Release
 * B4: Ramp / Transition, Click-and-Release
 * 
 * Mode 3 (M3 On) - Transition Effects
 * B1: Ramp up (200Hz)
 * B2: Bounce (250Hz)
 * B3: Click Bounce
 * B4: Pulse (Hold to repeat)
 * 
 * Mode 2 (M1 On) - FIFO Mode and RAM Mode Demonstration
 * B1: Sharp Click - FIFO Mode
 * B2: Bump - FIFO Mode
 * B3: Alert - FIFO Mode
 * B4: Robotic Click - RAM Mode
 *
 * Mode 1 (M2 On) - CopyCat / Simon Game - Game where you repeat the pattern
 * 1) Press any button (B1-B4) to start the game
 * 2) The game counts down and then shows a pattern of signals
 * 3) Once you have seen the pattern, repeat the pattern by touching the buttons
 * 		in the same order
 * 4) If you repeat the entire pattern correctly, the board will repeat the
 * 		pattern plus one additional signal.  Continue playing until you repeat
 * 		the sequence incorrectly.
 * 5) If you repeat the pattern incorrectly, the board will buzz and the game
 * 		is over.  The mode LEDs will scroll and then you score will appear in
 * 		binary.
 *
 * Mode 0 (M0 On) - Recorder
 * B1: Record
 * B2: Create Pattern
 * B3: Playback Pattern
 * B4: -
 *
 ******************************************************************************/
#include "msp430g2553.h"
#include "CapTouchBoard.h"
#include "Actuator_Waveforms.h"
#include "SimonGame.h"
#include "BinaryModes.h"
#include "LibraryModes.h"
#include "Accelerometer.h"
#include "Recorder.h"
#include "Haptics.h"

extern struct Element* modePtr;
extern struct Element* buttonPtr;
struct Element* lastButtonPtr;
uint8_t IsAnalogModeActive = 0;

const uint8_t buttonLeds[NUMBER_BUTTONS] =
{
  BIT0,
  BIT1,
  BIT2,
  BIT3
};

extern const struct Element* const buttonList[NUMBER_BUTTONS] =
{
  &button1,
  &button2,
  &button3,
  &button4
};            

unsigned int i;

// Main Function
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
  
  // Initialize MSP430 Clocks
  BCSCTL1 = CALBC1_8MHZ;                // Set DCO to 1, 8, 12 or 16MHz
  DCOCTL = CALDCO_8MHZ;					// Set MODx to 3
  BCSCTL1 |= DIVA_0;                    // ACLK / (0:1,1:2,2:4,3:8)
  BCSCTL2 |= DIVS_0;                    // SMCLK /(0:1,1:2,2:4,3:8)
  BCSCTL3 |= LFXT1S_2;                  // LFXT1 = VLO
  
  // Set GPIO functions
  P2SEL &= 0x3F;
  P2SEL |= 0x3F;						// P2.0-P2.5 = cap sense, P2.6-P2.7 = I/O
  P2SEL2 |= 0x3F;						// P2.0-P2.5 = cap sense, P2.6-P2.7 = I/O
  
  // Set GPIO directions
  P1DIR |= (BUTTON1+BUTTON2+BUTTON3+BUTTON4);		// Set P1.0-P1.3 to outputs for colored LEDs
  P2DIR |= 0xC0;									// Set P2.6-P2.7 = Output
  P3DIR |= (MODE0+MODE1+MODE2+MODE3+MODE4+BIT1);  	// Set P3.1 = EN LED (Output), P3.3-P3.7 = MODE0-MODE4 LEDs (Output)
  
  // Set GPIO values
  P1OUT = 0x00;							// Colored LEDs off
  P3OUT = 0x00;                    		// White LEDs off, EN = off
 
  // TimerA Setup
  // Set PWM Frequency,  TimerA gives 255 counts at SMCLK
  // Set PWM Frequency 8M/425 ~146 Hz*128
  TA1CCR0 = 0x00FE;               		
  
  CapTouch_Init();						// Initialize the cap touch board setting and status
  Haptics_Init();						// Initialize Haptics settings
  CapTouch_PowerUpSequence();			// CapTouch Board PowerUp Sequence
  Recorder_Init();						// Initialize Recorder
  
  // Main loop starts here
  while (1)
  {
	/* Detect Button Presses */
    modePtr = (struct Element *)TI_CAPT_Buttons(&mode_change);		// detect mode button press
    buttonPtr = (struct Element *)TI_CAPT_Buttons(&buttons);		// detect effect button press

    Recorder_Record();												// Recorder button press sequence recorder
    Recorder_Playback();											// Recorder play back

/********************************************************************************
 * Mode Buttons - this if statement determines the mode when "+" or "-" are
 * 					pressed.  This will most likely not need to be edited.
 ********************************************************************************/
    if(modePtr)
    {

		CapTouch_UnfreezeLEDs(); 		// If color LEDs are frozen, unfreeze
		Simon_Reset(); 					// If the mode button is pressed, cancel current Simon game

		if(CapTouch_isModeBtnReleased)		// if this is first time to register a mode button press (button is not being held)
		{
			CapTouch_isModeBtnReleased = 0; 	// Do not increment mode again until mode button is released and then pressed again

			LED_EN(OFF);						// Turn off EN LEDs

			if(IsAnalogModeActive)				// If in analog input mode, reset to digital mode
			{
				Haptics_SendWaveform(analogModeExit); 		// Reset hardware to default voltage
				IsAnalogModeActive = 0;						// Reset analogMode flag
			}

			/* Use the "+" and "-" buttons to change modes */
			if(modePtr == &mode_up)		// '+' button pressed
			{
				CapTouch_IncrementMode();
				CapTouch_EnableOnHold(CapTouch_SetBinaryMode);		// change mode counter style to binary
			}
			else						// '-' button pressed
			{
				CapTouch_DecrementMode();
				CapTouch_EnableOnHold(CapTouch_UnsetBinaryMode);	// change mode counter style to carousel
			}

			/* Perform special functions when entering a mode */
			if(CapTouch_GetBinaryMode() == 0)	// If in demo mode
			{
				if(CapTouch_mode == MODE0)	// if MODE0
				{
					sleep(500);
					Haptics_SendWaveform(analogMode150);	// Set DRV2667 and board to 150Vpp mode
					P1OUT |= (BUTTON1+BUTTON2+BUTTON3);		// Set LEDs to default gain and voltage setting
					CapTouch_FreezeLEDs();					// Freeze color LEDs to represent voltage level
					IsAnalogModeActive = 1;
				}
			}
			else if(CapTouch_GetBinaryMode() == BINARYMODESLEVEL)	// If in extra binary modes
			{
				if((CapTouch_mode >> 3) == 7)
				{
					sleep(500);
					Haptics_SendWaveform(analogMode150);	// Set DRV2667 and board to 150Vpp mode
					P1OUT |= (BUTTON1+BUTTON2+BUTTON3);		// Set LEDs to default gain and voltage setting
					CapTouch_FreezeLEDs();					// Freeze color LEDs to represent voltage level
					IsAnalogModeActive = 1;
				}
				else if((CapTouch_mode >> 3) == 30)			// Set the Button LEDs to the current boost seting
				{
					switch(Haptics_GetDefaultBoostVoltage())
					{
						case VBST_30:
							P1OUT |= BUTTON1;
						break;
						case VBST_55:
							P1OUT |= BUTTON1+BUTTON2;
						break;
						case VBST_80:
							P1OUT |= BUTTON1+BUTTON2+BUTTON3;
						break;
						case VBST_105:
							P1OUT |= BUTTON1+BUTTON2+BUTTON3+BUTTON4;
						break;
					}

					CapTouch_FreezeLEDs();					// Freeze color LEDs to represent voltage level
				}
			}

		}
    }
    else		// else no mode button is pressed
    {
    	CapTouch_ModeRepeatReset();			// Reset status and counters when repeat mode is finished
    	CapTouch_isModeBtnReleased = 1;		// Mode button has been released
    }

    if(!CapTouch_isModeLEDFrozen)
    	CapTouch_SetModeLEDs();			// Set Mode LEDs to current mode

    // Manage the colored LEDs
    if(!CapTouch_isLEDFrozen)		// if LEDs are not frozen
    	CapTouch_ButtonLEDsOff();

/********************************************************************************
 * Effect Buttons - set button functions below
 * 1) The mode can be determined by "CapTouch_mode == [mode]"
 * 2) The four button actions are in a switch statement.  Each button has a
 * 		separate case statement.
 ********************************************************************************/
    if(buttonPtr)		// if an effect button is pressed
    {
		P1OUT |= buttonPtr->referenceNumber;	// Turn on a colored LED for button B1-B4 pressed

		lastButtonPtr = buttonPtr;				// Store the button pressed as the lastButton

		if(CapTouch_isEffectBtnReleased)
		{
			CapTouch_isEffectBtnReleased = 0;		// Do not repeat effect

			if(CapTouch_GetBinaryMode() == 0)		// Check if binary mode is disable
			{
				if(CapTouch_mode == MODEOFF)		// Piezo Alert Effects
				{
					switch(buttonPtr->referenceNumber)
					{
					case BUTTON1:
					{
						Haptics_SendWaveform(alert1);
						break;
					}
					case BUTTON2:
					{
						Haptics_SendWaveform(alert2);
						break;
					}
					case BUTTON3:
					{
						Haptics_SendWaveform(alert3);	// Gallop
						break;
					}
					case BUTTON4:
					{
						Haptics_SendWaveform(alert4);	// Repeat Buzz
						break;
					}
					default: __no_operation();
					}
					CapTouch_RepeatOnHold(500);
				}
				else if(CapTouch_mode == MODE4)			// Click and Touch-and-Release Effects
				{
					switch(buttonPtr->referenceNumber)
					{
					case BUTTON1:
					{
						Haptics_SendWaveform(click300);	 		// Short click, no release effect
						break;
					}
					case BUTTON2:
					{
						Haptics_SendWaveform(click200);			// Click 200Hz + Release Click 150Hz
						break;
					}
					case BUTTON3:
					{
						Haptics_SendWaveform(click250);			// Click 250Hz + Release Click 200Hz
						break;
					}
					case BUTTON4:
					{
						Haptics_SendWaveform(transition100);	// Ramp up + Spring on release
						break;
					}
					default: __no_operation();
					}
				}
				else if(CapTouch_mode == MODE3)  	// Transitions & Bounces
				{
					switch(buttonPtr->referenceNumber)
					{
					case BUTTON1:
					{
						Haptics_SendWaveform(transition200);	// Ramp up
						break;
					}
					case BUTTON2:
					{
						Haptics_SendWaveform(bounce250);		// Bounce
						break;
					}
					case BUTTON3:
					{
						Haptics_SendWaveform(longClickBounce);	// Click Bounce
						break;
					}
					case BUTTON4:
					{
						Haptics_SendWaveform(flyBy);			// Pulsing
						CapTouch_RepeatOnHold(500);
						break;
					}
					default: __no_operation();
					}
				}
				else if(CapTouch_mode == MODE2)		// FIFO and RAM Effects
				{
					switch(buttonPtr->referenceNumber)
					{
					case BUTTON1:
					{
						Haptics_SendWaveform(mode1_tick);		// FIFO tick
						break;
					}
					case BUTTON2:
					{
						Haptics_SendWaveform(mode1_click);		// FIFO click
						break;
					}
					case BUTTON3:
					{
						Haptics_SendWaveform(mode1_alert);		// FIFO alert
						break;
					}
					case BUTTON4:
					{
						Haptics_SendWaveform(mode2_twotone);	// Two tone effect
						break;
					}
					default: __no_operation();
					}
				}
				else if(CapTouch_mode == MODE1)		// Simon Game
				{
					Simon_Play(buttonPtr->sequenceNumber);		// Begin Simon game
				}
				else if(CapTouch_mode == MODE0)		// Analog Mode
				{
					CapTouch_ButtonLEDsOff();		// Reset LEDs if new button is pressed

					switch(buttonPtr->referenceNumber)
					{
					case BUTTON1:	// 30V Boost, 50V Gain
					{
						Haptics_SendWaveform(analogMode50);
						P1OUT |= BUTTON1;
						LED_EN(ON);
						break;
					}
					case BUTTON2:	// 55V Boost, 100V Gain
					{
						Haptics_SendWaveform(analogMode100);
						P1OUT |= BUTTON1+BUTTON2;
						LED_EN(ON);
						break;
					}
					case BUTTON3:	// 80V Boost, 150V Gain
					{
						Haptics_SendWaveform(analogMode150);
						P1OUT |= BUTTON1+BUTTON2+BUTTON3;
						LED_EN(ON);
						break;
					}
					case BUTTON4:	// 105V Boost, 200V Gain
					{
						Haptics_SendWaveform(analogMode200);
						P1OUT |= BUTTON1+BUTTON2+BUTTON3+BUTTON4;
						LED_EN(ON);
						break;
					}
					default: __no_operation();
					}

					CapTouch_FreezeLEDs();


				}

			}	// End if(CapTouch_GetBinaryMode()...)
			else
			{
				#ifdef BINARYMODES_H_
					/* Include extra binary modes - life test, external PWM mode, etc. */
					BinaryModes();
				#endif

				#ifdef  LIBRARYMODES_H_
					LibraryModes();
				#endif

			}
		}
    } 	// End If (buttonPtr)
    else	// else effect button is released
    {
    	if(!CapTouch_isEffectBtnReleased)	// if button has just been released
    	{
			if(CapTouch_mode == MODE4) 		// Up button effects for MODE4
			{
				switch(lastButtonPtr->referenceNumber)
				{
				case BUTTON1:
				{

					break;
				}
				case BUTTON2:
				{
					Haptics_SendWaveform(click150);
					break;
				}
				case BUTTON3:
				{
					Haptics_SendWaveform(click200);
					break;
				}
				case BUTTON4:
				{
					Haptics_SendWaveform(clickBounce);
					break;
				}
				default: __no_operation();
				}
			}
    	}

    	CapTouch_RepeatReset();					// Reset status and counters when repeat mode is finished
    	CapTouch_isEffectBtnReleased = 1;		// Button was released
    }

    if(Haptics_IsEffectActive())	// If effect is playing
	{
		if(I2C_ReadSingleByte(DRV2667_CONTROL2) != GO)	// Check if GO bit is high
		{
			Haptics_EffectActive(0);
		}
		else
		{
			//timerdelay(10000);
		}
	}

    //sleep(500);
  } // End While Loop
} // End Main

/******************************************************************************/
// Timer0_A0 Interrupt Service Routine:       
/******************************************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_Timer0_A0(void)
{
  TA0CTL &= ~(MC_1);
  TA0CCTL0 &= ~(CCIE);

  CapTouch_RandomNumber++;

  __bic_SR_register_on_exit(LPM0_bits+GIE);
}


#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A1_ISR (void)
{
  TA1CCTL1 &= ~CCIFG;   //Clear Interrupt Flag
}
