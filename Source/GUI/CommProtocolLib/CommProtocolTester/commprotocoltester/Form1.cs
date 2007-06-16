using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using CommProtocolLib;
using System.IO.Ports;
using Multimedia;

namespace commprotocoltester
{
    
    public partial class Form1 : Form
    {
        Multimedia.Timer mtimer = new Multimedia.Timer();
  
        DBHandler dbh;
        int SentPacketID = -1;
        int PacketInterval = 50;
        bool ManualMode = true;
        BatteryStatus Battery_Status = new BatteryStatus();
        HeadingSpeedAltitude HSA = new HeadingSpeedAltitude();
        Latitude Lat = new Latitude();
        Longitude Lon = new Longitude();
        PreFlightPacketData PFP = new PreFlightPacketData();
        Attitude attitude = new Attitude();
        string BufferCopy;
        CommProtocol cp;

        byte SetRPM = 0;
        byte SetYaw = 50;
        byte SetCollective = 50;
        byte SetPitch = 50;
        byte SetRoll = 50;

        int PWMinitState = 0;

        int ConsecutiveBadPackets = 0;
        int OutgoingPacketState = 0;

        public Form1()
        {
            this.KeyPreview = true;
            this.KeyPress += new KeyPressEventHandler(Form1_KeyPress);


            InitializeComponent();

            for (int i = 1; i < 12; i++)
            {
                comboBox1.Items.Add("COM" + i);
            }
            comboBox1.SelectedIndex = 0;

            if (!ManualMode)
            {
                dbh = new DBHandler(@"C:\Comms_Test.mdb");
            }
            cbxCtrlMode.SelectedIndex = 0;
            mtimer.Period = PacketInterval;
            mtimer.Resolution = 0;
            mtimer.Mode = TimerMode.Periodic;
            mtimer.Tick += new EventHandler(mtimer_Tick);
            mtimer.Start();

            txtRPM.Text = SetRPM.ToString();
            txtYaw.Text = SetYaw.ToString();
            txtRoll.Text = SetRoll.ToString();
            txtPitch.Text = SetPitch.ToString();
            txtCollective.Text = SetCollective.ToString();
        }

        #region incoming packets
        void cp_ExpectedResponseReceived(object sender, CommProtocol.ExpectedResponseReceivedEventArgs e)
        {
            ConsecutiveBadPackets = 0;

            //e.ReceivedPacket;
            textBox1.AppendText("Expected response recieved: "+e.Name+"\r\n");
            textBox1.ScrollToCaret();
            if (!ManualMode)
            {
                string Received_Packet = "";
                foreach(char c in e.ReceivedPacket)
                {
                    Received_Packet += CharToHex(c);
                }
                InsertRowToReceived_packetsTable("Expected Response receieved " + e.Name, Received_Packet);
            }

        }

        void cp_PreFlightPacketReceived(object sender, CommProtocol.PreFlightPacketReceivedEventArgs e)
        {
            ConsecutiveBadPackets = 0;

            PFP = e.PFP;

                textBox1.AppendText(" battery temperature: " + PFP.BatteryTemp +
                    " GPS Altitude: " + PFP.GPSAltitude +
                    " Lat degrees: " + PFP.Lat.Degrees +
                    " Lat minutes: " + PFP.Lat.Minutes +
                    " Lat minutes remainder: " + (float)(PFP.Lat.FractionalMinutes)/1000.0f +
                    " North degrees: " + PFP.Lat.North +
                    " Long degrees: " + PFP.Long.Degrees +
                    " Long minutes: " + PFP.Long.Minutes +
                    " Long minutes remainder: " + (float)(PFP.Long.FractionalMinutes) /1000.0f+
                    " East degrees: " + PFP.Long.East +
                    " sensor status: " + string.Format("{0:x2}", PFP.SensorStatus) +
                    " sonar altitude: " + PFP.SonarAltitude
                    + "\r\n");



            if (!ManualMode)
            {
                InsertRowToReceived_packetsTable("PreFlightPacket",
                    " battery temperature: " + PFP.BatteryTemp + 
                    " GPS Altitude: " + PFP.GPSAltitude +  
                    " Lat degrees: " + PFP.Lat.Degrees +  
                    " Lat minutes: " + PFP.Lat.Minutes +
                    " Lat minutes remainder: " + (float)(PFP.Lat.FractionalMinutes) / 1000.0f +  
                    " North degrees: " + PFP.Lat.North +  
                    " Long degrees: " + PFP.Long.Degrees +  
                    " Long minutes: " + PFP.Long.Minutes +
                    " Long minutes remainder: " + (float)(PFP.Long.FractionalMinutes) / 1000.0f +  
                    " East degrees: " + PFP.Long.East +  
                    " sensor status: " + string.Format("{0:x2}", PFP.SensorStatus) +  
                    " sonar altitude: " + PFP.SonarAltitude);
            }
        }

        void cp_HandShakeAckReceived(object sender, EventArgs e)
        {
            ConsecutiveBadPackets = 0;

            textBox1.AppendText("Hand shake ack received\r\n");
            textBox1.ScrollToCaret();

        }

        void cp_AttitudePacketReceived(object sender, CommProtocol.AttitudePacketReceivedEventArgs e)
        {
            ConsecutiveBadPackets = 0;

            attitude = e.attitude;

            textBox1.AppendText("Attitude packet received: PITCH: " + attitude.Pitch + " ROLL: " + attitude.Roll + " YAW: " + attitude.Yaw + "\r\n");
            
            if (!ManualMode)
            {
                InsertRowToReceived_packetsTable("Attitude", "PITCH: " + attitude.Pitch + " ROLL: " + attitude.Roll + " YAW: " + attitude.Yaw);
            }
        }

        void cp_HeadingSpeedAltitudePacketReceived(object sender, CommProtocol.HeadingSpeedAltitudePacketReceivedEventArgs e)
        {
            ConsecutiveBadPackets = 0;

            HSA = e.HSA;

            textBox1.AppendText("Heading/Speed/Altitude packet received: Altitude: " + HSA.Altitude + " Heading: " + HSA.Heading + " Speed: " + HSA.Speed + "\r\n");

            if (!ManualMode)
            {
                InsertRowToReceived_packetsTable("HSA", "Altitude: " + HSA.Altitude + " Heading: " + HSA.Heading + " Speed: " + HSA.Speed);
            }
        }

        void cp_BatteryStatusPacketReceived(object sender, CommProtocol.BatteryStatusPacketReceivedEventArgs e)
        {
            ConsecutiveBadPackets = 0;

            Battery_Status = e.BattStat;

            textBox1.AppendText("Battery status packet received: Current: " + Battery_Status.CurrentDraw + " Temperature: " + Battery_Status.Temperature + " Voltage: " + Battery_Status.Voltage + "\r\n");
           
            if (!ManualMode)
            {
                InsertRowToReceived_packetsTable("BatteryStatus", "Current: " + Battery_Status.CurrentDraw + " Temperature: " + Battery_Status.Temperature + " Voltage: " + Battery_Status.Voltage);
            }
        }

        private void cp_LocationPacketReceived(object sender, CommProtocol.LocationPacketReceivedEventArgs e)
        {
            ConsecutiveBadPackets = 0;

            Lat = e.Lat;
            Lon = e.Long;

            Double Longitude = -(Lon.Degrees + Lon.Minutes / 60.0 + (double)(Lon.FractionalMinutes) / 60000.0);
            Double Latitude = Lat.Degrees + Lat.Minutes / 60.0 + (double)(Lat.FractionalMinutes) / 60000.0;

            textBox1.AppendText("Location packet received: Location" +
                                " Lat degrees: " + Lat.Degrees +
                                " Lat minutes: " + Lat.Minutes +
                                " Lat minutes remainder: " + Lat.FractionalMinutes +
                                " North: " + Lat.North +
                                " floating Latitude: " + Latitude +
                                " Long degrees: " + Lon.Degrees +
                                " Long minutes: " + Lon.Minutes +
                                " Long minutes remainder: " + Lon.FractionalMinutes +
                                " East: " + Lon.East +
                                " floating Longitude: " + Longitude + "\r\n");

            if (!ManualMode)
            {
                InsertRowToReceived_packetsTable("Location",
                                    " Lat degrees: " + Lat.Degrees +
                                    " Lat minutes: " + Lat.Minutes +
                                    " Lat minutes remainder: " + Lat.FractionalMinutes +
                                    " North: " + Lat.North +
                                    " Long degrees: " + Lon.Degrees +
                                    " Long minutes: " + Lon.Minutes +
                                    " Long minutes remainder: " + Lon.FractionalMinutes +
                                    " East: " + Lon.East);
            }
        }

        private void cp_BadPacketReceived(object sender, CommProtocol.BadPacketReceivedEventArgs e)
        {
            if (ConsecutiveBadPackets >= 5)
            {
                mtimer.Period = 1000;
                //back off the packet rate
                ConsecutiveBadPackets = 0;
            }
            ConsecutiveBadPackets++;

            string BadPacketHex = "";
            textBox1.AppendText( e.ErrorMessage+ ". ");// + e.BadPacket);
            foreach (char c in e.BadPacket)
            {
                BadPacketHex += CharToHex(c) + " ";
            }
            textBox1.AppendText(BadPacketHex + "\r\n");
           
            textBox1.ScrollToCaret();

            if (!ManualMode)
            {
                InsertRowToReceived_packetsTable("BadPacket", e.ErrorMessage + " " + BadPacketHex);
            }
        }

        private void cp_OnBoardErrorPacketReceived(object sender, CommProtocol.OnBoardErrorPacketReceivedEventArgs e)
        {
            ConsecutiveBadPackets = 0;

            textBox1.AppendText("Error received: "+ e.ErrorCode.ToString() + "\r\n");
            textBox1.ScrollToCaret();
            InsertRowToReceived_packetsTable("Error Packet", e.ErrorCode.ToString());
        }

        void cp_MotorRPMPacketReceived(object sender, CommProtocol.MotorRPMPacketReceivedEventArgs e)
        {

            ConsecutiveBadPackets = 0;

            textBox1.AppendText("RPM packet received. RPM:" + e.RPM + "\r\n");
            
            if(!ManualMode)
            {
                InsertRowToReceived_packetsTable("RPM", e.RPM.ToString());
            }
        }

        void cp_ResponseTimeout(object sender, CommProtocol.ResponseTimeoutEventArgs e)
        {
            string _BufferContents = "";
            textBox1.AppendText("Timeout for " + e.ExpectedResponse.Name + ".");
            if (e.ExpectedResponse.ExpectedPacket != "UNDEFINED")
            {
                textBox1.AppendText(" Expected: ");
                foreach (char c in e.ExpectedResponse.ExpectedPacket)
                {
                    textBox1.AppendText(CharToHex(c) + " ");
                }
            }
            textBox1.AppendText(" Buffer contents:");
            foreach (char c in e.BufferContents)
            {
                _BufferContents += CharToHex(c) + " ";
            }
            textBox1.AppendText(_BufferContents + "\r\n");

            textBox1.ScrollToCaret();

            if (!ManualMode)
            {
                InsertRowToTimeoutTable(_BufferContents);
            }
        }
        #endregion

        private void InsertRowToReceived_packetsTable(string Type, string Contents)
        {
            dbh.ExecuteSQLNonQuery(
                "INSERT INTO received_packets (Sent_Packet_ID, Received_packet_type, Received_packet_contents, time_received) VALUES " +
                "('" + SentPacketID + "', '" + Type + "', '" + Contents + "', '" + DateTime.Now + "')"
                );
        }
        private void InsertRowToTimeoutTable(string BufferContents)
        {
            dbh.ExecuteSQLNonQuery(
                "INSERT INTO timeouts (Sent_Packet_ID, Buffer_Contents, time_received) VALUES " +
                "('" + SentPacketID + "', '" + BufferContents + "', '" + DateTime.Now + "')"
            );
        }

        private void InsertRowToSentPackets(string packet_name)
        {
            dbh.ExecuteSQLNonQuery(
                "INSERT INTO sent_packets (Sent_Packet_ID,packet_name,time_sent) VALUES " +
                "('" + SentPacketID + "','" + packet_name + "','" + DateTime.Now + "')"
                );


        }

        #region outgoing commands
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
            lat.FractionalMinutes = 0;
            
            Longitude lon = new Longitude();
            lon.Degrees = 0;
            lon.Minutes = 0;
            lon.East = true;
            lon.FractionalMinutes = 0;

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
            SetYaw = Convert.ToByte(txtYaw.Text);
            cp.SetAntiTorque(SetYaw);
        }

        private void btnSetCollective_Click(object sender, EventArgs e)
        {
            SetCollective = Convert.ToByte(txtCollective.Text);
            cp.SetCollective(SetCollective);
        }

        private void btnSetCyclicPitch_Click(object sender, EventArgs e)
        {
            SetPitch = Convert.ToByte(txtPitch.Text);
            cp.SetCyclicPitch(SetPitch);
        }

        private void btnSetCyclicRoll_Click(object sender, EventArgs e)
        {
            SetRoll = Convert.ToByte(txtRoll.Text);
            cp.SetCyclicRoll(SetRoll);
        }

        private void btnSetMotorRPM_Click(object sender, EventArgs e)
        {
            SetRPM = Convert.ToByte(txtRPM.Text);
            cp.SetMotorRPM(SetRPM);
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

        #endregion
        void mtimer_Tick(object sender, EventArgs e)
        {

            mtimer.Period = PacketInterval;

            
            if (cp != null && ManualMode)
            {
                //cp.RequestForInformation(0x5A);



            }
            else if (cp != null && !ManualMode)
            {
                if (!cp.ExpectedResponse.ResponseExpected)
                {
                    SentPacketID++;
      //              OutgoingPacketState = 0;
                    switch (OutgoingPacketState)
                    {
                        case 0:
                            btnDiscreetMovementControl_Click(this, new EventArgs());
                            InsertRowToSentPackets("DiscreetMovement");
                            OutgoingPacketState = 1;
                            break;
                        case 1:
                            btnEngageEngine_Click(this, new EventArgs());
                            InsertRowToSentPackets("EngageEngine");
                            OutgoingPacketState = 2;
                            break;
                        case 2:
                            btnGoto_Click(this, new EventArgs());
                            InsertRowToSentPackets("Goto");
                            OutgoingPacketState = 3;
                            break;
                        case 3:
                            btnHover_Click(this, new EventArgs());
                            InsertRowToSentPackets("Hover");
                            OutgoingPacketState = 4;
                            break;
                        case 4:
                            btnRequestPreFlightPacket_Click(this, new EventArgs());
                            InsertRowToSentPackets("RequestPreFlightPacket");
                            OutgoingPacketState = 5;
                            break;
                        case 5:
                            btnReturnToBase_Click(this, new EventArgs());
                            InsertRowToSentPackets("ReturnToBase");
                            OutgoingPacketState = 6;
                            break;
                        case 6:
                            btnSetAntiTorque_Click(this, new EventArgs());
                            InsertRowToSentPackets("SetAntiTorque");
                            OutgoingPacketState = 7;
                            break;
                        case 7:
                            btnSetCollective_Click(this, new EventArgs());
                            InsertRowToSentPackets("SetCollective");
                            OutgoingPacketState = 8;
                            break;
                        case 8:
                            btnSetCyclicPitch_Click(this, new EventArgs());
                            InsertRowToSentPackets("SetCyclicPitch");
                            OutgoingPacketState = 9;
                            break;
                        case 9:
                            btnSetMotorRPM_Click(this, new EventArgs());
                            InsertRowToSentPackets("SetMotorRPM");
                            OutgoingPacketState = 10;
                            break;
                        case 10:
                            btnRequestLocation_Click(this, new EventArgs());
                            InsertRowToSentPackets("RequestLocation");
                            OutgoingPacketState = 11;
                            break;
                        case 11:
                            btnRequestBatteryStatus_Click(this, new EventArgs());
                            InsertRowToSentPackets("RequestBatteryStatus");
                            OutgoingPacketState = 12;
                            break;
                        case 12:
                            btnRequestHSA_Click(this, new EventArgs());
                            InsertRowToSentPackets("RequestHSA");
                            OutgoingPacketState = 13;
                            break;
                        case 13:
                            btnRequestAttitude_Click(this, new EventArgs());
                            InsertRowToSentPackets("Request Attitude");
                            OutgoingPacketState = 14;
                            break;
                        case 14:
                            btnRequestMotorRPM_Click(this, new EventArgs());
                            InsertRowToSentPackets("Request motor RPM");
                            OutgoingPacketState = 0;
                            break;

                    }
                }
            }
        }


        private void button1_Click(object sender, EventArgs e)
        {
            cp.NonVolatileIncomingDataBuffer += "\r\n";
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {

            cp = new CommProtocol(comboBox1.SelectedItem.ToString(),19200, Parity.None, 8, StopBits.One, 200, this);

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
                cp.GPDataReceived += new CommProtocol.GPDataReceivedEventHandler(cp_GPDataReceived); 
            }
        }

        void cp_GPDataReceived(object sender, CommProtocol.GPDataReceivedEventArgs e)
        {
            string data = "";
            foreach(byte b in e.data)
            {
                data += string.Format("{0:x2}", b);
            }
            textBox1.AppendText("GP packet receieved: " + data + "\r\n");
        }




        void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            char c = e.KeyChar;
            switch (c)
            {
                case 'd':
                    if (SetYaw < 100)
                    {
                        SetYaw++;
                    }
                    textBox1.AppendText("yaw set to: " + SetYaw + "\r\n");
                    txtYaw.Text = SetYaw.ToString();
                    cp.SetAntiTorque(SetYaw);
                    break;

                case 'a':
                    if (SetYaw > 0)
                    {
                        SetYaw--;
                    }
                    textBox1.AppendText("yaw set to: " + SetYaw + "\r\n");
                    txtYaw.Text = SetYaw.ToString();
                    cp.SetAntiTorque(SetYaw);
                    break;

                case 's':
                    if (SetCollective < 100)
                    {
                        SetCollective++;
                    }
                    textBox1.AppendText("Collective set to: " + SetCollective + "\r\n");
                    txtCollective.Text = SetCollective.ToString();
                    cp.SetCollective(SetCollective);
                    break;

                case 'w':
                    if (SetCollective > 0)
                    {
                        SetCollective--;
                    }
                    textBox1.AppendText("Collective set to: " + SetCollective + "\r\n");
                    txtCollective.Text = SetCollective.ToString();
                    cp.SetCollective(SetCollective);
                    break;

                case 'i':
                    if (SetPitch < 100)
                    {
                        SetPitch++;
                    }
                    textBox1.AppendText("Pitch set to: " + SetPitch + "\r\n");
                    txtPitch.Text = SetPitch.ToString();
                    cp.SetCyclicPitch(SetPitch);
                    break;

                case 'k':
                    if (SetPitch > 0)
                    {
                        SetPitch--;
                    }
                    textBox1.AppendText("Pitch set to: " + SetPitch + "\r\n");
                    txtPitch.Text = SetPitch.ToString();
                    cp.SetCyclicPitch(SetPitch);
                    break;

                case 'l':
                    if (SetRoll < 100)
                    {
                        SetRoll++;
                    }
                    textBox1.AppendText("Roll set to: " + SetRoll + "\r\n");
                    txtRoll.Text = SetRoll.ToString();
                    cp.SetCyclicRoll(SetRoll);
                    break;
                case 'j':
                    if (SetRoll > 0)
                    {
                        SetRoll--;
                    }
                    textBox1.AppendText("Roll set to: " + SetRoll + "\r\n");
                    txtRoll.Text = SetRoll.ToString();
                    cp.SetCyclicRoll(SetRoll);
                    break;
                case 't':
                    if (SetRPM < 100)
                    {
                        SetRPM++;
                    }
                    textBox1.AppendText("RPM SET to: " + SetRPM + "\r\n");
                    txtRPM.Text = SetRPM.ToString();
                    cp.SetMotorRPM(SetRPM);
                    break;
                case 'g':
                    if (SetRPM > 0)
                    {
                        SetRPM--;
                    }
                    textBox1.AppendText("RPM SET to: " + SetRPM + "\r\n");
                    txtRPM.Text = SetRPM.ToString();
                    cp.SetMotorRPM(SetRPM);
                    break;
                default:
                    break;
            }
        }

        private void btnstop_Click(object sender, EventArgs e)
        {
            if (mtimer.IsRunning)
            {
                mtimer.Stop();
                //timer1.Stop();
            }
            else
            {
                mtimer.Start();
                //timer1.Start();
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void btnGPSCorrection_Click(object sender, EventArgs e)
        {
            GPSCorrection gpsc;
            gpsc.Altitude = 10;
            gpsc.LatMinutes = 5;
            gpsc.LongMinutes = 5;
            gpsc.Time = new DateTime();
            gpsc.Time.AddHours(2.43);

            cp.SendGPSCorrectionFactor(gpsc);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

            if (cp != null)
            {
                if (BufferCopy != cp.NonVolatileIncomingDataBuffer)
                {
                    BufferCopy = cp.NonVolatileIncomingDataBuffer;
                    textBox2.Clear();

                    textBox2.AppendText(cp.NonVolatileIncomingDataBuffer);
                    textBox2.ScrollToCaret();
                }
            }
        }

        private void btnInitPWMS_Click(object sender, EventArgs e)
        {
            PWMinitState = 0;
            timer2.Start();

        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            if (!cp.ExpectedResponse.ResponseExpected)
            {
                switch (PWMinitState)
                {
                    case 0:
                        cp.SetAntiTorque(50);
                        SetYaw = 50;
                        txtYaw.Text = "50";
                        PWMinitState++;
                        break;
                    case 1:
                        cp.SetCollective(50);
                        SetCollective = 50;
                        txtCollective.Text = "50";
                        PWMinitState++;
                        break;
                    case 2:
                        cp.SetCyclicRoll(50);
                        SetRoll = 50;
                        txtRoll.Text = "50";
                        PWMinitState++;
                        break;
                    case 3:
                        cp.SetMotorRPM(0);
                        SetRPM = 0;
                        txtRPM.Text = "0";
                        PWMinitState++;
                        break;
                    case 4:
                        cp.SetCyclicPitch(50);
                        SetPitch = 50;
                        txtPitch.Text = "50";
                        PWMinitState++;
                        break;
                    default:
                        PWMinitState = 0;
                        txtRPM.Text = "0";
                        timer2.Stop();
                        break;
                }
            }
        }

        private void btnSetOpsMode_Click(object sender, EventArgs e)
        {
            cp.SetOpsMode((byte)cbxCtrlMode.SelectedIndex);
        }

        private void btnKillMotor_Click(object sender, EventArgs e)
        {
            cp.SetMotorRPM(0);
        }

        private void btnClearWindows_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
        }



        
    }
}