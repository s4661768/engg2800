/*
**************************************************************************************************************
* file: uart.c
* brief:
* author: Peter Sutton, ENGG2800 Team 7
**************************************************************************************************************
*/

#include <avr/io.h>

/*
 * This code was written by Peter Sutton for CSSE2010. It has been adapted by 
 * Team 7 for the Atmega328P for ENGG2800.
 * This code implements a simple UART interface by redirecting stdio to the UART. 
 * This means you can use stdio functions such as printf and fprintf to send out bytes and
 * functions like fgetc to receive UART bytes. Before attempting to send and receive anything, 
 * UART must be initialised using the init_serial_stdio() function. A circular buffer and 
 * interrupts based output are used o store output messages allowing you to print many 
 * characters at once to the buffer). If the buffer is full, the function will either
 * (1) block until there is room in it (if interrupts are enabled)
 * (2) discard the character if (if interrupts aren't enabled)
 * Input is a blocking request and will block forever if interrupts aren't enabled so use
 * input_available() function to check the state of stdin 
 */

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/interrupt.h>
#include <avr/io.h>

/* Setting clock rate */
#define SYSCLK 8000000L
#define F_CPU 8000000
#include <util/delay.h>

/* Global variables */
/* Circular buffer to hold outgoing characters. The insert_pos variable
 * keeps track of the position (0 to OUTPUT_BUFFER_SIZE-1) that the next
 * outgoing character should be written to. bytes_in_buffer keeps
 * count of the number of characters currently stored in the buffer 
 * (ranging from 0 to OUTPUT_BUFFER_SIZE). This number of bytes immediately
 * prior to the current insert_pos are the bytes waiting to be output.
 * If the insert_pos reaches the end of the buffer it will wrap around
 * to the beginning (assuming those bytes have been output).
 * NOTE - OUTPUT_BUFFER_SIZE can not be larger than 255 without changing
 * the type of the variables below (currently defined as 8 bit unsigned ints).
 */
#define OUTPUT_BUFFER_SIZE 255
volatile char out_buffer[OUTPUT_BUFFER_SIZE];
volatile uint8_t out_insert_pos;
volatile uint8_t bytes_in_out_buffer;

/* Circular buffer to hold incoming characters. Works on same principle
 * as output buffer
 */
#define INPUT_BUFFER_SIZE 16
volatile char input_buffer[INPUT_BUFFER_SIZE];
volatile uint8_t input_insert_pos;
volatile uint8_t bytes_in_input_buffer;
volatile uint8_t input_overrun;

static int8_t do_echo;

/* FUNCTION PROTOTYPES */
static int uart_put_char(char, FILE*);
static int uart_get_char(FILE*);

/** Set up a stream that utilises UART put_char and get_char functions*/
static FILE myStream = FDEV_SETUP_STREAM(uart_put_char, uart_get_char,
    _FDEV_SETUP_RW);

void init_serial_stdio(long baudrate, int8_t echo)
{
    uint16_t ubrr;
    /* Initialising the buffer */
    out_insert_pos = 0;
    bytes_in_out_buffer = 0;
    input_insert_pos = 0;
    bytes_in_input_buffer = 0;
    input_overrun = 0;

    do_echo = echo;

    /* Set the baud rate for UART */
    ubrr = ((SYSCLK / (8 * baudrate)) + 1) / 2 - 1;
    UBRR0 = ubrr;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable RX and TX for UART.

    UCSR0B |= (1 << RXCIE0); // Enable receive complete interrupt.

    /* Redirection stdio streams to UART streams */
    stdout = &myStream;
    stdin = &myStream;
}

/** Checks if there is data waiting to be read in the buffer
*
* Returns:
* boolean: false if the buffer is empty, true otherwise/
*/
int8_t serial_input_available(void)
{
    return (bytes_in_input_buffer != 0);
}

static int uart_put_char(char c, FILE* stream)
{
    uint8_t interrupts_enabled;

    if (c == '\n') { // nNw line char is swapped for a carriage return.
        uart_put_char('\r', stream);
    }

    /* If interrupts are enabled we output the char if the buffer has space.
	* If the buffer doesn't have space we will loop until it does.
	* If the buffer is full and interrupts are not enabled the function exits
	*/
    interrupts_enabled = bit_is_set(SREG, SREG_I);
    while (bytes_in_out_buffer >= OUTPUT_BUFFER_SIZE) {
        if (!interrupts_enabled) {
            return 1;
        }
    }

    /* Add the character to the buffer for transmission if there
	 * is space to do so. Increment insert_pos. Interrupts are disable to 
	 * prevent concurrent modification of the 
	 * the buffer by the ISR.
	*/
    cli();
    out_buffer[out_insert_pos++] = c;
    bytes_in_out_buffer++;
    if (out_insert_pos == OUTPUT_BUFFER_SIZE) {
        /* Wrap around buffer pointer if necessary */
        out_insert_pos = 0;
    }

    /* Re-enable interrupts */
    UCSR0B |= (1 << UDRIE0);
    if (interrupts_enabled) {
        sei();
    }
    return 0;
}

int uart_get_char(FILE* stream)
{
    /* Block until char is received */
    while (bytes_in_input_buffer == 0) {
        /* do nothing */
    }

    /* Disable interrupts, remove char, re-enable interrupts */
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);
    cli();
    char c;
    if (input_insert_pos - bytes_in_input_buffer < 0) {
        /* Need to wrap around */
        c = input_buffer[input_insert_pos - bytes_in_input_buffer
            + INPUT_BUFFER_SIZE];
    } else {
        c = input_buffer[input_insert_pos - bytes_in_input_buffer];
    }

    /* Decrement our count of bytes in the input buffer */
    bytes_in_input_buffer--;
    if (interrupts_enabled) {
        sei();
    }
    return c;
}

/** Uart Data Register Empty ISR. */
ISR(USART_UDRE_vect)
{
    if (bytes_in_out_buffer > 0) { // Check if we have data in our buffer.
        /* Output the pending char */
        char c;
        if (out_insert_pos - bytes_in_out_buffer < 0) {
            /* Need to wrap around */
            c = out_buffer[out_insert_pos - bytes_in_out_buffer
                + OUTPUT_BUFFER_SIZE];
        } else {
            c = out_buffer[out_insert_pos - bytes_in_out_buffer];
        }
        /* Decrement counter for bytes in the the buffer */
        bytes_in_out_buffer--;

        UDR0 = c; // Output the char
    } else {
        /* Empty buffer. Disable the UART Data
		 * Register Empty interrupt otherwise it 
		 * will trigger again immediately this ISR exits. 
		 * The interrupt is re-enabled when a character is
		 * placed in the buffer.
		 */
        UCSR0B &= ~(1 << UDRIE0);
    }
}

/** Uart Receive Complete ISR.
*  
* Reads in char and puts it in the buffer.
*/
ISR(USART_RX_vect)
{

    char c;
    c = UDR0; // Read the char.

    if (do_echo && bytes_in_out_buffer < OUTPUT_BUFFER_SIZE) { // Echo the char is echo is enabled.
        uart_put_char(c, 0);
    }

    /* Check if buffer is full */
    if (bytes_in_input_buffer >= INPUT_BUFFER_SIZE) { // If full set overrun flag and ignore char.
        input_overrun = 1;
    } else {
        if (c == '\r') { // Convert carriage return to new line.
            c = '\n';
        }

        /* The buffer is not full */
        input_buffer[input_insert_pos++] = c;
        bytes_in_input_buffer++;
        if (input_insert_pos == INPUT_BUFFER_SIZE) {
            input_insert_pos = 0;
        }
    }
}
