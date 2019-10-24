/******************************************************************************
 * Haptics.c
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 * Author: a0866685
 *
 * Description: This file contains the functions for sending haptics waveforms.
 *
 ******************************************************************************/
#include "Haptics.h"
#include "CapTouchBoard.h"

// private variables
static uint16_t tickdelay;
static uint8_t 	playEffect = 1;					// if 1 = play, 0 = do not play
static uint8_t  EffectActive = 0;				// if 1 = active, 0 = not active
static uint8_t  boostVoltageSetting = VBST; 	// default boost setting

// private functions
static void SendWaveform_Mode1(unsigned char bin, unsigned int PacketLength);

/**
 * Haptics_Init - initialize haptics variables and settings
 */
void Haptics_Init(void)
{
	tickdelay = (unsigned int) TICKDELAY;

	// Initialize DRV2667
	I2C_WriteSingleByte(DRV2667_CONTROL2, 0x00);    // Come out of STANDBY
	I2C_WriteSingleByte(DRV2667_CONTROL, GAIN);    	// Set DRV2667 GAIN

	// Load header into RAM
	I2C_WriteSingleByte(DRV2667_PAGE, 0x01);       	// Set to Page 1
	I2C_WriteMultiByte((unsigned char*) DRV2667_HEADER, DRV2667_HEADERSIZE);	// Send header data

	// Load waveform data into RAM
	I2C_WriteSingleByte(DRV2667_PAGE, 0x02);       	// Set to Page 2
	I2C_WriteMultiByte((unsigned char*) DRV2667_DATA, DRV2667_DATASIZE);		// Send waveform data

	// Load waveform Mode3 data into RAM
	I2C_WriteSingleByte(DRV2667_PAGE, 0x03);       	// Set to Page 3
	I2C_WriteMultiByte((unsigned char*) TWOTONE, TWOTONE_SIZE);

	// Set the page back to zero
	I2C_WriteSingleByte(DRV2667_PAGE, 0x00);       	// Set to Page 0

	// Set the boost voltage
	P2OUT |= boostVoltageSetting;
}

/**
 * Haptics_SendWaveform - setup and send haptic waveform
 * @param struct Waveform - the waveform output type, length in bytes, and data)
 */
void Haptics_SendWaveform(const Waveform waveform)
{
	if(playEffect)
	{
		Haptics_HardwareMode(waveform.outputMode);	// Set hardware control pins
		Haptics_OutputWaveform(waveform);			// Control the PWM or I2C
	}
}

/**
 * Haptics_SendWaveformPriority - setup and send haptic waveform, status flags do not affect this method
 * @param struct Waveform - the waveform output type, length in bytes, and data)
 */
void Haptics_SendWaveformPriority(const Waveform waveform)
{
	Haptics_HardwareMode(waveform.outputMode);	// Set hardware control pins
	Haptics_OutputWaveform(waveform);			// Control the PWM or I2C
}


/**
 * Haptics_OutputWaveform - control the PWM output pattern
 * @param struct Waveform - the waveform output type, length in bytes, and data
 */
void Haptics_OutputWaveform(const Waveform waveform)
{
	uint8_t i;

	switch(waveform.outputMode)
	{
	case PIEZO_MODE1: 	// Streaming Mode
		LED_EN(ON);
		SendWaveform_Mode1(waveform.data[0], waveform.data[1]);
		LED_EN(OFF);
		break;
	case PIEZO_MODE2:	// RAM Mode
		Haptics_DisableTrigger(); 	// Stop sequence if already playing
		for(i = 0; i < waveform.length; i += 2)
		{
			I2C_WriteSingleByte(waveform.data[i], waveform.data[(i+1)]);   // Send (register, value)
		}
		Haptics_EnableTrigger();	// Go
		break;
	case PIEZO_MODE3:	// Waveform Generator Mode
		//I2C_WriteSingleByte(DRV2667_CONTROL2, STOP); 	// Stop sequence if already playing
		for(i = 0; i < waveform.length; i += 2)
		{
			I2C_WriteSingleByte(waveform.data[i], waveform.data[(i+1)]);   // Send (register, value)
		}
		Haptics_EnableTrigger();	// Go
		break;

	case PIEZO_ANALOGOFF:		// Analog Input Mode, Off
	case PIEZO_ANALOGMODE50:	// Analog Input Mode, 50Vpp
	case PIEZO_ANALOGMODE100:	// Analog Input Mode, 100Vpp
	case PIEZO_ANALOGMODE150:	// Analog Input Mode, 150Vpp
	case PIEZO_ANALOGMODE200:	// Analog Input Mode, 200Vpp
		for(i = 0; i < waveform.length; i += 2)
		{
			I2C_WriteSingleByte(waveform.data[i], waveform.data[(i+1)]);   // Send (register, value)
		}
		break;
	}
}

/**
 * Haptics_HardwareMode - Set the hardware pins to the appropriate setting
 * @param unsigned char outputMode - the waveform output type
 */
void Haptics_HardwareMode(uint8_t outputMode)
{
	switch(outputMode)
	{
	case PIEZO_ANALOGMODE50:
		P2OUT &= 0x3F;
		P2OUT |= VBST_30;
		break;
	case PIEZO_ANALOGMODE100:
		P2OUT &= 0x3F;
		P2OUT |= VBST_55;
		break;
	case PIEZO_ANALOGMODE150:
		P2OUT &= 0x3F;
		P2OUT |= VBST_80;
		break;
	case PIEZO_ANALOGMODE200:
		P2OUT &= 0x3F;
		P2OUT |= VBST_105;
		break;
	default:
		P2OUT &= 0x3F;
		P2OUT |= boostVoltageSetting;
		break;
	}
}

/**
 * Haptics_SetDefaultBoostVoltage - sets the board's boost voltage (30V, 55V, 80V, or 105V)
 * @param boostVoltage - select from the four VBST_xx constants in haptics.h
 */
void Haptics_SetDefaultBoostVoltage(unsigned char boostVoltage)
{
	boostVoltageSetting = boostVoltage;		// Set default boost voltage variable
}

/**
 * Haptics_GetDefaultBoostVoltage - get default boost voltage
 * @return unsigned char VBST_xx constant
 */
unsigned char Haptics_GetDefaultBoostVoltage()
{
	return boostVoltageSetting;
}


/**
 * Haptics_SetGain - set the gain register of the DRV2667 (50V, 100V, 150V, or 200V)
 * @param gain - select from the four GAIN_xx constants in DRV2667.h
 */
void Haptics_SetGain(unsigned char gain)
{
	unsigned char controlRegister;

	controlRegister = I2C_ReadSingleByte(DRV2667_CONTROL);				// Read control register
	controlRegister &= 0xFC;											// Set gain bits to zero
	I2C_WriteSingleByte(DRV2667_CONTROL, (controlRegister | gain));    	// Set DRV2667 gain
}

/**
 * Haptics_EnableTrigger - Enable the GO bit in the DRV2667 via I2C
 */
void Haptics_EnableTrigger()
{
	Haptics_EffectActive(1);
	I2C_WriteSingleByte(DRV2667_CONTROL2, GO);   	// Go
}

/**
 * Haptics_DisableTrigger - Disable the GO bit in the DRV2667 via I2C
 */
void Haptics_DisableTrigger()
{
	Haptics_EffectActive(0);
	I2C_WriteSingleByte(DRV2667_CONTROL2, STOP); 	// Stop sequence if already playing
}

/**
 * Haptics_OutputEnableSet - enable/disable play back of all waveforms
 */
void Haptics_OutputEnableSet(uint8_t enable)
{
	playEffect = enable;
}

/**
 * Haptics_IsOutputEnabled - check if waveform play back is enabled
 */
uint8_t Haptics_IsOutputEnabled(void)
{
	return playEffect;
}

/**
 * Haptics_EffectActive - set if effect is active
 * @param unsigned char active - set activity status
 */
void Haptics_EffectActive(uint8_t active)
{
	EffectActive = active;
	LED_EN(active);
}

/**
 * Haptics_IsEffectActive - check if effect is active
 * @return unsigned char - 0  = inactive, 1 = active
 */
uint8_t Haptics_IsEffectActive(void)
{
	return EffectActive;
}


/**
 *  SendWaveform_Mode1 - send arbitrary model waveform to DRV266x over I2C
 *  @param  bin   Number of sinewave to skip per increment;
 *                higher the value higher the frequency
 *  @param  PacketLength  I2C packet length
 */
static void SendWaveform_Mode1(unsigned char bin, unsigned int PacketLength)
{
  unsigned char packet[2*MAX_PACKET_SZ];
  unsigned int packet_ndx, byte_ndx, bytes_left;
  signed int byte_ptr;
  unsigned char GotNAK = 0;

  byte_ptr = 0;

  /* Setup command byte to point to FIFO address */
  packet[0] = DRV2667_FIFO;

  // Send all packets
  for (packet_ndx=0; packet_ndx < PacketLength; packet_ndx++) {

      // Fill the array with sine value at 'bin' frequency
      for(byte_ndx = 1; byte_ndx < MAX_PACKET_SZ+1; byte_ndx++) {
          packet[byte_ndx] = sinewaveform[byte_ptr];
          byte_ptr += bin;                    // next table value index
          if(byte_ptr >= SINEWAVEFORM_SIZE) {           // handle wrap-around
              byte_ptr = byte_ptr - SINEWAVEFORM_SIZE;
          }
      }

      //If last packet, ensure ending with a full sine wave
      if (packet_ndx == (PacketLength - 1)) {
          // Right on without extension
          if (byte_ptr == 0) {
              packet[byte_ndx] = sinewaveform[byte_ptr];
              I2C_WriteMultiByte(packet, MAX_PACKET_SZ+2);          //Send Packet
          }
          // Complete the sine wave with packet extension
          else {
              while (byte_ptr < SINEWAVEFORM_SIZE) {
                  packet[byte_ndx] = sinewaveform[byte_ptr];
                  byte_ptr += bin;
                  byte_ndx++;
              }
              packet[byte_ndx]=sinewaveform[0];
              byte_ndx++;
              I2C_WriteMultiByte(packet, byte_ndx);    //Send Packet
          }
      }
      else {
          I2C_WriteMultiByte(packet, MAX_PACKET_SZ+1);    //Send Packet
      }

      // Wait 5ms and check for NACK
      timerdelay(tickdelay);
      GotNAK = I2C_IsNAK();
      bytes_left = I2C_GetNAKValue();

      //If NAK Occurred, resend the remaining bytes in the packet
      if (GotNAK) {
          /* Add back the byte that didn't make it */
          //bytes_left++;

          //  Back up the sine table pointer
          for (byte_ndx=0; byte_ndx<bytes_left; byte_ndx++) {
              byte_ptr -= bin;
              if(byte_ptr < 0) {    //  Handle the wrap-around
                  byte_ptr = SINEWAVEFORM_SIZE + byte_ptr;
              }
          }
          // Repopulate partial packet
          for (byte_ndx=1; byte_ndx<(bytes_left+1); byte_ndx++) {
              packet[byte_ndx]=sinewaveform[byte_ptr];
              byte_ptr += bin;
              if(byte_ptr >= SINEWAVEFORM_SIZE) {   // Handle wrap-around
                  byte_ptr = byte_ptr - SINEWAVEFORM_SIZE;
              }
          }
          // Send Packet with dropped data
          I2C_WriteMultiByte(packet, bytes_left+1);
          timerdelay(tickdelay);
      }
  }
}

