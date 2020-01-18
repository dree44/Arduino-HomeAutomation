#ifndef extendedsd
#define extendedsd

#include <SD.h>
#include <GeneralLog.h>
#include <Ethernet.h>

#define SD_VIRTUAL_OUTPUT_PIN 53 //todo setting

class ExtendedSD {
	Sd2Card card;
	SdVolume volume;
	SdFile root;
	boolean initok;
public:
	ExtendedSD() { initok = false; }
	void DetailedCheck(String&);
	void Init();
	void Status(WRITING);
	boolean LoadIO(String&);
	void SaveIO();
	boolean ReadFile(String, WRITING, EthernetClient*);
};

extern ExtendedSD SDCard;

#endif