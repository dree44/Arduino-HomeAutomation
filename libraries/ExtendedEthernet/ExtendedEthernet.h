#ifndef extendedethernet
#define extendedethernet

#include "Arduino.h"
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <GeneralLog.h>


class ExtendedEthernet {
	EthernetUDP Udp;
	byte mac[6];
	IPAddress ip;
	unsigned int localPort;

	byte udpMaxSegment;
	char* udpPayload;
public:
	ExtendedEthernet() : ip(192, 168, 1, 177) { 
		mac[0]=0xDE;
		mac[1]=0xAD;
		mac[2]=0xBE;
		mac[3]=0xEF;
		mac[4]=0xFE;
		mac[5]=0xED;
		localPort=8888;
		udpMaxSegment=0xff;
		udpPayload=0;
		}
	void DetailedCheck(WRITING);
	void Init();
	void Status(WRITING);
	void Receive();
	void UDPParseStream(String udpStream,String& response);
	void UDPSendback(String response);
};



#endif