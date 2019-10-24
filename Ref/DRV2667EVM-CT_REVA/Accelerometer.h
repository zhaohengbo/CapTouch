/*
 * Accelerometer.h
 *
 *  Created on: Jun 16, 2012
 *      Author: Brian
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "CapTouchBoard.h"

#define ACCEL_ADDR  	0x53		// Accelerometer I2C Address

#define DEVID			0x01
#define THRESH_TAP 		0x1D		// Tap threshold
#define OFSX			0x1E
#define OFSY			0x1F
#define OFSZ 			0x20
#define DUR 			0x21
#define LATENT			0x22
#define WINDOW			0x23
#define THRESH_ACT 		0x24		// Activity threshold
#define THRESH_INACT 	0x25
#define TIME_INACT 		0x26
#define ACT_INACT_CTL	0x27
#define THRESH_FF		0x28
#define TIME_FF			0x29
#define TAP_AXES		0x2A
#define ACT_TAP_STATUS	0x2B
#define BW_RATE 		0x2C		// Bandwidth
#define POWER_CTL		0x2D		// Power, Measure mode
#define INT_ENABLE		0x2E
#define INT_MAP			0x2F
#define INT_SOURCE		0x30
#define DATA_FORMAT		0x31		// Data format control
#define DATAX0			0x32		// Data registers
#define DATAX1 			0x33
#define DATAY0			0x34
#define DATAY1			0x35
#define DATAZ0			0x36
#define DATAZ1			0x37
#define FIFO_CTL		0x38		// FIFO Control
#define FIFO_STATUS 	0x39		// FIFO Status

#define BW_1600	0x0F
#define BW_800	0x0E
#define BW_400 	0x0D
#define BW_200	0x0C
#define BW_100	0x0B
#define BW_50 	0x0A
#define BW_25	0x09
#define BW_12	0x08
#define BW_6	0x07
#define BW_3	0x06

/* Statuses */
#define DATA_READY 0x07
#define SINGLE_TAP 0x06
#define DOUBLE_TAP 0x05
#define ACTIVITY   0x04
#define INACTIVITY 0x03
#define FREE_FALL  0x02
#define WATERMARK  0x01
#define OVERRUNY   0x00

/**
 * Accelerometer_Init - initialize accelerometer
 */
void Accelerometer_Init();

/**
 * Accelerometer_ReadFIFO - read the accelerometer X,Y,and Z values
 */
void Accelerometer_ReadFIFO();

void Accelerometer_PowerOn();
void Accelerometer_SetActivityAll(uint8_t state);

void Accelerometer_SetClocks();
void Accelerometer_ResetClocks();

#endif /* ACCELEROMETER_H_ */
