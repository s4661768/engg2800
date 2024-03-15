/*
**************************************************************************************************************
* file: uart.h
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#ifndef __UART_H__
#define __UART_H__

/** Initialises UART communication for the Atmega */
void init_serial_stdio(long baudrate, int8_t echo);

/** Checks if there is data waiting to be read in the buffer 
*
* Returns:
* boolean: false if the buffer is empty, true otherwise/
*/
int8_t serial_input_available(void);

#endif