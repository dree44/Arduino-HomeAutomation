#ifndef extendedeeprom
#define extendedeeprom

#include "Arduino.h"
#include <EEPROM.h>

#define EEPROM_RFID_START 0
#define EEPROM_RFID_COUNT 16
#define EEPROM_RFID_RECORDSIZE 57 //1 validity, 8+1 serial, 1 sector, 12+1 key, 32+1 content


class ExtendedEEPROMClass {
public:
	unsigned int getFreeAuthzRecord();
	void SerialPrintContent(unsigned int,unsigned int);
	void EraseContent(unsigned int, unsigned int);

	void ReadByte(unsigned int& pos, byte& b);
	void ReadString(unsigned int& pos, String& string);
	void ReadInt(unsigned int& pos, int& value);
	void ReadUnsignedShort(unsigned int& pos, unsigned short& value);
	void ReadUnsignedInt(unsigned int& pos, unsigned int& value);
	void WriteByte(unsigned int& pos, byte b);
	void WriteString(unsigned int& pos, String string);
	void WriteInt(unsigned int& pos, int value);
	void WriteUnsignedShort(unsigned int& pos, unsigned short value);
	void WriteUnsignedInt(unsigned int& pos, unsigned int value);
};
#endif