using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Multimedia;
using System.IO.Ports;

namespace CommProtocolLib
{
    /// <summary>
    /// Structure for storing incoming GPS data
    /// </summary>
    public struct GPSData
    {
        /// <summary>
        /// GPS latitude
        /// </summary>
        public Latitude Lat;
        /// <summary>
        /// GPS longitude
        /// </summary>
        public Longitude Long;
        /// <summary>
        /// Time and date of received gps packet
        /// </summary>
        public DateTime GPSDateTime;
        /// <summary>
        /// GPS altitude 
        /// </summary>
        public Double Altitude;
        /// <summary>
        /// GPS velocity (m/s) in the direction of the GPS course
        /// </summary>
        public Double Velocity;
        /// <summary>
        /// GPS Course in degrees 
        /// </summary>
        public Double Course;
    }
    /// <summary>
    /// Class to handle NMEA strings over a serial port, and return GPS data
    /// </summary>
    public class GPSParser
    {
        /// <summary>
        /// Timer to poll the incoming serial port buffer
        /// </summary>
        private System.Timers.Timer BufferPollTimer;

        Form ParentForm;
        
        private bool Disposed = false;

        private GPSData gpsdata;
        /// <summary>
        /// Flag indicating whether or not the gps module is connected and sending valid GPS data
        /// </summary>
        public bool connected = false;
        bool CheckingForPacket = false;
        SerialPort SP;
        string IncomingDataBuffer = "";
        /// <summary>
        /// Create a new GPSParser
        /// </summary>
        /// <param name="COMPort">com port to connect to eg "COM1"</param>
        /// <param name="ParentForm">the form that owns this instance of the class</param>
        public GPSParser(string COMPort, Form ParentForm)
        {

            this.ParentForm = ParentForm;
            gpsdata.Lat.Degrees = 0xFF;
            SP = new SerialPort(COMPort, 19200, Parity.None, 8, StopBits.One);
            SP.Open();
            BufferPollTimer = new System.Timers.Timer();
            BufferPollTimer.Interval = 50;
            BufferPollTimer.Elapsed += new System.Timers.ElapsedEventHandler(BufferPollTimer_Elapsed);
            BufferPollTimer.Start();            
        }
        /// <summary>
        /// Close the gps parser, this will ensure that no timer events trigger Invoke() for a disposed parent
        /// </summary>
        public void Dispose()
        {
            Disposed = true;
            BufferPollTimer.Dispose();
            SP.Dispose();
            
        }
        private void Invoke(Delegate Method, params object[] args)
        {
            if (!Disposed)
            {
                ParentForm.Invoke(Method, args);
            }
        }
        void BufferPollTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            if (!CheckingForPacket)
            {
                CheckingForPacket = true;
                CheckForPacket();
            }
        }

        private bool parseNMEAStrings(string NMEAstrings)
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
                    gpsdata.Lat.FractionalMinutes = Convert.ToUInt16((GPGGA_Lat % 1) * 10000.0);

                    Double GPGGA_Long = Convert.ToDouble(GPGGA_Data[4]);
                    gpsdata.Long.Degrees = (byte)(GPGGA_Long / 100.0);
                    gpsdata.Long.Minutes = (byte)(GPGGA_Long - gpsdata.Long.Degrees * 100);
                    gpsdata.Long.FractionalMinutes = Convert.ToUInt16((GPGGA_Long % 1) * 10000.0);


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

        private void Invoke()
        {

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
                        Invoke(GPSStringReceived, new object[] { this, new GPSStringReceivedEventArgs(gpsdata) });
                        
                        ClearBuffer();
                        CheckingForPacket = false;
                        return;
                    }
                    else
                    {
                        //Invoke(CommProtocol.BadPacketReceived, new object[] { this, new BadPacketReceivedEventArgs("Invalid GPS Packet", "Invalid GPS packet.") });
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
        /// <summary>
        /// Event handler for a GPS packet being recieved from the module
        /// </summary>
        /// <param name="sender">object from which the event originated</param>
        /// <param name="e">event args containing GPS data</param>
        public delegate void GPSStringReceivedEventHandler(object sender, GPSStringReceivedEventArgs e);
        /// <summary>
        /// GPSStringReceived event
        /// </summary>
        public event GPSStringReceivedEventHandler GPSStringReceived;

        /// <summary>
        /// class containing GPS data
        /// </summary>
        public class GPSStringReceivedEventArgs : EventArgs
        {
            /// <summary>
            /// data receievd from the gps module
            /// </summary>
            public GPSData data;
            /// <summary>
            /// constructor for GPSStringReceivedEventArgs
            /// </summary>
            /// <param name="data">GPS data recieved</param>
            public GPSStringReceivedEventArgs(GPSData data)
            {

                this.data = data;
            }


        }
        #endregion
    }
    
}
