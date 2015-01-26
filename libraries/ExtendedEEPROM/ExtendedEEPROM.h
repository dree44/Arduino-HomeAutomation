#ifndef extendedeeprom
#define extendedeeprom

#include "Arduino.h"
#include <EEPROM.h>

void EEPROMReadByte(unsigned int& pos, byte& b);
void EEPROMReadString(unsigned int& pos, String& string);
void EEPROMReadInt(unsigned int& pos, int& value);
void EEPROMReadUnsignedShort(unsigned int& pos, unsigned short& value);
void EEPROMReadUnsignedInt(unsigned int& pos, unsigned int& value);
void EEPROMWriteByte(unsigned int& pos, byte b);
void EEPROMWriteString(unsigned int& pos, String string);
void EEPROMWriteInt(unsigned int& pos, int value);
void EEPROMWriteUnsignedShort(unsigned int& pos, unsigned short value);
void EEPROMWriteUnsignedInt(unsigned int& pos, unsigned int value);

#endif