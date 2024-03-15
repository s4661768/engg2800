/*
**************************************************************************************************************
* file: hardware.h
* brief: 
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

#include "macros.h"
#include <util/delay.h>

#define BUTTON_BIT_MASK_PINC ~((0X01 << PINC0) | (0x01 << PINC1) | (0x01 << PINC2) | (0x01 << PINC3) | (0x01 << PINC4) | (0x01 << PINC5))
#define BUTTON_BIT_MASK_PIND ~((0x01 << PIND3))
#define JOYSTICK_PORTD_BITMASK ((1 << PIND4) | (1 << PIND7))
#define JOYSTICK_PORTB_BITMASK ((1 << PINB7) | (1 << PINB6))

static volatile uint8_t dutyCycleRed = 0;
static volatile uint8_t dutyCycleGreen = 0;
static volatile uint8_t dutyCycleBlue = 0;

/** Set GPIO pins for RGB LEDs */
void rgb_led_init(void);

/*Initialise and control PWM for rgb LEDs*/
void rgb_led_PWM_init(void);

/** Sets the duty cycle variables for the LEDs and saves them to EEPROM.
*
* Variables:
* red: duty cycle for the red colour of the LEDs
* green: duty cycle for the green colour of the LEDs
* blue: duty cycle for the blue colour of the LEDs
*/
void set_duty_cycles(uint8_t red, uint8_t green, uint8_t blue);

/** Initialises the button pins as inputs with internal pull-up resistors */
void button_init_2(void);

/** Poll the input pins for the buttons
*
* Checks the state of each pin. If a pin is low (button pressed) a 1 is bit shifted into
* buttonsPressed by the number corresponding to the button pressed. E.g. 0b00000010 means
* button 2 on the controller has been pressed.
*
* Returns:
* buttonsPressed: byte representing the state of the buttons according to the turtle
*	communication protocol.
*/
int poll_button_press(void);

/** Initialises the joystick pins as inputs with internal pull-up resistors */
void joystick_init_2(void);

/** Poll the joystick X controls for pins: PD4, PD7
*
* Returns:
*	joystick: the current state of the joysticks X position 0 for 0, 7 for +X, and 10 for -X.
*/
int poll_joystick_x(void);

/** Poll the joystick Y controls for pins: PB6, PB7
*
* Returns:
* joystick: the current state of the joysticks Y position 0 for 0, 8 for +Y, and 9 for -Y.
*/
int poll_joystick_y(void);

#endif