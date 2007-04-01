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
    public struct HeadingSpeedAltitude
    {
        /// <summary>
        /// 
        /// </summary>
        public UInt16 Heading;
        /// <summary>
        /// Speed in meters per second, on the current heading
        /// </summary>
        public byte Speed;
        /// <summary>
        /// Altitude in meters
        /// </summary>
        public UInt16 Altitude;

    }

    public class CommProtocol
    {
        
        #region datamembers
        ArrayList ClassExceptions = new ArrayList();
        SerialPort SP;
        string IncomingDataBuffer;
        #endregion
        
        #region constructors
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
        #endregion
        
        #region out-going packets
        #region Testing and tuning commands
        /// <summary>
        /// Set the helicopter's motor's RPM
        /// </summary>
        /// <param name="RPM">A value between 0 and 100 representing percentage engine speed</param>
        public void SetMotorRPM(UInt16 RPM)
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
        /// <param name="PitchCyclic">A value between 0 and 100 representing the servo angle</param>
        public void SetCyclicPitch(UInt16 CyclicPitch)
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

        #endregion

        #region incoming packets
        
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

            #region Location Packet recieved
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
                    UInt16 sum = 0;
                    for(int i = 2; i<14; i++)
                    {
                        sum+=Packet[i];
                    }
                    byte chk1 = (byte)((sum & 0x0000FF00) >> 8);
                    byte chk2 = (byte)(sum & 0x000000FF);
                    if (chk1 != Packet[14] || chk2 != Packet[15])
                    {
                        OnBadPacketReceived(
                            new BadPacketReceivedEventArgs(Packet,
                            string.Format("Invalid Location packet: invalid checksum recieved {0:X4}, expected {0:X4}",
                            (Convert.ToUInt16(Packet[14]) << 8) + Packet[15], sum))
                            );
                        Packet = "";
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
                            OnBadPacketReceived(new BadPacketReceivedEventArgs(Packet, "Invalid packet: The north south byte of the Latitude in a recieved location packet is invalid."));
                            Packet = "";
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
                            OnBadPacketReceived(new BadPacketReceivedEventArgs(Packet, "Invalid packet: The east west byte of the Longitude in a recieved location packet is invalid."));
                            Packet = "";
                        }
                        //invoke the event
                        OnLocationPacketRecieved(new LocationPacketRecievedEventArgs(Lat,Long));
                    }
                }
            }
            #endregion
            #region Heading Speed altitude recieved
            else if (Packet.Contains(HeadingSpeedAltitude.ToString()))
            {
                /*
                    Heading/Speed/Altitude:
                  1 0xA5    SOT1
                    0x5A    SOT2
                    0x74	Report type “Telemetry”
                    0x48	Report “Heading, Speed, and Altitude”

                  5 0xHHHH	Hex value (0x0000 – 0x0167) representing current heading. Can potentially use extra bits for partial degrees.

                  7 0xSS	Hex value representing helicopter speed (m/s)

                  8 0xAAAA	2-byte value representing altitude in meters
                    
                 10 0xXX    Checksum 1
                    0xXX    Checksum 2
                    
                 12 0xCC    EOT1
                 13 0x33    EOT2
                 */
                if (Packet.Length == 13 && Packet[11] == 0xCC && Packet[12] == 0x33)
                {
                    //calculate checksum
                    UInt16 sum = 0;
                    for(int i = 2; i<8; i++)
                    {
                        sum+=Packet[i];
                    }
                    byte chk1 = (byte)((sum & 0x0000FF00) >> 8);
                    byte chk2 = (byte)(sum & 0x000000FF);
                    if (chk1 != Packet[9] || chk2 != Packet[10])
                    {
                        OnBadPacketReceived(
                           new BadPacketReceivedEventArgs(Packet,
                           string.Format("Invalid heading speed altitude packet: invalid checksum recieved {0:X4}, expected {0:X4}",
                           (Convert.ToUInt16(Packet[9]) << 8) + Packet[10], sum))
                           );
                        Packet = "";
                    }
                    else
                    {
                        HeadingSpeedAltitude HSA = new HeadingSpeedAltitude();
                        HSA.Heading = Convert.ToUInt16(Packet[4] << 8 + Packet[5]);
                        HSA.Speed = Convert.ToByte(Packet[6]);
                        HSA.Altitude = Convert.ToUInt16(Packet[7] << 8 + Packet[8]);
                        //invoke the event
                        OnHeadingSpeedAltitudePacketRecieved(new HeadingSpeedAltitudePacketRecievedEventArgs(HSA));
                    }

                }
            }
            #endregion
            #region bad packet header or bad packet command
            else if (Packet.Length > 4 && Packet[0] == 0xA5 && Packet[1] == 0x5A)
            {
                OnBadPacketReceived(new BadPacketReceivedEventArgs(Packet, "Invalid packet: unknown command"));
                Packet = "";//dump the packet
            }
            else if (Packet.Length > 4)
            {
                Packet = "";
                OnBadPacketReceived(new BadPacketReceivedEventArgs(Packet, "Invalid packet: no packet header"));
            }
            #endregion
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
        #region HeadingSpeedAltitude Packet recieved

        public delegate void HeadingSpeedAltitudePacketRecievedEventHandler(object sender, HeadingSpeedAltitudePacketRecievedEventArgs e);
        public event HeadingSpeedAltitudePacketRecievedEventHandler HeadingSpeedAltitudePacketRecieved;
        protected virtual void OnHeadingSpeedAltitudePacketRecieved(HeadingSpeedAltitudePacketRecievedEventArgs e)
        {
            if (HeadingSpeedAltitudePacketRecieved != null)
            {
                HeadingSpeedAltitudePacketRecieved(this, e);
            }

        }

        public class HeadingSpeedAltitudePacketRecievedEventArgs : System.EventArgs
        {
            public HeadingSpeedAltitude HSA;

            public HeadingSpeedAltitudePacketRecievedEventArgs(HeadingSpeedAltitude HSA)
            {
                this.HSA = HSA;
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
        #endregion
    }

}
