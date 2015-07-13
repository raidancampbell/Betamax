// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
*
* Atmel Corporation
*
* - File              : t2313_softpwm.h
* - Compiler          : IAR EWAAVR 4.11A or GCC 3.4.3
*
* - Support mail      : avr@atmel.com
*
* - Supported devices : ATtiny2313.
*
* - AppNote           : AVR136 - Low-Jitter Multi-Channel Software PWM
*
* - Description       : Header file for t2313_softpwm.c
*
* $Revision: 1.2 $
* $Date: Tuesday, December 20, 2005 14:11:04 UTC $
*****************************************************************************/

/*! \brief  Select or deselect ISR Timing Debugging, 0 = disabled, 1 = enabled
 */
#define DEBUG 0

//! Compiler specific defines
#if defined(__ICCAVR__)
  // correct an IAR definition for Watchdog Timer Control Register,
  // may not be needed in later versions of IAR or for other AVR devices.
  #define WDTCSR  WDTCR
#elif __GNUC__ > 0
  // redefine some macros
  #define __C_task int
  #define __enable_interrupt()  sei()
  #define __watchdog_reset()    wdt_reset()
#endif

//! Pin mappings
#define CHMAX       12    // maximum number of PWM channels
#define PWMDEFAULT  0x80  // default PWM value at start up for all channels

#define CH0_CLEAR (pinlevelD &= ~(1 << PD0)) // map CH0 to PD0
#define CH1_CLEAR (pinlevelD &= ~(1 << PD1)) // map CH1 to PD1
#define CH2_CLEAR (pinlevelD &= ~(1 << PD2)) // map CH2 to PD2
#define CH3_CLEAR (pinlevelD &= ~(1 << PD3)) // map CH3 to PD3
#define CH4_CLEAR (pinlevelD &= ~(1 << PD4)) // map CH4 to PD4
#define CH5_CLEAR (pinlevelD &= ~(1 << PD5)) // map CH5 to PD5
#define CH6_CLEAR (pinlevelD &= ~(1 << PD6)) // map CH6 to PD6
#define CH7_CLEAR (pinlevelD &= ~(1 << PD7)) // map CH7 to PD7

#define CH8_CLEAR (pinlevelB &= ~(1 << PB0)) // map CH8 to PB0
#define CH9_CLEAR (pinlevelB &= ~(1 << PB1)) // map CH9 to PB1
#define CH10_CLEAR (pinlevelB &= ~(1 << PB2)) // map CH10 to PB2
#define CH11_CLEAR (pinlevelB &= ~(1 << PB3)) // map CH11 to PB3

//! Set bits corresponding to pin usage above
#define PORTB_MASK  (1 << PB0)|(1 << PB1)|(1 << PB2)|(1 << PB3)
#define PORTD_MASK  (1 << PD0)|(1 << PD1)|(1 << PD2)|(1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6)|(1 << PD7)

#define DEBUGPIN_ON   PORTB |= (1 << PB7)
#define DEBUGPIN_OFF  PORTB &= ~(1 << PB7)
#define DEBUGSET      DDRB |= (1 << DDB7)

//! prototypes
void Init(void);
unsigned char HexToASCII (unsigned char ch);
unsigned char GetNextChar(void);
void ManualControl(void);
void DebugMonitor(void);
void USART0_Transmit( unsigned char data );
unsigned char USART0_Receive( void );