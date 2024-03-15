/*
**************************************************************************************************************
* file: pot.c
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#include "communication.h"
#include "macros.h"
#include "memory.h"

/** Sets the the wiper value in the digital potentiometer to wiperVal and 
* saves that new value to EEPROM using save_wiper_val().
*
* Variables:
* volume: the wiper value for the digital potentiometer.
*/
void set_volume(uint8_t volume)
{
    if (volume > 127) {
        volume = 127;
    }

    uint8_t byte_1 = 0x00;
    uint8_t wiperVal = 0;
    wiperVal = 255 - 2 * volume;

    if (volume != 0) {
        ; // Do nothing.

    } else {
        byte_1 = 0x01; // Set the 9th bit high to completely mute the speaker.
    }

    //pot_update(wiperVal, 0x00); Use this for actual code
    pot_update(byte_1, wiperVal);
    save_wiper_val(volume);
}
