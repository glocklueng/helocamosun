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
        string BufferCopy;
        CommProtocol cp;
        public Form1()
        {
            InitializeComponent();

            for (int i = 1; i < 10; i++)
            {
                comboBox1.Items.Add("COM" + i);
            }
            comboBox1.SelectedIndex = 0;
        }

        void cp_ExpectedResponseReceived(object sender, CommProtocol.ExpectedResponseReceivedEventArgs e)
        {
            //e.ReceivedPacket;
            textBox1.AppendText("Expected response recieved: "+e.Name+"\r\n");
            textBox1.ScrollToCaret();
        }

        void cp_PreFlightPacketReceived(object sender, CommProtocol.PreFlightPacketReceivedEventArgs e)
        {
            PFP = e.PFP;

            MessageBox.Show(
                "Pre-flight packet received: \r\n" +
                "battery temperature: " + PFP.BatteryTemp + "\r\n" +
                "GPS Altitude: " + PFP.GPSAltitude + "\r\n" +
                "Lat degrees: " + PFP.Lat.Degrees + "\r\n" +
                "Lat minutes: " + PFP.Lat.Minutes + "\r\n" +
                "Lat seconds: " + (float)(PFP.Lat.SecondsH)+(PFP.Lat.SecondsL/1024f) + "\r\n" +
                "North degrees: " + PFP.Lat.North + "\r\n" +
                "Long degrees: " + PFP.Long.Degrees + "\r\n" +
                "Long minutes: " + PFP.Long.Minutes + "\r\n" +
                "Long seconds: " + (float)(PFP.Long.SecondsH) + (PFP.Long.SecondsL / 1024f) + "\r\n" +
                "East degrees: " + PFP.Long.East + "\r\n" +
                "sensor status: " + string.Format("{0:x2}",PFP.SensorStatus) + "\r\n" +
                "sonar altitude: " + PFP.SonarAltitude
                );
        }

        void cp_HandShakeAckReceived(object sender, EventArgs e)
        {
            //cp.CommsHandShakeTerminate();
            textBox1.AppendText("Hand shake ack received\r\n");
            textBox1.ScrollToCaret();
        }

        void cp_AttitudePacketReceived(object sender, CommProtocol.AttitudePacketReceivedEventArgs e)
        {
            attitude = e.attitude;
            textBox2.AppendText(e.attitude.Yaw + " ");
           // MessageBox.Show("Attitude packet received: PITCH: " + attitude.Pitch + " ROLL: " + attitude.Roll + " YAW: " + attitude.Yaw);
        }

        void cp_HeadingSpeedAltitudePacketReceived(object sender, CommProtocol.HeadingSpeedAltitudePacketReceivedEventArgs e)
        {
            HSA = e.HSA;

            MessageBox.Show("Heading/Speed/Altitude packet received: Altitude: " + HSA.Altitude + " Heading: " + HSA.Heading + " Speed: " + HSA.Speed);
        }

        void cp_BatteryStatusPacketReceived(object sender, CommProtocol.BatteryStatusPacketReceivedEventArgs e)
        {
            Battery_Status = e.BattStat;
            MessageBox.Show("Battery status packet received: Current: " + Battery_Status.CurrentDraw + " Temperature: " + Battery_Status.Temperature + " Voltage: " + Battery_Status.Voltage);
        }

        void cp_ResponseTimeout(object sender, CommProtocol.ResponseTimeoutEventArgs e)
        {
            textBox1.AppendText( "Timeout for "+e.ExpectedResponse.Name+". Expected: ");
            foreach (char c in e.ExpectedResponse.ExpectedPacket)
            {
                textBox1.AppendText( CharToHex(c) + " ");
            }
            textBox1.AppendText(" Buffer contents:");
            foreach (char c in e.BufferContents)
            {
                textBox1.AppendText( CharToHex(c) + " ");
            }
            textBox1.AppendText("\r\n");

            textBox1.ScrollToCaret();
        }
        private void cp_LocationPacketReceived(object sender, CommProtocol.LocationPacketReceivedEventArgs e)
        {
            Lat = e.Lat;
            Lon = e.Long;
            
            MessageBox.Show(
                            "Location packet received: \r\n" +
                            "Lat degrees: " + Lat.Degrees + "\r\n" +
                            "Lat minutes: " + Lat.Minutes + "\r\n" +
                            "Lat seconds: " + (float)(Lat.SecondsH) + (Lat.SecondsL / 1024f) + "\r\n" +
                            "North: " + Lat.North + "\r\n" +
                            "Long degrees: " + Lon.Degrees + "\r\n" +
                            "Long minutes: " + Lon.Minutes + "\r\n" +
                            "Long seconds: " + (float)(Lon.SecondsH) + (Lon.SecondsL / 1024f) + "\r\n" +
                            "East: " + Lon.East + "\r\n"
                            );
        }

        private void cp_BadPacketReceived(object sender, CommProtocol.BadPacketReceivedEventArgs e)
        {
            textBox1.AppendText( e.ErrorMessage+ ". Contents of bad packet: ");
            foreach (char c in e.BadPacket)
            {
                textBox1.AppendText( CharToHex(c) + " ");
            }
            textBox1.AppendText( "\r\n");
            textBox1.ScrollToCaret();
        }

        private void cp_OnBoardErrorPacketReceived(object sender, CommProtocol.OnBoardErrorPacketReceivedEventArgs e)
        {
            textBox1.AppendText("Error received: "+ e.ErrorCode.ToString() + "\r\n");
            textBox1.ScrollToCaret();
        }


        void cp_MotorRPMPacketReceived(object sender, CommProtocol.MotorRPMPacketReceivedEventArgs e)
        {
            MessageBox.Show("RPM packet received. RPM:"+e.RPM);
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
            lat.Minutes = 0;
            lat.North = true;
            lat.SecondsH = 0;
            lat.SecondsL = 0;
            
            Longitude lon = new Longitude();
            lon.Degrees = 0;
            lon.Minutes = 0;
            lon.East = true;
            lon.SecondsH = 0;
            lon.SecondsL = 0;

            cp.Goto(lat,lon,0x48,50);
        }

        private void btnHover_Click(object sender, EventArgs e)
        {
            cp.Hover(0x50,(ushort)0);
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
            cp.SetAntiTorque(Convert.ToByte(textBox3.Text));
        }

        private void btnSetCollective_Click(object sender, EventArgs e)
        {
            cp.SetCollective(Convert.ToByte(textBox4.Text));
        }

        private void btnSetCyclicPitch_Click(object sender, EventArgs e)
        {
            cp.SetCyclicPitch(Convert.ToByte(textBox5.Text));
        }

        private void btnSetCyclicRoll_Click(object sender, EventArgs e)
        {
            cp.SetCyclicRoll(Convert.ToByte(textBox6.Text));
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
        private void btnRequestLocation_Click(object sender, EventArgs e)
        {
            /*		0x4C	Locartion
		            0x48	Heading/Speed/Altitude
		            0x5A 	Attitude
		            0x42	Battery Status
             */
            cp.RequestForInformation(0x4C);

        }
        private void btnRequestBatteryStatus_Click(object sender, EventArgs e)
        {
            /*		0x4C	Locartion
        0x48	Heading/Speed/Altitude
        0x5A 	Attitude
        0x42	Battery Status
 */


            cp.RequestForInformation(0x42);
        }

        private void btnRequestHSA_Click(object sender, EventArgs e)
        {
            cp.RequestForInformation(0x48);
        }

        private void btnRequestAttitude_Click(object sender, EventArgs e)
        {
            cp.RequestForInformation(0x5A);
        }

        private void btnRequestMotorRPM_Click(object sender, EventArgs e)
        {
            cp.RequestForInformation(0x52);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

            if (cp != null)
            {
                cp.RequestForInformation(0x5A);

                /*
                if (BufferCopy != cp.NonVolatileIncomingDataBuffer)
                {
                    BufferCopy = cp.NonVolatileIncomingDataBuffer;
                    textBox2.Clear();

                    textBox2.AppendText(cp.NonVolatileIncomingDataBuffer);
                    textBox2.ScrollToCaret();
                }
                 */
            }
        }


        private void button1_Click(object sender, EventArgs e)
        {
            cp.NonVolatileIncomingDataBuffer += "\r\n";
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {

            cp = new CommProtocol(comboBox1.SelectedItem.ToString(), 19200, Parity.None, 8, StopBits.One, 500, this);

            if (cp != null)
            {
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
                cp.MotorRPMPacketReceived += new CommProtocol.MotorRPMPacketReceivedEventHandler(cp_MotorRPMPacketReceived);
                
            }
        }







    }
}