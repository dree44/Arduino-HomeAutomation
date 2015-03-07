using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace DesktopAppForArduino
{
    class UDPSender
    {
        private int UDP_TX_PACKET_MAX_SIZE = 25;
        Socket sendSocket;
        IPEndPoint remoteEndPoint;
        
        uint udpMaxSegment=0xff;
        string udpReceivedPayload="";

        void delay(int ms)
        {
            long start = DateTime.Now.Ticks;
            for (; DateTime.Now.Ticks < start + ms; ) ;

        }
        internal void init(string ip, int port, string localip, int localport)
        {
            if (sendSocket!=null) sendSocket.Close();
            sendSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            remoteEndPoint = new IPEndPoint(IPAddress.Parse(ip), port);
            sendSocket.Bind(new IPEndPoint(IPAddress.Parse(localip), localport));

        }
        internal void SendUDP(string p)
        {
            // string text = p;
            //byte[] send_buffer = Encoding.ASCII.GetBytes(text);

            int size = p.Length;
            //sock.SendTo(send_buffer, offset, size,SocketFlags.None,endPoint);

            byte segmensNum = (byte)(size / (UDP_TX_PACKET_MAX_SIZE - 1) + 1);
            byte[] backBuffer0 = new byte[3];
            byte[] backBuffer1 = new byte[UDP_TX_PACKET_MAX_SIZE];
            byte[] backBuffer2 = new byte[2];
            // send back answer
            backBuffer0[0] = 0xff;
            backBuffer0[1] = 0x00;
            backBuffer0[2] = segmensNum;
            sendSocket.SendTo(backBuffer0,remoteEndPoint);
            delay(100000);
            for (int k = 0, i = 0; i < segmensNum; ++i)
            {
                backBuffer1[0] = (byte)i;
                int j;
                for (j = 0; j < UDP_TX_PACKET_MAX_SIZE - 1 && k < size; ++j, ++k)
                {
                    backBuffer1[1 + j] = (byte)p[k];
                }
                sendSocket.SendTo(backBuffer1, 1 + j, SocketFlags.None, remoteEndPoint);
                delay(100000);
            }

            backBuffer2[0] = 0xff;
            backBuffer2[1] = 0x01;
            sendSocket.SendTo(backBuffer2, remoteEndPoint);

        }
        internal string ReceiveUDP()
        {
            IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);
            EndPoint Remote = (EndPoint)(sender);
            byte[] buffer=new byte[UDP_TX_PACKET_MAX_SIZE];
            if (sendSocket.Available > 0)
            {
		        // read the packet into packetBuffer
                int packetSize=sendSocket.ReceiveFrom(buffer, ref Remote);
		        byte packetType=buffer[0];
		        if(packetType==0xff) { // control packet // udp stream starting packet with segment number
			        bool start=(buffer[1]==0);
			        if(start) {
                        udpReceivedPayload = "";
				        udpMaxSegment=buffer[2];
			        } else if(udpMaxSegment!=0xff) { //end
				        udpMaxSegment=0xff;
                        return udpReceivedPayload;
                    }
		        } else if(udpMaxSegment!=0xff) { // data segment after valid initialization
			        uint segmentNum=buffer[0];
			        uint i;
			        for(i=1;i<packetSize;++i) {
                        udpReceivedPayload += (char)buffer[i];
			        }
                }
            }
            return "";
        }
    }
}
