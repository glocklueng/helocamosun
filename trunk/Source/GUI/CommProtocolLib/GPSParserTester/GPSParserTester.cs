using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using CommProtocolLib;
using GoogleMapsControl;

namespace GPSParserTester
{
    
    public partial class GPSParserTester : Form
    {
        GoogleMapControl gmc = new GoogleMapControl();
        GPSParser gpsp;
        public GPSParserTester()
        {

            InitializeComponent();
            for (int i = 1; i < 12; i++)
            {
                comboBox1.Items.Add("COM" + i);
            }
            comboBox1.SelectedIndex = 0;

            this.Controls.Add(gmc);
        }


        void gpsp_GPSStringReceived(object sender, GPSParser.GPSStringReceivedEventArgs e)
        {
            if (!e.connected)
            {
                textBox1.AppendText("GPS not connected\r\n");
            }
            else
            {
                double Longitude = -(e.data.Long.Degrees + e.data.Long.Minutes / 60.0 + (e.data.Long.Seconds / 1000.0) / 3600.0);
                double Latitude = e.data.Lat.Degrees + e.data.Lat.Minutes / 60.0 + (e.data.Lat.Seconds / 1000.0) / 3600.0;
                gmc.GotoLoc(Latitude, Longitude);
                
                textBox1.AppendText(
                    "Altitude: " + e.data.Altitude.ToString() + "\r\n" +
                    "course: " + e.data.Course.ToString() + "\r\n" +
                    "date/time: " + e.data.GPSDateTime.ToString() + "\r\n" +
                    "Lat: DD: " + e.data.Lat.Degrees + " MM: " + e.data.Lat.Minutes + " ms: " + e.data.Lat.Seconds + "\r\n" +
                    "Long: DD: " + e.data.Long.Degrees + " MM: " + e.data.Long.Minutes + " ms: " + e.data.Long.Seconds + "\r\n" +
                    "Velocity: " + e.data.Velocity.ToString() + "\r\n"
            );
            }
        }
        private void btnConnect_Click(object sender, EventArgs e)
        {
            gpsp = new GPSParser(comboBox1.SelectedItem.ToString(), this);
            gpsp.GPSStringReceived += new GPSParser.GPSStringReceivedEventHandler(gpsp_GPSStringReceived);
        }
    }


}