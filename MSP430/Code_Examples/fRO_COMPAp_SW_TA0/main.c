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
 *  This example is uses the MSP430G2211 to support the fRO_COMPAp_SW_TA0
 *  implementation.  There are 2 elements used for discrete buttons.
 *  
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
 * Build with optimization level 1 to fit into 2K device.
 */
#include "CTS_Layer.h"

void init(void);
void initTouch(void);
void initDevice(void);
void delay(void);
void updateLED(struct Element *);

//uint16_t cnts;

// Main Function
int main(void)
{ 
	uint8_t ledUpdate;
    init();
    // Main loop starts here
    while (1)
    {
        ledUpdate = 0;
        P2OUT = ledUpdate;
//        TI_CAPT_Raw(&button1,&cnts);
    	if(TI_CAPT_Button(&button1))
    	{
            ledUpdate |= BIT7;
    	}
    	if(TI_CAPT_Button(&button2))
    	{
            ledUpdate |= BIT6;
    	}
    	P2OUT = ledUpdate;
//        P2OUT = BIT6;
    	delay();
    }
} // End Main

void init(void)
{
    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
    initDevice();
    initTouch();
}

void initTouch(void)
{
    // establish baseline
    TI_CAPT_Init_Baseline(&button1);
    TI_CAPT_Update_Baseline(&button1,5);
    TI_CAPT_Init_Baseline(&button2);
    TI_CAPT_Update_Baseline(&button2,5);
}

void initDevice(void)
{
    /*
     * Init IO
     */
	P2DIR = (BIT7+BIT6);
	P2SEL &= ~(BIT7+BIT6);
	P2OUT = 0;
}

void delay(void)
{
    __delay_cycles(100);
}
