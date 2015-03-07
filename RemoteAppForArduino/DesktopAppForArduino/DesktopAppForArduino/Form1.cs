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

        public Form1()
        {
            InitializeComponent();
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            localIPBox.Text=host.AddressList.FirstOrDefault(ip => ip.AddressFamily == AddressFamily.InterNetwork).ToString();
        }

        private void clearAllButton_Click(object sender, EventArgs e)
        {

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
            receiveBox.ResetText();
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
            udp.SendUDP(sendBox.Text);
        }

        private void ipBox_Leave(object sender, EventArgs e)
        {
            udp.init(ipBox.Text, int.Parse(remotePortBox.Text), localIPBox.Text, int.Parse(localPortBox.Text));
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            udp=new UDPSender();
            udp.init(ipBox.Text, int.Parse(remotePortBox.Text), localIPBox.Text, int.Parse(localPortBox.Text));
        }

        private void portBox_Leave(object sender, EventArgs e)
        {
            udp.init(ipBox.Text, int.Parse(remotePortBox.Text), localIPBox.Text, int.Parse(localPortBox.Text));
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            String packet=udp.ReceiveUDP();
            if (packet != "") receiveBox.Text = packet;

        }

        private void localPortBox_Leave(object sender, EventArgs e)
        {
            udp.init(ipBox.Text, int.Parse(remotePortBox.Text), localIPBox.Text, int.Parse(localPortBox.Text));
        }

    }
}
