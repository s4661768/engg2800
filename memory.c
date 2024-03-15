/*
**************************************************************************************************************
* file: memory.c
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#include "memory.h"
#include "eeprom.h"
#include "macros.h"
#include <avr/io.h>

/** Saves the LED duty cycle variables to EEPROM using EEPROM_update() .
*
* Variables:
* red: duty cycle for the red colour of the LEDs
* green: duty cycle for the green colour of the LEDs
* blue: duty cycle for the blue colour of the LEDs
*/
void save_duty_cycles(uint8_t red, uint8_t green, uint8_t blue)
{
    EEPROM_update(LED_R_ADDR, red);
    EEPROM_update(LED_G_ADDR, green);
    EEPROM_update(LED_B_ADDR, blue);
}

/** Saves the wiper value of the digital potentiometer to EEPROM using
* EEPROM_update().
*
* Variables:
* wiperVal: the wiper value for the digital potentiometer.
*/
void save_wiper_val(uint8_t wiperVal)
{
    EEPROM_update(POT_ADDR, wiperVal);
}

/** Saves the DPAD emulation mode variable to EEPROM using EEPROM_update().
*
* Variables:
* emMode: the emulation mode
*/
void save_em_mode(uint8_t em_mode)
{
    EEPROM_update(DPAD_ADDR, em_mode);
}

/** Reads the LED duty cycle variables from EEPROM and saves them to the
* corresponding duty cycle variables using EEPROM_read().
*
* Variables:
* dutyCycleRed: pointer to the variable to store the duty cycle of the
*	red colour
* dutyCycleGreen: pointer to the variable to store the duty cycle
*	of the green colour
* dutyCycleBlue: pointer to the variable to store the duty cycle of the
*	blue colour
*/
void get_duty_cycles(uint8_t* dutyCycleRed, uint8_t* dutyCycleGreen, uint8_t* dutyCycleBlue)
{
    EEPROM_read(LED_R_ADDR, dutyCycleRed);
    EEPROM_read(LED_G_ADDR, dutyCycleGreen);
    EEPROM_read(LED_B_ADDR, dutyCycleRed);
}

/** Reads the wiper value from EEPROM and saves it to wiperVal
* using EEPROM_read().
*
* Variables:
* wiperVal: the pointer to the variable to store the wiper value of the
*	digital potentiometer
*/
void get_wiper_val(uint8_t* wiperVal)
{
    EEPROM_read(POT_ADDR, wiperVal);
}

/** Reads the emulation mode variable from EEPROM and saves it to emMode
* using EEPROM_read().
*
* Variables:
* emMode: pointer to the variable to store the emulation mode variable
*/
void get_em_mode(uint8_t* em_mode)
{
    EEPROM_read(DPAD_ADDR, em_mode);
}