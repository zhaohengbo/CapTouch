#ifndef USCI_LIB
#define USCI_LIB

#include "CTS/structure.h"

#define SDA_PIN 			0x80    	// SDA Pin
#define SCL_PIN 			0x40		// SCL Pin

// I2C Timing
#define BUSSPEED			I2C_FAST	// I2C Bus Speed
#define I2C_FAST  			0x12		// I2C Fast
#define I2C_NORMAL			0x15		// I2C Normal (100kHz)
#define TRANSACTIONDELAY	150			// Delay between transactions (typically 100)

// Set the slave address
void I2C_SetSlaveAddress(unsigned char address);

/**
 * TI_USCI_I2C_Write_SingleByte
 * @param 	regAddr - register address inside the device
 * @param	data - byte of data to be transmitted
 */
void I2C_WriteSingleByte(uint8_t regAddr, uint8_t data);

/**
 * TI_USCI_I2C_Write_MultiByte
 * @param 	i2cArray - pointer to I2C data array
 * @param	numBytes - number of bytes in i2cArray
 */
void I2C_WriteMultiByte(uint8_t *i2cArray, uint16_t numBytes);

unsigned char I2C_ReadSingleByte(unsigned char regAddress);
void I2C_ReadMultiByte(uint8_t regAddr, uint8_t numBytes, uint8_t *data);

unsigned char I2C_IsNAK(void);
unsigned int I2C_GetNAKValue(void);
unsigned char I2C_IsStopSet(void);

void TI_USCI_I2C_receiveinit(unsigned char slave_address, unsigned char prescale);
void TI_USCI_I2C_transmitinit(unsigned char slave_address, unsigned char prescale);

void TI_USCI_I2C_receive(unsigned char byteCount, unsigned char *field);
void TI_USCI_I2C_transmit(uint16_t byteCount, unsigned char *field);

unsigned char TI_USCI_I2C_slave_present(unsigned char slave_address);
unsigned char TI_USCI_I2C_notready();

#endif
