using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace DesktopAppForArduino
{
    class TCPSender
    {
        IPEndPoint localEndPoint;
        IPEndPoint remoteEndPoint;
        TcpClient tcpClient;
        bool disposing = false;
        bool reconnecting=false;
        bool unreachable = false;
        
        internal bool Connected()
        {
            return tcpClient.Connected;
        }
        internal bool Disposing()
        {
            return disposing;
        }
        internal bool Reconnecting()
        {
            return reconnecting;
        }
        internal void Start(string ip, int port, string localip, int localport, System.Windows.Forms.Label stat)
        {
            //f.UseWaitCursor = true;
            
            
            if (tcpClient == null)
            {
                localEndPoint = new IPEndPoint(IPAddress.Parse(localip), localport);
                tcpClient = new TcpClient(localEndPoint);
            }
            else
            {
                //End();
                try {
                    if (this.disposing)
                    {
                        localEndPoint = new IPEndPoint(IPAddress.Parse(localip), localport);
                        tcpClient = new TcpClient(localEndPoint);
                        disposing = false;
                    }
                }
                catch {}
            }
            try {
                if (!tcpClient.Connected && !unreachable)
                {
                    stat.Text = "CONNECTING...";
                    stat.BackColor = Color.Yellow;
                    stat.Refresh();
                    remoteEndPoint = new IPEndPoint(IPAddress.Parse(ip), port);
                    tcpClient.Connect(remoteEndPoint);
                    reconnecting = false;
                }
            } catch (SocketException se) {
                if(se.SocketErrorCode==SocketError.AddressAlreadyInUse) {
                    reconnecting = true;
                } else if(se.SocketErrorCode==SocketError.TimedOut) {
                    unreachable = true;
                    reconnecting = false;
                }

            }
            //f.UseWaitCursor = false;

        }
        internal void End()
        {
            unreachable = false;
            try
            {
                tcpClient.GetStream().Close();
                tcpClient.Close();
                disposing = true;
            }
            catch 
            {
                Console.WriteLine("There is no opened TCP to CLOSE");
                disposing = true;
            }

            
        }
        internal void Send(string p)
        {
            try
            {
                tcpClient.GetStream().Write(System.Text.Encoding.ASCII.GetBytes(p), 0, p.Length);
                Console.WriteLine("Sent: {0}", p);
            } catch (Exception) {
                Console.WriteLine("There is no opened TCP to Send");
            }
        }
        internal String Receive()
        {
            String responseData = String.Empty;
            int rec=0;
            try
            {
                while (this.tcpClient.Available > 0)
                {
                    rec = this.tcpClient.Available;
                    if (rec > 0)
                    {
                        byte[] data;
                        data = new byte[rec];
                        Int32 bytes = this.tcpClient.GetStream().Read(data, 0, rec);
                        responseData += System.Text.Encoding.ASCII.GetString(data, 0, rec);
                        Console.WriteLine("Received: {0}", responseData);
                        System.Threading.Thread.Sleep(10);
                    }
                }
                return responseData;
            }
            catch (Exception)
            {
             //   Console.WriteLine("There is no opened TCP to Receive");
                return "";
            }
        }
    }
}