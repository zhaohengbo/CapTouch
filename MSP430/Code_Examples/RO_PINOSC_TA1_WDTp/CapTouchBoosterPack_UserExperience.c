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
/******************************************************************************
 *         MSP430G2-LaunchPad CapTouch BoosterPack User Experience
 *
 * This application operates on the LaunchPad platform using the MSP430G2452
 * device and the CapTouch BoosterPack plugin board. The capacitive touch and
 * proximity sensing are enabled by the pin oscillator feature new to the
 * MSP430G2xx2 family devices. The User Experience application also utilizes
 * the cap touch library to realize & measure the capacitive touch and proximity
 * sensors. The cap touch library also provides layers of abstractions to
 * generate higher logical outputs such as logical touches, geometry (in this
 * hardware, a four-button wheel), and even gestures.
 *
 * The User Experience application starts up and remains in 'sleep' mode,
 * sampling the proximity sensor every ~8.3ms [VLO/100=12kHz/100=120Hz]. Upon
 * registering a valid proximity event [hand/finger/object hovering ~3-5cm from
 * the BoosterPack], the application wakes up to operate in the 'active' mode.
 *
 * In active mode, the application samples and registers individual finger touches
 * on the 16-position wheel or the center button as well as simple gestures
 * [Clockwise & Counter-clockwise] while the finger moves along and remains on
 * the wheel.
 *
 * a 9600 baud UART link is also implemented using SW TimerA to provide
 * application and cap touch data back to the PC via the UART-USB back channel.
 * The application sends UART data upon events such as wake up, sleep, touch,
 * or gesture.
 *
 * D. Dang
 * Texas Instruments, Inc.
 * Ver 0.90 Feb 2011
 *
 * T. Hwang
 * Texas Instruments, Inc.
 * Updated for RO_PINOSC_TA1_WDTp HAL with MSP430G2559, Mar 2013
 *
 * Update 1/21/16
 * UART Implemented with the USCIA0
 ******************************************************************************/

#include "CTS_Layer.h"

#define WAKE_UP_UART_CODE       0xBE
#define WAKE_UP_UART_CODE2      0xEF
#define SLEEP_MODE_UART_CODE    0xDE
#define SLEEP_MODE_UART_CODE2   0xAD
#define MIDDLE_BUTTON_CODE      0x80
#define INVALID_GESTURE         0xFD
#define GESTURE_START           0xFC
#define GESTURE_STOP            0xFB
#define COUNTER_CLOCKWISE       1
#define CLOCKWISE               2
#define GESTURE_POSITION_OFFSET 0x20
#define WHEEL_POSITION_OFFSET   0x30

#define WHEEL_TOUCH_DELAY		12			    //Delay between re-sendings of touches
#define MAX_IDLE_TIME           200
#define PROXIMITY_THRESHOLD     130				//

unsigned int wheel_position=ILLEGAL_SLIDER_WHEEL_POSITION, last_wheel_position=ILLEGAL_SLIDER_WHEEL_POSITION;
unsigned int deltaCnts[1];
unsigned int prox_raw_Cnts;

/*----------------- LED definition---------------------------------------------
 * There are 8 LEDs to represent different positions around the wheel. They are
 * controlled by 5 pins of Port 1 using a muxing scheme. The LEDs are divided
 * vertically into two groups of 4, in which each LED is paired up [muxed] with
 * the LED mirrored on the other side of the imaginary center vertical line via
 * the use of pin P2.5 and one specific port pin.
 * Specifically, the pairs are LEDs [0,7], [1,6], [2,5], [3,4], as shown in the
 * diagram below.
 *     LED                        Position (degrees, clockwise)
 * --RIGHT SIDE--
 *      0       P2.4,!P2.5                  45
 *      1       P3.3,!P2.5                  80
 *      2       P3.2,!P2.5                 100
 *      3       P3.1,!P2.5                 135 *
 *
 * --LEFT SIDE--
 *      4       P2.5,(P2.4,P3.3,P3.2)       225
 *      5       P2.5,(P2.4,P3.3,P3.1)       260
 *      6       P2.5,(P2.4,P3.2,P3.1)       280
 *      7       P2.5,(P3.3,P3.2,P3.1)       315
 *----------------------------------------------------------------------------*/
#define P2MASK7                   BIT4
#define P2MASK6                   0
#define P2MASK5                   0
#define P2MASK4                   0

#define P2MASK3                   (BIT5+BIT4)
#define P2MASK2                   (BIT5+BIT4)
#define P2MASK1                   (BIT5+BIT4)
#define P2MASK0                   (BIT5)

#define P3MASK7                   0
#define P3MASK6                   BIT3
#define P3MASK5                   BIT2
#define P3MASK4                   BIT1

#define P3MASK3                   (BIT3+BIT2)
#define P3MASK2                   (BIT3+BIT1)
#define P3MASK1                   (BIT2+BIT1)
#define P3MASK0                   (BIT3+BIT2+BIT1)

const uint8_t P2LedWheelPosition[16] =
{
    P2MASK0,
	P2MASK0,
	P2MASK0 & P2MASK1,
	P2MASK1,
    P2MASK1 & P2MASK2,
	P2MASK2,
	P2MASK2 & P2MASK3,
	P2MASK3,
    P2MASK4,
	P2MASK4,
	P2MASK4 | P2MASK5,
	P2MASK5,
    P2MASK5 | P2MASK6,
	P2MASK6,
	P2MASK6 | P2MASK7,
	P2MASK7
};
const uint8_t P2startSequence[8] =
{
    P2MASK0,
    P2MASK1,
    P2MASK2,
    P2MASK3,
    P2MASK4,
    P2MASK5,
    P2MASK6,
    P2MASK7
};

const uint8_t P3LedWheelPosition[16] =
{
    P3MASK0,
    P3MASK0,
    P3MASK0 & P3MASK1,
    P3MASK1,
    P3MASK1 & P3MASK2,
    P3MASK2,
    P3MASK2 & P3MASK3,
    P3MASK3,
    P3MASK4,
    P3MASK4,
    P3MASK4 | P3MASK5,
    P3MASK5,
    P3MASK5 | P3MASK6,
    P3MASK6,
    P3MASK6 | P3MASK7,
    P3MASK7
};
const uint8_t P3startSequence[8] =
{
    P3MASK0,
    P3MASK1,
    P3MASK2,
    P3MASK3,
    P3MASK4,
    P3MASK5,
    P3MASK6,
    P3MASK7
};
/*----------------- LED definition------------------------------*/

void InitLaunchPadCore(void)
{
    // Port init
    P1OUT = 0;
    P1DIR = 0xFF;
    P2SEL = 0xC0;                         // with XTAL
    P2SEL2 = 0;
    P2OUT = 0;
    P2DIR = 0xFF;
    P3OUT = 0;
    P3DIR = 0xFF;
    P4OUT = 0;
    P4DIR = 0xFF;

    BCSCTL1 |= DIVA_0;                    // ACLK/(0:1,1:2,2:4,3:8)
    BCSCTL3 = 0;                          // LFXT1 = XT1, 32khz xtal

    BCSCTL1 = CALBC1_12MHZ;
    DCOCTL = CALDCO_12MHZ;

    P3DIR |= BIT4;
    P3SEL |= (BIT4 + BIT5);        // P3.5 = RXD, P3.4=TXD
    P3SEL2 &= ~(BIT4 + BIT5);      // P3.5 = RXD, P3.4=TXD

    /*** Configure UART ***/
    UCA0CTL1 |= UCSWRST;                 // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL_2;                // SMCLK

    UCA0BR0 = 226;                       // 12 MHz 9600 (see User's Guide)
    UCA0BR1 = 4;                         // 12 MHz 9600, 1250
    UCA0MCTL |= UCBRS_0+UCBRF_0;         // Modulation UCBRSx=0, UCBRFx=0
                                         // UC0S16 = 0, oversampling disabled

    UCA0CTL1 &= ~UCSWRST;                // **Initialize USCI state machine**
}

void SendByte(uint8_t touch)
{
    while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
    UCA0TXBUF = touch;                      // TX
}

/* ----------------CapTouchIdleMode-----------------------------------------
 * Device stays in LPM3 'sleep' mode, only Proximity Sensor is used to detect
 * any movement triggering device wake up
 * ------------------------------------------------------------------------*/
void CapTouchIdleMode(void)
{
    /* Send status via UART: 'sleep' = [0xDE, 0xAD] */
    SendByte(SLEEP_MODE_UART_CODE);
    SendByte(SLEEP_MODE_UART_CODE2);

    P3OUT |= BIT0;                            // Turn on center LED
    deltaCnts[0] = 0;

    /* Sleeping in LPM3 with ACLK/100 = 12Khz/100 = 120Hz wake up interval */
    /* Measure proximity sensor count upon wake up */
    /* Wake up if proximity deltaCnts > THRESHOLD */
    do
    {
        TACCR0 = 100;
        TACTL = TASSEL_1 + MC_1;
        TACCTL0 |= CCIE;
        __bis_SR_register(LPM3_bits+GIE);
        TACCTL0 &= ~CCIE;
        TI_CAPT_Custom(&proximity_sensor,deltaCnts);
    }
    while (deltaCnts[0] <= PROXIMITY_THRESHOLD);

    P3OUT &= ~BIT0;                           // Turn off center LED
}

/* ----------------MeasureCapBaseLine--------------------------------------
 * Re-measure the baseline capacitance of the wheel elements & the center
 * button. To be called after each wake up event.
 * ------------------------------------------------------------------------
 */
void MeasureCapBaseLine(void)
{
    P3OUT |= BIT0;

    TI_CAPT_Init_Baseline(&wheel);
    TI_CAPT_Update_Baseline(&wheel,2);
    TI_CAPT_Init_Baseline(&middle_button);
    TI_CAPT_Update_Baseline(&middle_button,2);
}

/* ----------------LedStartUpSequence--------------------------------------
 * Display an LED lighting sequence to indicate the wake up event
 * ------------------------------------------------------------------------*/
void LedStartUpSequence(void)
{
    unsigned char i;
    TACCTL0 = CCIE;                           // CCR0 interrupt enabled
    TACTL |= TACLR;
    TACCR0 = TAR + 500;                       // 50ms
    TACTL = TASSEL_1 + MC_1;                  // ACLK, upmode

    /* Slow clockwise sequence */
    for(i=0; i<8; i++)
    {
        P2OUT = P2startSequence[i];
        P3OUT = P3startSequence[i];
        __bis_SR_register(LPM3_bits+GIE);
        __delay_cycles(1000000);
        TACCR0 = TAR + 500;   // 50ms
    }

    P3OUT |= BIT0;
    /* Fast counter-clockwise sequence */
    while(i)
    {
        i--;
        P2OUT = P2startSequence[i];
        P3OUT = P3startSequence[i];
        __bis_SR_register(LPM3_bits+GIE);
        TACCR0 = TAR + 500;   // 50ms
    }
    TACCTL0 &= ~CCIE;                         // CCR0 interrupt disabled
    P2OUT = 0;
    P3OUT = 0;                                // Turn off all LEDs
}

/* ----------------GetGesture----------------------------------------------
 * Determine immediate gesture based on current & previous wheel position
 * ------------------------------------------------------------------------*/
uint8_t GetGesture(unsigned char wheel_position)
{
    uint8_t gesture = INVALID_GESTURE, direction, ccw_check, cw_check;
// ******************************************************************************
// gesturing
// determine if a direction/swipe is occuring
// the difference between the initial position and
// the current wheel position should not exceed 8
// 0-1-2-3-4-5-6-7-8-9-A-B-C-D-E-F-0...
//
// E-F-0-1-2:  cw, 4
// 2-1-0-F-E: ccw, 4
// A-B-C-D-E-F

  //if(initial_wheel_position == INVALID_WHEEL_POSITION)
  //{
    //gesture = 0;
    //initial_wheel_position = wheel_position;
  //}
  //else

  if(last_wheel_position != ILLEGAL_SLIDER_WHEEL_POSITION)
  {
    if(last_wheel_position  > wheel_position)
    {
      // E-D-C-B-A:  ccw, 4
      // counter clockwise: 0 < (init_wheel_position - wheel_position) < 8
      //                    gesture = init_wheel_position - wheel_position
      //
      // E-F-0-1-2:  cw, 4
      // clockwise:        0 < (init_wheel_position+wheel_position)-16 <8
      //
      ccw_check = last_wheel_position  - wheel_position;
      if(ccw_check < 8)
      {
        gesture = ccw_check;
        direction = COUNTER_CLOCKWISE;
      }
      else
      {
        // E-F-0-1-2:  cw, 4
        // 16 - 14 + 2 = 4
        cw_check = 16 - last_wheel_position  + wheel_position ;
        if(cw_check < 8)
        {
            gesture = cw_check;
            direction = CLOCKWISE;
        }
      }
    }
    else
    {
      // initial_wheel_position <= wheel_position
      //
      // 2-1-0-F-E: ccw, 4
      // counter clockwise:
      //                    0 < (init_wheel_position+wheel_position)-16 <8
      //                    gesture = init_wheel_position - wheel_position
      //
      // 0-1-2-3-4:  cw, 4
      // clockwise:        0 < (wheel_position - init_wheel_position) < 8
      //
      cw_check = wheel_position - last_wheel_position ;
      if(cw_check < 8)
      {
        gesture = cw_check;
        direction = CLOCKWISE;
      }
      else
      {
        // 2-1-0-F-E: ccw, 4
        // 16 + 2 - 14 = 4
        ccw_check = 16 + last_wheel_position  - wheel_position ;
        if(ccw_check < 8)
        {
          gesture = ccw_check;
          direction = COUNTER_CLOCKWISE;
        }
      }
    }
  }
  if (gesture == INVALID_GESTURE)
  	return gesture;
  if (direction == COUNTER_CLOCKWISE)
    return (gesture + 16);
  else
    return gesture;
}

/* ----------------CapTouchActiveMode----------------------------------------------
 * Determine immediate gesture based on current & previous wheel position
 *
 *
 *
 *
 *
 *
 *
 * -------------------------------------------------------------------------------*/
void CapTouchActiveMode()
{
    uint8_t idleCounter, activeCounter;
    uint8_t gesture, gestureDetected;
    uint8_t centerButtonTouched = 0;
    uint16_t wheelTouchCounter = WHEEL_TOUCH_DELAY - 1;

    gesture = INVALID_GESTURE;            // Wipes out gesture history

  /* Send status via UART: 'wake up' = [0xBE, 0xEF] */
  SendByte(WAKE_UP_UART_CODE);
  SendByte(WAKE_UP_UART_CODE2);

  idleCounter = 0;
  activeCounter = 0;
  gestureDetected = 0;

  while (idleCounter++ < MAX_IDLE_TIME)
  {

    TACCTL0 &= ~CCIE;

    wheel_position = ILLEGAL_SLIDER_WHEEL_POSITION;
    wheel_position = TI_CAPT_Wheel(&wheel);

    __no_operation();

    /* Process wheel touch/position/gesture  if a wheel touch is registered*/
    /* Wheel processing has higher priority than center button*/

    if(wheel_position != ILLEGAL_SLIDER_WHEEL_POSITION)
    {
      centerButtonTouched = 0;

      /* Adjust wheel position based: rotate CCW by 2 positions */
      if (wheel_position < 0x08)
      {
         wheel_position += 0x40 - 0x08;
      }
      else
      {
         wheel_position -= 0x08;
            /* Adjust wheel position based: rotate CCW by 2 positions */
      }

      wheel_position = wheel_position >>2;  // divide by four

      gesture = GetGesture(wheel_position);

      /* Add hysteresis to reduce toggling between wheel positions if no gesture
       * has been TRULY detected. */

      if ( (gestureDetected==0) && ((gesture<=1) || (gesture==0x11) || (gesture==0x10)))
      {
        if (last_wheel_position != ILLEGAL_SLIDER_WHEEL_POSITION)
            wheel_position = last_wheel_position;
        gesture = 0;
      }

      /* Turn on corresponding LED(s) */
      P2OUT = P2LedWheelPosition[wheel_position];
      P3OUT = (P3OUT & BIT0) | P3LedWheelPosition[wheel_position];


      if ((gesture != 0) && (gesture != 16) && (gesture != INVALID_GESTURE))
      { /* A gesture has been detected */

        if (gestureDetected ==0)
        { /* Starting of a new gesture sequence */
          gestureDetected = 1;

          /* Transmit gesture start status update & position via UART to PC */
          SendByte(GESTURE_START);
          SendByte(last_wheel_position + GESTURE_POSITION_OFFSET);
        }

        /* Transmit gesture & position via UART to PC */
        SendByte(gesture);
        SendByte(wheel_position + GESTURE_POSITION_OFFSET);
      }
      else
        if (gestureDetected==0)
        { /* If no gesture was detected, this is constituted as a touch/tap */
          if (++wheelTouchCounter >= WHEEL_TOUCH_DELAY)
          {
          	/* Transmit wheel position [twice] via UART to PC */
          	wheelTouchCounter = 0;
          	SendByte(wheel_position + WHEEL_POSITION_OFFSET );
          	SendByte(wheel_position + WHEEL_POSITION_OFFSET );
          }
        }
       	else
          wheelTouchCounter = WHEEL_TOUCH_DELAY - 1;

      idleCounter = 0;                      // Reset idle counter
      activeCounter++;
      last_wheel_position = wheel_position;
    }
    else
    { /* no wheel position was detected */

      /* Turn off wheel LED(s) */
      P2OUT = 0;
      P3OUT = (P3OUT & BIT0);

      if(TI_CAPT_Button(&middle_button))
      { /* Middle button was touched */
        if (centerButtonTouched==0)
        {
          /* Transmit center button code [twice] via UART to PC */
          SendByte(MIDDLE_BUTTON_CODE);
          SendByte(MIDDLE_BUTTON_CODE);

          centerButtonTouched = 1;

          P3OUT = (P3OUT&BIT0) ^ BIT0;      // Toggle Center LED
        }
        idleCounter = 0;
      }
      else
      {
    	  /*
    	   * No touch was registered at all [Not wheel or center button
    	   */
          centerButtonTouched = 0;
          P3OUT &= BIT0;
          if((gesture == INVALID_GESTURE) || (gestureDetected ==0))
          { /* No gesture was registered previously */
              if(last_wheel_position  != ILLEGAL_SLIDER_WHEEL_POSITION)
              {
                  /* Transmit last wheel position [twice] via UART to PC */
                  SendByte(last_wheel_position  + WHEEL_POSITION_OFFSET );
                  SendByte(last_wheel_position  + WHEEL_POSITION_OFFSET );
                  wheelTouchCounter = WHEEL_TOUCH_DELAY - 1;
              }
          }
          if(gestureDetected == 1)
          {   /* A gesture was registered previously */

              /* Transmit status update: stop gesture tracking [twice] via UART to PC */
              SendByte(GESTURE_STOP);
              SendByte(GESTURE_STOP);
          }
      }

      // Reset all touch conditions, turn off LEDs,
      last_wheel_position= ILLEGAL_SLIDER_WHEEL_POSITION;
      gesture = INVALID_GESTURE;
      gestureDetected = 0;
    }

  /* ------------------------------------------------------------------------
   * Option:
   * Add delay/sleep cycle here to reduce active duty cycle. This lowers power
   * consumption but sacrifices wheel responsiveness. Additional timing
   * refinement must be taken into consideration when interfacing with PC
   * applications GUI to retain proper communication protocol.
   * -----------------------------------------------------------------------*/
  }
}

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer

    InitLaunchPadCore();

    /* Establish baseline for the proximity sensor */
    TI_CAPT_Init_Baseline(&proximity_sensor);
    TI_CAPT_Update_Baseline(&proximity_sensor,5);

    while (1)
    {
        CapTouchIdleMode();
        MeasureCapBaseLine();
        LedStartUpSequence();
        CapTouchActiveMode();
    }
}

//------------------------------------------------------------------------------
// Timer_A UART - Transmit Interrupt Handler
//------------------------------------------------------------------------------
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0_ISR(void)
{
    __bic_SR_register_on_exit(LPM3_bits+GIE);
}

#pragma vector=PORT1_VECTOR,PORT2_VECTOR,                                   \
  TIMER0_A1_VECTOR,TIMER1_A0_VECTOR,TIMER1_A1_VECTOR,TIMERB0_VECTOR,        \
  TIMERB1_VECTOR,USCIAB0RX_VECTOR,USCIAB0TX_VECTOR,                         \
  ADC10_VECTOR,NMI_VECTOR,COMPARATORA_VECTOR
__interrupt void ISR_trap(void)
{
  // the following will cause an access violation which results in a PUC reset
  WDTCTL = 0;
}
