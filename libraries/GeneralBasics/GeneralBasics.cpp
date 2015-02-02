#include "GeneralBasics.h"
#include <MemoryFree.h>

int CharToHex(char in) 
{ 
 byte c = (byte) in; 
 if (c >= '0' && c <= '9') { 
 return c - '0'; 
 } else if (c >= 'a' && c <= 'f') { 
 return c - 'a' + 10; 
 } else if (c >= 'A' && c <= 'F') { 
 return c - 'A' + 10; 
 } else { 
 return -1; // getting here is bad: it means the character was invalid 
 } 
}

String ByteToHex(byte c) {
  String result;
  byte first=c/16;
  byte second=c%16;
  if(first<10) result+=char(first+'0'); else result+=char(first+'A'-10);
  if(second<10) result+=char(second+'0'); else result+=char(second+'A'-10);
  return result;
}

String doubleToString(double temp)
{
  char ascii[32];
  int frac;
  frac=(unsigned int)(temp*1000)%1000;  //get three numbers to the right of the deciaml point

  itoa((int)temp,ascii,10);
  strcat(ascii,".");
  itoa(frac,&ascii[strlen(ascii)],10); //put the frac after the deciaml

  String result;
  for(int i=0;i<strlen(ascii);++i) result+=ascii[i];
  return ascii;
}

boolean IsLetter(char c) {
  if(c>='a' && c <='z' || c>='A' && c <='Z') return true;
  return false;
}

boolean IsNumber(char c) {
  if(c>='0' && c <='9' || c=='.') return true;
  return false;
}

/*
void MString(String& str,prog_uchar* ptr) {
  char buffer[MSTRINGMAX];
  char c;
  for(int i=0;i<MSTRINGMAX;++i) {
    c = pgm_read_byte_near(ptr+i);
    str+=c;
    if(c=='\0') break;
  }
}
*/

void FREEMEM() {
  Serial.print(F("Free memory: "));
  Serial.print(freeMemory());
  Serial.println(F(" bytes"));
}	