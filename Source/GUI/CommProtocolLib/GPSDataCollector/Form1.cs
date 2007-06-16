using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using CommProtocolLib;

namespace GPSDataCollector
{
    public partial class Form1 : Form
    {
        GPSParser gpsp1;
        GPSParser gpsp2;
        string gpsDataFile = "GPS_Data.xls";
        
        DataTable GPS1DataTable;

        DataTable GPS2DataTable;

        bool GPSData1Ready = false;
        bool GPSData2Ready = false;

        public Form1()
        {
                       
            InitializeComponent();

            this.FormClosing += new FormClosingEventHandler(Form1_FormClosing);


            GPS1DataTable = new DataTable();
            GPS1DataTable.Columns.Add("Latitude", System.Type.GetType("System.Double"));
            GPS1DataTable.Columns.Add("Longitude", System.Type.GetType("System.Double"));
            GPS1DataTable.Columns.Add("Altitude", System.Type.GetType("System.Double"));
            GPS1DataTable.Columns.Add("Velocity", System.Type.GetType("System.Double"));
            GPS1DataTable.Columns.Add("Time", System.Type.GetType("System.DateTime"));

            GPS2DataTable = new DataTable();
            GPS2DataTable.Columns.Add("Latitude", System.Type.GetType("System.Double"));
            GPS2DataTable.Columns.Add("Longitude", System.Type.GetType("System.Double"));
            GPS2DataTable.Columns.Add("Altitude", System.Type.GetType("System.Double"));
            GPS2DataTable.Columns.Add("Velocity", System.Type.GetType("System.Double"));
            GPS2DataTable.Columns.Add("Time", System.Type.GetType("System.DateTime"));
        }

        void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            button2_Click(this, new EventArgs());
        }

        void gpsp2_GPSStringReceived(object sender, GPSParser.GPSStringReceivedEventArgs e)
        {
            double Longitude = -(e.data.Long.Degrees + (e.data.Long.Minutes + (e.data.Long.FractionalMinutes / 10000.0)) / 60.0);
            double Latitude = e.data.Lat.Degrees + (e.data.Lat.Minutes + (e.data.Lat.FractionalMinutes / 10000.0)) / 60.0;

            DataRow newGPSDataRow = GPS2DataTable.NewRow();
            newGPSDataRow["Latitude"] = Latitude;
            newGPSDataRow["Longitude"] = Longitude;
            newGPSDataRow["Altitude"] = e.data.Altitude;
            newGPSDataRow["Velocity"] = e.data.Velocity;
            newGPSDataRow["Time"] = e.data.GPSDateTime;
            GPS2DataTable.Rows.Add(newGPSDataRow);

            textBox1.AppendText("GPS2 data: ");
            foreach (object o in newGPSDataRow.ItemArray)
            {
                textBox1.AppendText(o.ToString() + "\t");
            }
            textBox1.AppendText("\r\n");
        }

        void gpsp1_GPSStringReceived(object sender, GPSParser.GPSStringReceivedEventArgs e)
        {
            double Longitude = -(e.data.Long.Degrees + (e.data.Long.Minutes + (e.data.Long.FractionalMinutes / 10000.0)) / 60.0);
            double Latitude = e.data.Lat.Degrees + (e.data.Lat.Minutes + (e.data.Lat.FractionalMinutes / 10000.0)) / 60.0;

            DataRow newGPSDataRow = GPS1DataTable.NewRow();
            newGPSDataRow["Latitude"] = Latitude;
            newGPSDataRow["Longitude"] = Longitude;
            newGPSDataRow["Altitude"] = e.data.Altitude;
            newGPSDataRow["Velocity"] = e.data.Velocity;
            newGPSDataRow["Time"] = e.data.GPSDateTime;
            GPS1DataTable.Rows.Add(newGPSDataRow);

            textBox1.AppendText("GPS1 data: ");
            foreach (object o in newGPSDataRow.ItemArray)
            {
                textBox1.AppendText(o.ToString() + "\t");
            }
            textBox1.AppendText("\r\n");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            gpsp1 = new GPSParser("COM9", this);
            gpsp1.GPSStringReceived += new GPSParser.GPSStringReceivedEventHandler(gpsp1_GPSStringReceived);

            gpsp2 = new GPSParser("COM11", this);
            gpsp2.GPSStringReceived += new GPSParser.GPSStringReceivedEventHandler(gpsp2_GPSStringReceived);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string GPSData = "";

            if (!File.Exists(gpsDataFile))
            {
                File.Create(gpsDataFile);
            }
            else
            {
                File.Delete(gpsDataFile);

                
            }
            string titles = "";

            foreach (DataColumn dc in GPS1DataTable.Columns)
            {
                titles += dc.ColumnName+ "1\t";
            }
            foreach (DataColumn dc in GPS2DataTable.Columns)
            {
                titles += dc.ColumnName + "2\t";
            }
            titles += "\n";

            File.AppendAllText(gpsDataFile, titles);
            foreach (DataRow dr in GPS1DataTable.Rows)
            {
                DataRow[] matchingRows = GPS2DataTable.Select("Time = '" + dr["Time"] + "'");
                if (matchingRows.Length == 1)
                {
                    foreach (object gps1dataItems in dr.ItemArray)
                    {
                        GPSData += gps1dataItems.ToString() + "\t";
                    }
                    foreach (object gps2dataItems in matchingRows[0].ItemArray)
                    {
                        GPSData += gps2dataItems.ToString() + "\t";
                    }
                    GPSData += "\n";
                }
            }
            File.AppendAllText(gpsDataFile, GPSData);
        }
    }
}