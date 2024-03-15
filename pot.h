/*
**************************************************************************************************************
* file: pot.h
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#ifndef __POT_H__
#define __POT_H__

/** Sets the the wiper value in the digital potentiometer to wiperVal and
* saves that new value to EEPROM using save_wiper_val().
*
* Variables:
* volume: the wiper value for the digital potentiometer.
*/
void set_volume(uint8_t volume);

#endif