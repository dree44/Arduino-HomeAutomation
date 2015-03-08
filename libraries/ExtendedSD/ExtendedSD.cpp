#include "ExtendedSD.h"
#include <GeneralIO.h>
#include <ExtendedEthernet.h>

ExtendedSD SDCard;

void ExtendedSD::DetailedCheck(WRITING out) {
  Serial.print(F("SD card...  "));
  if (!initok) {
	  Serial.println(F("FAILED."));
  } else
  if (!card.init(SPI_HALF_SPEED, SD_CHIP_SELECT_PIN)) {
    Serial.println(F("FAILED. - Card initialization failed."));
  } else {
    if (!volume.init(card)) {
      Serial.println(F("FAILED. - Partition initialization failed."));
    } else {
      double volumesize;
      volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
      volumesize *= volume.clusterCount();       // we'll have a lot of clusters
//      double freevol=volumesize*volume.freeClusterCount();
      volumesize *= 512;                            // SD card blocks are always 512 bytes
      volumesize /= 1024;
      volumesize /= 1024;
      volumesize /= 1024;
//      freevol *= 512;                            // SD card blocks are always 512 bytes
//      freevol /= 1024;
//      freevol /= 1024;
//      freevol /= 1024;
      Serial.print(F("OK.     - [ ")); 
//      Serial.print(freevol);
//      Serial.print(F(" / ")); 
      Serial.print(volumesize);
      Serial.println(F(" Gb ]"));
      root.openRoot(volume);
	  root.ls();
    }
  }
}

void ExtendedSD::Init() {
	pinMode(SD_VIRTUAL_OUTPUT_PIN, OUTPUT); 
	initok=SD.begin();
}

void ExtendedSD::Status(WRITING out) {
}

void ExtendedSD::SaveIO() {
  SD.remove("ioset.jsn");
  File settings=SD.open("ioset.jsn",FILE_WRITE);
  byte tabs=0;
  settings.print("{\n");
  tabs++;
  gSensors.SDSave(settings,tabs);
  settings.print(",\n");
  gActuators.SDSave(settings,tabs);
  settings.print(",\n");
  gEvents.SDSave(settings,tabs);
  settings.print(",\n");
  gTriggers.SDSave(settings,tabs);
  tabs--;
  settings.print("\n}\n");
  settings.close();
/*
  File myFile = SD.open("ioset.jsn");
  if (myFile) {
    Serial.println("ioset.jsn:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
        Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening ioset.jsn");
  }
*/
}

boolean ExtendedSD::LoadIO(String& error) {
	File settings=SD.open("ioset.jsn",FILE_READ);
	if(!settings) {
		error=F("Could not open \"ioset.jsn\" to read.");
		return false;
	}
	String listname="";
	for(;settings.available();) {
		for(char c=settings.read();settings.available() && c!='\"';c=settings.read());
		if(!settings.available()) break;
		for(char c=settings.read();settings.available() && c!='\"';c=settings.read()) listname+=c;
		for(char c=settings.read();settings.available() && c!='{';c=settings.read());
		if(!settings.available()) break;
		if(listname==F("sensors")) {
			if (!gSensors.SDLoad(settings, error)) break;
		} else if(listname==F("actuators")) {
			if (!gActuators.SDLoad(settings, error)) break;
		}
		else if (listname == F("events")) {
			if (!gEvents.SDLoad(settings, error)) break;
		}
		else if (listname == F("triggers")) {
			if (!gTriggers.SDLoad(settings, error)) break;
		}
		else return false;
		listname="";
	}
	settings.close();
	return true;
}

boolean ExtendedSD::ReadFile(String file,WRITING how) {
	digitalWrite(10, HIGH);
	digitalWrite(4, LOW);

	File myFile = SD.open(file.c_str());
	if (myFile) {
		if (how == GW_SERIAL) {
			while (myFile.available()) {
				Serial.write(myFile.read());
			}
		}
		else if (how == GW_UDP) {
			ETH.UDPSendbackFile(myFile);
			digitalWrite(10, HIGH);
			digitalWrite(4, LOW);
		}
		// close the file:
		myFile.close();
		digitalWrite(4, HIGH);
		return true;
	}
	else {
		digitalWrite(4, HIGH);
		return false;
	}
}