/*
**************************************************************************************************************
* file: communication.c
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#include "communication.h"
#include "macros.h"
#include "spi.h"
#include "uart.h"
#include <stdio.h>
#include <util/delay.h>

/** Updates the GamePad by sending 'reg' and 'data' bytes to turtle followed by 'SEND_REPORT'
* over SPI.
*
* reg: is the appropriate register macro for SPI protocol as per macros.h (e.g. BR0, JSX, DPAD, etc.)
* data: is the data byte to be sent via SPI.
*
* SEND_REPORT must be sent to the turtle in order to actually push the changes to the GamePad.
*/
void spi_update(char reg, char data)
{
    _delay_us(100); //Ensures SS has been held high long enough after previous message
    select_turtle();
    spi_master_transmit(reg);
    spi_master_transmit(data);
    deselect_turtle();

    _delay_ms(1); // Ensures SS is held high for long enough to be recognised by turtle.

    // Telling the Turtle to update the GamePad
    select_turtle();
    spi_master_transmit(SEND_REPORT);
    spi_master_transmit(0x00);
    deselect_turtle();
}

/** Sends the bytes passed to it using UART communication.
*
* addr: is the appropriate register macro for UART protocol as per macros.h (e.g. BUTTON, RLED, etc.)
* data: is the data byte to sent via UART
*
* The message is sent via a printf call of this format:
*	printf("%s%c\n", addr, data);
*/
void uart_update(char* addr, char data)
{
    printf("%s%c\n", addr, data);
}

/** Updates the GUI and GamePad with the data passed to it.
*
* addr: is the appropriate register macro for UART protocol as per macros.h (e.g. BUTTON, RLED, etc.)
* reg: is the appropriate register macro for SPI protocol as per macros.h (e.g. BR0, JSX, DPAD, etc.)
* data: is the data byte to be sent via UART and SPI
*
* This method calls uart_update() and spi_update to send the macro and data bytes over the respective
* communication platform.
*/
void update(char* addr, char reg, char data)
{
    spi_update(reg, data);
    uart_update(addr, data);
}

/** Updates the potentiometer wiper values using SPI.
*
* w1: the value to be written to wiper 1 of the potentiometer
* w2: the value to be written to wiper 2 of the potentiometer
*/
void pot_update(char w1, char w2)
{
    select_pot();
    spi_master_transmit(w1);
    spi_master_transmit(w2);
    deselect_pot();
}