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
            //if (!e.connected)
            //{
            //    textBox1.AppendText("GPS not connected\r\n");
           // }
            //else
            {
                double Longitude = -(e.data.Long.Degrees + (e.data.Long.Minutes + (e.data.Long.FractionalMinutes / 10000.0)) / 60.0);
                double Latitude = e.data.Lat.Degrees + (e.data.Lat.Minutes + (e.data.Lat.FractionalMinutes / 10000.0)) / 60.0;
                gmc.GotoLoc(Latitude, Longitude);
                
                textBox1.AppendText(
                    "Altitude: " + e.data.Altitude.ToString() + "\r\n" +
                    "course: " + e.data.Course.ToString() + "\r\n" +
                    "date/time: " + e.data.GPSDateTime.ToString() + "\r\n" +
                    "Lat: DD: " + e.data.Lat.Degrees + " MM: " + e.data.Lat.Minutes + " RM: " + e.data.Lat.FractionalMinutes + "\r\n" +
                    "Long: DD: " + e.data.Long.Degrees + " MM: " + e.data.Long.Minutes + " RM: " + e.data.Long.FractionalMinutes + "\r\n" +
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