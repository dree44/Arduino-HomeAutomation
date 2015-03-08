#ifndef extendedethernet
#define extendedethernet

#include "Arduino.h"
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <GeneralLog.h>
#include <SD.h>


class ExtendedEthernet {
	byte mac[6];
	IPAddress ip;
	IPAddress gateway;
	IPAddress subnet;
	unsigned int localPort;

	unsigned short udpMaxSegment;
	char* udpPayload;
public:
	EthernetUDP Udp;
	ExtendedEthernet() {
		udpMaxSegment=0xffff;
		udpPayload=0;
	}
	void DetailedCheck(WRITING);
	void Init();
	void Status(WRITING);
	String PrintIP(IPAddress);
	String PrintMAC();
	void Receive();
	void UDPParseStream(String udpStream,String& response);
	void UDPSendback(String response);
	void UDPSendbackFile(File& file);
};

extern ExtendedEthernet ETH;


#endif