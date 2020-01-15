#include "GeneralConfig.h"

G_Config gConfig;

String G_Config::bipEventName() {
	return "@2bip";
}

String G_Config::bipSensorName() {
	return "bip";
}

/*
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
*/
unsigned long G_Config::mac1() {
	unsigned long r = 222;
	r <<= 8;
	r += 173;
	r <<= 8;
	r += 190;
	r <<= 8;
	r += 4;
	return r;
}

unsigned int G_Config::mac2() {
	unsigned int r = 254;
	r <<= 8;
	r += 237;
	return r;
}

unsigned long G_Config::ip() {
	unsigned long r = 192;
	r <<= 8;
	r += 169;
	r <<= 8;
	r += 3;
	r <<= 8;
	r += 2;
	return r;
}

unsigned int G_Config::localPort() {
	return 65000;
}

unsigned long G_Config::subnet() {
	unsigned long r = 255;
	r <<= 8;
	r += 255;
	r <<= 8;
	r += 0;
	r <<= 8;
	r += 0;
	return r;
}

unsigned long G_Config::gateway() {
	unsigned long r = 192;
	r <<= 8;
	r += 169;
	r <<= 8;
	r += 0;
	r <<= 8;
	r += 1;
	return r;
}

