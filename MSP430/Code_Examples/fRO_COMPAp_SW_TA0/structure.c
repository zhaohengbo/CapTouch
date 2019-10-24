/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*!
 * strucutre.c  
 *                                   MSP430G2211
 *                               +---------------+
 *                               |
 *                            +--<Vcc
 *                            |
 *                            R
 *                            |
 *                        +---+-->P1.1/CA1
 *                        |   |
 *                        R   R
 *                        |   |
 *                       GND  |
 *                            |
 *                            |
 * c-+-------------------+-R--+--<P1.7/CAOUT
 *   |                        |
 *   |                 c-+-R--+
 *   |                   |
 *   |                   +------->P2.6/CA6
 *   |
 *   +--------------------------->P1.2/CA2
 *
 */

#include "structure.h"

// P1.2, CA2
const struct Element element0 = {
              .inputBits =  P2CA2,  // CA2
              .threshold = 80
};

// P1.6, CA6
const struct Element element1 = {
              .inputBits =  P2CA3+P2CA2,  // CA6
              .threshold = 80
};


//*** Sensor *******************************************************/
// This defines the grouping of sensors, the method to measure change in
// capacitance, and the function of the group

const struct Sensor button1 =
{
                  .halDefinition = fRO_COMPAp_SW_TA0,
                  .numElements = 1,
                  .baseOffset = 0,
                  // Pointer to elements
                  .arrayPtr[0] = &element0,  // point to first element
                  // Reference Information
                  // CAOUT is P1.7
                  // TACLK is P1.0
                  .caoutDirRegister = (uint8_t *)&P1DIR,  // PxDIR
                  .caoutSelRegister = (uint8_t *)&P1SEL,  // PxSEL
                  .txclkDirRegister = (uint8_t *)&P1DIR,  // PxDIR
                  .txclkSelRegister = (uint8_t *)&P1SEL,  // SxSEL
                  .caoutBits = BIT7, // BITy
                  .txclkBits = BIT0,
				  /*
				   * This solution does not use ref pin, so giving
				   * CAx location which is overwritten by CAPDx.
				   * P1.2
				   */
                  .refPxoutRegister = (uint8_t *)&P1OUT,
                  .refPxdirRegister = (uint8_t *)&P1DIR,
                  .refBits = BIT2,           // BIT2
				  //
                  .refCactl2Bits = P2CA4,    // CACTL2-> P2CA4 , CA1
                  .capdBits = (0x7F),   // P1.1-P1.6

                  // Timer Information
				  .measGateSource = TIMER_SMCLK,          // use SMCLK for source
				  .sourceScale = TIMER_SOURCE_DIV_0,

                  .accumulationCycles = 50    // number of relaxation
                                               // oscillator cycles that
                                               // define measurement period
};

const struct Sensor button2 =
{ 
                  .halDefinition = fRO_COMPAp_SW_TA0,
                  .numElements = 1,
                  .baseOffset = 1,
                  // Pointer to elements
                  .arrayPtr[0] = &element1,  // point to first element
                  // Reference Information
                  // CAOUT is P1.7
                  // TACLK is P1.0
                  .caoutDirRegister = (uint8_t *)&P1DIR,  // PxDIR
                  .caoutSelRegister = (uint8_t *)&P1SEL,  // PxSEL
                  .txclkDirRegister = (uint8_t *)&P1DIR,  // PxDIR
                  .txclkSelRegister = (uint8_t *)&P1SEL,  // SxSEL
                  .caoutBits = BIT7, // BITy
                  .txclkBits = BIT0,
				  /*
				   * This solution does not use ref pin, so giving
				   * CAx location which is overwritten by CAPDx.
				   * P1.2
				   */
                  .refPxoutRegister = (uint8_t *)&P1OUT,
                  .refPxdirRegister = (uint8_t *)&P1DIR,
                  .refBits = BIT2,           // BIT2
				  //
                  .refCactl2Bits = P2CA4,    // CACTL2-> P2CA4 , CA1
                  .capdBits = (0x7F),   // P1.1-P1.6
                  
                  // Timer Information
				  .measGateSource = TIMER_SMCLK,          // use SMCLK for source         
				  .sourceScale = TIMER_SOURCE_DIV_0,            

                  .accumulationCycles = 50    // number of relaxation
                                               // oscillator cycles that 
                                               // define measurement period											   
};

