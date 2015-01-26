#ifndef general_basics
#define general_basics

#include "Arduino.h"

#define MSTRINGMAX 100 //!!!?

int CharToHex(char in);
String ByteToHex(byte c);
String doubleToString(double temp);
boolean IsLetter(char c);
boolean IsNumber(char c);
//void MString(String& str,prog_uchar* ptr);
void FREEMEM();

#endif
