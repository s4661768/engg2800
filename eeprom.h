/*
**************************************************************************************************************
* file: eeprom.h
* brief:
* author: ENGG2800 Team 7
**************************************************************************************************************
*/

#ifndef __EEPROM_H__
#define __EEPROM_H__

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
uint8_t EEPROM_update(uint16_t uiAddress, uint8_t ucData);

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
uint8_t EEPROM_read(uint16_t uiAddress, uint8_t* data);

#endif