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
        private int UDP_TX_PACKET_MAX_SIZE = 24;
        Socket sendSocket;
        IPEndPoint remoteEndPoint;
        
        ushort udpMaxSegment=0xffff;
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

            ushort segmensNum = (ushort)(size / (UDP_TX_PACKET_MAX_SIZE - 2) + 1);
            byte[] backBuffer0 = new byte[5];
            byte[] backBuffer1 = new byte[UDP_TX_PACKET_MAX_SIZE];
            byte[] backBuffer2 = new byte[3];
            // send back answer
            backBuffer0[0] = 0xff;
            backBuffer0[1] = 0xff;
            backBuffer0[2] = 0x00;
            backBuffer0[3] = (byte)((segmensNum >> 8) & 0xFF);
            backBuffer0[4] = (byte)(segmensNum & 0xFF);
            sendSocket.SendTo(backBuffer0, remoteEndPoint);
            delay(100000);
            for (ushort k = 0, i = 0; i < segmensNum; ++i)
            {
                backBuffer1[0] = (byte)((i >> 8) & 0xFF);
                backBuffer1[1] = (byte)(i & 0xFF);
                int j;
                for (j = 0; j < UDP_TX_PACKET_MAX_SIZE - 2 && k < size; ++j, ++k)
                {
                    backBuffer1[2 + j] = (byte)p[k];
                }
                sendSocket.SendTo(backBuffer1, 2 + j, SocketFlags.None, remoteEndPoint);
                delay(100000);
            }

            backBuffer2[0] = 0xff;
            backBuffer2[1] = 0xff;
            backBuffer2[2] = 0x01;
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
                Boolean control = (buffer[0] == 0xff && buffer[1] == 0xff);
		        if(control) { // control packet // udp stream starting packet with segment number
			        bool start=(buffer[2]==0);
			        if(start) {
                        udpReceivedPayload = "";
				        udpMaxSegment=(ushort)(buffer[3]*256+buffer[4]);
			        } else if(udpMaxSegment!=0xffff) { //end
				        udpMaxSegment=0xffff;
                        return udpReceivedPayload;
                    }
		        } else if(udpMaxSegment!=0xffff) { // data segment after valid initialization
			        ushort segmentNum=(ushort)(buffer[0]*256+buffer[1]);
			        ushort i;
			        for(i=2;i<packetSize;++i) {
                        udpReceivedPayload += (char)buffer[i];
			        }
                }
            }
            return "";
        }
    }
}
