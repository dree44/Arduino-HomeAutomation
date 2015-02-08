#ifndef general_basics
#define general_basics

#include "Arduino.h"
#include <GeneralLog.h>

#define MSTRINGMAX 100 //!!!?

int CharToHex(char in);
String DigitToHex(byte c);
String ByteToHex(byte c);
String doubleToString(double temp,byte n);
boolean IsLetter(char c);
boolean IsNumber(char c);
//void MString(String& str,prog_uchar* ptr);
void FREEMEM();
void MemoryCheck(WRITING out);

#endif
