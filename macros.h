/*
**************************************************************************************************************
* file: macros.h
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#ifndef __MACROS_H__
#define __MACROS_H__

#define F_CPU 8000000UL
#define SYSCLK 8000000L

// SPI Macros
#define BR0 0X00
#define JSX 0X02
#define JSY 0X03
#define DPAD 0X07
#define SEND_REPORT 0XFF

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// UART Atmega to GUI
#define BUTTON "B"
#define VOLUME "V"
#define JOYSTICK_X "X"
#define JOYSTICK_Y "Y"
#define DPAD_MODE "D"

// -127 in hex 0x81
// 127 in hex 0x7F
// JOYSTICK DIRECTIONS
#define POS 0x7F
#define NEG 0x81
#define ZERO 0x00

// Digital Potentiometer Macros
#define STACK_SELECT 0x00

// EEPROM Address Macros (10 bit address)
#define LED_R_ADDR 0x0000
#define LED_G_ADDR 0X0001
#define LED_B_ADDR 0x0002
#define POT_ADDR 0x0003
#define DPAD_ADDR 0x0004

// Other EEPROM Macros
#define EEPROM_SIZE 1023

enum {
    EEPROM_OK,
    EEPROM_WRITE_FAIL,
    EEPROM_INVALID_ADDR
};

#define B0 0
#define B1 1
#define B2 2
#define B3 3
#define B4 4
#define B5 5
#define B6 6
#define B7 7

#endif