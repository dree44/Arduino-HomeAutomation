#include "GeneralConfig.h"

G_Config gConfig;

String G_Config::bipEventName() {
	return "@2bip";
}

String G_Config::bipSensorName() {
	return "bip";
}

unsigned long G_Config::mac1() {
	unsigned long r = 170;
	r <<= 8;
	r += 170;
	r <<= 8;
	r += 170;
	r <<= 8;
	r += 170;
	return r;
}

unsigned int G_Config::mac2() {
	unsigned int r = 0;
	r <<= 8;
	r += 1;
	return r;
}

unsigned long G_Config::ip() {
	unsigned long r = 192;
	r <<= 8;
	r += 168;
	r <<= 8;
	r += 1;
	r <<= 8;
	r += 101;
	return r;
}

unsigned int G_Config::localPort() {
	return 8888;
}
