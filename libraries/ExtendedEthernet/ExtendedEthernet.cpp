#include "ExtendedEthernet.h"
#include <GeneralCommunication.h>
#include <GeneralConfig.h>

ExtendedEthernet ETH;

void ExtendedEthernet::DetailedCheck(WRITING out) {
	Serial.print(F("Ethernet... "));
	IPAddress local = Ethernet.localIP();
	if (local[0] == 0 && local[1] == 0 && local[2] == 0 & local[3] == 0) {
		Serial.println(F("FAILED."));
		//return;
	}
	Serial.print(F("OK.     - [ "));
	Serial.print(PrintMAC());
	Serial.print(F(" | "));
	Serial.print(PrintIP());
	Serial.print(F(":"));
	Serial.print(localPort);
	Serial.print(F(" ]"));

	Serial.println();
}

void ExtendedEthernet::Init() {
	unsigned long m1 = gConfig.mac1();
	unsigned int m2 = gConfig.mac2();
	mac[0] = (m1 >> 24) & 0xFF;
	mac[1] = (m1 >> 16) & 0xFF;
	mac[2] = (m1 >> 8) & 0xFF;
	mac[3] = m1 & 0xFF;
	mac[4] = (m2 >> 8) & 0xFF;
	mac[5] = m2 & 0xFF;
	unsigned long i = gConfig.ip();
	ip[0] = (i >> 24) & 0xFF;
	ip[1] = (i >> 16) & 0xFF;
	ip[2] = (i >> 8) & 0xFF;
	ip[3] = i & 0xFF;
	localPort = gConfig.localPort();

	Ethernet.begin(mac,ip);
	Udp.begin(localPort);
}

void ExtendedEthernet::Status(WRITING out) {
}

String ExtendedEthernet::PrintMAC() {
	String result;
	for (int i = 0; i < 6; i++) {
		result+=ByteToHex(mac[i]);
		if (i < 5) result += F(":");
	}
	return result;
}

String ExtendedEthernet::PrintIP() {
	String result;
	IPAddress local = Ethernet.localIP();
	for (int i = 0; i < 4; i++) {
		result += local[i];
		if (i < 3) result += F(".");
	}
	return result;
}

void ExtendedEthernet::Receive() {
	int packetSize = Udp.parsePacket();
	//Serial.println(packetSize);
	if(packetSize)
	{
		// read the packet into packetBuffer
		byte packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
		Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
		String response="";
		byte packetType=packetBuffer[0];
		if(packetType==0xff) { // control packet // udp stream starting packet with segment number
			bool start=(packetBuffer[1]==0);
			if(start) {
				if(udpPayload!=0) delete udpPayload;
				udpPayload=new char(udpMaxSegment*(UDP_TX_PACKET_MAX_SIZE-1)+1);
				udpPayload[0]='\0';
				udpMaxSegment=packetBuffer[2];
			} else if(udpMaxSegment!=0xff) { //end
				UDPParseStream(udpPayload,response);
				UDPSendback(response);
				udpMaxSegment=0xff;
				// log sending
				Serial.print("Sent to ");
				IPAddress remote = Udp.remoteIP();
				for (int i =0; i < 4; i++) {
					Serial.print(remote[i], DEC);
					if (i < 3)Serial.print(".");
				}
				Serial.print(", port ");
				Serial.print(Udp.remotePort());     
				Serial.print(": ");
				Serial.println(response);
			}
		} else if(udpMaxSegment!=0xff) { // date segment after valid initialization
			unsigned segmentNum=packetBuffer[0];
			unsigned i;
			for(i=1;i<packetSize;++i) {
				udpPayload[segmentNum*(UDP_TX_PACKET_MAX_SIZE-1)+i-1]=packetBuffer[i];
			}
			udpPayload[(segmentNum)*(UDP_TX_PACKET_MAX_SIZE-1)+i-1]='\0';
		}
	}
}

void ExtendedEthernet::UDPParseStream(String udpStream,String& response) {
    String errorString="";
    ParseTextCommand(udpStream,response,errorString);
    if(errorString!="") response="ERROR: "+errorString;
}

void ExtendedEthernet::UDPSendback(String response) {
    byte segmensNum=response.length()/(UDP_TX_PACKET_MAX_SIZE-1)+1;
    Serial.print("seg: ");
    Serial.println(segmensNum);
    uint8_t backBuffer0[3];
    uint8_t backBuffer1[UDP_TX_PACKET_MAX_SIZE];
    uint8_t backBuffer2[2];
    // send back answer
    backBuffer0[0]=0xff;
    backBuffer0[1]=0x00;
    backBuffer0[2]=segmensNum;
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(backBuffer0,3);
    Udp.endPacket();
    delay(1);
    int i,k;
    for(i=0,k=0;i<segmensNum;++i) {
		backBuffer1[0]=i;
		int j;
		for(j=0;j<UDP_TX_PACKET_MAX_SIZE-1 && k<response.length();++j,++k){
			backBuffer1[1+j]=response[k];
		}
		Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
		Udp.write(backBuffer1,j+1);
		Udp.endPacket();
		delay(1);
    }
    backBuffer2[0]=0xff;
    backBuffer2[1]=0x01;
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(backBuffer2,2);
    Udp.endPacket();
    delay(1);
}
