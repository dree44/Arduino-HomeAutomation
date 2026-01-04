#include "ExtendedEthernet.h"
#include <GeneralCommunication.h>
#include <GeneralConfig.h>
#include <MemoryFree.h>

ExtendedEthernet ETH;

void ExtendedEthernet::DetailedCheck(String& out) {
	digitalWrite(10, LOW);
	out += (F("Ethernet... "));
	IPAddress local = Ethernet.localIP();
	if (local[0] == 0 && local[1] == 0 && local[2] == 0 & local[3] == 0) {
		out+=(F("FAILED.\r\n"));
		//return;
	}
	
	
	
	out+=(F("OK.     - [ "));
	out += (PrintMAC());
	out += (F(" | "));
	out += (PrintIP(Ethernet.localIP()));
	out += (F(":"));
	out += (localPort);
	out += (F("/"));
	out += (PrintIP(subnet));
	out += (F(" GATEWAY "));
	out += (PrintIP(gateway));
	out += (F(" ]\r\n"));
	digitalWrite(10, HIGH);
}

void ExtendedEthernet::Init() {
	digitalWrite(10, LOW);
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

	serverPort = 84; //TODO

	unsigned long g = gConfig.gateway();
	gateway[0] = (g >> 24) & 0xFF;
	gateway[1] = (g >> 16) & 0xFF;
	gateway[2] = (g >> 8) & 0xFF;
	gateway[3] = g & 0xFF;
	unsigned long s = gConfig.subnet();
	subnet[0] = (s >> 24) & 0xFF;
	subnet[1] = (s >> 16) & 0xFF;
	subnet[2] = (s >> 8) & 0xFF;
	subnet[3] = s & 0xFF;

	Ethernet.begin(mac,ip,gateway,gateway,subnet);
	Udp.begin(localPort);
	tcpServer = new EthernetServer(serverPort);
	tcpServer->begin();
	digitalWrite(10, HIGH);
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

String ExtendedEthernet::PrintIP(IPAddress local) {
	String result;
	for (int i = 0; i < 4; i++) {
		result += local[i];
		if (i < 3) result += F(".");
	}
	return result;
}

void ExtendedEthernet::Receive() {
	digitalWrite(4, HIGH);
	digitalWrite(10, LOW);

	int packetSize = 0;// this->Udp.parsePacket();
	if (packetSize>0)
	{
		Serial.print("Receive / packetsize> ");
		Serial.println(packetSize);
		// read the packet into packetBuffer
		char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
		int x = this->Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
		String response="";
		bool controlPacket = (((byte)packetBuffer[0] == 0xff) && ((byte)packetBuffer[1] == 0xff));
		if (controlPacket) { // control packet // udp stream starting packet with segment number
			bool start = (packetBuffer[2] == 0 || packetBuffer[2] == 2);
			bool end = (packetBuffer[2] == 1);
			bool startDownload = (packetBuffer[2] == 2);
			if(start) {
				if(udpPayload!=0) delete udpPayload;
				udpMaxSegment = 0xffff;
				if (startDownload) {
					fileName = "";
					unsigned short i;
					for (i = 3; i < UDP_TX_PACKET_MAX_SIZE - 3 && packetBuffer[i] != '\0'; ++i)	{
						fileName += packetBuffer[i];
					}
					
					Serial.println("file open> ");
					Serial.println(fileName);
					digitalWrite(4, LOW);
					digitalWrite(10, HIGH);
					File xFile= SD.open(fileName.c_str(), O_CREAT | O_TRUNC );
					if (!xFile) Serial.println("not opened >(");
					Serial.println("ok> ");
					xFile.close();
					Serial.println("closed");
					digitalWrite(4, HIGH);
					digitalWrite(10, LOW);
					if (xFile) {
						Serial.println("failed");
					}
				}
				else {
					udpMaxSegment = packetBuffer[3] * 256 + packetBuffer[4];
					if (udpMaxSegment > freeMemory() / 2) { udpMaxSegment = 0xffff; Serial.println("No memory for receiving big packets."); }
					udpPayload = new char[udpMaxSegment*(UDP_TX_PACKET_MAX_SIZE - 2) + 1];
					udpPayload[0] = '\0';
				}
			} else if(end && udpMaxSegment!=0xffff) { //end and send back response
				UDPParseStream(NULL,udpPayload, response);
				if (response != "") {
					Serial.print("response size> ");
					Serial.println(response.length());
					UDPSendback(response);
					udpMaxSegment = 0xffff;
					// log sending
					Serial.print("Sent to ");
					IPAddress remote = Udp.remoteIP();
					for (int i = 0; i < 4; i++) {
						Serial.print(remote[i], DEC);
						if (i < 3)Serial.print(".");
					}
					Serial.print(", port ");
					Serial.print(Udp.remotePort());
					Serial.println(": ");
					Serial.println(".........................");
					Serial.println(response);
					Serial.println(".........................");
				}
			}
			else if (end) {
			}

		} else if(udpMaxSegment!=0xffff) { // data segment after valid initialization
			unsigned short segmentNum=packetBuffer[0]*256+packetBuffer[1];
			unsigned short i;
			for (i = 2; i<packetSize; ++i) {
				udpPayload[segmentNum*(UDP_TX_PACKET_MAX_SIZE-2)+i-2]=packetBuffer[i];
			}
			udpPayload[(segmentNum)*(UDP_TX_PACKET_MAX_SIZE-2)+i-2]='\0';
		}
		else if (udpMaxSegment == 0xffff) { // file iras
			Serial.println("most irjuk");
			if (fileName!="") {
				Serial.print("file> ");
				Serial.println(fileName);
				digitalWrite(4, LOW);
				digitalWrite(10, HIGH);
				File xFile = SD.open(fileName.c_str(), FILE_WRITE);
				if (!xFile) Serial.println("not opened");
				String k;
				for (unsigned short i = 2; packetBuffer[i] != '\0' && i < UDP_TX_PACKET_MAX_SIZE; ++i) {
					xFile.print('s');
					xFile.print(packetBuffer[i]);
					k += packetBuffer[i];
				}
				Serial.println(k);
				Serial.println("kesz");
				xFile.close();
				Serial.println("closed");
				digitalWrite(4, HIGH);
				digitalWrite(10, LOW);
			}
			else {
				Serial.println("Filename is not exisit");
			}
		}
	}

	//new tcp
	EthernetClient client = this->tcpServer->available();
	if (client) {
		Serial.println("TCP ARRIVED");
//		udpPayload = new char[256];
//		udpPayload[0] = '\0';
		String u;
		String response = "";
		while (client.available()) {
			char c = client.read();
			if (tcpState == NORMAL) {
				if (u == "UPLOAD") {
					Serial.println("UPLOAD");
					tcpState = UPLOADFILENAME;
					u = "";
				}
				else {
					u += c;
				}
			}
			else if (tcpState == UPLOADFILENAME) {
				if (c == ' ') {
					tcpState=UPLOADING;
					fileName = u;
					u = "";
					Serial.println(fileName);

					c = client.read();
					tcpPayloadLength = c;
					c = client.read();
					tcpPayloadLength = tcpPayloadLength *256 + c;
					Serial.println(tcpPayloadLength);
					fileStart = true;
				}
				else {
					u += c;
				}
			}
			else if (tcpState == UPLOADING){
				u += c;
			}
		}

		if (tcpState == NORMAL){
			Serial.println(u);
			UDPParseStream(&client, u, response);
			client.println(response);
			Serial.print("Sent> ");
			Serial.println(response);
			//		delete udpPayload;
		} else if(tcpState==UPLOADFILENAME) {
			response = "Failed";
			tcpState = NORMAL;
			tcpPayloadLength = 0;
			client.println(response);
			Serial.print("Sent> ");
			Serial.println(response);
		}
		else if (tcpState == UPLOADING) {
			response = "OK";

			Serial.println("MOST UPLOAD");
			Serial.println(u);

			digitalWrite(4, LOW);

			if (fileStart) {
				SD.remove(const_cast<char*>(fileName.c_str()));
				fileStart = false;
			}
			File file = SD.open(fileName.c_str(), FILE_WRITE);
			file.write(u.c_str());
			file.close();		
			digitalWrite(4, HIGH);

			client.println(response);
			Serial.print("Sent> ");
			Serial.println(response);

			Serial.print("payloadlen ");
			Serial.print(tcpPayloadLength);
			Serial.print("ulen ");
			Serial.println(u.length());
			if (tcpPayloadLength <= u.length()) {
				Serial.println("TCPSTATE RESET");
				tcpState = NORMAL;
				tcpPayloadLength = 0;
				fileStart = true;
			}
			tcpPayloadLength -= u.length();


		}
	}

	digitalWrite(10, HIGH);
}

void ExtendedEthernet::UDPParseStream(EthernetClient* client, String udpStream,String& response) {
    String errorString="";
    ParseTextCommand(GW_UDP,udpStream,response,errorString,client);
    if(errorString!="") response="ERROR: "+errorString;
}

void ExtendedEthernet::UDPSendback(String& response) {
    short segmensNum=response.length()/(UDP_TX_PACKET_MAX_SIZE-2)+1;
    char backBuffer0[5];
    uint8_t backBuffer1[UDP_TX_PACKET_MAX_SIZE];
    uint8_t backBuffer2[3];
//	Serial.print("segnum> ");
//	Serial.println(segmensNum);
//	Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxx");
//	Serial.println(response);
//	Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxx");
	// send back answer
    backBuffer0[0] = 0xff;
	backBuffer0[1] = 0xff;
	backBuffer0[2] = 0x00;
    backBuffer0[3] = (segmensNum >> 8) & 0xff;
	backBuffer0[4] = segmensNum & 0xff;
	Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
	Udp.write(backBuffer0,5);
    int x=Udp.endPacket();
    delay(1);
    int i,k;
    for(i=0,k=0;i<segmensNum;++i) {
		backBuffer1[0]=(i>>8) & 0xff;
		backBuffer1[1] = i & 0xff;
		int j;
		for(j=0;j<UDP_TX_PACKET_MAX_SIZE-2 && k<response.length();++j,++k){
			backBuffer1[2+j]=response[k];
		}
		Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
		Udp.write(backBuffer1,j+2);
		Udp.endPacket();
		delay(1);
    }
    backBuffer2[0] = 0xff;
	backBuffer2[1] = 0xff;
	backBuffer2[2] = 0x01;
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(backBuffer2,3);
    Udp.endPacket();
    delay(1);
}

void ExtendedEthernet::UDPSendbackFile(File& file) {
	short segmensNum = file.size() / (UDP_TX_PACKET_MAX_SIZE - 2) + 1;
	Serial.println(file.size());
	Serial.println(segmensNum);
	char backBuffer0[5];
	uint8_t backBuffer1[UDP_TX_PACKET_MAX_SIZE];
	uint8_t backBuffer2[3];
	// send back answer
	backBuffer0[0] = 0xff;
	backBuffer0[1] = 0xff;
	backBuffer0[2] = 0x00;
	backBuffer0[3] = (segmensNum >> 8) & 0xff;
	backBuffer0[4] = segmensNum & 0xff;
	Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
	Udp.write(backBuffer0, 5);
	int x = Udp.endPacket();
	delay(1);
	int i, k;
	for (i = 0, k = 0; i<segmensNum; ++i) {
		backBuffer1[0] = (i >> 8) & 0xff;
		backBuffer1[1] = i & 0xff;
		digitalWrite(10, HIGH);
		digitalWrite(4, LOW);
		int j;
		for (j = 0; j<UDP_TX_PACKET_MAX_SIZE - 2 && file.available(); ++j, ++k){
			byte b = file.read();
			if (b == 0xff) { --j; --k; continue; }
			backBuffer1[2 + j] = b;
		}
		digitalWrite(4, HIGH);
		digitalWrite(10, LOW);
		Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
		Udp.write(backBuffer1, j + 2);
		Udp.endPacket();
		delay(1);
	}
	backBuffer2[0] = 0xff;
	backBuffer2[1] = 0xff;
	backBuffer2[2] = 0x01;
	Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
	Udp.write(backBuffer2, 3);
	Udp.endPacket();
	delay(1);
}
