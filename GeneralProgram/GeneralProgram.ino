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
  delay(3000);// todo basicsetting
  Serial.begin(9600);

  SDCard.Init();
  SDCard.DetailedCheck(GW_SERIAL);
  
  ETH.Init();
  ETH.DetailedCheck(GW_SERIAL);
  
  String error;
  if(!SDCard.LoadIO(error)) Serial.println(error);

  MemoryCheck(GW_SERIAL);

  //  SDCard.SaveIO();
//  PrintPinSettings(GW_SERIAL);

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
	  ParseTextCommand(serialInputString, response, errorString);
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
