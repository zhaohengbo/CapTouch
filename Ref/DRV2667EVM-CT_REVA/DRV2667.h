/*
 * DRV2667.h
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 * Author: a0866685
 *
 * Description: This header file contains the addresses and constants to
 * 		used for the DRV2667 register map.
 *
 */

#ifndef DRV2667_H_
#define DRV2667_H_

// DRV2667 Register Constants
#define DRV2667_STATUS      0x00	// Status Register
#define DRV2667_CONTROL     0x01	// Control Register 1
#define DRV2667_CONTROL2    0x02	// Control Register 2
#define DRV2667_WAVSEQ0     0x03	// Waveform Sequence Register 0
#define DRV2667_WAVSEQ1     0x04	// Waveform Sequence Register 1
#define DRV2667_WAVSEQ2     0x05	// Waveform Sequence Register 2
#define DRV2667_WAVSEQ3     0x06	// Waveform Sequence Register 3
#define DRV2667_WAVSEQ4     0x07	// Waveform Sequence Register 4
#define DRV2667_WAVSEQ5     0x08	// Waveform Sequence Register 5
#define DRV2667_WAVSEQ6     0x09	// Waveform Sequence Register 6
#define DRV2667_WAVSEQ7     0x0A	// Waveform Sequence Register 7
#define DRV2667_FIFO        0x0B	// Beginning of FIFO
#define DRV2667_PAGE        0xFF	// Page Select Register

// DRV2667 Register1 Constants
#define INPUT_MUX           0x04	// Input Mux Register
#define GAIN_50	    		0x00	// 50Vpp, 28.8dB
#define GAIN_100	    	0x01	// 100Vpp, 34.8dB
#define GAIN_150	    	0x02	// 150Vpp, 38.4dB
#define GAIN_200	    	0x03	// 200Vpp, 40.7dB

// DRV2667 Register2 Constants
#define STANDBY	            0x40	// Standby Mode
#define EN_OVERRIDE         0x02	// Permanently enable DRV2667
#define GO	            	0x01	// Begin Waveform Sequence
#define STOP				0x00	// Stop waveform play back

// DRV2667 Registers
#define REG_0				0x00	// Register 0

#endif /* DRV2667_H_ */
