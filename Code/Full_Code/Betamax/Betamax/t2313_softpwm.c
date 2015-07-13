// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
*
* Atmel Corporation
*
* - File              : t2313_softpwm.c
* - Compiler          : IAR EWAAVR 4.11A or GCC 3.4.3
*
* - Support mail      : avr@atmel.com
*
* - Supported devices : ATtiny2313.
*
* - AppNote           : AVR136 - Low-Jitter Multi-Channel Software PWM
*
* - Description       : Tiny2313 software PWM demo. This program demonstrates the
*                       generation of low-jitter PWM signals on multiple I/O ports
*                       for LED driving, meter driving, voltage generation etc.
*
* $Revision: 1.2 $
* $Date: Tuesday, December 20, 2005 14:10:56 UTC $
*****************************************************************************/
/*! \mainpage
 * \section Intro Introduction
 * This documents data structures, functions, variables, defines, enums, and
 * typedefs in the software for application note AVR136.
 *
 * \section CI Compilation Info
 * This software was written for the both IAR Embedded Workbench 4.11A and GCC 3.4.3.
 *
 * To make project when using IAR Embedded Workbench:
 * <ol>
 * <li> Add the file t2313_softpwm.c to project.
 * <li> Under processor configuration, select ATtiny2313
 * <li> Enable bit definitions in I/O include files
 * </ol>
 *
 * \section DI Device Info
 * The included source code is written for ATtiny2313, using the STK500 platform
 * with a 7.3728MHz external crystal.
 * - Program the CKSEL fuse for external clock source (Ext.Clock)
 *
 * \section TS Target Setup
 * - Connect PD0 to RXD and PD1 to TXD of the RS232 SPARE header on the STK500.
 * - Connect the RS232 port to a PC running a terminal program set for 8N1 at 38,400 Baud.
 * Send commands using the four-character format: \#nHH
 * where:
 * - # is a sync character
 * - n = 0-9 for required channel number
 * - HH = hex value corresponding to 0-255 for PWM high period
 *
 * Channels are mapped to specific port pins in the t2313_softpwm.h file
 * \section PWMinfo PWM frequency and crystal selection
 *
 * The PWM base frequency is the crystal frequency divided by 65536, e.g. for
 * a 7.3728MHz crystal the PWM base frequency will be 112.5Hz. The standard
 * STK500 3.6864MHz oscillator could be used as a clock source, but the PWM
 * base frequency would be reduced which may result in unacceptable ripple.
 * Jitter will be +/-1 clock cycle max, or +/-0.0015% of base frequency.
 *
 * This demonstration shows ten PWM channels, for GCC the ISR uses less than
 * 50% of processing time during the softcount=0 interrupt. The principles
 * shown should accomodate up to 24 channels on suitable AVR devices whilst
 * maintaining PWM accuracy, ISR optimisation may improve this even further.
 *
 * \section TL ToDo List
 * \todo Select if ISR Timing Debugging is desired in \ref DEBUG.
 *
 * \section OA Original author
 * Andy Gayne is the original author of this application note.
 */

#include <ctype.h>
#include "t2313_softpwm.h"
#if defined(__ICCAVR__)
  #include <iotiny2313.h>
  #include <inavr.h>
#elif __GNUC__ > 0
  #include <avr/io.h>
  #include <avr/interrupt.h>
  #include <avr/signal.h>
  #include <avr/wdt.h>
#else
# error "Unsupported compiler."
#endif

//! global buffers
unsigned char compare[CHMAX];
volatile unsigned char compbuff[CHMAX];

#if DEBUG
volatile unsigned int debugTimer;
#endif

/*! \brief Example of PWM control via the serial port. Waits for a synchronization character and acts upon it.
 */
void ManualControl(void)
{
  unsigned char rxdata, channel, temp, error;

    error = 0;                          // clear error flag
    while(USART0_Receive() != '#');     // wait for sync character
    USART0_Transmit('#');               // echo sync character
    channel = USART0_Receive() - 0x30;  // receive channel number
    if(channel >= CHMAX) error = 1;     // error if invalid channel
    USART0_Transmit(channel + 0x30);    // echo received character

    temp = GetNextChar();         // fetch upper nibble
    USART0_Transmit(temp);        // echo received character
    if (isxdigit(temp))           // check for a hex character
    {
      if ( temp > '9') temp -= 7; // subtract offset for A-F
      temp -= 0x30;               // subtract ASCII offset
    }
    else error = 1;               // error if not hex
    rxdata = temp << 4;           // store received upper nibble

    temp = GetNextChar();         // fetch lower nibble
    USART0_Transmit(temp);        // echo received character
    if (isxdigit(temp))           // check for a hex character
    {
      if ( temp > '9') temp -= 7; // subtract offset for A-F
      temp -= 0x30;               // subtract ASCII offset
    }
    else error = 1;               // error if not hex
    rxdata += temp;               // add lower nibble to upper nibble

    if(!error)                    // if data is good
    {
      compbuff[channel] = rxdata; // update compare buffer

      USART0_Transmit(':');       // send OK message
      USART0_Transmit('O');
      USART0_Transmit('K');
      USART0_Transmit('\r');
      USART0_Transmit('\n');
    }
    else                          // if data is not good
    {
      USART0_Transmit(':');       // send ERRor message
      USART0_Transmit('E');
      USART0_Transmit('R');
      USART0_Transmit('R');
      USART0_Transmit('\r');
      USART0_Transmit('\n');
    }
}

/*! \brief  Display debug timer value on attached terminal
 */
#if DEBUG
void DebugMonitor(void)
{
  signed char i;
  unsigned int temp;

  temp = debugTimer;              // fetch latest timer value
  USART0_Transmit('\r');
  USART0_Transmit('0');
  USART0_Transmit('x');
  // transmit 16 bit timer value as four hex characters
  for(i=3;i>=0;i--) USART0_Transmit(HexToASCII(0x0F & (temp >> (4*i))));

}
#endif
/*! \brief Init function. This function initialises the hardware
 */
void Init(void)
{
  unsigned char i, pwm;

  CLKPR = (1 << CLKPCE);        // enable clock prescaler update
  CLKPR = 0;                    // set clock to maximum (= crystal)

  __watchdog_reset();           // reset watchdog timer
  MCUSR &= ~(1 << WDRF);        // clear the watchdog reset flag
  WDTCSR |= (1<<WDCE)|(1<<WDE); // start timed sequence
  WDTCSR = 0x00;                // disable watchdog timer

  DDRD = PORTD_MASK;            // set port pins to output
  DDRB = PORTB_MASK;            // set port pins to output

#if DEBUG
  DEBUGSET;                     // make debug pin output (header file macro)
  pwm = 0;                      // worst-case default PWM level
#else
  pwm = PWMDEFAULT;
#endif

  for(i=0 ; i<CHMAX ; i++)      // initialise all channels
  {
    compare[i] = pwm;           // set default PWM values
    compbuff[i] = pwm;          // set default PWM values
  }


  TIFR0 = (1 << TOV0);           // clear interrupt flag
  TIMSK0 = (1 << TOIE0);         // enable overflow interrupt
  TCCR0B = (1 << CS00);         // start timer, no prescale

	//TODO: uncomment this.
  //__enable_interrupt();         // enable interrupts
}

/*! \brief Interrupt Service Routine
 */
#if defined(__ICCAVR__)
  #pragma vector=TIMER0_OVF0_vect
  __interrupt void ISR_TOV0 (void)

#elif __GNUC__ > 0
  ISR (TIMER0_OVF_vect)
#endif
{
  static unsigned char pinlevelB=PORTB_MASK, pinlevelD=PORTD_MASK;
  static unsigned char softcount=0xFF;

  if(++softcount == 0){         // increment modulo 256 counter and update
                                // the compare values only when counter = 0.
    compare[0] = compbuff[0];   // verbose code for speed
    compare[1] = compbuff[1];
    compare[2] = compbuff[2];
    compare[3] = compbuff[3];
    compare[4] = compbuff[4];
    compare[5] = compbuff[5];
    compare[6] = compbuff[6];
    compare[7] = compbuff[7];
    compare[8] = compbuff[8];
    compare[9] = compbuff[9];
	compare[10] = compbuff[10];
	compare[11] = compbuff[11];  // last element should equal CHMAX - 1

    pinlevelB = PORTB_MASK;     // set all port pins high
    pinlevelD = PORTD_MASK;     // set all port pins high
  }
  // clear port pin on compare match (executed on next interrupt)
  if(compare[0] == softcount) CH0_CLEAR;
  if(compare[1] == softcount) CH1_CLEAR;
  if(compare[2] == softcount) CH2_CLEAR;
  if(compare[3] == softcount) CH3_CLEAR;
  if(compare[4] == softcount) CH4_CLEAR;

  if(compare[5] == softcount) CH5_CLEAR;
  if(compare[6] == softcount) CH6_CLEAR;
  if(compare[7] == softcount) CH7_CLEAR;
  if(compare[8] == softcount) CH8_CLEAR;
  if(compare[9] == softcount) CH9_CLEAR;
  if(compare[10] == softcount) CH10_CLEAR;
  if(compare[11] == softcount) CH11_CLEAR;
  
  PORTB = pinlevelB;            // update outputs
  PORTD = pinlevelD;            // update outputs

#if DEBUG
  DEBUGPIN_OFF;                 // clear debug pin
  TCCR1B = 0;                   // stop timer
  if(!softcount) debygTimer = TCNT1;  // store value for softcount=0 ISR
#endif
}

/*! \brief  HexToASCII - convert single hex digit to ASCII equivalent
 */
unsigned char HexToASCII (unsigned char ch)
{
  ch += 0x30;             // add offset to ASCII '0'
  if (ch > '9') ch += 7;  // if alphabetic add offset to ASCII 'A'
  return ch;
}

/*! \brief  GetNextChar - receive and normalise next character from USART
 */
unsigned char GetNextChar(void)
{
  char tmp;

  tmp = USART0_Receive(); // get next character
  tmp = toupper(tmp);     // force to upper case
  return(tmp);
}

