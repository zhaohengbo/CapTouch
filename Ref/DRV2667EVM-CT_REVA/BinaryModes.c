/******************************************************************************
 * BinaryModes.h
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 * Author: a0866685
 *
 * Desc: This file contains the binary mode functions and effects. Include this
 * 		file to compile with extra modes.
 *
 ******************************************************************************/

#ifndef BINARYMODES_C_
#define BINARYMODES_C_

#include "BinaryModes.h"
#include "I2C.h"
#include "Haptics.h"
#include "Actuator_Waveforms.h"
#include "Recorder.h"
#include "CapTouchBoard.h"
#include "WaveformData.h"

#define DYNAMIC_OFFSET		0xC0
#define DYNAMIC1_OFFSET		0xC4
#define DYNAMIC1_REPEAT		0xBE

struct Element* modePtr;
struct Element* buttonPtr;

static unsigned char dynamicWaveform_amplitue = DYNAMIC_AMP;
static unsigned char dynamicWaveform_freq = DYNAMIC_FREQ;
static unsigned char dynamicWaveform_duration = DYNAMIC_DURATION;
static unsigned char dynamicWaveform_rampup = DYNAMIC_RAMPUP;
static unsigned char dynamicWaveform_rampdown = DYNAMIC_RAMPDOWN;
static unsigned char BinaryCapTouch_mode;

/**
 * BinaryModes - function containing the extra binary mode button functions and effects
 */
void BinaryModes(void)
{
	if(CapTouch_GetBinaryMode() == BINARYMODESLEVEL)
	{
		BinaryCapTouch_mode = CapTouch_mode >> 3;

		switch(BinaryCapTouch_mode)
		{
			unsigned char i;
			static char enableStorage = 0;

			static unsigned char sequencer1[9] = {0x03,0,0,0,0,0,0,0,0};
			static unsigned char sequencer2[9] = {0x03,0,0,0,0,0,0,0,0};

			case 0:		// Mode 0, GUI Mode
			{
				switch(buttonPtr->referenceNumber)
				{
				case BUTTON1:	// Enter Memory Storage
				{
					if(enableStorage == 1)
					{
						CapTouch_UnfreezeLEDs();
						enableStorage = 0;
					}
					else
					{
						CapTouch_FreezeModeLEDs();
						enableStorage = 1;
						CapTouch_ModeLEDsOn();
					}
					break;
				}
				case BUTTON2:	// Play/Set Sequencer 1
				{
					if(enableStorage == 1)
					{
						//I2C_ReadMultiByte(0x03,8,sequencer1);
						for(i = 0; i < 8; i++)
						{
							sequencer1[i+1] = I2C_ReadSingleByte(0x03 + i);
						}
					}

					if(Haptics_IsEffectActive())
					{
						Haptics_DisableTrigger();
					}
					else
					{
						I2C_WriteMultiByte(sequencer1,9);
						Haptics_EnableTrigger();
					}
					break;
				}
				case BUTTON3:	// Play/Set Sequencer 2
				{
					if(enableStorage == 1)
					{
						//I2C_ReadMultiByte(0x03,8,sequencer2);
						for(i = 0; i < 8; i++)
						{
							sequencer2[i+1] = I2C_ReadSingleByte(0x03 + i);
						}
					}

					if(Haptics_IsEffectActive())
					{
						Haptics_DisableTrigger();
					}
					else
					{
						I2C_WriteMultiByte(sequencer2,9);
						Haptics_EnableTrigger();
					}
					break;
				}
				case BUTTON4:	// Play Active Sequencer
				{
					if(Haptics_IsEffectActive())
					{
						Haptics_DisableTrigger();
					}
					else
					{
						Haptics_EnableTrigger();
					}
					break;
				}
				default: __no_operation();
				}
			}
			break;
			case 1:		// Mode 1, Dynamic Waveform Playback
			{
				switch(buttonPtr->referenceNumber)
				{
				case BUTTON1:	// Stop Effects
				{
					Haptics_DisableTrigger();	// Stop
					I2C_WriteSingleByte(DRV2667_PAGE, 0x01);
					I2C_WriteSingleByte(DYNAMIC1_REPEAT,0x01);
					I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
					break;
				}
				case BUTTON2:	// Play dynamic effect once
				{
					Haptics_DisableTrigger();
					I2C_WriteSingleByte(DRV2667_PAGE, 0x01);
					I2C_WriteSingleByte(DYNAMIC1_REPEAT,0x01);
					I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
					Haptics_SendWaveform(dynamicWaveformNoRepeat);
					break;
				}
				case BUTTON3:	// Dynamic effect infinite repeat
				{
					Haptics_DisableTrigger();
					I2C_WriteSingleByte(DRV2667_PAGE, 0x01);
					I2C_WriteSingleByte(DYNAMIC1_REPEAT,0x00);
					I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
					Haptics_SendWaveform(dynamicWaveformNoRepeat);
					break;
				}
				case BUTTON4:	// Dynamic effect repeat with 1s wait
				{
					Haptics_DisableTrigger();
					I2C_WriteSingleByte(DRV2667_PAGE, 0x01);
					I2C_WriteSingleByte(DYNAMIC1_REPEAT,0x01);
					I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
					Haptics_SendWaveform(dynamicWaveformNoRepeatWait1s);
					break;
				}
				default: __no_operation();
				}
			}
			break;
			case 2:		// Mode 2, Frequency Adjust
			{
				switch(buttonPtr->referenceNumber)
				{
				case BUTTON1:	// Continuous Buzz
				{
					if(Haptics_IsEffectActive())
					{
						Haptics_DisableTrigger();	// Stop
					}
					else
					{
						Haptics_SendWaveform(dynamicWaveform);
					}
					break;
				}
				case BUTTON2:	// Duration set in Mode 2
				{
					Haptics_DisableTrigger();
					Haptics_SendWaveform(dynamicWaveformNoRepeat);
					break;
				}
				case BUTTON3:	// Decrease Frequency
				{
					dynamicWaveform_freq--;
					I2C_WriteSingleByte(DRV2667_PAGE, 0x02);
					I2C_WriteSingleByte(DYNAMIC_OFFSET+1, dynamicWaveform_freq);
					I2C_WriteSingleByte(DYNAMIC1_OFFSET+1, dynamicWaveform_freq);
					I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
					Haptics_SendWaveform(dynamicWaveform);
					LED_EN(ON);
					CapTouch_RepeatOnHold(3000);
					break;
				}
				case BUTTON4:	// Increase Frequency
				{
					dynamicWaveform_freq++;
					I2C_WriteSingleByte(DRV2667_PAGE, 0x02);
					I2C_WriteSingleByte(DYNAMIC_OFFSET+1, dynamicWaveform_freq);
					I2C_WriteSingleByte(DYNAMIC1_OFFSET+1, dynamicWaveform_freq);
					I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
					Haptics_SendWaveform(dynamicWaveform);
					LED_EN(ON);
					CapTouch_RepeatOnHold(3000);
					break;
				}
				default: __no_operation();
				}
			}
			break;
			case 3:		// Mode 3, Amplitude Adjust
			{
				switch(buttonPtr->referenceNumber)
				{
				case BUTTON1:	// Continuous Buzz
				{
					if(Haptics_IsEffectActive())
					{
						Haptics_DisableTrigger(); 		// Stop
					}
					else
					{
						Haptics_SendWaveform(dynamicWaveform);
					}
					break;
				}
				case BUTTON2:	// Duration set in Mode 2
				{
					Haptics_DisableTrigger();
					Haptics_SendWaveform(dynamicWaveformNoRepeat);
					break;
				}
				case BUTTON3:	// Decrease Amplitude
				{
					if(dynamicWaveform_amplitue > 0)
					{
						dynamicWaveform_amplitue--;
						I2C_WriteSingleByte(DRV2667_PAGE, 0x02);
						I2C_WriteSingleByte(DYNAMIC_OFFSET, dynamicWaveform_amplitue);
						I2C_WriteSingleByte(DYNAMIC1_OFFSET, dynamicWaveform_amplitue);
						I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
						Haptics_SendWaveform(dynamicWaveform);
						LED_EN(ON);
					}
					break;
				}
				case BUTTON4:	// Increase Amplitude
				{
					if(dynamicWaveform_amplitue < 0xFF)
					{
						dynamicWaveform_amplitue++;
						I2C_WriteSingleByte(DRV2667_PAGE, 0x02);
						I2C_WriteSingleByte(DYNAMIC_OFFSET, dynamicWaveform_amplitue);
						I2C_WriteSingleByte(DYNAMIC1_OFFSET, dynamicWaveform_amplitue);
						I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
						Haptics_SendWaveform(dynamicWaveform);
						LED_EN(ON);
					}
					break;
				}
				default: __no_operation();
				}
			}
			break;
			case 4:		// Mode 4, Duration Adjust
			{
				switch(buttonPtr->referenceNumber)
				{
				case BUTTON1:	// Continuous Buzz
				{
					if(Haptics_IsEffectActive())
					{
						Haptics_DisableTrigger(); 		// Stop
					}
					else
					{
						Haptics_SendWaveform(dynamicWaveform);
					}
					break;
				}
				case BUTTON2:	// Duration set in Mode 2
				{
					Haptics_DisableTrigger();
					Haptics_SendWaveform(dynamicWaveformNoRepeat);
					break;
				}
				case BUTTON3:	// Decrease Duration
				{
					if(dynamicWaveform_duration > 0)
					{
						dynamicWaveform_duration--;
						I2C_WriteSingleByte(DRV2667_PAGE, 0x02);
						I2C_WriteSingleByte(DYNAMIC1_OFFSET+2, dynamicWaveform_duration);
						I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
						Haptics_SendWaveform(dynamicWaveformNoRepeat);
					}
					break;
				}
				case BUTTON4:	// Increase Duration
				{
					if(dynamicWaveform_duration < 0xFF)
					{
						dynamicWaveform_duration++;
						I2C_WriteSingleByte(DRV2667_PAGE, 0x02);
						I2C_WriteSingleByte(DYNAMIC1_OFFSET+2, dynamicWaveform_duration);
						I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
						Haptics_SendWaveform(dynamicWaveformNoRepeat);
					}
					break;
				}
				default: __no_operation();
				}
			}
			break;
			case 5:		// Mode 5, Ramp Up Adjust
			{
				switch(buttonPtr->referenceNumber)
				{
				case BUTTON1:	// Continuous Buzz
				{
					if(Haptics_IsEffectActive())
					{
						Haptics_DisableTrigger(); 		// Stop
					}
					else
					{
						Haptics_SendWaveform(dynamicWaveform);
					}
					break;
				}
				case BUTTON2:	// Duration set in Mode 2
				{
					Haptics_DisableTrigger();
					Haptics_SendWaveform(dynamicWaveformNoRepeat);
					break;
				}
				case BUTTON3:	// Decrease Duration
				{
					if(dynamicWaveform_rampup > 0)
					{
						dynamicWaveform_rampup--;
						I2C_WriteSingleByte(DRV2667_PAGE, 0x02);
						I2C_WriteSingleByte(DYNAMIC1_OFFSET+3, ((dynamicWaveform_rampup << 4) & 0xF0) | (dynamicWaveform_rampdown & 0x0F));
						I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
						Haptics_SendWaveform(dynamicWaveformNoRepeat);
					}
					break;
				}
				case BUTTON4:	// Increase Duration
				{
					if(dynamicWaveform_rampup < 15)
					{
						dynamicWaveform_rampup++;
						I2C_WriteSingleByte(DRV2667_PAGE, 0x02);
						I2C_WriteSingleByte(DYNAMIC1_OFFSET+3, ((dynamicWaveform_rampup << 4) & 0xF0) | (dynamicWaveform_rampdown & 0x0F));
						I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
						Haptics_SendWaveform(dynamicWaveformNoRepeat);
					}
					break;
				}
				default: __no_operation();
				}
			}
			break;
			case 6:		// Mode 6, Ramp Down Adjust
			{
				switch(buttonPtr->referenceNumber)
				{
				case BUTTON1:	// Continuous Buzz
				{
					if(Haptics_IsEffectActive())
					{
						Haptics_DisableTrigger(); 		// Stop
					}
					else
					{
						Haptics_SendWaveform(dynamicWaveform);
					}
					break;
				}
				case BUTTON2:	// Duration set in Mode 2
				{
					Haptics_DisableTrigger();
					Haptics_SendWaveform(dynamicWaveformNoRepeat);
					break;
				}
				case BUTTON3:	// Decrease Duration
				{
					if(dynamicWaveform_rampdown > 0)
					{
						dynamicWaveform_rampdown--;
						I2C_WriteSingleByte(DRV2667_PAGE, 0x02);
						I2C_WriteSingleByte(DYNAMIC1_OFFSET+3, ((dynamicWaveform_rampup << 4) & 0xF0) | (dynamicWaveform_rampdown & 0x0F));
						I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
						Haptics_SendWaveform(dynamicWaveformNoRepeat);
					}
					break;
				}
				case BUTTON4:	// Increase Duration
				{
					if(dynamicWaveform_rampdown < 15)
					{
						dynamicWaveform_rampdown++;
						I2C_WriteSingleByte(DRV2667_PAGE, 0x02);
						I2C_WriteSingleByte(DYNAMIC1_OFFSET+3, ((dynamicWaveform_rampup << 4) & 0xF0) | (dynamicWaveform_rampdown & 0x0F));
						I2C_WriteSingleByte(DRV2667_PAGE, 0x00);
						Haptics_SendWaveform(dynamicWaveformNoRepeat);
					}
					break;
				}
				default: __no_operation();
				}
			}
			break;
			case 7:		// Mode 7, Analog Mode
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
			break;
			case 8:
			{
				switch(buttonPtr->referenceNumber)
				{
				case BUTTON1:
				{
					Recorder_RecordToggle();
					break;
				}
				case BUTTON2:
				{

					break;
				}
				case BUTTON3:
				{
					Recorder_PlayToggle();
					break;
				}
				case BUTTON4:
				{
					break;
				}
				default: __no_operation();
				}
			}
			break;
			case 30:	// Mode 30, Set default boost voltage and DRV2667 gain setting
			{
				CapTouch_ButtonLEDsOff();		// Reset LEDs if new button is pressed

				switch(buttonPtr->referenceNumber)
				{
					case BUTTON1:
					{
						Haptics_SetDefaultBoostVoltage(VBST_30);	// Set boost voltage
						Haptics_SetGain(GAIN_50);					// Set DRV2667 gain
						Haptics_SendWaveform(click150);				// Send click
						P1OUT |= BUTTON1;							// Set button LEDs
						break;
					}
					case BUTTON2:
					{
						Haptics_SetDefaultBoostVoltage(VBST_55);	// Set boost voltage
						Haptics_SetGain(GAIN_100);					// Set DRV2667 gain
						Haptics_SendWaveform(click150);				// Send click
						P1OUT |= BUTTON1+BUTTON2;					// Set button LEDs
						break;
					}
					case BUTTON3:
					{
						Haptics_SetDefaultBoostVoltage(VBST_80);	// Set boost voltage
						Haptics_SetGain(GAIN_150);					// Set DRV2667 gain
						Haptics_SendWaveform(click150);				// Send click
						P1OUT |= BUTTON1+BUTTON2+BUTTON3;			// Set button LEDs
						break;
					}
					case BUTTON4:
					{
						Haptics_SetDefaultBoostVoltage(VBST_105);	// Set boost voltage
						Haptics_SetGain(GAIN_200);					// Set DRV2667 gain
						Haptics_SendWaveform(click150);				// Send click
						P1OUT |= BUTTON1+BUTTON2+BUTTON3+BUTTON4;	// Set button LEDs
						break;
					}
					default: __no_operation();
				}

				CapTouch_FreezeLEDs();
			}
			break;
			case 31:
			{
				switch(buttonPtr->referenceNumber)
				{
					case BUTTON1:
					{
						CapTouch_FlashDeviceID();		// Flash device ID
						break;
					}
					case BUTTON2:
					{
						CapTouch_FlashDeviceRevision();	// Flash device revision
						break;
					}
					case BUTTON3:
					{
						CapTouch_FlashCodeRevision();	// Flash Software Code Revision
						break;
					}
					case BUTTON4:
					{

						break;
					}
					default: __no_operation();
				}
			}
			break;
			default:
			{

			}
			break;
		};	/* switch(CapTouch_mode) */
	}  /* if (...BINARYMODESLEVEL) */
}


#endif /* EXTRAMODES_C_ */
