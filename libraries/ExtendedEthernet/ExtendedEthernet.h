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
	unsigned int serverPort;

	unsigned short udpMaxSegment;
	char* udpPayload;
	String fileName;
	
public:
	EthernetUDP Udp;
	EthernetServer* tcpServer=0;
	ExtendedEthernet() {
		udpMaxSegment=0xffff;
		udpPayload=0;
	}
	void DetailedCheck(String&);
	void Init();
	void Status(WRITING);
	String PrintIP(IPAddress);
	String PrintMAC();
	void Receive();
	void UDPParseStream(String udpStream,String& response);
	void UDPSendback(String& response);
	void UDPSendbackFile(File& file);
};

extern ExtendedEthernet ETH;


#endif