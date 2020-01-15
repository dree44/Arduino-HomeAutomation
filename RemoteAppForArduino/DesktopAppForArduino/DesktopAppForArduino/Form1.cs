using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DesktopAppForArduino
{
    public partial class Form1 : Form
    {
        UDPSender udp;
        TCPSender tcp=new TCPSender();
        String[] receivedPackets;
        uint nReceivedPackets=0;
        uint shownReceivedPackets = 0;

        public Form1()
        {
           
            receivedPackets=new String[99];
            InitializeComponent();
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            localIPBox.Text=host.AddressList.FirstOrDefault(ip => ip.AddressFamily == AddressFamily.InterNetwork).ToString();
        }

        private void clearAllButton_Click(object sender, EventArgs e)
        {
            nReceivedPackets = 0;
            receiveBox.Text = "";
            packetNumBox.Text = "";
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void inputLoadButton_Click(object sender, EventArgs e)
        {
            DialogResult result = inputOpenFileDialog.ShowDialog(); // Show the dialog.
            if (result == DialogResult.OK) // Test result.
            {
                string file = inputOpenFileDialog.FileName;
                try
                {
                    sendBox.Text = File.ReadAllText(file);
                }
                catch (IOException)
                {
                }
            }
        }

        private void inputClearButton_Click(object sender, EventArgs e)
        {
            sendBox.ResetText();
        }

        private void inputSaveButton_Click(object sender, EventArgs e)
        {
            DialogResult result = inputSaveFileDialog.ShowDialog(); // Show the dialog.
            if (result == DialogResult.OK) // Test result.
            {
                string file = inputSaveFileDialog.FileName;
                try
                {
                    File.WriteAllText(file, sendBox.Text);
                }
                catch (IOException)
                {
                }
            }
        }

        private void outputClearButton_Click(object sender, EventArgs e)
        {
            if (nReceivedPackets <= 0) return;
            receiveBox.ResetText();
            uint i = shownReceivedPackets;
            receivedPackets[i] = "";
            for (; i < nReceivedPackets - 1; ++i) receivedPackets[i] = receivedPackets[i + 1];
            nReceivedPackets--;
            shownReceivedPackets--;
            if (shownReceivedPackets < 0) shownReceivedPackets = 0;
            if(nReceivedPackets>0) receiveBox.Text=receivedPackets[shownReceivedPackets];
        }

        private void outputSaveButton_Click(object sender, EventArgs e)
        {
            DialogResult result = outputSaveFileDialog.ShowDialog(); // Show the dialog.
            if (result == DialogResult.OK) // Test result.
            {
                string file = outputSaveFileDialog.FileName;
                try
                {
                    File.WriteAllText(file, receiveBox.Text);
                }
                catch (IOException)
                {
                }
            }
        }

        private void listButton_Click(object sender, EventArgs e)
        {
            sendBox.ResetText();
            sendBox.Text = "list";
            udp.SendUDP(sendBox.Text);
        }

        private void helpButton_Click(object sender, EventArgs e)
        {
            sendBox.ResetText();
            sendBox.Text = "help";
            udp.SendUDP(sendBox.Text);
        }

        private void statusButton_Click(object sender, EventArgs e)
        {
            sendBox.ResetText();
            sendBox.Text = "status";
            udp.SendUDP(sendBox.Text);
        }

        private void sendButton_Click(object sender, EventArgs e)
        {
            //IPEndPoint("12",12);
            tcp.SendTCP(sendBox.Text);

        }

        private void ipBox_Leave(object sender, EventArgs e)
        {
            udp.init(ipBox.Text, int.Parse(remotePortBox.Text), localIPBox.Text, int.Parse(localPortBox.Text));
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            udp=new UDPSender();
            udp.init(ipBox.Text, int.Parse(remotePortBox.Text), localIPBox.Text, int.Parse(localPortBox.Text));

            tcp.init(ipBox.Text, int.Parse(remotePortBox.Text), localIPBox.Text, int.Parse(localPortBox.Text));
        
        }

        private void portBox_Leave(object sender, EventArgs e)
        {
            udp.init(ipBox.Text, int.Parse(remotePortBox.Text), localIPBox.Text, int.Parse(localPortBox.Text));
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            String packet=udp.ReceiveUDP();
            if (packet != "") {
                if(nReceivedPackets<99) {
                    receivedPackets[nReceivedPackets]=packet; 
                    ++nReceivedPackets;
                    shownReceivedPackets = nReceivedPackets - 1;
                } else {
                    for(uint i=1; i<99; ++i) receivedPackets[i-1]=receivedPackets[i]; 
                    receivedPackets[99]=packet;
                }
                receiveBox.Text = packet;
                packetNumBox.Text = shownReceivedPackets.ToString();
            }

        }

        private void localPortBox_Leave(object sender, EventArgs e)
        {
            udp.init(ipBox.Text, int.Parse(remotePortBox.Text), localIPBox.Text, int.Parse(localPortBox.Text));
        }

        private void minusButton_Click(object sender, EventArgs e)
        {
            if (nReceivedPackets == 0) return;
            if (shownReceivedPackets == 0) shownReceivedPackets = nReceivedPackets-1;
            else shownReceivedPackets--;
            packetNumBox.Text = shownReceivedPackets.ToString();
            receiveBox.Text = receivedPackets[shownReceivedPackets];
        }

        private void plusButton_Click(object sender, EventArgs e)
        {
            if (nReceivedPackets == 0) return;
            if (shownReceivedPackets == nReceivedPackets-1) shownReceivedPackets = 0;
            else shownReceivedPackets++;
            packetNumBox.Text = shownReceivedPackets.ToString();
            receiveBox.Text = receivedPackets[shownReceivedPackets];
        }

        private void dlIOButton_Click(object sender, EventArgs e)
        {
            sendBox.ResetText();
            sendBox.Text = "download IOSET.JSN";
            udp.SendUDP(sendBox.Text);
        }

        private void dlBasicButton_Click(object sender, EventArgs e)
        {
            sendBox.ResetText();
            sendBox.Text = "download BASIC.JSN";
            udp.SendUDP(sendBox.Text);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            udp.SendUploadUDP("2.prb",sendBox.Text);
        }

    }
}
