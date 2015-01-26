#include "ExtendedEthernet.h"
#include <GeneralCommunication.h>

void ExtendedEthernet::DetailedCheck(WRITING out) {
  Serial.print(F("Ethernet: "));
}

void ExtendedEthernet::Init() {
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
}

void ExtendedEthernet::Status(WRITING out) {
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
