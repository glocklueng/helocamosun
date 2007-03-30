using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.Timers;

namespace CommProtocolLib
{
    public struct Latitude
    {
        /*
        0xDD	Degrees (0-180) Latitude
        0xMM	Minutes (0-59) Latitude
        0xSSSS	Seconds (0-59) Latitude. Use first six bits for integer portion of seconds, use remaining 10 bits for decimal portion of seconds. There are approximately 31 meters in one second of latitude (and slightly less in 1 minute of longitude at our distance from the equator). 31m / 2^10 = 3 cm accuracy, which is way more accuracy than the GPS will determine.
        North or South
        */
        public byte Degrees;
        public byte Minutes;
        public byte SecondsH;
        public UInt16 SecondsL;
        public float SecondsF;
        public bool North;
    }
    public struct Longitude
    {
        /*
        0xDD	Degrees (0-180) Longitude
        0xMM	Minutes (0-59) Longitude
        0xSSSS	Seconds (0-59) Longitude. Use first six bits for integer portion of seconds, use remaining 10 bits for decimal portion of seconds. There are approximately 31 meters in one second of latitude (and slightly less in 1 minute of longitude at our distance from the equator). 31m / 2^10 = 3 cm accuracy, which is way more accuracy than the GPS will determine.
        East or West
        */
        public byte Degrees;
        public byte Minutes;
        public byte SecondsH;
        public UInt16 SecondsL;
        public float SecondsF;
        public bool East;
    }

    public class CommProtocol
    {
        ArrayList ClassExceptions = new ArrayList();
        SerialPort SP;
        string IncomingDataBuffer;
        public CommProtocol(string PortName, int BaudRate)
        {
            try
            {
                SP = new SerialPort(PortName, BaudRate);
                SP.Encoding = Encoding.Unicode;//char type uses unicode for strings
                SP.Open();
                SP.DataReceived += new SerialDataReceivedEventHandler(SP_DataReceived);
            }
            catch (Exception ex)
            {
                ClassExceptions.Add(ex);
            }
        }
        public CommProtocol(string PortName, int BaudRate, Parity parity, int DataBits, StopBits stopBits )
        {
            try
            {
                SP = new SerialPort(PortName, BaudRate, parity, DataBits, stopBits);
                SP.Encoding = Encoding.Unicode;//char type uses unicode for strings
                SP.Open();
                SP.DataReceived += new SerialDataReceivedEventHandler(SP_DataReceived);
            }
            catch (Exception ex)
            {
                ClassExceptions.Add(ex);
            }
        }
        #region Testing and tuning commands
        /// <summary>
        /// Set the helicopter's motor's RPM
        /// </summary>
        /// <param name="RPM">A value between 0 and 100 representing percentage engine speed</param>
        public void SetMotorRPM(int RPM)
        {
            if (RPM >= 0 && RPM <= 100)
            {
                Byte[] OutData = new byte[9];
                OutData[0] = 0xA5;//Start of transmission 1
                OutData[1] = 0x5A;//Start of transmission 2
                OutData[2] = 0x54;//Command type “Testing/Tuning”
                OutData[3] = 0x45;//Command “Engine RPM adjust”
                //value from 0x00 to 0x64 representing the desired percentage of engine speed.
                OutData[4] = Convert.ToByte(RPM);
                //Calculate checksum high byte
                OutData[5] = Convert.ToByte((((short)OutData[2] + (short)OutData[3] + (short)OutData[4]) & 0xFF00) >> 8);
                //Calculate checksum low byte
                OutData[6] = Convert.ToByte(((short)OutData[2] + (short)OutData[3] + (short)OutData[4]) & 0x00FF);
                OutData[7] = 0xCC;//End of transmission 1
                OutData[8] = 0x33;//End of transmission 2
                try
                {
                    SP.Write(OutData, 0, 9);//write the data to the serial port
                }
                catch (Exception ex)
                {
                    ClassExceptions.Add(ex);
                }
            }
            else
            {
                throw new Exception("CommProtocol Error: SetMotorRPM, specified RPM value '" + RPM +
                                    "' is out of range.  Use a value >= 0 and <= 100");
            }
        }
        /// <summary>
        /// Set the cyclic pitch control from 0 to 100
        /// </summary>
        /// <param name="PitchCyclic"></param>
        public void SetCyclicPitch(int CyclicPitch)
        {
            if (CyclicPitch >= 0 && CyclicPitch <= 100)
            {
                Byte[] OutData = new byte[9];
                OutData[0] = 0xA5;//Start of transmission 1
                OutData[1] = 0x5A;//Start of transmission 2
                OutData[2] = 0x54;//Command type “Testing/Tuning”
                OutData[3] = 0x50;//Command “Pitch Servo adjust”
                //value from 0x00 to 0x64 representing the desired percentage of engine speed.
                OutData[4] = Convert.ToByte(CyclicPitch);
                //Calculate checksum high byte
                OutData[5] = Convert.ToByte((((short)OutData[2] + (short)OutData[3] + (short)OutData[4]) & 0xFF00) >> 8);
                //Calculate checksum low byte
                OutData[6] = Convert.ToByte(((short)OutData[2] + (short)OutData[3] + (short)OutData[4]) & 0x00FF);
                OutData[7] = 0xCC;//End of transmission 1
                OutData[8] = 0x33;//End of transmission 2
                try
                {
                    SP.Write(OutData, 0, 9);//write the data to the serial port
                }
                catch (Exception ex)
                {
                    ClassExceptions.Add(ex);
                }
            }
            else
            {
                throw new Exception("CommProtocol Error:  SetCyclicPitch, specified Cyclic Pitch value '" + CyclicPitch +
                    "' is out of range.  Use a value >= 0 and <= 100");
            }

        }
        #endregion


        private void SP_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {           
            IncomingDataBuffer += SP.ReadExisting();
            MatchIncomingPacket(ref IncomingDataBuffer);
        }
        public void MatchIncomingPacket(ref string Packet)
        {
            string LocationPacket = new string(new char[]{ (char)0xA5, (char)0x5A, (char)0x74, (char)0x4C });
            //first 2 bytes are header, second pair are location command packet

            string HeadingSpeedAltitude = new string(new char[] { (char)0xA5, (char)0x5A, (char)0x74, (char)0x48 });
            //first 2 bytes are header, second pair are HeadingSpeedAltitude command packet

            if (Packet.Contains(LocationPacket))
            {
                /*
                Location Packet:
                  1 0xA5    SOT1
                    0x5A    SOT2
                    0x74	Report type “Telemetry”
                  4 0x4C	Report “Location”

                    0xDD	Degrees (0-180) Latitude
                    0xMM	Minutes (0-59) Latitude
                    0xSSSS	Seconds (0-59) Latitude. Use first six bits for integer portion of seconds, use remaining 10 bits for decimal portion of seconds. There are approximately 31 meters in one second of latitude (and slightly less in 1 minute of longitude at our distance from the equator). 31m / 2^10 = 3 cm accuracy, which is way more accuracy than the GPS will determine.
                  9 0xNS	0x4E = North or 0x53 = South

                    0xDD	Degrees (0-180) Longitude
                    0xMM	Minutes (0-59) Longitude
                    0xSSSS	Seconds (0-59) Longitude. See above for more details.
                 14 0xEW	0x45 = East or 0x57 = West
                    
                    0xXX    Checksum 1
                    0xXX    Checksum 2
                    
                    0xCC    EOT1
                 18 0x33    EOT2
                */
                if (Packet.Length == 18 && Packet[16] == 0xCC && Packet[17] == 0x33)
                {
                    //calculate checksum
                    int sum = 0;
                    for(int i = 2; i<14; i++)
                    {
                        sum+=Packet[i];
                    }
                    byte chk1 = (byte)((sum & 0x0000FF00) >> 8);
                    byte chk2 = (byte)(sum & 0x000000FF);
                    if (chk1 != Packet[14] || chk2 != Packet[15])
                    {
                        Packet = "";
                        throw new Exception("Invalid packet: invalid checksum");
                    }
                    else
                    {
                        Latitude Lat = new Latitude();
                        Lat.Degrees = (byte)Packet[4];
                        Lat.Minutes = (byte)Packet[5];
                        Lat.SecondsH = (byte)(Packet[6] & 0x00FC);//upper 6 bits for seconds
                        Lat.SecondsL = Convert.ToUInt16(((byte)Packet[6]&0x03)<<10 + Convert.ToUInt16(Packet[7]));//10 bit decimal portion of seconds
                        Lat.SecondsF = (float)Lat.SecondsH + (float)Lat.SecondsL / 1024.0f;
                        if ((byte)Packet[8] == 0x4E)
                        {
                            Lat.North = true;
                        }
                        else if ((byte)Packet[8] == 0x4E)
                        {
                            Lat.North = false;
                        }
                        else
                        {
                            Packet = "";
                            throw new Exception("Invalid packet: The north south byte of the Latitude in a recieved location packet is invalid.");
                        }

                        Longitude Long = new Longitude();
                        Long.Degrees = (byte)Packet[9];
                        Long.Minutes = (byte)Packet[10];
                        Long.SecondsH = (byte)(Packet[11] & 0x00FC);//upper 6 bits for seconds
                        Long.SecondsL = Convert.ToUInt16(((byte)Packet[11] & 0x03) << 10 + Convert.ToUInt16(Packet[12]));//10 bit decimal portion of seconds
                        Long.SecondsF = (float)Long.SecondsH + (float)Long.SecondsL / 1024.0f;
                        if ((byte)Packet[13] == 0x45)
                        {
                            Long.East = true;
                        }
                        else if ((byte)Packet[13] == 0x57)
                        {
                            Long.East = false;
                        }
                        else
                        {
                            Packet = "";
                            throw new Exception("Invalid packet: The east west byte of the Longitude in a recieved location packet is invalid.");
                        }
                        //invoke the event
                        OnLocationPacketRecieved(new LocationPacketRecievedEventArgs(Lat,Long));
                    }
                }
            }
            else if (Packet.Contains(HeadingSpeedAltitude.ToString()))
            {

            }
            else if (Packet.Length > 4 && Packet[0] == 0xA5 && Packet[1] == 0x5A)
            {
                Packet = "";
                throw new Exception("Invalid packet: unknown command");
            }
            else if (Packet.Length > 4)
            {
                Packet = "";
                throw new Exception("Invalid packet: no packet header");
            }
        }

        #region custom event code

        #region location Packet recieved
        public delegate void LocationPacketRecievedEventHandler(object sender, LocationPacketRecievedEventArgs e);
        public event LocationPacketRecievedEventHandler LocationPacketRecieved;
        protected virtual void OnLocationPacketRecieved(LocationPacketRecievedEventArgs e)
        {
            if (LocationPacketRecieved != null)
            {
                LocationPacketRecieved(this, e);
            }
        }
        /// <summary>
        /// event args definition for a location recieved packet
        /// </summary>
        public class LocationPacketRecievedEventArgs : System.EventArgs
        {
            public Latitude Lat;
            public Longitude Long;
            public LocationPacketRecievedEventArgs(Latitude Lat, Longitude Long)
            {
                this.Lat = Lat;
                this.Long = Long;
            }
        }
        #endregion

        #region bad packet recieved
        public delegate void BadPacketRecievedEventHandler(object sender, BadPacketReceivedEventArgs e);
        public event BadPacketRecievedEventHandler BadPacketReceived;
        protected virtual void OnBadPacketReceived(BadPacketReceivedEventArgs e)
        {
            if (BadPacketReceived != null)
            {
                BadPacketReceived(this, e);
            }
        }

        public class BadPacketReceivedEventArgs : System.EventArgs
        {
            public string BadPacket;
            public string ErrorMessage;
            public BadPacketReceivedEventArgs(string BadPacket, string ErrorMessage)
            {
                this.BadPacket = BadPacket;
                this.ErrorMessage = ErrorMessage;
            }
        }
        #endregion
        #endregion
    }

}
