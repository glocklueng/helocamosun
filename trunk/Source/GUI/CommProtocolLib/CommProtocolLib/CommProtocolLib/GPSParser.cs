using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Multimedia;
using System.IO.Ports;

namespace CommProtocolLib
{
    public struct GPSData
    {
        public Latitude Lat;
        public Longitude Long;
        public DateTime GPSDateTime;
        public Double Altitude;
        public Double Velocity;
        public Double Course;
    }
    public class GPSParser
    {
        /// <summary>
        /// Timer to poll the incoming serial port buffer
        /// </summary>
        private Multimedia.Timer BufferPollTimer;

        Form ParentForm;
        public GPSData gpsdata;
        public bool connected = false;
        bool CheckingForPacket = false;
        SerialPort SP;
        string IncomingDataBuffer = "";

        public GPSParser(string COMPort, Form ParentForm)
        {

            this.ParentForm = ParentForm;
            gpsdata.Lat.Degrees = 0xFF;
            SP = new SerialPort(COMPort, 19200, Parity.None, 8, StopBits.One);
            SP.Open();
            BufferPollTimer = new Multimedia.Timer();
            BufferPollTimer.Period = 10;
            BufferPollTimer.Resolution = 1;
            BufferPollTimer.Mode = TimerMode.Periodic;
            BufferPollTimer.Tick += new EventHandler(BufferPollTimer_Tick);
            BufferPollTimer.Start();            
        }

        public bool parseNMEAStrings(string NMEAstrings)
        {
            gpsdata = new GPSData();
            string[] SplitNMEAStrings;
            string GPGGA = "";
            string GPRMC = "";
            
                SplitNMEAStrings = NMEAstrings.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
                foreach (string SplitNMEAString in SplitNMEAStrings)
                {
                    if (SplitNMEAString.Contains("$GPGGA"))
                    {
                        GPGGA = SplitNMEAString;
                    }
                    else if (SplitNMEAString.Contains("GPRMC"))
                    {
                        GPRMC = SplitNMEAString;
                    }
                }

                if (GPGGA == "" || GPRMC == "")
                {
                    return false;
                }


                string[] GPRMC_Data = GPRMC.Split(new string[] { "," }, StringSplitOptions.None);
                string[] GPGGA_Data = GPGGA.Split(new string[] { "," }, StringSplitOptions.None);
                try
                {
                    Double GPGGA_Lat = Convert.ToDouble(GPGGA_Data[2]);
                    gpsdata.Lat.Degrees = (byte)(GPGGA_Lat / 100.0);
                    gpsdata.Lat.Minutes = (byte)(GPGGA_Lat - gpsdata.Lat.Degrees * 100);       
                    gpsdata.Lat.Seconds = Convert.ToUInt16((GPGGA_Lat % 1) * 60000.0);

                    Double GPGGA_Long = Convert.ToDouble(GPGGA_Data[4]);
                    gpsdata.Long.Degrees = (byte)(GPGGA_Long / 100.0);
                    gpsdata.Long.Minutes = (byte)(GPGGA_Long - gpsdata.Long.Degrees * 100);
                    gpsdata.Long.Seconds = Convert.ToUInt16((GPGGA_Long % 1) * 60000.0);


                    gpsdata.Altitude = Convert.ToDouble(GPGGA_Data[9]);

                    if (GPGGA_Data[4].ToUpper() == "N")
                    {
                        gpsdata.Lat.North = true;
                    }
                    else
                    {
                        gpsdata.Lat.North = false;
                    }
                    if (GPGGA_Data[6].ToUpper() == "E")
                    {
                        gpsdata.Long.East = true;
                    }
                    else
                    {
                        gpsdata.Long.East = false;
                    }
                    Double GPRMC_Time = Convert.ToDouble(GPRMC_Data[1]);

                    int hour = (int)(GPRMC_Time / 10000.0);
                    int min = (int)((GPRMC_Time - hour * 10000) / 100.0);
                    int d_sec = (int)(GPRMC_Time - hour * 10000 - min * 100);
                    int r_sec = (int)(GPRMC_Time % 1 * 1000);

                    string GPRMC_Date = GPRMC_Data[9];

                    int year = Convert.ToInt32(GPRMC_Date[4].ToString() + GPRMC_Date[5].ToString());
                    int month = Convert.ToInt32(GPRMC_Date[2].ToString() + GPRMC_Date[3].ToString());
                    int day = Convert.ToInt32(GPRMC_Date[0].ToString() + GPRMC_Date[1].ToString());

                    gpsdata.GPSDateTime = new DateTime(year + 2000, month, day, hour, min, d_sec, r_sec);

                    gpsdata.Velocity = Convert.ToDouble(GPRMC_Data[7]);
                    gpsdata.Course = Convert.ToDouble(GPRMC_Data[8]);

                    connected = true;
                }
                catch (Exception ex)
                {
                    if (ex.Message == "Input string was not in a correct format.")
                    {
                        //this means the gps is not connected to satellites
                        connected = false;
                        return true;
                    }
                }

                return true;
        }


        void BufferPollTimer_Tick(object sender, EventArgs e)
        {
            if (!CheckingForPacket)
            {
                CheckingForPacket = true;
                CheckForPacket();
            }
        }

        private void CheckForPacket()
        {
            //bool GPSPacket = false;
            for (int i = 0; i < SP.BytesToRead; i++)
            {
                byte incomingByte = (byte)SP.ReadByte();
                char incomingChar = (char)incomingByte;
                IncomingDataBuffer += incomingChar;
            }
            if (IncomingDataBuffer.Length > 0)
            {


                if (IncomingDataBuffer.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries).Length >= 9 && IncomingDataBuffer.EndsWith("\r\n"))
                {
                    if (parseNMEAStrings(IncomingDataBuffer))
                    {
                        //a gps packet was successfully received, or no satellites are connected
                        if (!connected)
                        {
                            ParentForm.Invoke(GPSStringReceived, new object[] { this, new GPSStringReceivedEventArgs(gpsdata, false) });                                //no satellites
                        }
                        else
                        {
                            ParentForm.Invoke(GPSStringReceived, new object[] { this, new GPSStringReceivedEventArgs(gpsdata, true) });
                        }
                        ClearBuffer();
                        CheckingForPacket = false;
                        return;
                    }
                    else
                    {
                        //ParentForm.Invoke(CommProtocol.BadPacketReceived, new object[] { this, new BadPacketReceivedEventArgs("Invalid GPS Packet", "Invalid GPS packet.") });
                        ClearBuffer();
                        CheckingForPacket = false;
                        return;
                    }
                }
                else if (IncomingDataBuffer.Length < 500)
                {
                    //the gps packet is not complete
                    CheckingForPacket = false;
                    return;
                }

            }

            CheckingForPacket = false;
        }
        private void ClearBuffer()
        {
            IncomingDataBuffer = "";
        }
        #region gps string received event
        public delegate void GPSStringReceivedEventHandler(object sender, GPSStringReceivedEventArgs e);

        public event GPSStringReceivedEventHandler GPSStringReceived;


        public class GPSStringReceivedEventArgs : EventArgs
        {
            public GPSData data;
            public bool connected;
            public GPSStringReceivedEventArgs(GPSData data, bool connected)
            {
                this.connected = connected;
                this.data = data;
            }


        }
        #endregion
    }
    
}
