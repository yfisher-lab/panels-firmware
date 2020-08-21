/*****************************************************************************
*
* Atmel Corporation
*
* File              : serial.c
* Compiler          : IAR C 3.10C Kickstart, AVR-GCC/avr-libc(>= 1.2.5)
* Revision          : $Revision: 1 $
* Date              : $Date: Wednesday, April 22, 2009 $
* Updated by        : $Author: raapeland 
*
* Support mail      : avr@atmel.com
*
* Target platform   : All AVRs with bootloader support
*
* AppNote           : AVR109 - Self-programming
*
* Description       : UART communication routines
****************************************************************************/
#include "defines.h"
/*! \brief Initializing UART communcation.
 *
 *  This function initializes the UART communication with generic parameters as mentioned below. 
 *  Both Enabling both TRASMISSION and RECEPTION
 *  BAUD RATE configured to BRREG_VALUE
 *  As this is important function of initializing the UART, it has to be called prior to start the communication.
 *
 */

/* 
void initbootuart(void)
{
	UART_PORT.DIRSET |= UART_TX_PIN;
	BAUD_RATE_LOW_REG = BRREG_VALUE;
  	UART_CONTROL_REG = 	(1 << ENABLE_RECEIVER_BIT) | 
						(1 << CLK2X_BIT) |
                    	(1 << ENABLE_TRANSMITTER_BIT); // enable receive and transmit 
}
*/

// setup the uart for 115200 baud
void initbootuart(void)
{
  UART_PORT.DIRSET |= UART_TX_PIN;
  USARTC0.BAUDCTRLA = 0x17;   // BSCALE = -6, BSEL = 1047
  USARTC0.BAUDCTRLB = 0xA4;   // ==> 115211 bps (~115.2kbps)
  USARTC0.CTRLB = 0x18;       // Enable RX and TX
}

/*! \brief Transmitting a character UART communcation.
 *
 *  This function takes the unsigned char input given to the function and transmits out in the UART communication.
 *
 *  This function is called whenever a single character has to be transmitted in the UART communication.
 *  \param  c     Character value to be transmitted.
 *
 */
void sendchar(unsigned char c)
{ 
    UART_DATA_REG = c; // prepare transmission
    while (!(UART_STATUS_REG & (1 << TRANSMIT_COMPLETE_BIT)));
    // wait until byte sendt
    UART_STATUS_REG |= (1 << TRANSMIT_COMPLETE_BIT); // delete TXCflag
}

/*! \brief Receiving a character in UART communcation.
 *
 *  This function confirms the reception of data in UART, receives that character and returns the received character to the called function.
 *
 *  This function is called whenever a single charater has to be received from the UART communication.
 *
 *  \return  Character value received from UART communication.
 */

unsigned char recchar(void)
{
    unsigned char ret;
	while(!(UART_STATUS_REG & (1 << RECEIVE_COMPLETE_BIT)));  // wait for data
    ret = UART_DATA_REG;
  	return ret;
}

