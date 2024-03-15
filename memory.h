/*
**************************************************************************************************************
* file: memory.h
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>

/** Saves the LED duty cycle variables to EEPROM using EEPROM_update() .
*
* Variables:
* red: duty cycle for the red colour of the LEDs
* green: duty cycle for the green colour of the LEDs
* blue: duty cycle for the blue colour of the LEDs
*/
void save_duty_cycles(uint8_t red, uint8_t green, uint8_t blue);

/** Saves the wiper value of the digital potentiometer to EEPROM using 
* EEPROM_update().
*
* Variables:
* wiperVal: the wiper value for the digital potentiometer.
*/
void save_wiper_val(uint8_t wiperVal);

/** Saves the DPAD emulation mode variable to EEPROM using EEPROM_update().
*
* Variables:
* emMode: the emulation mode 
*/
void save_em_mode(uint8_t emMode);

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
void get_duty_cycles(uint8_t* dutyCycleRed, uint8_t* dutyCycleGreen, uint8_t* dutyCycleBlue);

/** Reads the wiper value from EEPROM and saves it to wiperVal 
* using EEPROM_read().
*
* Variables:
* wiperVal: the pointer to the variable to store the wiper value of the 
*	digital potentiometer
*/
void get_wiper_val(uint8_t* wiperVal);

/** Reads the emulation mode variable from EEPROM and saves it to emMode
* using EEPROM_read().
*
* Variables:
* emMode: pointer to the variable to store the emulation mode variable
*/
void get_em_mode(uint8_t* emMode);

#endif
