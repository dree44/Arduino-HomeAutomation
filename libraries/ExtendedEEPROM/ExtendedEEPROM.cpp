#include "ExtendedEEPROM.h"

void EEPROMReadByte(unsigned int& pos, byte& b) {
  b=EEPROM.read(pos);
  pos++;
}

void EEPROMReadString(unsigned int& pos, String& string) {
  byte snum;
  snum=EEPROM.read(pos);
  pos++;
  for(int j=0;j<snum;++j) string+=char(EEPROM.read(pos+j));
  pos+=snum;
}

void EEPROMReadInt(unsigned int& pos, int& value) {
  value=(EEPROM.read(pos)<<24) & 0xFF000000;
  value=((EEPROM.read(pos+1)<<16) & 0xFF0000) | value;
  value=((EEPROM.read(pos+2)<<8) & 0xFF00) | value;
  value=(EEPROM.read(pos+3)) | value;
  pos+=4;
}

void EEPROMReadUnsignedShort(unsigned int& pos, unsigned short& value) {
  value=(EEPROM.read(pos)<<8) & 0xFF00;
  value=(EEPROM.read(pos+1)) | value;
  pos+=2;
}

void EEPROMReadUnsignedInt(unsigned int& pos, unsigned int& value) {
  value=(EEPROM.read(pos)<<24) & 0xFF000000;
  value=((EEPROM.read(pos+1)<<16) & 0xFF0000) | value;
  value=((EEPROM.read(pos+2)<<8) & 0xFF00) | value;
  value=(EEPROM.read(pos+3)) | value;
  pos+=4;
}

void EEPROMWriteByte(unsigned int& pos, byte b) {
  EEPROM.write(pos,b);
  pos++;
}

void EEPROMWriteString(unsigned int& pos, String string) {
  byte snum=string.length();
  EEPROM.write(pos,snum);
  pos++;
  for(int j=0;j<snum;++j) EEPROM.write(pos+j,string[j]);
  pos+=snum;
}

void EEPROMWriteInt(unsigned int& pos, int value) {
  EEPROM.write(pos,(value>>24) & 0xFF);
  EEPROM.write(pos+1,(value>>16) & 0xFF);
  EEPROM.write(pos+2,(value>>8) & 0xFF);
  EEPROM.write(pos+3,value & 0xFF);
  pos+=4;
}

void EEPROMWriteUnsignedShort(unsigned int& pos, unsigned short value) {
  EEPROM.write(pos,(value>>8) & 0xFF);
  EEPROM.write(pos+1,value & 0xFF);
  pos+=2;
}

void EEPROMWriteUnsignedInt(unsigned int& pos, unsigned int value) {
  EEPROM.write(pos,(value>>24) & 0xFF);
  EEPROM.write(pos+1,(value>>16) & 0xFF);
  EEPROM.write(pos+2,(value>>8) & 0xFF);
  EEPROM.write(pos+3,value & 0xFF);
  pos+=4;
}
