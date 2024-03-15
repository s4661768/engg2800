/*
**************************************************************************************************************
* file: spi.h
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>

/** 
* Initialises SPI for Atmega328P. SPI pins are on DDRB
*/
void spi_master_init(void);

/** Transmits the given byte to the slave using SPI */
uint8_t spi_master_transmit(char data);

/** Selects the turtle as the SPI slave by setting SS pin on Atmega low.
* SS pin is active low
*/
void select_turtle(void);

/** De-selects the turtle as the SPI slave by setting SS pin on Atmega high
* SS pin is active low
*/
void deselect_turtle(void);

/** Selects the digital potentiometer as the SPI slave by setting PORTD2 high.
* PORTD2 is active high
*/
void select_pot(void);

/** De-selects the digital potentiometer as the SPI slave by setting PORTD2 low.
* PORTD2 is active high
*/
void deselect_pot(void);

#endif