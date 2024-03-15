/*
**************************************************************************************************************
* file: hardware.c
* brief: 
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#include "hardware.h"
#include "memory.h"

int countJoystick = 0;
int countButtons = 0;
int countJoystickX = 0;
int countJoystickY = 0;

/** Sets the duty cycle variables for the LEDs and saves them to EEPROM.
*
* Variables:
* red: duty cycle for the red colour of the LEDs
* green: duty cycle for the green colour of the LEDs
* blue: duty cycle for the blue colour of the LEDs
*/
void set_duty_cycles(uint8_t red, uint8_t green, uint8_t blue)
{
    save_duty_cycles(red, green, blue);
}

/** Set GPIO pins for RGB LEDs */
void rgb_led_init(void)
{
    DDRD |= (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD3); // D5 OC0B = GREEN, D6 OC0A = RED, D3 OC2B = BLUE
}

/*Initialise and control PWM for LEDs*/
void rgb_led_PWM_init(void)
{

    /* Set fast PWM */
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
    TCCR2A = (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);

    /* Set overflow interrupt */
    TIMSK0 = (1 << TOIE0);
    TIMSK2 = (1 << TOIE2);

    /* Set output compare registers */
    OCR0A = (0 / 100) * 255;
    OCR2B = (0 / 100) * 255;
    OCR0B = (0 / 100) * 255;

    /* Start timers, no prescalar */
    TCCR0B = (1 << CS01);
    TCCR2B = (1 << CS21);
}

/** Initialises the joystick pins as inputs with internal pull-up resistors */
void joystick_init_2(void)
{
    DDRD &= ~((1 << PORTD4) | (1 << PORTD7));
    PORTD |= (1 << PORTD4) | (1 << PORTD7);

    DDRB &= ~((1 << PORTB6) | (1 << PORTB7));
    PORTB |= (1 << PORTB6) | (1 << PORTB7);
}

/** Initialises the button pins as inputs with internal pull-up resistors */
void button_init_2(void)
{
    DDRC &= ~((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3) | (1 << PORTC4) | (1 << PORTC5));
    PORTC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3) | (1 << PORTC4) | (1 << PORTC5);

    DDRB &= ~((1 << PORTB1));
    PORTB |= ((1 << PORTB1));
}

/** Poll the joystick X controls for pins: PD4, PD7
*
* Returns:
*	joystick: the current state of the joysticks X position 0 for 0, 7 for +X, and 10 for -X.
*/
int poll_joystick_x(void)
{
    int joystickX = 0;

    if (!((PIND & (1 << PIND4)) == (1 << PIND4))) { // POSITIVE X
        joystickX = 7;
        countJoystickX++;
    }
    if (!((PIND & (1 << PIND7)) == (1 << PIND7))) { // NEGATIVE X
        joystickX = 10;
        countJoystickY++;
    }

    _delay_ms(1);

    if (countJoystickX == 12) {
        return joystickX;
    }
}

/** Poll the joystick Y controls for pins: PB6, PB7
*
* Returns:
* joystick: the current state of the joysticks Y position 0 for 0, 8 for +Y, and 9 for -Y.
*/
int poll_joystick_y(void)
{
    int joystickY = 0;

    if (!((PINB & (1 << PINB6)) == (1 << PINB6))) { // POSITIVE Y
        joystickY = 8;
        countJoystickX++;
    }
    if (!((PINB & (1 << PINB7)) == (1 << PINB7))) { // NEGATIVE Y
        joystickY = 9;
        countJoystickY++;
    }

    _delay_ms(1);

    if (countJoystickY == 12) {
        return joystickY;
    }
}

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
int poll_button_press(void)
{

    char buttonsPressed = 0x00;

    if (!((PINC & (1 << PINC0)) == (1 << PINC0))) {
        countButtons++;
        buttonsPressed |= (1 << 0);
    }
    if (!((PINC & (1 << PINC1)) == (1 << PINC1))) {
        countButtons++;
        buttonsPressed |= (1 << 1);
    }
    if (!((PINC & (1 << PINC2)) == (1 << PINC2))) {
        countButtons++;
        buttonsPressed |= (1 << 2);
    }
    if (!((PINC & (1 << PINC3)) == (1 << PINC3))) {
        countButtons++;
        buttonsPressed |= (1 << 3);
    }
    if (!((PINC & (1 << PINC4)) == (1 << PINC4))) {
        countButtons++;
        buttonsPressed |= (1 << 4);
    }
    if (!((PINC & (1 << PINC5)) == (1 << PINC5))) {
        countButtons++;
        buttonsPressed |= (1 << 5);
    }
    if (!((PINB & (1 << PINB1)) == (1 << PINB1))) {
        countButtons++;
        buttonsPressed |= (1 << 6);
    }

    _delay_ms(1);

    if (countButtons == 12) {
        return buttonsPressed;
    }
}