using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using helopanel;

namespace DemoApplication
{
    public partial class Form1 : Form
    {
        int guageSzStd = 180;
        CenterDial Altitude;
        CenterDial BatteryLife;
        CenterDial RPM;
        ArtificialHorizon ArtHorizon;
        ScrollableMap map;
        float value = 0;
        public Form1()
        {
            InitializeComponent();
            this.Width = guageSzStd * 4;
            this.Height = guageSzStd + 400;
            setUpAltimeter();
            setUpBatteryLife();
            setUpRPM();
            setUpArtHorizon();
            setUpmap();

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
            Altitude.RedLineThreshold = 720;
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
            BatteryLife.MinorLabel = "%";
            BatteryLife.MajorTickDegrees = 30;
            BatteryLife.MajorTickColor = Color.DarkRed;
            BatteryLife.MinorTickDegrees = BatteryLife.MajorTickDegrees / 5;
            BatteryLife.MinorTickColor = Color.Red;
            BatteryLife.NeedleColor = Color.Black;
            BatteryLife.Min = 0;
            BatteryLife.MinDisplayedValue = 0;
            BatteryLife.Max = 120;
            BatteryLife.MaxDisplayedValue = 100;
            BatteryLife.RedLineThreshold = 120;
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
            RPM.RedLineThreshold = 7;
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
        private void setUpmap()
        {
            map = new ScrollableMap();
            map.Path = "..\\..\\cc.jpg";
            map.LoadMap();
            map.Location = new Point(0, guageSzStd);
            map.Size = new Size(guageSzStd * 4, 400);
            this.Controls.Add(map);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            map.ImageTopLeftX += 0.2f;
            if (map.ImageTopLeftX > 100)
            {
                map.ImageTopLeftX = 0;
            }
            map.ImageTopLeftY =2f* (float)Math.Sin(map.ImageTopLeftX)+100;
            /*if (map.ImageTopLeftY > 100)
            {
                map.ImageTopLeftY = 
            }*/

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
            RPM.SetValue(value / 100);
            BatteryLife.SetValue(100 - value / 10);
            Altitude.SetValue(value);
            if (value > 500)
            {
                value = 0;
            }
        }
    }
}