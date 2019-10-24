//******************************************************************************
//   MSP430 USCI I2C Transmitter and Receiver
//
//  Description: This code configures the MSP430's USCI module as 
//  I2C master capable of transmitting and receiving bytes. 
//
//  ***THIS IS THE MASTER CODE***
//
//                    Master                   
//                 MSP430F2619             
//             -----------------          
//         /|\|              XIN|-   
//          | |                 |     
//          --|RST          XOUT|-    
//            |                 |        
//            |                 |        
//            |                 |       
//            |         SDA/P3.1|------->
//            |         SCL/P3.2|------->
//
// Note: External pull-ups are needed for SDA & SCL
//
// Uli Kretzschmar
// Texas Instruments Deutschland GmbH
// November 2007
// Built with IAR Embedded Workbench Version: 3.42A
//******************************************************************************
#include "msp430g2553.h"
#include "I2C.h"
#include "Timer.h"

uint16_t byteCtr;
uint16_t left_count;
unsigned char NAK_Occurred;
unsigned char *TI_receive_field;
unsigned char *TI_transmit_field;

static unsigned char slaveAddr = 0x59;			// Default slave address
static unsigned char MultiByteReadFG = 0; 		// Mutli-byte Read status flag


void I2C_Init()
{
}

/*!
 *******************************************************************************
 *
 *  @fn       void TI_USCI_I2C_SetSlaveAddress(unsigned char address)
 *
 *  @brief    I2C Master Write Single Byte
 *
 *  @param	  address - slave address
 *
 *  @note     SMCLK Divider Must be 0 for this to work properly
 *
 *******************************************************************************
 */
void I2C_SetSlaveAddress(uint8_t address)
{
	while(TI_USCI_I2C_notready());
	slaveAddr = address;
}

/*!
 *******************************************************************************
 *
 *  @fn       void I2C_Write_Single(unsigned char devAddr,
 *                                  unsigned char devData)
 *
 *  @brief    I2C Master Write Single Byte
 *
 *  @note     SMCLK Divider Must be 0 for this to work properly
 *
 *******************************************************************************
 */
void I2C_WriteSingleByte(uint8_t regAddr, uint8_t data)
{
    unsigned char tmp_i2cArray[2];

    tmp_i2cArray[0] = regAddr;
    tmp_i2cArray[1] = data;
    I2C_WriteMultiByte(tmp_i2cArray, 2);
}

/*!
 *******************************************************************************
 *
 *  @fn       void I2C_Write_Multi(unsigned char *i2cArray,
 *                                unsigned int numBytes)
 *
 *  @brief    I2C Master Write Multi Byte
 *
 *  @note     SMCLK Divider Must be 0 for this to work properly
 *
 *******************************************************************************
 */
void I2C_WriteMultiByte(uint8_t *i2cArray, uint16_t numBytes)
{
    TI_USCI_I2C_transmitinit(slaveAddr, BUSSPEED);  // init transmitting with
    while ( TI_USCI_I2C_notready() );         		// wait for bus to be free
    TI_USCI_I2C_transmit(numBytes,i2cArray);       	// start transmitting
    __bis_SR_register(LPM0_bits+GIE);
    timerdelay(TRANSACTIONDELAY);                   //1 ms Startup delay
}


/*******************************************************************************
 *
 *  @fn       void I2C_ReadSingleByte(unsigned char regAddress)
 *
 *  @brief    I2C Master Read Single Byte
 *
 *  @note     SMCLK Divider Must be 0 for this to work properly
 *
 *******************************************************************************/
unsigned char I2C_ReadSingleByte(uint8_t regAddress)
{
    uint8_t readArray[2];
    uint8_t tmp_i2cArray[1];
    uint8_t returnValue;

    tmp_i2cArray[0] = regAddress;
    I2C_WriteMultiByte(tmp_i2cArray, 1);
    TI_USCI_I2C_receiveinit(slaveAddr, BUSSPEED);
    while (TI_USCI_I2C_notready());
    TI_USCI_I2C_receive(1, readArray);
    __bis_SR_register(LPM0_bits+GIE);
    timerdelay(TRANSACTIONDELAY);			//1 ms Startup delay

    /* Return val */
    returnValue = readArray[0];
    return returnValue;
}

void I2C_ReadMultiByte(uint8_t regAddr, uint8_t numBytes, uint8_t *data)
{
	uint8_t startAddress[] = {regAddr};

	while(MultiByteReadFG || TI_USCI_I2C_notready());

	MultiByteReadFG = 1;							// Multi-byte transaction starting

	I2C_WriteMultiByte(startAddress,1);				// Write the read register to slave device

	TI_USCI_I2C_receiveinit(slaveAddr, BUSSPEED);	// Initialize I2C read operation
	while (TI_USCI_I2C_notready());					// Wait for bus to be available
	TI_USCI_I2C_receive(numBytes, data);			// I2C read
	timerdelay(TRANSACTIONDELAY);					// 1 ms Startup delay
}

unsigned char IsMultiByteRead()
{
	return MultiByteReadFG;
}


unsigned char I2C_IsNAK(void)
{
    unsigned char temp;
    temp = NAK_Occurred;
    NAK_Occurred = 0;
    return(temp);
}

unsigned int I2C_GetNAKValue(void)
{
    return(left_count);
}

unsigned char I2C_IsStopSet(void)
{
	return (UCB0CTL1 && UCTXSTP);
}

//------------------------------------------------------------------------------
// void TI_USCI_I2C_receiveinit(unsigned char slave_address, 
//                              unsigned char prescale)
//
// This function initializes the USCI module for master-receive operation. 
//
// IN:   unsigned char slave_address   =>  Slave Address
//       unsigned char prescale        =>  SCL clock adjustment 
//-----------------------------------------------------------------------------
void TI_USCI_I2C_receiveinit(unsigned char slave_address, 
                             unsigned char prescale){
  P1SEL |= SDA_PIN + SCL_PIN;                 // Assign I2C pins to USCI_B0
  P1SEL2 |= SDA_PIN + SCL_PIN;                // Assign I2C pins to USCI_B0
  UCB0CTL1 = UCSWRST;                         // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;       // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;              // Use SMCLK, keep SW reset
  UCB0BR0 = prescale;                         // Set pre-scaler
  UCB0BR1 = 0;
  UCB0I2CSA = slave_address;                  // set slave address
  UCB0CTL1 &= ~UCSWRST;                       // Clear SW reset, resume operation
  UCB0I2CIE = UCNACKIE;
  IE2 = UCB0RXIE;                             // Enable RX interrupt
}

//------------------------------------------------------------------------------
// void TI_USCI_I2C_transmitinit(unsigned char slave_address, 
//                               unsigned char prescale)
//
// This function initializes the USCI module for master-transmit operation. 
//
// IN:   unsigned char slave_address   =>  Slave Address
//       unsigned char prescale        =>  SCL clock adjustment 
//------------------------------------------------------------------------------
void TI_USCI_I2C_transmitinit(unsigned char slave_address, 
                          unsigned char prescale){
  P1SEL |= SDA_PIN + SCL_PIN;                // Assign I2C pins to USCI_B0
  P1SEL2 |= SDA_PIN + SCL_PIN;               // Assign I2C pins to USCI_B0
  UCB0CTL1 = UCSWRST;                        // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;      // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;             // Use SMCLK, keep SW reset
  UCB0BR0 = prescale;                        // set prescaler
  UCB0BR1 = 0;
  UCB0I2CSA = slave_address;                 // Set slave address
  UCB0CTL1 &= ~UCSWRST;                      // Clear SW reset, resume operation
  UCB0I2CIE = UCNACKIE;						 // Enable NACK Interrupt
  IE2 = UCB0TXIE;                            // Enable TX ready interrupt
}

//------------------------------------------------------------------------------
// void TI_USCI_I2C_receive(unsigned char byteCount, unsigned char *field)
//
// This function is used to start an I2C commuincation in master-receiver mode. 
//
// IN:   unsigned char byteCount  =>  number of bytes that should be read
//       unsigned char *field     =>  array variable used to store received data
//------------------------------------------------------------------------------
void TI_USCI_I2C_receive(unsigned char byteCount, unsigned char *field){
  TI_receive_field = field;
  if ( byteCount == 1 ){
    byteCtr = 0 ;
    __disable_interrupt();					  // Disable I2C Interrupts
    UCB0CTL1 |= UCTXSTT;                      // Send start condition
    while (UCB0CTL1 & UCTXSTT);               // Start condition sent?
    UCB0CTL1 |= UCTXSTP;                      // Send stop condition
    __enable_interrupt();
  } else if ( byteCount > 1 ) {
    byteCtr = byteCount - 1 ;
    UCB0CTL1 |= UCTXSTT;                      // Send I2C start condition
  } else
    while (1);                                // illegal parameter
}

//------------------------------------------------------------------------------
// void TI_USCI_I2C_transmit(unsigned char byteCount, unsigned char *field)
//
// This function is used to start an I2C commuincation in master-transmit mode. 
//
// IN:   unsigned char byteCount  =>  number of bytes that should be transmitted
//       unsigned char *field     =>  array variable. Its content will be sent.
//------------------------------------------------------------------------------
void TI_USCI_I2C_transmit(uint16_t byteCount, unsigned char *field){
  TI_transmit_field = field;
  byteCtr = byteCount;
  left_count = 0;
  UCB0CTL1 |= UCTR + UCTXSTT;                 // I2C TX, start condition
}

//------------------------------------------------------------------------------
// unsigned char TI_USCI_I2C_slave_present(unsigned char slave_address)
//
// This function is used to look for a slave address on the I2C bus.  
//
// IN:   unsigned char slave_address  =>  Slave Address
// OUT:  unsigned char                =>  0: address was not found, 
//                                        1: address found
//------------------------------------------------------------------------------
unsigned char TI_USCI_I2C_slave_present(unsigned char slave_address){
  unsigned char ie2_bak, slaveadr_bak, ucb0i2cie, returnValue;
  ucb0i2cie = UCB0I2CIE;                      // restore old UCB0I2CIE
  ie2_bak = IE2;                              // store IE2 register
  slaveadr_bak = UCB0I2CSA;                   // store old slave address
  UCB0I2CIE &= ~ UCNACKIE;                    // no NACK interrupt
  UCB0I2CSA = slave_address;                  // set slave address
  IE2 &= ~(UCB0TXIE + UCB0RXIE);              // no RX or TX interrupts
  __disable_interrupt();
  UCB0CTL1 |= UCTR + UCTXSTT + UCTXSTP;       // I2C TX, start condition
  while (UCB0CTL1 & UCTXSTP);                 // wait for STOP condition
  
  returnValue = !(UCB0STAT & UCNACKIFG);
  __enable_interrupt();
  IE2 = ie2_bak;                              // restore IE2
  UCB0I2CSA = slaveadr_bak;                   // restore old slave address
  UCB0I2CIE = ucb0i2cie;                      // restore old UCB0CTL1
  return returnValue;                         // return whether or not 
                                              // a NACK occured
}

//------------------------------------------------------------------------------
// unsigned char TI_USCI_I2C_notready()
//
// This function is used to check if there is commuincation in progress. 
//
// OUT:  unsigned char  =>  0: I2C bus is idle, 
//                          1: communication is in progress
//------------------------------------------------------------------------------
unsigned char TI_USCI_I2C_notready(){
  return (UCB0STAT & UCBBUSY);
}

/*!
 *******************************************************************************
 *
 *  @fn       void USCIABORX_ISR(void)
 *
 *  @brief    I2C Rx interrupt handler
 *
 *******************************************************************************
 */
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
  if (UCB0STAT & UCNACKIFG) {            
      UCB0CTL1 |= UCTXSTP;    					// Send STOP if slave sends NACK
      UCB0STAT &= ~UCNACKIFG;					// Clear NACK Flag
      NAK_Occurred = 1;							// Set NACK status flag
      left_count = byteCtr + 1;					// Record the number of bytes sent
      __bic_SR_register_on_exit(LPM0_bits+GIE);	// Put in low power mode
     // __bis_SR_register_on_exit(LPM0_bits);
      __delay_cycles(10);
  }
}

/*!
 *******************************************************************************
 *
 *  @fn       void USCIABOTX_ISR(void)
 *
 *  @brief    I2C Tx interrupt handler
 *
 *******************************************************************************
 */
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
  if (IFG2 & UCB0RXIFG) {							// RX - if a receive interrupt (byte received in buffer)
    if ( byteCtr == 0 ) {							// if number of remaining read bytes == 0
        UCB0CTL1 |= UCTXSTP;                    	// Send STOP condition
        *TI_receive_field = UCB0RXBUF;				// Store received data in data array
        TI_receive_field++;							// Increment data array pointer
        MultiByteReadFG = 0;						// Multi-byte transaction done
        __bic_SR_register_on_exit(LPM0_bits+GIE);	// Put in low power mode
    }
    else {
        *TI_receive_field = UCB0RXBUF;				// Store received data in data array
        TI_receive_field++;							// Increment data array pointer
        byteCtr--;									// Decrement number of remaining bytes to read
    }
  }
  else {											// TX - if not a read, then a transmit
    if (byteCtr == 0) {
        UCB0CTL1 |= UCTXSTP;                    	// I2C stop condition
        IFG2 &= ~UCB0TXIFG;                     	// Clear USCI_B0 TX int flag
        __bic_SR_register_on_exit(LPM0_bits+GIE);
    }
    else {
        UCB0TXBUF = *TI_transmit_field;
        TI_transmit_field++;
        byteCtr--;
    }
  }
}

//------------------------------------------------------------------------------

