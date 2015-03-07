#ifndef general_config
#define general_config

#include "Arduino.h"

class G_Config {
public:
	//stats
	boolean ethernet;
	boolean sd;

	//params
	String bipSensorName();
	String bipEventName();

	unsigned long mac1();
	unsigned int mac2();
	unsigned long ip();
	unsigned int localPort();
	unsigned long subnet();
	unsigned long gateway();

};

extern G_Config gConfig;

#endif
