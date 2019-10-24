/******************************************************************************
 * Haptics.c
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 * Author: a0866685
 *
 * Description: This file contains the functions for sending haptics waveforms.
 * 		Edit this file when changing actuators.
 *
 * @TODO - Update this file when changing between actuators types
 * 			(ERM, LRA, Piezo)
 *
 ******************************************************************************/

#ifndef HAPTICS_H_
#define HAPTICS_H_

#include "msp430g2553.h"
#include "CTS/structure.h"
#include "I2C.h"
#include "Timer.h"
#include "WaveformData.h"
#include "DRV2667.h"

// Settings
// @TODO - Adjust output voltage for the Piezo Actuator
#define GAIN			GAIN_150	// DRV2667 I2C GAIN Setting (see DRV2667.h)
#define VBST			VBST_80		// Boost voltage set by feedback resistors (VBST = GAIN / 2 + 5V)
#define VPEAK			100			// Peak Voltage %, a percent of the maximum output voltage (VOUT = GAIN * VPEAK (%))

// I2C and Timing Constants
#define DRV2667_ADDR	0x59		// I2C address of DRV2667
#define TICKDELAY 		31900		// TICKDELAY for calculating piezo frequency
#define MAX_PACKET_SZ   40			// MODE1 Packet size (default = 40)

// VBST Constants
#define VBST_30		0x00	// VBST = 30V, 	OUT = 50Vpp
#define VBST_55		0x40	// VBST = 55V, 	OUT = 100Vpp
#define VBST_80		0x80	// VBST = 80V, 	OUT = 150Vpp
#define	VBST_105	0xC0	// VBST = 105V, OUT = 200Vpp

// DRV2667 Board Modes
#define PIEZO_MODE1				1	// Streaming Mode
#define PIEZO_MODE2				2	// RAM Mode
#define PIEZO_MODE3				3	// Waveform Generator Mode
#define PIEZO_ANALOGMODE50		4	// Analog Input Modes, 50Vpp
#define PIEZO_ANALOGMODE100		5	// 100Vpp Output
#define PIEZO_ANALOGMODE150		6	// 150Vpp Output
#define PIEZO_ANALOGMODE200		7	// 200Vpp Output
#define PIEZO_ANALOGOFF			8	// Turn off analog mode

// Frequency settings for DRV2665/7 Mode 1
#define MODE1_FREQ100	3
#define MODE1_FREQ133	4
#define MODE1_FREQ166	5
#define MODE1_FREQ200	6
#define MODE1_FREQ233	7
#define MODE1_FREQ266	8
#define MODE1_FREQ300	9
#define MODE1_FREQ333	10

// Waveform Structure Type Definition
typedef struct Haptics_Waveform {
	const unsigned char 	outputMode; 		// ERM, LRA_AUTOON, or LRA_AUTOOFF (see output modes above)
	const unsigned char		length;				// size of array in bytes
	const unsigned char* 	data;				// pointer to waveform array data (waveform array is in (amplitude, time) pairs
} Waveform;

/**
 * Haptics_Init - initialize haptics variables and settings
 */
void Haptics_Init(void);

/**
 * Haptics_SendWaveform - setup and send haptic waveform
 * @param struct Waveform - the waveform output type, length in bytes, and data)
 */
void Haptics_SendWaveform(const Waveform waveform);

/**
 * Haptics_SendWaveformPriority - setup and send haptic waveform, status flags do not affect this method
 * @param struct Waveform - the waveform output type, length in bytes, and data)
 */
void Haptics_SendWaveformPriority(const Waveform waveform);

/**
 * Haptics_OutputWaveform - control the PWM output pattern
 * @param struct Waveform - the waveform output type, length in bytes, and data
 * @TODO - Modify this function to change actuator types (ERM, LRA, Piezo)
 */
void Haptics_OutputWaveform(const Waveform waveform);

/**
 * Haptics_HardwareMode - Set the hardware pins to the appropriate setting
 * @param unsigned char outputMode - the waveform output type
 * @TODO - Modify this function to change actuator types (ERM, LRA, Piezo)
 */
void Haptics_HardwareMode(uint8_t outputMode);

/**
 * Haptics_SetDefaultBoostVoltage - sets the board's boost voltage (30V, 55V, 80V, or 105V)
 * @param boostVoltage - select from the four VBST_xx constants in haptics.h
 */
void Haptics_SetDefaultBoostVoltage(unsigned char boostVoltage);

/**
 * Haptics_GetDefaultBoostVoltage - get default boost voltage
 * @return unsigned char VBST_xx constant
 */
unsigned char Haptics_GetDefaultBoostVoltage();

/**
 * Haptics_SetGain - set the gain register of the DRV2667 (50V, 100V, 150V, or 200V)
 * @param gain - select from the four GAIN_xx constants in DRV2667.h
 */
void Haptics_SetGain(unsigned char gain);

/**
 * Haptics_EnableTrigger - Enable the GO bit in the DRV2667 via I2C
 */
void Haptics_EnableTrigger();

/**
 * Haptics_DisableTrigger - Disable the GO bit in the DRV2667 via I2C
 */
void Haptics_DisableTrigger();

/**
 * Haptics_OutputEnableSet - enable/disable play back of all waveforms
 */
void Haptics_OutputEnableSet(uint8_t enable);

/**
 * Haptics_EffectActive - set if effect is active
 * @param unsigned char active - set activity status
 */
void Haptics_EffectActive(uint8_t active);

/**
 * Haptics_IsEffectActive - check if effect is active
 * @return unsigned char - 0  = inactive, 1 = active
 */
uint8_t Haptics_IsEffectActive(void);

/**
 * Haptics_IsOutputEnabled - check if waveform play back is enabled
 */
uint8_t Haptics_IsOutputEnabled(void);

#endif /* HAPTICS_H_ */
