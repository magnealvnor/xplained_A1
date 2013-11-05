/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/* Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <stdio.h>
#include <avr/io.h>

static int uart_putchar(char c, FILE *stream);
static void uart_init (void);

static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);


int main (void)
{
	uart_init();
	board_init();
	stdout = &mystdout;
	
	while (1)
	printf("Hello, world!\n");
}


static int uart_putchar (char c, FILE *stream)
{
	if (c == '\n')
	uart_putchar('\r', stream);
	
	// Wait for the transmit buffer to be empty
	while ( !( USARTC0.STATUS & USART_DREIF_bm) );
	
	// Put our character into the transmit buffer
	USARTC0.DATA = c;
	
	return 0;
}


// Init USART.  Transmit only (we're not receiving anything)
// We use USARTC0, transmit pin on PC3.
// Want 9600 baud. Have a 2 MHz clock. BSCALE = 0
// BSEL = ( 2000000 / (2^0 * 16*9600)) -1 = 12
// Fbaud = 2000000 / (2^0 * 16 * (12+1))  = 9615 bits/sec
static void uart_init (void)
{
	// Set the TxD pin high - set PORTC DIR register bit 3 to 1
	PORTC.OUTSET = PIN3_bm;
	
	// Set the TxD pin as an output - set PORTC OUT register bit 3 to 1
	PORTC.DIRSET = PIN3_bm;
	
	// Set baud rate & frame format
	USARTC0.BAUDCTRLB = 0;			// BSCALE = 0 as well
	USARTC0.BAUDCTRLA = 12;
	
	// Set mode of operation
	USARTC0.CTRLA = 0;				// no interrupts please
	USARTC0.CTRLC = 0x03;			// async, no parity, 8 bit data, 1 stop bit
	
	// Enable transmitter only
	USARTC0.CTRLB = USART_TXEN_bm;
}