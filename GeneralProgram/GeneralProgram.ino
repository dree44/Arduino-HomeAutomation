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

String serialString;

ExtendedSD SDCard;
ExtendedEthernet ETH;

#define BIPSENSOR_PIN 21 //todo basicsetting
//String startbipevent; //todo basicsetting
//------------------------------------------------------
void setup() {
  delay(6000);// todo basicsetting
  Serial.begin(9600);
  FREEMEM();
  serialString="";

  SDCard.Init();
  SDCard.DetailedCheck(GW_SERIAL);
  
  ETH.Init();
  ETH.DetailedCheck(GW_SERIAL);
  
  String error;
  if(!SDCard.LoadIO(error)) Serial.println(error);
  //  SDCard.SaveIO();
  PrintPinSettings(GW_SERIAL);

// todo 
// bip->set(&startbipevent->value);
      digitalWrite(BIPSENSOR_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(10);               // wait for a second
      digitalWrite(BIPSENSOR_PIN, LOW);    // turn the LED off by making the voltage LOW
      delay(100);              // wait for a second
      digitalWrite(BIPSENSOR_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(10);               // wait for a second
      digitalWrite(BIPSENSOR_PIN, LOW);    // turn the LED off by making the voltage LOW
      delay(100);               // wait for a second
      digitalWrite(BIPSENSOR_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(10);               // wait for a second
      digitalWrite(BIPSENSOR_PIN, LOW);    // turn the LED off by making the voltage LOW
      delay(10);               // wait for a second
      
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
      serialString+=inChar;
    } else {
      String errorString="";
      String response="";
      ParseTextCommand(serialString,response,errorString);
      if(errorString!="") {
        Serial.print("ERROR: ");
        Serial.println(errorString);
      } else {
        Serial.println(response);
      }
      serialString="";
    }
  }
}
