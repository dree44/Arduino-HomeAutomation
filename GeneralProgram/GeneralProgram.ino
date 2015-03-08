#include <GeneralConfig.h>
#include <GeneralCommunication.h>
#include <GeneralLog.h>
#include <SD.h>
#include <ExtendedSD.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <GeneralBasics.h>
#include <GeneralIO.h>
#include <ExtendedEEPROM.h>
#include <EEPROM.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h> 
//#include <Time.h>
#include "DHT.h"
#include <MemoryFree.h>
#include <SPI.h>        
#include <ExtendedEthernet.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <avr/pgmspace.h>

//prog_uchar m_[] PROGMEM = {""};    
void setup() {
	pinMode(53, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(4, OUTPUT);
	digitalWrite(10,HIGH);
	digitalWrite(4, HIGH);
  delay(3000);// todo basicsetting
  Serial.begin(9600);

  digitalWrite(4, LOW);
  SDCard.Init();
  SDCard.DetailedCheck(GW_SERIAL);
  String error;
  if (!SDCard.LoadIO(error)) Serial.println(error);
  digitalWrite(4, HIGH);

  digitalWrite(10, LOW);
  ETH.Init();
  ETH.DetailedCheck(GW_SERIAL);
  digitalWrite(10, HIGH);

  MemoryCheck(GW_SERIAL);
 
  //  SDCard.SaveIO();
  //PrintPinSettings(GW_SERIAL);

  G_Name* bipName = (G_Name*)(gNames.find(gConfig.bipSensorName()));
  if (bipName) {
	  G_IOBase* bip = gActuators.digitalactuators.find(bipName->index);
	  if (bip) {
		  G_Name* beName = (G_Name*)(gNames.find(gConfig.bipEventName()));
		  if (beName) {
			  G_Event* be = (G_Event*)(gEvents.list.find(beName->index));
			  if(be) bip->set(&be->value);
		  }
	  }
  }

  //Clocksetup

}
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

void loop() {
  ETH.Receive();
  gSensors.read();
  gTriggers.trigger();
  gActuators.write();
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read(); 
    if(inChar!='\n') {
      serialInputString+=inChar;
    } else {
      String errorString="";
      String response="";
	  ParseTextCommand(GW_SERIAL,serialInputString, response, errorString);
      if(errorString!="") {
        Serial.print(F("ERROR: "));
        Serial.println(errorString);
      } else {
        Serial.println(response);
      }
	  serialInputString = "";
    }
  }
}
