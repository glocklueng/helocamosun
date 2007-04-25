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
        string PacketName;

        CommProtocol cp;
        public Form1()
        {
            InitializeComponent();
            cp = new CommProtocol("COM8", 9600, Parity.None, 8, StopBits.One);
            
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
            e.ReceivedPacket;
            e.Name;
        }

        void cp_PreFlightPacketReceived(object sender, CommProtocol.PreFlightPacketReceivedEventArgs e)
        {
            PFP = e.PFP;
        }

        void cp_HandShakeAckReceived(object sender, EventArgs e)
        {
            
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
            //e.BadPacket;
            //e.ErrorMessage;
        }

        private void cp_OnBoardErrorPacketReceived(object sender, CommProtocol.OnBoardErrorPacketReceivedEventArgs e)
        {
            //e.ErrorCode;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

        }


    }
}