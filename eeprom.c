/*
**************************************************************************************************************
* file: eeprom.c
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#include "macros.h"
#include <avr/io.h>

/* 
* EEPROM addr
* Addresses 0-2 will be for LED colour values (0 - 255)
* Address 3 will be for the potentiometer wiper value (0 - 255)
* Address 4 will be for the DPAD emulation variable ('1' or '0')
*/

/** Reads the data at the address uiAddress and saves it to data.
*
* Variables:
* uiAddress: the 16 bit address byte of the data (EEPROM addresses range from 0 - 1023)
* data: a pointer to the variable the data is to be stored in
*
* Returns:
* EEPROM_INVALID_ADDR: returned if the address is out of bounds (greater than 1023)
* EEPROM_OK: returned if the read was successful.
*/
uint8_t EEPROM_read(uint16_t uiAddress, uint8_t* data)
{
    if (uiAddress > EEPROM_SIZE) {
        return EEPROM_INVALID_ADDR;
    }
    /* Wait for completion of previous write */
    while (EECR & (1 << EEPE))
        ;
    /* Set up address register */
    EEARH = (uiAddress & 0xFF00) >> 8;
    EEARL = (uiAddress & 0x00FF);
    /* Start eeprom read by writing EERE */
    EECR |= (1 << EERE);
    /* Return data from Data Register */
    *data = EEDR;
    return EEPROM_OK;
}

/** Writes the byte ucData to the memory at the address uiAddress.
*
* Variables:
* uiAddress: the 16 bit address byte of the data (EEPROM addresses range from 0 - 1023)
* ucData: the data byte to be written to the memory location
*
* Returns:
* EEPROM_INVALID_ADDR: returned if the address is out of bounds (greater than 1023)
* EEPROM_OK: returned if the write was successful.
*/
uint8_t EEPROM_write(uint16_t uiAddress, uint8_t ucData)
{
    if (uiAddress > EEPROM_SIZE) {
        return EEPROM_INVALID_ADDR;
    }

    /* Wait for completion of previous write */
    while (EECR & (1 << EEPE))
        ;
    /* Set up address and Data Registers */
    EEARH = (uiAddress & 0xFF00) >> 8;
    EEARL = (uiAddress & 0x00FF);
    //EEAR = uiAddress;
    EEDR = ucData;
    /* Write logical one to EEMPE */
    EECR |= (1 << EEMPE);
    /* Start eeprom write by setting EEPE */
    EECR |= (1 << EEPE);

    return EEPROM_OK;
}

/** Updates the data at the given address only if the data at the address if different from 
* the ucData. This is to minimise the number of writes to EEPROM. 
*	
* Variables:
* uiAddress: the 16 bit address byte of the data (EEPROM addresses range from 0 - 1023)
* ucData: the data byte to be written to the memory location
* 
* Returns:
* EEPROM_INVALID_ADDR: returned if the address is out of bounds (greater than 1023)
* EEPROM_WRITE_FAIL: returned if the write failed
* EEPROM_OK: returned if the write was successful or no write was needed
*/
uint8_t EEPROM_update(uint16_t uiAddress, uint8_t ucData)
{
    uint8_t err = EEPROM_OK;

    if (uiAddress > EEPROM_SIZE) {
        return EEPROM_INVALID_ADDR;
    }

    uint8_t value = 0;

    err = EEPROM_read(uiAddress, &value);
    if (err != EEPROM_OK) {
        return err;
    }
    /* Checking the value at the addr is different from the ucData value to reduce the number of */
    /* unnecessary writes. */
    if (value == ucData) {
        return EEPROM_OK;
    }

    err = EEPROM_write(uiAddress, ucData);
    if (err != EEPROM_OK) { // Checking it actually attempted to write.
        return err;
    }

    err = EEPROM_read(uiAddress, &value); // Checking that the above write was actually successful.
    if (err != EEPROM_OK) {
        return err;
    }

    if (value != ucData) {
        return EEPROM_WRITE_FAIL;
    }

    return EEPROM_OK;
}
