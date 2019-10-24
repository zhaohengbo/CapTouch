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

#ifndef BINARYMODES_H_
#define BINARYMODES_H_

#include "msp430g2553.h"
#include "CapTouchBoard.h"
#include "Actuator_Waveforms.h"

#define BINARYMODESLEVEL		2 	// Sets the depth of the binary modes

/**
 * BinaryModes - function containing the extra binary mode button functions and effects
 */
void BinaryModes(void);

#endif /* BINARYMODES_H_ */
