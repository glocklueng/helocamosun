using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using CommProtocolLib;

namespace commprotocoltester
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            CommProtocol cp = new CommProtocol("COM1", 19200);


            char[] bytestring = { (char)0xA5, (char)0x5A,
                (char)0x74, (char)0x4C,
                (char)0x00,
                (char)0x01,
                (char)0x00,
                (char)0x01,
                (char)0x4E,
                (char)0x00,
                (char)0x01,
                (char)0x00,
                (char)0x01,
                (char)0x4E,
                (char)0x01,
                (char)0x60,
                (char)0xCC,
                (char)0x33
            };
            string Packet = new string(bytestring);
            cp.LocationPacketRecieved += new CommProtocol.LocationPacketRecievedEventHandler(cp_LocationPacketRecieved);
            cp.MatchIncomingPacket(ref Packet);

        }
        private void cp_LocationPacketRecieved(object sender, CommProtocol.LocationPacketRecievedEventArgs e)
        {
            Latitude lat = e.Lat;
            Longitude lon = e.Long;
        }

    }
}