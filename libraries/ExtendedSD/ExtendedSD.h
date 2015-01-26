#ifndef extendedsd
#define extendedsd

#include <SD.h>
#include <GeneralLog.h>"

#define SD_VIRTUAL_OUTPUT_PIN 53 //todo setting

class ExtendedSD {
	Sd2Card card;
	SdVolume volume;
	SdFile root;
public:
	void DetailedCheck(WRITING);
	void Init();
	void Status(WRITING);
	boolean LoadIO(String&);
	void SaveIO();
};

#endif