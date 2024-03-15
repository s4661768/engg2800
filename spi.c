/*
**************************************************************************************************************
* file: spi.c
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/
#include "spi.h"
#include <avr/interrupt.h>
#include <avr/io.h>

/** Initialises everything needed for SPI communication */
void spi_master_init(void)
{ /* Set SS as output  |  Set MOSI and SCK output, all others input */
    DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);

    /* Setting SS for potentiometer */
    DDRD |= (1 << DDD2);

    /* Setting SS as active low */
    PORTB |= (1 << PORTB2);

    /* Setting potentiometer SS as active high*/
    PORTD |= (1 << PORTD2);

    /* Enable SPI, Master, set clock rate fck/16, set clock mode */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (0 << CPOL) | (0 << CPHA);
}

/** Transmits the given byte to the slave using SPI */
uint8_t spi_master_transmit(char data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)))
        ;
    return SPDR;
}

/** Selects the turtle as the SPI slave by setting SS pin on Atmega low.
* SS pin is active low 
*/
void select_turtle(void)
{
    PORTB &= ~(1 << PORTB2);
}

/** De-selects the turtle as the SPI slave by setting SS pin on Atmega high 
* SS pin is active low 
*/
void deselect_turtle(void)
{
    PORTB |= (1 << PORTB2);
}

/** Selects the digital potentiometer as the SPI slave by setting PORTD2 high.
* PORTD2 is active high
*/
void select_pot(void)
{
    PORTD &= ~(1 << PORTD2);
}

/** De-selects the digital potentiometer as the SPI slave by setting PORTD2 low.
* PORTD2 is active high
*/
void deselect_pot(void)
{
    PORTD |= (1 << PORTD2);
}