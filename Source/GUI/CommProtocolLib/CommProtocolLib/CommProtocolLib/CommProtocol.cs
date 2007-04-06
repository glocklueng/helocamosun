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
        /// <summary>
        /// The decimal remainder of seconds, the value is 1/2^10 of SecondsH
        /// </summary>
        public UInt16 SecondsL;
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
        /// <summary>
        /// The decimal remainder of seconds, the value is 1/2^10 of SecondsH
        /// </summary>
        public UInt16 SecondsL;
        public bool East;
    }
    public struct HeadingSpeedAltitude
    {
        /// <summary>
        /// heading in degrees, North is zero degrees, increases in clockwise direction
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
    public struct Attitude
    {
        /// <summary>
        /// Helicopter's current roll value: 0 degrees is upright; increases as helicopter rolls in the starboard direction
        /// </summary>
        public UInt16 Roll;
        /// <summary>
        /// Helicopter's current pitch value: 0 degrees is upright; increases as helicopter pitches forward
        /// </summary>
        public UInt16 Pitch;
        /// <summary>
        /// Helicopters current yaw angle: 0 degrees is north; increases as helicopter rotates counter clockwise, as seen from above the helicopter
        /// </summary>
        public UInt16 Yaw;
    }

    public class CommProtocol
    {
        
        #region datamembers
        public ArrayList ClassExceptions = new ArrayList();
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
        public void SetMotorRPM(byte RPM)
        {
            if (RPM >= 0 && RPM <= 100)
            {
                Byte[] OutData = new byte[9];
                OutData[0] = 0xA5;//Start of transmission 1
                OutData[1] = 0x5A;//Start of transmission 2
                OutData[2] = 0x54;//Command type “Testing/Tuning”
                OutData[3] = 0x45;//Command “Engine RPM adjust”
                //value from 0x00 to 0x64 representing the desired percentage of engine speed.
                OutData[4] = RPM;
                //Calculate checksum high byte
                OutData[5] = Convert.ToByte((((short)OutData[2] + (short)OutData[3] + (short)OutData[4]) & 0xFF00) >> 8);
                //Calculate checksum low byte
                OutData[6] = Convert.ToByte((OutData[2] + OutData[3] + OutData[4]) & 0x00FF);
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
        public void SetCyclicPitch(byte CyclicPitch)
        {
            if (CyclicPitch >= 0 && CyclicPitch <= 100)
            {
                Byte[] OutData = new byte[9];
                OutData[0] = 0xA5;//Start of transmission 1
                OutData[1] = 0x5A;//Start of transmission 2
                OutData[2] = 0x54;//Command type “Testing/Tuning”
                OutData[3] = 0x50;//Command “Pitch Servo adjust”
                //value from 0x00 to 0x64 representing the desired percentage of cyclic pitch.
                OutData[4] = CyclicPitch;
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
        /// <summary>
        /// Set the cyclic roll control from 0 to 100
        /// </summary>
        /// <param name="CyclicRoll"></param>
        public void SetCyclicRoll(byte CyclicRoll)
        {

            if (CyclicRoll >= 0 && CyclicRoll <= 100)
            {
                Byte[] OutData = new byte[9];
                OutData[0] = 0xA5;//Start of transmission 1
                OutData[1] = 0x5A;//Start of transmission 2
                OutData[2] = 0x54;//Command type “Testing/Tuning”
                OutData[3] = 0x52;//Command “Roll Servo adjust”
                //value from 0x00 to 0x64 representing the desired percentage of cyclic roll.
                OutData[4] = CyclicRoll;
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
                throw new Exception("CommProtocol Error:  SetCyclicRoll, specified Cyclic Roll value '" + CyclicRoll +
                    "' is out of range.  Use a value >= 0 and <= 100");
            }
        }
        /// <summary>
        /// Set the collective control from 0 to 100 percent
        /// </summary>
        /// <param name="Collective">percentage of collective</param>
        public void SetCollective(byte Collective)
        {
            if (Collective >= 0 && Collective <= 100)
            {
                Byte[] OutData = new byte[9];
                OutData[0] = 0xA5;//Start of transmission 1
                OutData[1] = 0x5A;//Start of transmission 2
                OutData[2] = 0x54;//Command type “Testing/Tuning”
                OutData[3] = 0x43;//Command “Collective Servo adjust”
                //value from 0x00 to 0x64 representing the desired percentage of collective.
                OutData[4] = Collective;
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
                throw new Exception("CommProtocol Error:  SetCollective, specified Collective value '" + Collective +
                    "' is out of range.  Use a value >= 0 and <= 100");
            }

        }
        /// <summary>
        /// Set the anti-torque servo to the desired percentage
        /// </summary>
        /// <param name="AntiTorque">a percentage servo value</param>
        public void SetAntiTorque(byte AntiTorque)
        {
            if (AntiTorque >= 0 && AntiTorque <= 100)
            {
                Byte[] OutData = new byte[9];
                OutData[0] = 0xA5;//Start of transmission 1
                OutData[1] = 0x5A;//Start of transmission 2
                OutData[2] = 0x54;//Command type “Testing/Tuning”
                OutData[3] = 0x51;//Command “Collective Servo adjust”
                //value from 0x00 to 0x64 representing the desired percentage of collective.
                OutData[4] = AntiTorque;
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
                throw new Exception("CommProtocol Error:  SetAntiTorque, specified AntiTorque value '" + AntiTorque +
                    "' is out of range.  Use a value >= 0 and <= 100");
            }

        }


        #endregion


        #region Flight operation commands
        /// <summary>
        /// Start the motor
        /// </summary>
        public void EngageEngine()
        {
            //packet formation: header(0xA55A, command(0x4645), checksum(0x004B), footer(0xCC33)
            byte[] Packet = { 0xA5, 0x5A, 0x46, 0x45, 0x00, 0x4B, 0xCC, 0x33 };
            try
            {
                SP.Write(Packet, 0, 8);//write the data to the serial port
            }
            catch (Exception ex)
            {
                ClassExceptions.Add(ex);
            }
        }
        /// <summary>
        /// Hover using a preset altitude, the current altitude, or the specified altitude
        /// </summary>
        /// <param name="mode">
        /// 0x50 to use preset altitude, 0x43 to use current altitude, and 0x4D to use specified. 
        /// If 4D you must also specify the Altitude parameter. Otherwise it is ignored.
        /// </param>
        /// <param name="Altitude">
        /// The specified altitude this value is ignored unless the mode parameter is set to 0x4D.
        /// In this case the helicopter hovers at the specified value in meters
        /// </param>
        public void Hover(byte mode, UInt16 Altitude)
        {
            if (mode == 0x50 || mode == 0x43)
            {
                byte[] Packet = new byte[9];
                Packet[0] = 0xA5;
                Packet[1] = 0x5A;//packet header
                Packet[2] = 0x46;//Command type “Flight Ops”
                Packet[3] = 0x48;//Command “Hover”
                Packet[4] = mode;//Parameter set: 0x50 = Use preset altitude, 0x43 = hover at current altitude, 0x4D = use following altitude 
                Packet[5] = (byte)(((short)Packet[2] + (short)Packet[3] + (short)Packet[4]) >> 8);//checksum high
                Packet[6] = (byte)((Packet[2] + Packet[3] + Packet[4]) & 0x00FF);//checksum low
                Packet[7] = 0xCC;
                Packet[8] = 0x33;//Footer
                try
                {
                    SP.Write(Packet, 0, 9);//write the data to the serial port
                }
                catch (Exception ex)
                {
                    ClassExceptions.Add(ex);
                }
            }
            else if (mode == 0x4D)
            {
                byte[] Packet = new byte[10];
                Packet[0] = 0xA5;
                Packet[1] = 0x5A;//packet header
                Packet[2] = 0x46;//Command type “Flight Ops”
                Packet[3] = 0x48;//Command “Hover”
                Packet[4] = mode;//Parameter set: 0x50 = Use preset altitude, 0x43 = hover at current altitude, 0x4D = use following altitude 
                Packet[5] = (byte)((Altitude & 0xFF00) >> 8);
                Packet[6] = (byte)(Altitude & 0x00FF);
                Packet[7] = (byte)(((short)Packet[2] + (short)Packet[3] + (short)Packet[4] + (short)Packet[5] + (short)Packet[6]) >> 8);//checksum high
                Packet[8] = (byte)((Packet[2] + Packet[3] + Packet[4] + Packet[5] + Packet[6]) & 0x00FF);//checksum low
                Packet[9] = 0xCC;
                Packet[10] = 0x33;//Footer
                try
                {
                    SP.Write(Packet, 0, 10);//write the data to the serial port
                }
                catch (Exception ex)
                {
                    ClassExceptions.Add(ex);
                }

            }
            else
            {
                throw new Exception("Hover(): Invalid mode argument. Must be either 0x50, 0x43 or 0x4D");
            }

        }
        /// <summary>
        /// Send the helicopter to a gps cooridinate, and specify the action on arrival
        /// </summary>
        /// <param name="Lat">A filled Latitude struct</param>
        /// <param name="Long">a filled longitude struct</param>
        /// <param name="action">0x48 to hover on arrival and 0x53 to circle at arrival</param>
        /// <param name="Altitude">Altitude to perform action at (metres)</param>
        public void Goto(Latitude Lat, Longitude Long, byte action, UInt16 Altitude)
        {
            /*
          1 0xA5    header
            0x5A
            
            0x46	Command type “Flight Ops”
            0x47	Command “Go To”

          5 0xDD	Degrees (0-180) Latitude
            0xMM	Minutes (0-59) Latitude
            0xSSSS	Seconds(0-59) Latitude. Use upper six bits for integer portion of seconds, use remaining 10 bits for decimal portion of seconds. There are approximately 31 meters in one second of latitude (and slightly less in 1 minute of longitude at our distance from the equator). 31m / 2^10 = 3 cm accuracy, which is way more accuracy than the GPS will determine.
            0xNS	0x4E = North
	                0x53 = South

         10 0xDD	Degrees (0-180) Longitude
            0xMM	Minutes (0-59) Longitude
            0xSSSS	Seconds(0-59) Longitude. See above for more details.
            0xEW	0x45 = East
	                0x57 = West

         15 0xAA	Action on Arrival:
		            IF 0x48 Hover at given altitude ELSE IF 0x53 Circle at given altitude
            0xAAAA	Given altitude

         18 0xXX checksum High
            0xXX checksum Low
            0xCC
         21 0x33 footer
            */

            byte[] Packet = new byte[21];
            Packet[0] = 0xA5;
            Packet[1] = 0x5A;
            Packet[2] = 0x46;//Command type “Flight Ops”
            Packet[3] = 0x47;
            Packet[4] = Lat.Degrees;
            Packet[5] = Lat.Minutes;
            //highest 2 bits of the first seconds packet were used for bits 9 and 10 of the secondsL value
            Packet[6] = (byte)(Lat.SecondsH + (byte)((Lat.SecondsL & 0xFC00) >> 8));
            Packet[7] = (byte)(Lat.SecondsL & 0x00FF);
            if (Lat.North)
            {
                Packet[8] = 0x4E;
            }
            else
            {
                Packet[8] = 0x53;
            }
            Packet[9] = Long.Degrees;
            Packet[10] = Long.Minutes;
            //highest 2 bits of the first seconds packet were used for bits 9 and 10 of the secondsL value
            Packet[11] = (byte)(Long.SecondsH + (byte)((Long.SecondsL & 0xFC00) >> 8));
            Packet[12] = (byte)(Long.SecondsL & 0x00FF);
            if (Long.East)
            {
                Packet[13] = 0x45;
            }
            else
            {
                Packet[13] = 0x57;
            }
            if (action == 0x48)
            {
                Packet[14] = 0x48;
            }
            else if (action == 0x53)
            {
                Packet[14] = 0x53;
            }
            else
            {
                throw new Exception("CommProtocol.Goto(): invalid action specified.  Must be either 0x48 or 0x53");
            }
            Packet[15] = (byte)((Altitude & 0xFF00) >> 8);//altitude high byte
            Packet[16] = (byte)(Altitude & 0x00FF);//altitude low byte
            //calculate checksum
            short checksum = 0;
            for (int i = 2; i < 17; i++)//calculate checksum
            {
                checksum += Packet[i];
            }
            Packet[17] = (byte)((checksum & 0xFF00) >> 8);//checksum high
            Packet[18] = (byte)(checksum & 0x00FF);//checksum low
            Packet[19] = 0xCC;
            Packet[20] = 0x33;//footer
            try
            {
                SP.Write(Packet, 0, 21);//write the data to the serial port
            }
            catch (Exception ex)
            {
                ClassExceptions.Add(ex);
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

            string Attitude = new string(new char[] { });

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
            else if (Packet.Contains(HeadingSpeedAltitude))
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
                    for (int i = 2; i < 9; i++)
                    {
                        sum += Packet[i];
                    }
                    byte chk1 = (byte)((sum & 0x0000FF00) >> 8);
                    byte chk2 = (byte)(sum & 0x000000FF);
                    if (chk1 != Packet[9] || chk2 != Packet[10])
                    {
                        OnBadPacketReceived(
                           new BadPacketReceivedEventArgs(Packet,
                           string.Format("Invalid heading speed altitude packet: invalid checksum: recieved {0:X4}, expected {0:X4}",
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
            #region Attitude packet received
            else if (Packet.Contains(Attitude))
            {
                /*
                Attitude
                
                1 0xA5
                  0x5A    Packet Header
                
                3 0x74	Report type “Telemetry”
                  0x5A	Report “Attitude”

                5 0xRRRR	Hex value (0x0000 – 0x0167) representing current roll angle, can potentially use extra bits for partial degrees 
                  (increases as helicopter rolls in the starboard direction)
                 
                7 0xPPPP	Hex value (0x0000 – 0x0167) representing current pitch angle, can potentially use extra bits for partial degrees 
                  (increases as helicopter pitches forward)
                
                9 0xYYYY	Hex value (0x0000 – 0x0167) representing current yaw angle, can potentially use extra bits for partial degrees 
                  (increases as helicopter rotates counter clockwise, as seen from above the helicopter)

               11 0xXX //checksum high
                  0xXX //checksum low
                
               13 0xCC
               14 0x33 //footer
                */
                if (Packet.Length == 14 && Packet[12] == 0xCC && Packet[13] == 0x33)
                {
                    //calculate checksum
                    UInt16 sum = 0;
                    for (int i = 2; i < 10; i++)
                    {
                        sum += Packet[i];
                    }
                    byte chk1 = (byte)((sum & 0xFF00) >> 8);
                    byte chk2 = (byte)(sum & 0x00FF);
                    if (chk1 != Packet[10] || chk2 != Packet[11])
                    {
                        OnBadPacketReceived(
                           new BadPacketReceivedEventArgs(Packet,
                           string.Format("Invalid attitude packet: invalid checksum: recieved {0:X4}, expected {0:X4}",
                           (Convert.ToUInt16(Packet[10]) << 8) + Packet[11], sum))
                           );
                        Packet = "";
                    }
                }
                else
                {
                    Attitude a = new Attitude();
                    a.Roll = (ushort)((Packet[4]<<8) + Packet[5]);
                    a.Pitch = (ushort)((Packet[6] << 8) + Packet[7]);
                    a.Yaw = (ushort)((Packet[8] << 8) + Packet[9]);
                    //invoke the event
                    OnAttitudePacketRecieved(new AttitudePacketRecievedEventArgs(a));
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
        #region Attitude packet recieved

        public delegate void AttitudePacketRecievedEventHandler(object sender, AttitudePacketRecievedEventArgs e);
        public event AttitudePacketRecievedEventHandler AttitudePacketRecieved;
        protected virtual void OnAttitudePacketRecieved(AttitudePacketRecievedEventArgs e)
        {
            if (AttitudePacketRecieved != null)
            {
                AttitudePacketRecieved(this, e);
            }
        }

        public class AttitudePacketRecievedEventArgs
        {
            public Attitude attitude;

            public AttitudePacketRecievedEventArgs(Attitude a)
            {
                attitude = a;
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
