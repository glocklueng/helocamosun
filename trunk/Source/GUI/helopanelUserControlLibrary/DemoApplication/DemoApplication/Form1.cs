using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using helopanel;
using GoogleMapsControl;

namespace DemoApplication
{
    public partial class Form1 : Form
    {
        int guageSzStd = 150;
        CenterDial Altitude;
        CenterDial BatteryLife;
        CenterDial RPM;
        ArtificialHorizon ArtHorizon;
        GoogleMapControl gmc = new GoogleMapControl();
        float value = 0;
        public Form1()
        {
            InitializeComponent();
            this.Width = guageSzStd * 4;
            this.Height = guageSzStd + 800;
            setUpAltimeter();
            setUpBatteryLife();
            setUpRPM();
            setUpArtHorizon();
            setUpGoogleMap();

        }
        private void setUpAltimeter()
        {
            Altitude = new CenterDial();
            Altitude.Size = new Size(guageSzStd, guageSzStd);
            Altitude.MajorLabel = "Altitude";
            Altitude.MinorLabel = "meters";
            Altitude.MajorTickDegrees = 30;
            Altitude.CounterClockWise = true;
            Altitude.MinorTickDegrees = Altitude.MajorTickDegrees / 5;
            Altitude.Min = 0;
            Altitude.MinDisplayedValue = 0;
            Altitude.Max = 720;
            Altitude.MaxDisplayedValue = 600;
            this.Controls.Add(Altitude);
            Altitude.SetValue(100);
        }
        private void setUpBatteryLife()
        {
            BatteryLife = new CenterDial();
            BatteryLife.Size = new Size(guageSzStd, guageSzStd);
            BatteryLife.Location = new Point(guageSzStd, 0);
            BatteryLife.GaugeSurfaceColor = Color.WhiteSmoke;
            BatteryLife.DialOutlineColor = Color.Black;
            BatteryLife.MajorLabel = "BatteryLife";
            BatteryLife.LabelColor = Color.Black;
            BatteryLife.TickLabelColor = Color.Black;
            BatteryLife.MinorLabel = "%";
            BatteryLife.MajorTickDegrees = 30;
            BatteryLife.MajorTickColor = Color.Black;
            BatteryLife.MinorTickDegrees = BatteryLife.MajorTickDegrees / 5;
            BatteryLife.MinorTickColor = Color.Black;
            BatteryLife.NeedleColor = Color.Black;
            BatteryLife.Min = 0;
            BatteryLife.MinDisplayedValue = 0;
            BatteryLife.Max = 120;
            BatteryLife.MaxDisplayedValue = 100;
            
            RedLineRange CriticalRange = new RedLineRange();
            CriticalRange.LowerBound = 0;
            CriticalRange.UpperBound = 30;
            CriticalRange.OverrideTickLabelColor = true;
            CriticalRange.MajorTickColor = Color.Red;
            CriticalRange.MinorTickColor = Color.Red;
            CriticalRange.TickLabelColor = Color.Black;
            BatteryLife.AddRedLineRange(CriticalRange);

            RedLineRange WarningRange = new RedLineRange();
            WarningRange.LowerBound = 30;
            WarningRange.UpperBound = 50;
            WarningRange.OverrideTickLabelColor = true;
            WarningRange.MajorTickColor = Color.DarkGoldenrod;
            WarningRange.MinorTickColor = Color.DarkGoldenrod;
            WarningRange.TickLabelColor = Color.Black;
            BatteryLife.AddRedLineRange(WarningRange);

            RedLineRange OkRange = new RedLineRange();
            OkRange.LowerBound = 50;
            OkRange.UpperBound = 100;
            OkRange.OverrideTickLabelColor = true;
            OkRange.MajorTickColor = Color.Green;
            OkRange.MinorTickColor = Color.Green;
            OkRange.TickLabelColor = Color.Black;
            BatteryLife.AddRedLineRange(OkRange);
            
            this.Controls.Add(BatteryLife);
            BatteryLife.SetValue(78.7f);

        }
        private void setUpRPM()
        {
            RPM = new CenterDial();
            RPM.Size = new Size(guageSzStd, guageSzStd);
            RPM.Location = new Point(guageSzStd*2, 0);
            RPM.MajorLabel = "RPM";
            RPM.MinorLabel = "x1000";
            RPM.MajorTickDegrees = 18;
            RPM.MinorTickDegrees = RPM.MajorTickDegrees / 5;
            RPM.Min = 0;
            RPM.MinDisplayedValue = 0;
            RPM.Max = 20;
            RPM.MaxDisplayedValue = 10;


            RedLineRange WarningRange = new RedLineRange();
            WarningRange.LowerBound = 8;
            WarningRange.UpperBound = 10;
            WarningRange.OverrideTickLabelColor = true;
            WarningRange.MajorTickColor = Color.Red;
            WarningRange.MinorTickColor = Color.Red;
            WarningRange.TickLabelColor = Color.YellowGreen;
            RPM.AddRedLineRange(WarningRange);


            this.Controls.Add(RPM);
            RPM.Value =4.0f;

        }
        private void setUpArtHorizon()
        {
            ArtHorizon = new ArtificialHorizon();
            ArtHorizon.Size = new Size(guageSzStd, guageSzStd);
            ArtHorizon.Location = new Point(guageSzStd * 3, 0);
            this.Controls.Add(ArtHorizon);
            ArtHorizon.Roll=(4.0f);
            ArtHorizon.Pitch=(-1f);
        }


        private void timer1_Tick(object sender, EventArgs e)
        {
            
            gmc.GotoLoc(48.490928+ (double)value/1000000.0, -123.415958+ (double)value/1000000.0);


            ArtHorizon.Roll += 0.2f;
            if (ArtHorizon.Roll > 30)
            {
                ArtHorizon.Roll = -15;
            }
            ArtHorizon.Pitch += 0.2f;
            if (ArtHorizon.Pitch > 20)
            {
                ArtHorizon.Pitch = -20;
            }
            value+=1;
            RPM.SetValue(value);
            BatteryLife.SetValue(100 - value / 10);
            Altitude.SetValue(value);
            if (value > 500)
            {
                value = 0;
            }
        }
        private void setUpGoogleMap()
        {
            this.Controls.Add(gmc);
            gmc.Location = new Point(0, guageSzStd);
            gmc.Show();

        }
    }
}