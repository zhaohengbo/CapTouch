/*
 * Accelerometer.c
 *
 *  Created on: Jun 16, 2012
 *      Author: Brian
 */

#include "Accelerometer.h"
#include "I2C.h"
#include "CapTouchBoard.h"

/**
 * Accelerometer_Init - initialize accelerometer
 */
void Accelerometer_Init()
{
	uint8_t accelMeasurement[7];

	Accelerometer_SetClocks();

	I2C_SetSlaveAddress(ACCEL_ADDR);			// Set accelerometer slave address

	Accelerometer_PowerOn();					// Power on accelerometer
	I2C_WriteSingleByte(THRESH_ACT, 75);		// 62.5mg per increment
	I2C_WriteSingleByte(THRESH_INACT, 75);		// 62.5mg per increment
	I2C_WriteSingleByte(TIME_INACT,10);			// Inactive after 10s of no activity

	Accelerometer_SetActivityAll(1);

	I2C_WriteSingleByte(THRESH_TAP, 0x40);		// 4G
	I2C_WriteSingleByte(THRESH_ACT, 0x40);		// 4G
	I2C_WriteSingleByte(BW_RATE, 0x0D);			// 400Hz bandwidth
	I2C_WriteSingleByte(DATA_FORMAT,0x01);		// 4G range
	I2C_WriteSingleByte(FIFO_CTL, 0x5F);		// FIFO mode, 32 sample trigger

	I2C_ReadMultiByte(DATAX0,6,accelMeasurement);	// Read data
	CapTouch_ButtonLEDOnSequence();
	I2C_ReadMultiByte(DATAX0,6,accelMeasurement);	// Read data

	Accelerometer_ResetClocks();


}

/**
 *	Accelerometer_PowerOn - power on the accelerometer and set to normal
 */
void Accelerometer_PowerOn()
{
	Accelerometer_SetClocks();

	I2C_WriteSingleByte(POWER_CTL, 0);
	I2C_WriteSingleByte(POWER_CTL, 16);
	I2C_WriteSingleByte(POWER_CTL, 8);

	Accelerometer_ResetClocks();
}

/**
 * Accelerometer_ReadFIFO - read the accelerometer X,Y,and Z values
 */
void Accelerometer_ReadFIFO()
{
	uint8_t accelMeasurement[7];

	Accelerometer_SetClocks();

	I2C_SetSlaveAddress(ACCEL_ADDR);				// Set accelerometer slave address

	//I2C_WriteSingleByte(FIFO_CTL, 0x5F);			// FIFO mode, 32 sample trigger
	I2C_ReadMultiByte(DATAX0,6,accelMeasurement);	// Read data

	Accelerometer_ResetClocks();

	CapTouch_ButtonLEDOnSequence();
}

void Accelerometer_SetActivityAll(uint8_t state)
{
	state == 1 ? I2C_WriteSingleByte(ACT_INACT_CTL, 0x70) : I2C_WriteSingleByte(ACT_INACT_CTL, 0x00);
}

void Accelerometer_SetClocks()
{
	BCSCTL2 = DIVS_0;
}

void Accelerometer_ResetClocks()
{
	BCSCTL2 |= DIVS_3;                    // SMCLK/(0:1,1:2,2:4,3:8)
}
