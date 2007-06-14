using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using helopanel;
using GoogleMapsControl;
using CommProtocolLib;
using System.Threading;

namespace HeliGui
{
    public partial class frmAirWulf : Form
    {
        #region datamembers
        
        PreFlightPacketData pfp;

        ushort RPM;
        
        Attitude Att;

        HeadingSpeedAltitude HSA;
        
        BatteryStatus BattStat;
        
        Latitude Lat;
        Longitude Lon;

        GoogleMapControl GoogleMapCtrl;
        
        CommProtocol ComProt;

        GPSParser GPSP;

        int RequestState = 0;

        int ReceivedPacketTimeout;
        bool PacketReceived;
        bool TimedOut = false;
        int AltitudeSamples = 0;
        double BaseStationAltitude = 0;
        #region gauges
        //Gauges to display flight data
        
        CenterDial BatteryLevelGauge;
        CenterDial BatteryTemperatureGauge;
        CenterDial BatteryVoltageGauge;
        CenterDial BatteryCurrentGauge;

        CenterDial AltitudeGauge;
        ArtificialHorizon Arthorizon;
        Compass compass;//direction the craft is pointing
        CenterDial HeadingGauge;
        CenterDial SpeedGauge;
        CenterDial RPMGauge;

        Size GaugeSize;
        #endregion
        #region IndicatorLights
        IndicatorLight ConnectionLight;
        IndicatorLight CommPacketRXLight;
        IndicatorLight BatteryLowLight;
        IndicatorLight BatteryTempLight;
        IndicatorLight BatteryVoltageLight;
        IndicatorLight BatteryCurrentLight;
        IndicatorLight RPMLight;
        IndicatorLight BadPacketLight;
        IndicatorLight PacketTimeoutLight;

        #endregion
        #endregion

        #region constructors
        public frmAirWulf()
        {
            InitializeComponent();

            this.FormClosing += new FormClosingEventHandler(frmAirWulf_FormClosing);
            SetupFormElements();
 
        }

        /// <summary>
        /// called once by the constructor at startup
        /// </summary>
        private void SetupFormElements()
        {
            //Load_GPS_Parser();

            this.Resize += new EventHandler(frmAirWulf_Resize);
            for (int i = 0; i < 12; i++)
            {
                cbxComPortSelect.Items.Add("COM" + (i + 1).ToString());
            }
            cbxComPortSelect.SelectedIndex = 0;
            GoogleMapCtrl = new GoogleMapControl();
            GoogleMapCtrl.Location = new Point(0, 70);
            this.Controls.Add(GoogleMapCtrl);

            GaugeSize =
                new Size(this.Size.Height / 5,
                this.Size.Height / 5
                );

            SetUpBatteryLevelGauge();
            SetUpBatteryTemperatureGauge();
            SetUpBatteryVoltageGauge();
            SetUpBatteryCurrentGauge();

            SetUpAltitudeGauge();
            SetUpArtHorizon();
            SetUpCompass();
            SetUpHeadingGauge();
            SetUpSpeedGauge();
            SetUpRPMGauge();

            SetUpIndicatorLights();

            this.OnResize(new EventArgs());

        }
        #endregion

        void Load_GPS_Parser()
        {
            GPSP = new GPSParser("COM11", this);
            GPSP.GPSStringReceived += new GPSParser.GPSStringReceivedEventHandler(GPSP_GPSStringReceived);
        }
        void frmAirWulf_Resize(object sender, EventArgs e)
        {

            GaugeSize =
                new Size(Convert.ToInt32((this.Size.Width - GoogleMapCtrl.Size.Width) / 3.1),
                Convert.ToInt32((this.Size.Width - GoogleMapCtrl.Size.Width) / 3.1)
                );

            Point Row0Col0 = new Point(GoogleMapCtrl.Size.Width, 0);
            Point Row0Col1 = new Point(GoogleMapCtrl.Size.Width + GaugeSize.Width, 0);
            Point Row0Col2 = new Point(GoogleMapCtrl.Size.Width + GaugeSize.Width * 2, 0);

            Point Row1Col0 = new Point(GoogleMapCtrl.Size.Width, GaugeSize.Height);
            Point Row1Col1 = new Point(GoogleMapCtrl.Size.Width + GaugeSize.Width, GaugeSize.Height);
            Point Row1Col2 = new Point(GoogleMapCtrl.Size.Width + GaugeSize.Width * 2, GaugeSize.Height);

            Point Row2Col0 = new Point(GoogleMapCtrl.Size.Width, GaugeSize.Height * 2);
            Point Row2Col1 = new Point(GoogleMapCtrl.Size.Width + GaugeSize.Width, GaugeSize.Height * 2);
            Point Row2Col2 = new Point(GoogleMapCtrl.Size.Width + GaugeSize.Width * 2, GaugeSize.Height * 2);

            Point Row3Col0 = new Point(GoogleMapCtrl.Size.Width, GaugeSize.Height * 3);
            
            BatteryLevelGauge.Size = GaugeSize;
            BatteryLevelGauge.Location = Row0Col0;
            BatteryLevelGauge.Invalidate();

            BatteryTemperatureGauge.Size = GaugeSize;
            BatteryTemperatureGauge.Location = Row0Col1;
            BatteryTemperatureGauge.Invalidate();

            BatteryVoltageGauge.Size = GaugeSize;
            BatteryVoltageGauge.Location = Row0Col2;
            BatteryVoltageGauge.Invalidate();

            BatteryCurrentGauge.Size = GaugeSize;
            BatteryCurrentGauge.Location = Row1Col0;
            BatteryCurrentGauge.Invalidate();

            AltitudeGauge.Size = GaugeSize;
            AltitudeGauge.Location = Row1Col1;
            AltitudeGauge.Invalidate();

            Arthorizon.Size = GaugeSize;
            Arthorizon.Location = Row1Col2;
            Arthorizon.Invalidate();

            compass.Size = GaugeSize;
            compass.Location = Row2Col0;
            compass.Invalidate();

            HeadingGauge.Size = GaugeSize;
            HeadingGauge.Location = Row2Col1;
            HeadingGauge.Invalidate();

            SpeedGauge.Size = GaugeSize;
            SpeedGauge.Location = Row2Col2;
            SpeedGauge.Invalidate();

            RPMGauge.Size = GaugeSize;
            RPMGauge.Location = Row3Col0;
            RPMGauge.Invalidate();

            Point IndicatorLightTopLeftReference = new Point(GoogleMapCtrl.Width + GaugeSize.Width, GaugeSize.Height * 3);
            float gapSize = 3.0f * (float)GaugeSize.Width / 150.0f;

            Size IndicatorLightSize = new Size(
                Convert.ToInt32(2 * GaugeSize.Width / 3.0f - gapSize),
                Convert.ToInt32(GaugeSize.Height / 5.0f - gapSize));

            BatteryLowLight.Size = IndicatorLightSize;
            BatteryLowLight.Location = GetIndicatorLocation(IndicatorLightTopLeftReference, IndicatorLightSize, gapSize, 0, 0);

            BatteryTempLight.Size = IndicatorLightSize;
            BatteryTempLight.Location = GetIndicatorLocation(IndicatorLightTopLeftReference, IndicatorLightSize, gapSize, 0, 1);

            BatteryVoltageLight.Size = IndicatorLightSize;
            BatteryVoltageLight.Location = GetIndicatorLocation(IndicatorLightTopLeftReference, IndicatorLightSize, gapSize, 0, 2);

            BatteryCurrentLight.Size = IndicatorLightSize;
            BatteryCurrentLight.Location = GetIndicatorLocation(IndicatorLightTopLeftReference, IndicatorLightSize, gapSize, 1, 0);

            RPMLight.Size = IndicatorLightSize;
            RPMLight.Location = GetIndicatorLocation(IndicatorLightTopLeftReference, IndicatorLightSize, gapSize, 1, 1);  

            BadPacketLight.Size = IndicatorLightSize;
            BadPacketLight.Location = GetIndicatorLocation(IndicatorLightTopLeftReference, IndicatorLightSize, gapSize, 1, 2);

            PacketTimeoutLight.Size = IndicatorLightSize;
            PacketTimeoutLight.Location = GetIndicatorLocation(IndicatorLightTopLeftReference, IndicatorLightSize, gapSize, 2, 0);
        }
        private Point GetIndicatorLocation(Point TopReference, Size IndicatorSize, float gapSize, int yOrder, int xOrder)
        {
            return new Point(Convert.ToInt32(TopReference.X + (gapSize * xOrder) + (IndicatorSize.Width * xOrder) + gapSize),
                             Convert.ToInt32(TopReference.Y + (gapSize * yOrder) + (IndicatorSize.Height * yOrder) + gapSize));
        }
        private void btnReloadGMC_Click(object sender, EventArgs e)
        {
            GoogleMapCtrl.Dispose();
            GoogleMapCtrl = new GoogleMapControl();
            GoogleMapCtrl.Location = new Point(0, 70);
            this.Controls.Add(GoogleMapCtrl);
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {



            if (ComProt == null)
            {
                ComProt = new CommProtocol(
                    cbxComPortSelect.SelectedItem.ToString(),
                    19200, System.IO.Ports.Parity.None,
                    8,
                    System.IO.Ports.StopBits.One,
                    500,
                    this);
                if (ComProt.SerialPortOpen)
                {

                    //event handlers
                    ComProt.LocationPacketReceived += new CommProtocol.LocationPacketReceivedEventHandler(ComProt_LocationPacketReceived);
                    ComProt.BadPacketReceived += new CommProtocol.BadPacketReceivedEventHandler(ComProt_BadPacketReceived);
                    ComProt.OnBoardErrorPacketReceived += new CommProtocol.OnBoardErrorPacketReceivedEventHandler(ComProt_OnBoardErrorPacketReceived);
                    ComProt.ResponseTimeout += new CommProtocol.ResponseTimeoutEventHandler(ComProt_ResponseTimeout);
                    ComProt.BatteryStatusPacketReceived += new CommProtocol.BatteryStatusPacketReceivedEventHandler(ComProt_BatteryStatusPacketReceived);
                    ComProt.HeadingSpeedAltitudePacketReceived += new CommProtocol.HeadingSpeedAltitudePacketReceivedEventHandler(ComProt_HeadingSpeedAltitudePacketReceived);
                    ComProt.AttitudePacketReceived += new CommProtocol.AttitudePacketReceivedEventHandler(ComProt_AttitudePacketReceived);
                    ComProt.ExpectedResponseReceived += new CommProtocol.ExpectedResponseReceivedEventHandler(ComProt_ExpectedResponseReceived);
                    ComProt.HandShakeAckReceived += new CommProtocol.HandShakeAckReceivedEventHandler(ComProt_HandShakeAckReceived);
                    ComProt.PreFlightPacketReceived += new CommProtocol.PreFlightPacketReceivedEventHandler(ComProt_PreFlightPacketReceived);
                    ComProt.MotorRPMPacketReceived += new CommProtocol.MotorRPMPacketReceivedEventHandler(ComProt_MotorRPMPacketReceived);
                    

                    
                    RequestInfoTimer.Start();
                }
                else
                {
                    ComProt.Dispose();
                }
            }
        }


        #region safe closing routine

        void frmAirWulf_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (GPSP != null)
            {
                GPSP.Dispose();
            }
            if (ComProt != null)
            {
                if (ComProt.ExpectedResponse.ResponseExpected)
                {
                    e.Cancel = true;

                    ComProt.Dispose();


                    System.Windows.Forms.Timer closeTimer = new System.Windows.Forms.Timer();
                    closeTimer.Interval = 500;
                    closeTimer.Start();
                    closeTimer.Tick += new EventHandler(closeTimer_Tick);
                }
            }

        }

        void closeTimer_Tick(object sender, EventArgs e)
        {
            this.Dispose();
        }

        #endregion



        #region gauge setup routines
        private void SetUpRPMGauge()
        {
            RPMGauge = new CenterDial();
            RPMGauge.Value = 1000;
            RPMGauge.MajorLabel = "Main Rotor";
            RPMGauge.MinorLabel = "RPM";
            RPMGauge.Min = 0;
            RPMGauge.Max = 3600;
            RPMGauge.MinDisplayedValue = 0;
            RPMGauge.MaxDisplayedValue = 3000;
            RPMGauge.LowestValueAngle = 60;

            RedLineRange GoodRPM = new RedLineRange();
            GoodRPM.LowerBound = 1400;
            GoodRPM.UpperBound = 1600;
            GoodRPM.MajorTickColor = Color.Green;
            GoodRPM.MinorTickColor = Color.Green;
            GoodRPM.TickLabelColor = Color.Green;
            RPMGauge.AddRedLineRange(GoodRPM);

            RedLineRange WarningRPM = new RedLineRange();
            WarningRPM.LowerBound = 1200;
            WarningRPM.UpperBound = 1800;
            WarningRPM.MajorTickColor = Color.Yellow;
            WarningRPM.MinorTickColor = Color.Yellow;
            WarningRPM.TickLabelColor = Color.Yellow;
            RPMGauge.AddRedLineRange(WarningRPM);

            RedLineRange OverRPM = new RedLineRange();
            OverRPM.LowerBound = 1801;
            OverRPM.UpperBound = 3600;
            RPMGauge.AddRedLineRange(OverRPM);
            this.Controls.Add(RPMGauge);


        }
        private void SetUpSpeedGauge()
        {
            SpeedGauge = new CenterDial();
            SpeedGauge.MajorLabel = "Speed";
            SpeedGauge.MinorLabel = "m/s";
            SpeedGauge.Max = 12;
            SpeedGauge.Min = 0;
            SpeedGauge.MaxDisplayedValue = 10;
            SpeedGauge.MinDisplayedValue = 1;
            SpeedGauge.LowestValueAngle = 80;
            this.Controls.Add(SpeedGauge);
        }
        private void SetUpHeadingGauge()
        {
            HeadingGauge = new CenterDial();//direction the craft is moving
            HeadingGauge.Max = 360;
            HeadingGauge.Min = 0;
            HeadingGauge.MaxDisplayedValue = 359;
            HeadingGauge.MinDisplayedValue = 0;
            HeadingGauge.MajorTickDegrees = 30;
            HeadingGauge.MinorTickDegrees = 6;
            HeadingGauge.LowestValueAngle = -90;
            HeadingGauge.MajorLabel = "Heading";
            HeadingGauge.MinorLabel = "Degrees";
            this.Controls.Add(HeadingGauge);
        }
        private void SetUpCompass()
        {
            compass = new Compass();//direction the craft is pointing

            this.Controls.Add(compass);
        }
        private void SetUpArtHorizon()
        {
            Arthorizon = new ArtificialHorizon();
            
            this.Controls.Add(Arthorizon);
            Arthorizon.Roll = 5;
            Arthorizon.Pitch = -5;
        }
        private void SetUpAltitudeGauge()
        {
            AltitudeGauge = new CenterDial();
            AltitudeGauge.MajorLabel = "Altitude";
            AltitudeGauge.MinorLabel = "metres";
            AltitudeGauge.Max = 60;
            AltitudeGauge.Min = 0;
            AltitudeGauge.MinDisplayedValue = 0;
            AltitudeGauge.MaxDisplayedValue = 50;
            this.Controls.Add(AltitudeGauge);
        }
        private void SetUpBatteryCurrentGauge()
        {
            BatteryCurrentGauge = new CenterDial();
            BatteryCurrentGauge.MajorLabel = "Batt Current";
            BatteryCurrentGauge.MinorLabel = "Amperes";
            BatteryCurrentGauge.Min = 0;
            BatteryCurrentGauge.Max = 120;
            BatteryCurrentGauge.MinDisplayedValue = 0;
            BatteryCurrentGauge.MaxDisplayedValue = 70;
            BatteryCurrentGauge.LowestValueAngle = 15;
            RedLineRange BatteryOverCurrent = new RedLineRange();
            BatteryOverCurrent.LowerBound = 50;
            BatteryOverCurrent.UpperBound = 120;
            BatteryCurrentGauge.AddRedLineRange(BatteryOverCurrent);
            this.Controls.Add(BatteryCurrentGauge);
        }
        private void SetUpBatteryVoltageGauge()
        {
            BatteryVoltageGauge = new CenterDial();
            BatteryVoltageGauge.MajorLabel = "Batt Voltage";
            BatteryVoltageGauge.MinorLabel = "Volts";
            BatteryVoltageGauge.Min = 14;
            BatteryVoltageGauge.Max = 20;
            BatteryVoltageGauge.MinDisplayedValue = 15;
            BatteryVoltageGauge.MaxDisplayedValue = 19;
            BatteryVoltageGauge.CounterClockWise = false;
            BatteryVoltageGauge.LowestValueAngle = 90;

            RedLineRange BatteryUnderVoltage = new RedLineRange();
            BatteryUnderVoltage.UpperBound = 16;
            BatteryUnderVoltage.LowerBound = 0;
            BatteryVoltageGauge.AddRedLineRange(BatteryUnderVoltage);
            this.Controls.Add(BatteryVoltageGauge);
        }
        private void SetUpBatteryTemperatureGauge()
        {
            BatteryTemperatureGauge = new CenterDial();

            BatteryTemperatureGauge.MajorLabel = "Batt Temp";
            BatteryTemperatureGauge.MinorLabel = "Degrees C";
            BatteryTemperatureGauge.MajorTickDegrees = 20;
            BatteryTemperatureGauge.MinorTickDegrees = 4;
            BatteryTemperatureGauge.Min = -60;
            BatteryTemperatureGauge.Max = 120;
            BatteryTemperatureGauge.MinDisplayedValue = -40;
            BatteryTemperatureGauge.MaxDisplayedValue = 100;
            BatteryTemperatureGauge.LowestValueAngle = 120;

            RedLineRange BatteryOverTemp = new RedLineRange();
            BatteryOverTemp.UpperBound = 120;
            BatteryOverTemp.LowerBound = 50;
            BatteryTemperatureGauge.AddRedLineRange(BatteryOverTemp);

            RedLineRange BatteryUnderTemp = new RedLineRange();
            BatteryUnderTemp.LowerBound = -60;
            BatteryUnderTemp.UpperBound = 0;
            BatteryUnderTemp.TickLabelColor = Color.Blue;
            BatteryUnderTemp.OverrideTickLabelColor = true;
            BatteryUnderTemp.MinorTickColor = Color.Blue;
            BatteryUnderTemp.MajorTickColor = Color.Blue;
            BatteryTemperatureGauge.AddRedLineRange(BatteryUnderTemp);
            this.Controls.Add(BatteryTemperatureGauge);
        }
        private void SetUpBatteryLevelGauge()
        {
            BatteryLevelGauge = new CenterDial();
            BatteryLevelGauge.Size = GaugeSize;

            BatteryLevelGauge.MajorLabel = "Battery Level";
            BatteryLevelGauge.MinorLabel = "Percent";
            BatteryLevelGauge.CounterClockWise = true;
            BatteryLevelGauge.LabelColor = Color.White;
            BatteryLevelGauge.Min = 0;
            BatteryLevelGauge.Max = 120;

            BatteryLevelGauge.MinDisplayedValue = 0;
            BatteryLevelGauge.MaxDisplayedValue = 100;

            RedLineRange BatteryOK = new RedLineRange();
            BatteryOK.MajorTickColor = Color.LightGreen;
            BatteryOK.MinorTickColor = Color.LightGreen;
            BatteryOK.TickLabelColor = Color.LightGreen;
            BatteryOK.OverrideTickLabelColor = true;
            BatteryOK.LowerBound = 51;
            BatteryOK.UpperBound = 100;
            BatteryLevelGauge.AddRedLineRange(BatteryOK);

            RedLineRange BatteryLow = new RedLineRange();
            BatteryLow.TickLabelColor = Color.Yellow;
            BatteryLow.MajorTickColor = Color.Yellow;
            BatteryLow.MinorTickColor = Color.Yellow;
            BatteryLow.OverrideMajorTickColor = true;
            BatteryLow.LowerBound = 21;
            BatteryLow.UpperBound = 50;
            BatteryLevelGauge.AddRedLineRange(BatteryLow);

            RedLineRange BatteryCritical = new RedLineRange();
            BatteryCritical.TickLabelColor = Color.Red;
            BatteryCritical.OverrideTickLabelColor = true;
            BatteryCritical.LowerBound = 0;
            BatteryCritical.UpperBound = 20;
            BatteryLevelGauge.AddRedLineRange(BatteryCritical);

            this.Controls.Add(BatteryLevelGauge);
        }
        #endregion
        
        private void SetUpIndicatorLights()
        {
            ConnectionLight = new IndicatorLight("Not Connected");
            ConnectionLight.OnColor = Color.Orange;
            ConnectionLight.Location = new Point(315, 5);
            ConnectionLight.Size = new Size(75, 25);
            ConnectionLight.Blink = true;
            this.Controls.Add(ConnectionLight);

            CommPacketRXLight = new IndicatorLight("Received");
            CommPacketRXLight.OnColor = Color.Orange;
            CommPacketRXLight.Location = new Point(400, 5);
            CommPacketRXLight.Size = new Size(75, 25);
            this.Controls.Add(CommPacketRXLight);

            BatteryLowLight = new IndicatorLight("Battery Low");
            BatteryLowLight.OnColor = Color.Orange;
            this.Controls.Add(BatteryLowLight);

            BatteryTempLight = new IndicatorLight("Battery Over\r\n Temp");
            BatteryTempLight.OnColor = Color.Red;
            this.Controls.Add(BatteryTempLight);

            BatteryVoltageLight = new IndicatorLight("Battery Low\r\n voltage");
            BatteryVoltageLight.OnColor = Color.Orange;
            this.Controls.Add(BatteryVoltageLight);

            BatteryCurrentLight = new IndicatorLight("Battery over\r\n current");
            BatteryCurrentLight.OnColor = Color.Orange;
            this.Controls.Add(BatteryCurrentLight);

            RPMLight = new IndicatorLight("Over RPM");
            RPMLight.OnColor = Color.Orange;
            this.Controls.Add(RPMLight);

            BadPacketLight = new IndicatorLight("Bad Packet\r\nReceived");
            BadPacketLight.OnColor = Color.Orange;
            this.Controls.Add(BadPacketLight);

            PacketTimeoutLight = new IndicatorLight("Packet Timeout");
            PacketTimeoutLight.OnColor = Color.Orange;
            this.Controls.Add(PacketTimeoutLight);
        }
        #region event handlers
        void ComProt_MotorRPMPacketReceived(object sender, CommProtocol.MotorRPMPacketReceivedEventArgs e)
        {
            CommPacketRXLight.On = true;
            PacketReceived = true;
            RPM = e.RPM;
            RPMGauge.Value = RPM;
        }

        void ComProt_PreFlightPacketReceived(object sender, CommProtocol.PreFlightPacketReceivedEventArgs e)
        {
            CommPacketRXLight.On = true;
            PacketReceived = true;
            pfp = e.PFP;
        }

        void ComProt_HandShakeAckReceived(object sender, EventArgs e)
        {
            CommPacketRXLight.On = true;
            PacketReceived = true;
            ComProt.CommsHandShakeTerminate();
        }

        void ComProt_ExpectedResponseReceived(object sender, CommProtocol.ExpectedResponseReceivedEventArgs e)
        {
            CommPacketRXLight.On = true;
            PacketReceived = true;
          
        }

        void ComProt_AttitudePacketReceived(object sender, CommProtocol.AttitudePacketReceivedEventArgs e)
        {
            CommPacketRXLight.On = true;
            PacketReceived = true;
            Att = e.attitude;

            compass.Angle = Att.Yaw;
            if (Att.Roll > 128)
            {
                Arthorizon.Roll = -(255 - Att.Roll);

            }
            else
            {
                Arthorizon.Roll = Att.Roll;
            }
            if (Att.Pitch > 128)
            {
                Arthorizon.Pitch = -(255 - Att.Pitch);
            }
            else
            {
                Arthorizon.Pitch = Att.Pitch;
            }
        }

        void ComProt_HeadingSpeedAltitudePacketReceived(object sender, CommProtocol.HeadingSpeedAltitudePacketReceivedEventArgs e)
        {
            CommPacketRXLight.On = true;
            PacketReceived = true;
            HSA = e.HSA;

            SpeedGauge.Value = HSA.Speed;
            HeadingGauge.Value = HSA.Heading;
            AltitudeGauge.Value = HSA.Altitude;

        }

        void ComProt_BatteryStatusPacketReceived(object sender, CommProtocol.BatteryStatusPacketReceivedEventArgs e)
        {
            CommPacketRXLight.On = true;
            PacketReceived = true;
            BattStat = e.BattStat;

            BatteryVoltageGauge.Value = BattStat.Voltage;
            BatteryTemperatureGauge.Value = BattStat.Temperature;
            BatteryCurrentGauge.Value = BattStat.CurrentDraw;
        }

        void ComProt_ResponseTimeout(object sender, CommProtocol.ResponseTimeoutEventArgs e)
        {
            PacketTimeoutLight.BlinkRate = 1500;
            PacketTimeoutLight.OneShot = true;
            PacketTimeoutLight.Blink = true;

        }

        void ComProt_OnBoardErrorPacketReceived(object sender, CommProtocol.OnBoardErrorPacketReceivedEventArgs e)
        {
            CommPacketRXLight.On = true;
            PacketReceived = true;

        }

        void ComProt_BadPacketReceived(object sender, CommProtocol.BadPacketReceivedEventArgs e)
        {
            BadPacketLight.BlinkRate = 500;
            BadPacketLight.OneShot = true;
            BadPacketLight.Blink = true;
        }

        void ComProt_LocationPacketReceived(object sender, CommProtocol.LocationPacketReceivedEventArgs e)
        {
            CommPacketRXLight.On = true;
            PacketReceived = true;
            if (e.Long.Degrees != 0 && e.Lat.Degrees != 0)
            {
                Lon = e.Long;
                Lat = e.Lat;

                double Longitude = -(Lon.Degrees + ((Lon.Minutes + (Lon.FractionalMinutes / 10000.0)) / 60.0));
                double Latitude = Lat.Degrees + ((Lat.Minutes + (Lat.FractionalMinutes / 10000.0)) / 60.0);
                GoogleMapCtrl.GotoLoc(Latitude, Longitude);
            }
        }
        void GPSP_GPSStringReceived(object sender, GPSParser.GPSStringReceivedEventArgs e)
        {
            
            if (e.data.Long.Degrees != 0 && e.data.Lat.Degrees != 0)
            {
                Lon = e.data.Long;
                Lat = e.data.Lat;
                
                if (GoogleMapCtrl.BaseStationSet)
                {
                    RequestInfoTimer.Enabled = false;

                    double Longitude = -(Lon.Degrees + ((Lon.Minutes + (Lon.FractionalMinutes / 10000.0)) / 60.0));
                    double Latitude = Lat.Degrees + ((Lat.Minutes + (Lat.FractionalMinutes / 10000.0)) / 60.0);
                    GoogleMapCtrl.SetMarkerAtLoc(Latitude, Longitude);
                    double LatCorrection = 0;//GoogleMapCtrl.BaseStation.latitude - Latitude;
                    double LongCorrection = 0;//GoogleMapCtrl.BaseStation.longitude - Longitude;


                    GPSCorrection gpsc = new GPSCorrection();
                    if (AltitudeSamples < 10)
                    {
                        gpsc.Altitude = 0;//zero correction until we have enough base station samples
                        BaseStationAltitude += e.data.Altitude;
                        AltitudeSamples++;
                        
                    }
                    if (AltitudeSamples == 10)
                    {
                        gpsc.Altitude = 0;//zero correction until we have enough base station samples
                        AltitudeSamples = 11;
                        BaseStationAltitude = BaseStationAltitude / 10.0;
                    }
                    
                    if (AltitudeSamples == 11)
                    {
                        gpsc.Altitude = Convert.ToInt16((e.data.Altitude * 10) - (BaseStationAltitude * 10));
                        txtAltitudeCorrection.Text = gpsc.Altitude.ToString();
                    }
                    gpsc.Time = new DateTime();
                    gpsc.Time.AddSeconds(e.data.GPSDateTime.Second);

                    gpsc.LatMinutes = (short)(LatCorrection * 60.0 * 10000.0);
                    gpsc.LongMinutes = (short)-(LongCorrection * 60.0 * 10000.0);

                    txtLatCorrection.Text = gpsc.LatMinutes.ToString();
                    txtLongCorrection.Text = gpsc.LongMinutes.ToString();

                    if (ComProt != null)
                    {
                        int timeout = 0;
                        while (ComProt.ExpectedResponse.ResponseExpected)
                        {
                            if (timeout > 1000)
                            {
                                break;
                            }
                            timeout++;
                        }
                        ComProt.SendGPSCorrectionFactor(gpsc);

                        RequestInfoTimer.Enabled = true;
                    }
                }
            }
        }

        #endregion

        private void RequestInfoTimer_Tick(object sender, EventArgs e)
        {
            switch (RequestState)
            {
                case 0:
                    ComProt.RequestForInformation(0x4C);//location
                    RequestState++;
                    break;
                case 1:
                    ComProt.RequestForInformation(0x48);//heading speed altitude
                    RequestState++;
                    break;
                case 2:
                    ComProt.RequestForInformation(0x5A);//attitude
                    RequestState++;
                    break;
                case 3:
                    ComProt.RequestForInformation(0x42);//battery status
                    RequestState++;
                    break;
                case 4:
                    ComProt.RequestForInformation(0x52);//motor RPM
                    RequestState = 0;
                    break;

            }
        }

        private void PacketRXTimer_Tick(object sender, EventArgs e)
        {

            if (PacketReceived == true)
            {
                TimedOut = false;
                ReceivedPacketTimeout = 0;
                CommPacketRXLight.On = false;

                ConnectionLight.OnColor = Color.Green;
                ConnectionLight.Blink = false;
                ConnectionLight.On = true;
                ConnectionLight.WarningText = "Connected";
            }
            else
            {
                ReceivedPacketTimeout++;
            }
            PacketReceived = false;
            if (ReceivedPacketTimeout >= 4 && !TimedOut)
            {
                ReceivedPacketTimeout = 0;
                TimedOut = true;
                ConnectionLight.OnColor = Color.Orange;
                ConnectionLight.Blink = true;
                ConnectionLight.On = true;
                ConnectionLight.WarningText = "Not Connected";
            }
        }
    }
}