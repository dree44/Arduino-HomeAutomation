using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace DesktopAppForArduino
{
    class TCPSender
    {
        Socket sendSocket;
        IPEndPoint localEndPoint;
        IPEndPoint remoteEndPoint;

        internal void init(string ip, int port, string localip, int localport)
        {
            remoteEndPoint = new IPEndPoint(IPAddress.Parse(ip), port);
            localEndPoint = new IPEndPoint(IPAddress.Parse(localip), localport);

        }

        internal void SendTCP(string p)
        {
            TcpClient tcpClient = new TcpClient();
            tcpClient.Connect(remoteEndPoint);
            tcpClient.GetStream().Write(System.Text.Encoding.ASCII.GetBytes("123"), 0, 3);
            tcpClient.Close();

            /*
                    if (!sendSocket.Connected) sendSocket.Connect(remoteEndPoint);
                    byte[] b = new byte[5];
                    b[0] = 100;
                    sendSocket.Send(b);
                    */
        }
    }
}