/*
**************************************************************************************************************
* file: main.c
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "communication.h"
#include "eeprom.h"
#include "hardware.h"
#include "macros.h"
#include "memory.h"
#include "pot.h"
#include "spi.h"
#include "uart.h"

#include <util/delay.h>

/** Parses the message received from the GUI and calls the functions need to 
* properly act on the message contents.
*
* Variables:
* addr <char*>: the UART macro defining what the data byte is for
* data <data*>
*/
void parse_message(char addr, char data)
{
    data = (int)data;
    if (addr == 'R') {
        EEPROM_update(LED_R_ADDR, data);
    } else if (addr == 'G') {
        EEPROM_update(LED_G_ADDR, data);
    } else if (addr == 'B') {
        EEPROM_update(LED_B_ADDR, data);
    } else if (addr == 'V') {
        set_volume(data);
    } else if (addr == 'D') {
        EEPROM_update(DPAD_ADDR, data);
    } else {
        ; //Do nothing, invalid message;
    }
}

/** Reads in the chars sent to the Atmega by the GUI over UART and parses
* them using the parse_message() function.
*/
void read_uart()
{
    char addr, data;
    /* Read in 2 bytes */
    addr = fgetc(stdin);
    data = fgetc(stdin);
    parse_message(addr, data);
}

int main(void)
{
    /* Initialisations */
    sei(); // Enable global interrupts.
    joystick_init_2(); // Initialise Joystick.
    rgb_led_init(); // Initialise LED GPIO pins.
    rgb_led_PWM_init(); // Initialising LED PWM.
    init_serial_stdio(9600, 0); // Initialise UART.
    spi_master_init(); // Initialise SPI.
    button_init_2(); // Initialise buttons.

    char data = 0x00;
    char oldData = 0x00;
    int joystickDirectionX = 0;
    int joystickDirectionY = 0;
    int dpad_byte = 0;
    int X = 0;
    int Y = 0;

    uint8_t emMode = 0;
    uint8_t volume = 0;

    /* Read in volume from memory and set the potentiometer to it */
    EEPROM_read(POT_ADDR, &volume);
    set_volume(volume);

    while (1) {
        /* Sending emMode to GUI */

        EEPROM_read(DPAD_ADDR, &emMode);
        if (emMode == '1') {
            printf("%s%d\n", DPAD_MODE, 1);
        } else {
            printf("%s%d\n", DPAD_MODE, 0);
        }

        /* GUI Message Check and Parsing */
        if (serial_input_available()) { // Checking for and reading UART messages from GUI.
            read_uart();
        }

        /* Button polling and updating */
        data = poll_button_press();
        if (data != oldData) {
            oldData = 0x00;
            oldData = data;
            update(BUTTON, BR0, data);
        }

        /* Joystick Polling */
        joystickDirectionX = poll_joystick_x();
        joystickDirectionY = poll_joystick_y();

        switch (joystickDirectionX) {
        case 7: // POSITIVE X
            X = POS;
            dpad_byte |= (1 << RIGHT);
            printf("%s%d\n", JOYSTICK_X, 2);
            break;
        case 10: // NEGATIVE X
            X = NEG;
            dpad_byte |= (1 << LEFT);
            printf("%s%d\n", JOYSTICK_X, 1);
            break;
        default:
            X = ZERO;
            dpad_byte &= ~((1 << RIGHT) | (1 << LEFT));
            printf("%s%d\n", JOYSTICK_X, 0);
            break;
        }

        switch (joystickDirectionY) {
        case 8: // POSITVE Y
            Y = NEG;
            dpad_byte |= (1 << UP);
            printf("%s%d\n", JOYSTICK_Y, 2);
            break;
        case 9: // NEGATIVE Y
            Y = POS;
            dpad_byte |= (1 << DOWN);
            printf("%s%d\n", JOYSTICK_Y, 1);
            break;
        default:
            Y = ZERO;
            dpad_byte &= ~((1 << UP) | (1 << DOWN));
            printf("%s%d\n", JOYSTICK_Y, 0);
            break;
        }

        if (emMode == '1') {
            spi_update(DPAD, dpad_byte);
            spi_update(JSX, ZERO);
            spi_update(JSY, ZERO);
        } else {
            spi_update(DPAD, ZERO);
            spi_update(JSX, X);
            spi_update(JSY, Y);
        }
    }
    return 0;
}

/*ISR for timer 0 interrupts (8 bits)*/
ISR(TIMER0_OVF_vect)
{
    //Update output compare registers
    uint8_t red, green;

    EEPROM_read(LED_R_ADDR, &red);
    dutyCycleRed = 255 - red;
    OCR0A = dutyCycleRed;

    EEPROM_read(LED_G_ADDR, &green);
    dutyCycleGreen = 255 - green;
    OCR0B = dutyCycleGreen;
}

/*ISR for timer 2 interrupts (8 bits)*/
ISR(TIMER2_OVF_vect)
{
    //Update output compare registers
    uint8_t blue;

    EEPROM_read(LED_B_ADDR, &blue);
    dutyCycleBlue = 255 - blue;

    //Update output compare registers
    OCR2B = dutyCycleBlue;
}