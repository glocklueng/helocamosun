using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using CommProtocolLib;
using System.IO.Ports;

namespace commprotocoltester
{
    
    public partial class Form1 : Form
    {
        BatteryStatus Battery_Status = new BatteryStatus();
        HeadingSpeedAltitude HSA = new HeadingSpeedAltitude();
        Latitude Lat = new Latitude();
        Longitude Lon = new Longitude();
        PreFlightPacketData PFP = new PreFlightPacketData();
        Attitude attitude = new Attitude();
        
        CommProtocol cp;
        public Form1()
        {
            InitializeComponent();
            cp = new CommProtocol("COM6", 19200, Parity.None, 8, StopBits.One, this);
            
            //event handlers
            cp.LocationPacketReceived += new CommProtocol.LocationPacketReceivedEventHandler(cp_LocationPacketReceived);
            cp.BadPacketReceived += new CommProtocol.BadPacketReceivedEventHandler(cp_BadPacketReceived);
            cp.OnBoardErrorPacketReceived += new CommProtocol.OnBoardErrorPacketReceivedEventHandler(cp_OnBoardErrorPacketReceived);
            cp.ResponseTimeout += new CommProtocol.ResponseTimeoutEventHandler(cp_ResponseTimeout);
            cp.BatteryStatusPacketReceived += new CommProtocol.BatteryStatusPacketReceivedEventHandler(cp_BatteryStatusPacketReceived);
            cp.HeadingSpeedAltitudePacketReceived += new CommProtocol.HeadingSpeedAltitudePacketReceivedEventHandler(cp_HeadingSpeedAltitudePacketReceived);
            cp.AttitudePacketReceived += new CommProtocol.AttitudePacketReceivedEventHandler(cp_AttitudePacketReceived);
            cp.ExpectedResponseReceived += new CommProtocol.ExpectedResponseReceivedEventHandler(cp_ExpectedResponseReceived);
            cp.HandShakeAckReceived += new CommProtocol.HandShakeAckReceivedEventHandler(cp_HandShakeAckReceived);
            cp.PreFlightPacketReceived += new CommProtocol.PreFlightPacketReceivedEventHandler(cp_PreFlightPacketReceived);
            
        }

        void cp_ExpectedResponseReceived(object sender, CommProtocol.ExpectedResponseReceivedEventArgs e)
        {
            //e.ReceivedPacket;
            textBox1.Text+="Expected response recieved: "+e.Name+"\r\n";
        }

        void cp_PreFlightPacketReceived(object sender, CommProtocol.PreFlightPacketReceivedEventArgs e)
        {
            PFP = e.PFP;
        }

        void cp_HandShakeAckReceived(object sender, EventArgs e)
        {
            cp.CommsHandShakeTerminate();
        }

        void cp_AttitudePacketReceived(object sender, CommProtocol.AttitudePacketReceivedEventArgs e)
        {
            attitude = e.attitude;
        }

        void cp_HeadingSpeedAltitudePacketReceived(object sender, CommProtocol.HeadingSpeedAltitudePacketReceivedEventArgs e)
        {
            HSA = e.HSA;
        }

        void cp_BatteryStatusPacketReceived(object sender, CommProtocol.BatteryStatusPacketReceivedEventArgs e)
        {
            Battery_Status = e.BattStat;
        }

        void cp_ResponseTimeout(object sender, CommProtocol.ResponseTimeoutEventArgs e)
        {
            textBox1.Text += "Timeout for "+e.ExpectedResponse.Name+". Expected: ";
            foreach (char c in e.ExpectedResponse.ExpectedPacket)
            {
                textBox1.Text += CharToHex(c);
            }
            textBox1.Text += " Buffer contents:";
            foreach (char c in e.BufferContents)
            {
                textBox1.Text += CharToHex(c);
            }
            textBox1.Text +="\r\n";
            //e.BufferContents;
           // e.ExpectedResponse;
        }
        private void cp_LocationPacketReceived(object sender, CommProtocol.LocationPacketReceivedEventArgs e)
        {
            Lat = e.Lat;
            Lon = e.Long;
            
        }

        private void cp_BadPacketReceived(object sender, CommProtocol.BadPacketReceivedEventArgs e)
        {
            textBox1.Text += e.ErrorMessage+ ". Contents of bad packet: ";
            foreach (char c in e.BadPacket)
            {
                textBox1.Text += CharToHex(c);
            }
            textBox1.Text += "\r\n";

        }

        private void cp_OnBoardErrorPacketReceived(object sender, CommProtocol.OnBoardErrorPacketReceivedEventArgs e)
        {
            textBox1.Text = e.ErrorCode.ToString();
        }


        private void btnCommsHandShakeInitiate_Click(object sender, EventArgs e)
        {
           cp.CommsHandShakeInitiate();

        }

        private void btnCommsHandShakeTerminate_Click(object sender, EventArgs e)
        {
            cp.CommsHandShakeTerminate();
        }

        private void btnDiscreetMovementControl_Click(object sender, EventArgs e)
        {

            cp.DiscreetMovementControl(0x46);
        }

        private void btnEngageEngine_Click(object sender, EventArgs e)
        {
            cp.EngageEngine();
        }

        private void btnGoto_Click(object sender, EventArgs e)
        {
            Latitude lat = new Latitude();
            lat.Degrees= 0;
            lat.Minutes = 30;
            lat.North = true;
            lat.SecondsH = 60;
            lat.SecondsL = 4;
            
            Longitude lon = new Longitude();
            lon.Degrees = 0;
            lon.Minutes = 30;
            lon.East = true;
            lon.SecondsH = 60;
            lon.SecondsL = 4;

            cp.Goto(lat,lon,0x48,50);
        }

        private void btnHover_Click(object sender, EventArgs e)
        {
            cp.Hover(0x50,(ushort)0);
        }

        private void btnRequestForInformation_Click(object sender, EventArgs e)
        {
            cp.RequestForInformation((byte)0x42);
        }

        private void btnRequestPreFlightPacket_Click(object sender, EventArgs e)
        {
            cp.RequestPreFlightPacket();
        }

        private void btnReturnToBase_Click(object sender, EventArgs e)
        {
            cp.ReturnToBase();
        }

        private void btnSetAntiTorque_Click(object sender, EventArgs e)
        {
            cp.SetAntiTorque(0x00);
        }

        private void btnSetCollective_Click(object sender, EventArgs e)
        {
            cp.SetCollective(0x00);
        }

        private void btnSetCyclicPitch_Click(object sender, EventArgs e)
        {
            cp.SetCyclicPitch(0x00);
        }

        private void btnSetCyclicRoll_Click(object sender, EventArgs e)
        {
            cp.SetCyclicRoll(0x00);
        }

        private void btnSetMotorRPM_Click(object sender, EventArgs e)
        {
            cp.SetMotorRPM(0x00);
        }

        private void btnSetTunePoints_Click(object sender, EventArgs e)
        {
            cp.SetTunePoints(0x5A,0x00,0x00,0x00,0x00,0x00);
        }
        public string CharToHex(char c)
        {
            return string.Format("{0:x2}", (int)c);
        }
    }
}