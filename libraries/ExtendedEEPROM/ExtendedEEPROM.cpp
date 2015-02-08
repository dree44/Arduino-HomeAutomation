#include "ExtendedEEPROM.h"
#include "GeneralBasics.h"


unsigned int ExtendedEEPROMClass::getFreeAuthzRecord() {
	unsigned int recordpos = EEPROM_RFID_START;
	unsigned int pos;
	for (; recordpos < EEPROM_RFID_RECORDSIZE*EEPROM_RFID_COUNT; recordpos += EEPROM_RFID_RECORDSIZE) {
		pos = recordpos;
		byte lead;
		ReadByte(pos, lead);
		if (lead != 0x11) return recordpos;
	}
	return -1; //no place
}

void ExtendedEEPROMClass::SerialPrintContent(unsigned int from, unsigned int to) {
	String head, content;
	unsigned int pos;
	for (pos = from; pos <= to; ++pos) {
		if (pos % 32 == 0) {
			head = "0x";
			head += ByteToHex(pos / 256);
			head += ByteToHex(pos % 256);
			head += " ";
			content = " | ";
		}
		byte b = EEPROM.read(pos);
		head += ByteToHex(b);
		content += char(b);
		if (pos % 32 == 31) {
			Serial.print(head);
			Serial.println(content);
		}
	}
	if (pos % 32 == 31) {
		Serial.print(head);
		Serial.println(content);
	}
}

void ExtendedEEPROMClass::EraseContent(unsigned int from, unsigned int to) {
	unsigned int pos;
	for (pos = from; pos <= to; ++pos) {
		EEPROM.write(pos,0xff);
	}
}

void ExtendedEEPROMClass::ReadByte(unsigned int& pos, byte& b) {
  b=EEPROM.read(pos);
  pos++;
}

void ExtendedEEPROMClass::ReadString(unsigned int& pos, String& string) {
  byte snum;
  snum=EEPROM.read(pos);
  pos++;
  for(int j=0;j<snum;++j) string+=char(EEPROM.read(pos+j));
  pos+=snum;
}

void ExtendedEEPROMClass::ReadInt(unsigned int& pos, int& value) {
  value=(EEPROM.read(pos)<<24) & 0xFF000000;
  value=((EEPROM.read(pos+1)<<16) & 0xFF0000) | value;
  value=((EEPROM.read(pos+2)<<8) & 0xFF00) | value;
  value=(EEPROM.read(pos+3)) | value;
  pos+=4;
}

void ExtendedEEPROMClass::ReadUnsignedShort(unsigned int& pos, unsigned short& value) {
  value=(EEPROM.read(pos)<<8) & 0xFF00;
  value=(EEPROM.read(pos+1)) | value;
  pos+=2;
}

void ExtendedEEPROMClass::ReadUnsignedInt(unsigned int& pos, unsigned int& value) {
  value=(EEPROM.read(pos)<<24) & 0xFF000000;
  value=((EEPROM.read(pos+1)<<16) & 0xFF0000) | value;
  value=((EEPROM.read(pos+2)<<8) & 0xFF00) | value;
  value=(EEPROM.read(pos+3)) | value;
  pos+=4;
}

void ExtendedEEPROMClass::WriteByte(unsigned int& pos, byte b) {
  EEPROM.write(pos,b);
  pos++;
}

void ExtendedEEPROMClass::WriteString(unsigned int& pos, String string) {
  byte snum=string.length();
  EEPROM.write(pos,snum);
  pos++;
  for(int j=0;j<snum;++j) EEPROM.write(pos+j,string[j]);
  pos+=snum;
}

void ExtendedEEPROMClass::WriteInt(unsigned int& pos, int value) {
  EEPROM.write(pos,(value>>24) & 0xFF);
  EEPROM.write(pos+1,(value>>16) & 0xFF);
  EEPROM.write(pos+2,(value>>8) & 0xFF);
  EEPROM.write(pos+3,value & 0xFF);
  pos+=4;
}

void ExtendedEEPROMClass::WriteUnsignedShort(unsigned int& pos, unsigned short value) {
  EEPROM.write(pos,(value>>8) & 0xFF);
  EEPROM.write(pos+1,value & 0xFF);
  pos+=2;
}

void ExtendedEEPROMClass::WriteUnsignedInt(unsigned int& pos, unsigned int value) {
  EEPROM.write(pos,(value>>24) & 0xFF);
  EEPROM.write(pos+1,(value>>16) & 0xFF);
  EEPROM.write(pos+2,(value>>8) & 0xFF);
  EEPROM.write(pos+3,value & 0xFF);
  pos+=4;
}
