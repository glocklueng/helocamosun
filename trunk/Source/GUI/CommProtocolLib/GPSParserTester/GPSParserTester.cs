using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using CommProtocolLib;
using GoogleMapsControl;
using System.IO;

namespace GPSParserTester
{
    
    public partial class GPSParserTester : Form
    {
        GoogleMapControl gmc = new GoogleMapControl();
        GPSParser gpsp, gpsp2;
        public GPSParserTester()
        {

            InitializeComponent();
            for (int i = 1; i < 12; i++)
            {
                comboBox1.Items.Add("COM" + i);
                comboBox2.Items.Add("COM" + i);
            }
            comboBox1.SelectedIndex = 0;

            this.Controls.Add(gmc);
        }


        void gpsp_GPSStringReceived(object sender, GPSParser.GPSStringReceivedEventArgs e)
        {

            double Longitude = -(e.data.Long.Degrees + (e.data.Long.Minutes + (e.data.Long.FractionalMinutes / 10000.0)) / 60.0);
            double Latitude = e.data.Lat.Degrees + (e.data.Lat.Minutes + (e.data.Lat.FractionalMinutes / 10000.0)) / 60.0;

            //gmc.ClearTracksAndMarkers();
            gmc.GotoLoc(Latitude, Longitude);
            gmc.SetMarkerAtCurrentLoc();
            textBox1.Clear();
            string gpsdata = "Altitude: " + e.data.Altitude.ToString() + "\r\n" +
                "course: " + e.data.Course.ToString() + "\r\n" +
                "date/time: " + e.data.GPSDateTime.ToString() + "\r\n" +
                "Lat: DD: " + e.data.Lat.Degrees + " MM: " + e.data.Lat.Minutes + " RM: " + e.data.Lat.FractionalMinutes + "\r\n" +
                "Long: DD: " + e.data.Long.Degrees + " MM: " + e.data.Long.Minutes + " RM: " + e.data.Long.FractionalMinutes + "\r\n" +
                "Velocity: " + e.data.Velocity.ToString() + "\r\n";
            textBox1.AppendText(gpsdata);

            File.AppendAllText("COM9_GPS.txt", "Longitude: " + Longitude.ToString() + "\r\n" + "Latitude: " + Latitude.ToString() + "\r\n" + gpsdata + "\r\n");

        }
        private void btnConnect_Click(object sender, EventArgs e)
        {
            try
            {
                File.Delete("COM9_GPS.txt");
            }
            catch(Exception ex)
            {

            } gpsp = new GPSParser(comboBox1.SelectedItem.ToString(), this);
            gpsp.GPSStringReceived += new GPSParser.GPSStringReceivedEventHandler(gpsp_GPSStringReceived);

        }

        void gpsp2_GPSStringReceived(object sender, GPSParser.GPSStringReceivedEventArgs e)
        {
            double Longitude = -(e.data.Long.Degrees + (e.data.Long.Minutes + (e.data.Long.FractionalMinutes / 10000.0)) / 60.0);
            double Latitude = e.data.Lat.Degrees + (e.data.Lat.Minutes + (e.data.Lat.FractionalMinutes / 10000.0)) / 60.0;
  
            gmc.SetMarkerAtLoc(Latitude, Longitude);
            textBox2.Clear();
            string gpsdata =
                "Altitude: " + e.data.Altitude.ToString() + "\r\n" +
                "course: " + e.data.Course.ToString() + "\r\n" +
                "date/time: " + e.data.GPSDateTime.ToString() + "\r\n" +
                "Lat: DD: " + e.data.Lat.Degrees + " MM: " + e.data.Lat.Minutes + " RM: " + e.data.Lat.FractionalMinutes + "\r\n" +
                "Long: DD: " + e.data.Long.Degrees + " MM: " + e.data.Long.Minutes + " RM: " + e.data.Long.FractionalMinutes + "\r\n" +
                "Velocity: " + e.data.Velocity.ToString() + "\r\n";
            textBox2.AppendText(gpsdata);

            File.AppendAllText("COM11_GPS.txt", "Longitude: " + Longitude.ToString() + "\r\n" + "Latitude: " + Latitude.ToString() + "\r\n" + gpsdata + "\r\n");
        }

        private void btnConnect2_Click(object sender, EventArgs e)
        {
            try
            {
                File.Delete("COM11_GPS.txt");
            }
            catch (Exception ex)
            {

            }
            gpsp2 = new GPSParser(comboBox2.SelectedItem.ToString(), this);
            gpsp2.GPSStringReceived += new GPSParser.GPSStringReceivedEventHandler(gpsp2_GPSStringReceived);
        }


    }


}