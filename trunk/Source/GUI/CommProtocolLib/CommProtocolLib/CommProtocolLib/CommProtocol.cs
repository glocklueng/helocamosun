using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.Timers;
using System.Threading;

namespace CommProtocolLib
{
    #region Public Data Structures
    /// <summary>
    /// standard global latitude data structure
    /// </summary>
    public struct Latitude
    {
        /// <summary>
        /// standard latitude degrees (0-180)
        /// </summary>
        public byte Degrees;
        /// <summary>
        /// standard latitude minutes (0 to 60)
        /// </summary>
        public byte Minutes;
        /// <summary>
        /// integer portion of seconds, a value between 0 and 60 inclusive
        /// </summary>
        public byte SecondsH;
        /// <summary>
        /// The decimal remainder of seconds, the value is 1/2^10 of SecondsH
        /// </summary>
        public UInt16 SecondsL;
        /// <summary>
        /// North latitude if true, otherwise south
        /// </summary>
        public bool North;
    }
    /// <summary>
    /// standard global longitude data structure
    /// </summary>
    public struct Longitude
    {
        /// <summary>
        /// standard longitude degrees (0-180)
        /// </summary>
        public byte Degrees;
        /// <summary>
        /// standard longitude minutes (0 to 60)
        /// </summary>
        public byte Minutes;
        /// <summary>
        /// integer portion of seconds, a value between 0 and 60 inclusive
        /// </summary>
        public byte SecondsH;
        /// <summary>
        /// The decimal remainder of seconds, the value is 1/2^10 of SecondsH
        /// </summary>
        public UInt16 SecondsL;
        /// <summary>
        /// East latitude if true, otherwise West
        /// </summary>
        public bool East;
    }
    /// <summary>
    /// Data structure to keep track of heading speed and altitude of the helicopter
    /// </summary>
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
    /// <summary>
    /// Data structure to keep track of the helicopter's attitude (roll, pitch and yaw)
    /// </summary>
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
    /// <summary>
    /// data structure for battery information packets 
    /// </summary>
    public struct BatteryStatus
    {
        /// <summary>
        /// battery voltage reading
        /// </summary>
        public ushort Voltage;
        /// <summary>
        /// battery current draw reading
        /// </summary>
        public ushort CurrentDraw;
        /// <summary>
        /// battery temperature reading
        /// </summary>
        public ushort Temperature;
    }
    /// <summary>
    /// Data structure containing pre-flight packet data
    /// </summary>
    public struct PreFlightPacketData
    {
        /// <summary>
        /// Pre-flight latitude
        /// </summary>
        public Latitude Lat;
        /// <summary>
        /// Pre-flight longitude
        /// </summary>
        public Longitude Long;
        /// <summary>
        /// Altiude of helicopter as reported by GPS
        /// </summary>
        public ushort GPSAltitude;
        /// <summary>
        /// Altitude of helicopter as reported by the acoustic sensor
        /// </summary>
        public ushort SonarAltitude;
        /// <summary>
        /// Main battery voltage
        /// </summary>
        public ushort BatteryVoltage;
        /// <summary>
        /// main battery temperature
        /// </summary>
        public ushort BatteryTemp;
        /// <summary>
        /// a bit field containing sensor status
        /// </summary>
        public byte SensorStatus;
    }
    #endregion

    /// <summary>
    /// Implementation of the helicopter communication protocol via the serial port
    /// </summary>
    public class CommProtocol
    {

        #region datamembers
        /// <summary>
        /// serial port encoding
        /// </summary>
        private Encoding encoding = Encoding.Default;
        /// <summary>
        /// When a packet is sent there is an expected response from the helicopter
        /// it is a full packet (with ACK)response or a data response.  This struct keeps track of the state.
        /// </summary>
        public ExpectedResponses ExpectedResponse = new ExpectedResponses();
        /// <summary>
        /// any serial port exceptions raised by this class are stored here
        /// </summary>
        public ArrayList ClassExceptions = new ArrayList();
        private SerialPort SP;
        private string IncomingDataBuffer;
        private byte[] OutGoingPacket;
        /// <summary>
        /// Time to wait for a response after sending a data packet.  
        /// If the timer expires before the expected packet is recieved the OnResponseTimeout event is invoked.
        /// </summary>
        private int TimeOut = 100;
        private System.Timers.Timer ResponseTimer;
        /// <summary>
        /// When a packet is sent there is an expected response from the helicopter
        /// it is a full packet (with ACK)response or a data response.
        /// </summary>
        public struct ExpectedResponses
        {
            /// <summary>
            /// If true a response is expected and no datapackets can be sent out until a response is recieved or a timeout occurs
            /// </summary>
            public bool ResponseExpected;
            /// <summary>
            /// The expected response packet, this is defined only for full packet responses
            /// where the response can be fully predicted
            /// </summary>
            public string ExpectedPacket;
            /// <summary>
            /// Name of initiating function that sent the original packet (for debugging purposes)
            /// </summary>
            public string Name;
            /// <summary>
            /// enumeration of the types of responses to expect after sending a packet
            /// </summary>
            public enum type
            {
                /// <summary>
                /// A full packet response with ACK (0x06) is expected
                /// </summary>
                FullPacketResponse,
                /// <summary>
                /// A packet containing telemetry or other information is expected
                /// </summary>
                DataResponse,
                /// <summary>
                /// no response is expected
                /// </summary>
                none
            }
            /// <summary>
            /// instance of the type enum
            /// </summary>
            public type ResponseType;
        }
        #endregion
        
        #region constructors

        /// <summary>
        /// Creates a new CommProtocol, with specified serial port parameters and a specified response timeout.  The other serial port parameters are all set to default
        /// </summary>
        /// <param name="PortName">the serial port name e.g. "COM1"</param>
        /// <param name="BaudRate">a valid serial port baud rate e.g. 9600</param>
        public CommProtocol(string PortName, int BaudRate)
        {
            try
            {
                SP = new SerialPort(PortName, BaudRate);
                SP.Encoding = encoding;
                SP.Open();
                SP.DataReceived += new SerialDataReceivedEventHandler(SP_DataReceived);
                Setup();
               
            }
            catch (Exception ex)
            {
                ClassExceptions.Add(ex);
            }
        }
        /// <summary>
        /// Creates a new CommProtocol, with specified serial port parameters and a specified response timeout
        /// </summary>
        /// <param name="PortName">the serial port name e.g. "COM1"</param>
        /// <param name="BaudRate">a valid serial port baud rate e.g. 9600</param>
        /// <param name="parity">a valid parity</param>
        /// <param name="DataBits">the number of data bits per frame</param>
        /// <param name="stopBits">the number of stop bits per frame</param>
        public CommProtocol(string PortName, int BaudRate, Parity parity, int DataBits, StopBits stopBits )
        {
            try
            {
                
                SP = new SerialPort(PortName, BaudRate, parity, DataBits, stopBits);
                SP.Encoding = encoding;
                SP.Open();
                SP.DataReceived += new SerialDataReceivedEventHandler(SP_DataReceived);
                Setup();
            }
            catch (Exception ex)
            {
                ClassExceptions.Add(ex);
            }
        }
        /// <summary>
        /// Creates a new CommProtocol, with specified serial port parameters and a specified response timeout
        /// </summary>
        /// <param name="PortName">the serial port name e.g. "COM1"</param>
        /// <param name="BaudRate">a valid serial port baud rate e.g. 9600</param>
        /// <param name="parity">a valid parity</param>
        /// <param name="DataBits">the number of data bits per frame</param>
        /// <param name="stopBits">the number of stop bits per frame</param>
        /// <param name="Timeout">Time to wait for a response after sending a data packet.  
        /// If the timer expires before the expected packet is recieved the OnResponseTimeout event is invoked.</param>
        public CommProtocol(string PortName, int BaudRate, Parity parity, int DataBits, StopBits stopBits, int Timeout)
        {
            try
            {

                SP = new SerialPort(PortName, BaudRate, parity, DataBits, stopBits);
                SP.Encoding = encoding;
                SP.Open();
                SP.DataReceived += new SerialDataReceivedEventHandler(SP_DataReceived);
                this.TimeOut = Timeout;
                Setup();
            }
            catch (Exception ex)
            {
                ClassExceptions.Add(ex);
            }
        }
        /// <summary>
        /// Called by all constructors
        /// </summary>
        private void Setup()
        {
            ResponseTimer = new System.Timers.Timer(TimeOut);
            ResponseTimer.Enabled = false;
            ResponseTimer.Elapsed += new ElapsedEventHandler(ResponseTimer_Elapsed);
        }
        #endregion
        
        #region out-going packets

        #region Communications
        /// <summary>
        /// Send out a ping to prove the helicopter is communicating
        /// </summary>
        public void CommsHandShakeInitiate()
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                OutGoingPacket = new byte[9];
                OutGoingPacket[0] = 0xA5;//header
                OutGoingPacket[1] = 0x5A;
                OutGoingPacket[2] = 0x02;//2 bytes
                OutGoingPacket[3] = 0x43;//communications command type
                OutGoingPacket[4] = 0x48;//handshake
                OutGoingPacket[5] = 0x00;//checksum high
                OutGoingPacket[6] = 0x8D;//checksum low
                OutGoingPacket[7] = 0xCC;
                OutGoingPacket[8] = 0x33;//footer
                SendPacket("CommsHandShakeInitiate", ExpectedResponses.type.DataResponse);
            }
        }
        /// <summary>
        /// Instruct the helicopter to stop sending acks at the end of the hand shake sequence
        /// </summary>
        public void CommsHandShakeTerminate()
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                OutGoingPacket = new byte[7];
                OutGoingPacket[0] = 0xA5;//header
                OutGoingPacket[1] = 0x5A;
                OutGoingPacket[2] = 0x02;//2 bytes
                OutGoingPacket[3] = 0x43;//communications command type
                OutGoingPacket[4] = 0x54;//terminate handshake
                OutGoingPacket[5] = 0x00;//checksum high
                OutGoingPacket[6] = 0x99;//checksum low
                OutGoingPacket[7] = 0xCC;
                OutGoingPacket[8] = 0x33;//footer
                SendPacket("CommsHandShakeTerminate", ExpectedResponses.type.none);//helicopter is not required to respond to this packet
            }
        }
        #endregion

        #region Testing and tuning commands
        /// <summary>
        /// Set the helicopter's motor's RPM
        /// </summary>
        /// <param name="RPM">A value between 0 and 100 representing percentage engine speed</param>
        public void SetMotorRPM(byte RPM)
        {
            if (ExpectedResponse.ResponseExpected == false)//dont send out a packet if a response is expected from the helicopter
            {
                if (RPM >= 0 && RPM <= 100)
                {
                    OutGoingPacket = new byte[10];
                    OutGoingPacket[0] = 0xA5;//Start of transmission 1
                    OutGoingPacket[1] = 0x5A;//Start of transmission 2
                    OutGoingPacket[2] = 0x03;//length: number of bytes in data portion
                    OutGoingPacket[3] = 0x54;//Command type �Testing/Tuning�
                    OutGoingPacket[4] = 0x45;//Command �Engine RPM adjust�
                    //value from 0x00 to 0x64 representing the desired percentage of engine speed.
                    OutGoingPacket[5] = RPM;
                    //Calculate checksum high byte
                    OutGoingPacket[6] = Convert.ToByte(((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + RPM) & 0xFF00) >> 8);
                    //Calculate checksum low byte
                    OutGoingPacket[7] = Convert.ToByte((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + RPM) & 0x00FF);
                    OutGoingPacket[8] = 0xCC;//End of transmission 1
                    OutGoingPacket[9] = 0x33;//End of transmission 2
                    SendPacket("SetMotorRPM",ExpectedResponses.type.FullPacketResponse);

                }
                else
                {
                    throw new Exception("CommProtocol Error: SetMotorRPM, specified RPM value '" + RPM +
                                        "' is out of range.  Use a value >= 0 and <= 100");
                }
            }
        }
        /// <summary>
        /// Set the cyclic pitch control from 0 to 100
        /// </summary>
        /// <param name="CyclicPitch">A value between 0 and 100 representing the servo angle</param>
        public void SetCyclicPitch(byte CyclicPitch)
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                if (CyclicPitch >= 0 && CyclicPitch <= 100)
                {
                    OutGoingPacket = new byte[10];
                    OutGoingPacket[0] = 0xA5;//Start of transmission 1
                    OutGoingPacket[1] = 0x5A;//Start of transmission 2
                    OutGoingPacket[2] = 0x03;//length: number of bytes in data portion
                    OutGoingPacket[3] = 0x54;//Command type �Testing/Tuning�
                    OutGoingPacket[4] = 0x50;//Command �Pitch Servo adjust�
                    //value from 0x00 to 0x64 representing the desired percentage of cyclic pitch.
                    OutGoingPacket[5] = CyclicPitch;
                    //Calculate checksum high byte
                    OutGoingPacket[6] = Convert.ToByte(((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + CyclicPitch) & 0xFF00) >> 8);
                    //Calculate checksum low byte
                    OutGoingPacket[7] = Convert.ToByte((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + CyclicPitch) & 0x00FF);
                    OutGoingPacket[8] = 0xCC;//End of transmission 1
                    OutGoingPacket[9] = 0x33;//End of transmission 2
                    SendPacket("SetCyclicPitch", ExpectedResponses.type.FullPacketResponse);
                }
                else
                {
                    throw new Exception("CommProtocol Error:  SetCyclicPitch, specified Cyclic Pitch value '" + CyclicPitch +
                        "' is out of range.  Use a value >= 0 and <= 100");
                }
            }
        }
        /// <summary>
        /// Set the cyclic roll control from 0 to 100
        /// </summary>
        /// <param name="CyclicRoll"></param>
        public void SetCyclicRoll(byte CyclicRoll)
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                if (CyclicRoll >= 0 && CyclicRoll <= 100)
                {
                    OutGoingPacket = new byte[10];
                    OutGoingPacket[0] = 0xA5;//Start of transmission 1
                    OutGoingPacket[1] = 0x5A;//Start of transmission 2
                    OutGoingPacket[2] = 0x03;//length: number of bytes in data portion
                    OutGoingPacket[3] = 0x54;//Command type �Testing/Tuning�
                    OutGoingPacket[4] = 0x52;//Command �Roll Servo adjust�
                    //value from 0x00 to 0x64 representing the desired percentage of cyclic roll.
                    OutGoingPacket[5] = CyclicRoll;
                    //Calculate checksum high byte
                    OutGoingPacket[6] = Convert.ToByte(((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + CyclicRoll) & 0xFF00) >> 8);
                    //Calculate checksum low byte
                    OutGoingPacket[7] = Convert.ToByte((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + CyclicRoll) & 0x00FF);
                    OutGoingPacket[8] = 0xCC;//End of transmission 1
                    OutGoingPacket[9] = 0x33;//End of transmission 2
                    SendPacket("SetCyclicRoll", ExpectedResponses.type.FullPacketResponse);
                }
                else
                {
                    throw new Exception("CommProtocol Error:  SetCyclicRoll, specified Cyclic Roll value '" + CyclicRoll +
                        "' is out of range.  Use a value >= 0 and <= 100");
                }
            }
        }
        /// <summary>
        /// Set the collective control from 0 to 100 percent
        /// </summary>
        /// <param name="Collective">percentage of collective</param>
        public void SetCollective(byte Collective)
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                if (Collective >= 0 && Collective <= 100)
                {
                    OutGoingPacket = new byte[10];
                    OutGoingPacket[0] = 0xA5;//Start of transmission 1
                    OutGoingPacket[1] = 0x5A;//Start of transmission 2
                    OutGoingPacket[2] = 0x03;//number of bytes
                    OutGoingPacket[3] = 0x54;//Command type �Testing/Tuning�
                    OutGoingPacket[4] = 0x43;//Command �Collective Servo adjust�
                    //value from 0x00 to 0x64 representing the desired percentage of collective.
                    OutGoingPacket[5] = Collective;
                    //Calculate checksum high byte
                    OutGoingPacket[6] = Convert.ToByte(((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + Collective) & 0xFF00) >> 8);
                    //Calculate checksum low byte
                    OutGoingPacket[7] = Convert.ToByte((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + Collective) & 0x00FF);
                    OutGoingPacket[8] = 0xCC;//End of transmission 1
                    OutGoingPacket[9] = 0x33;//End of transmission 2
                    SendPacket("SetCollective", ExpectedResponses.type.FullPacketResponse);
                }
                else
                {
                    throw new Exception("CommProtocol Error:  SetCollective, specified Collective value '" + Collective +
                        "' is out of range.  Use a value >= 0 and <= 100");
                }
            }
        }
        /// <summary>
        /// Set the anti-torque servo to the desired percentage
        /// </summary>
        /// <param name="AntiTorque">a percentage servo value</param>
        public void SetAntiTorque(byte AntiTorque)
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                if (AntiTorque >= 0 && AntiTorque <= 100)
                {
                    OutGoingPacket = new byte[10];
                    OutGoingPacket[0] = 0xA5;//Start of transmission 1
                    OutGoingPacket[1] = 0x5A;//Start of transmission 2
                    OutGoingPacket[2] = 0x03;//packet length
                    OutGoingPacket[3] = 0x54;//Command type �Testing/Tuning�
                    OutGoingPacket[4] = 0x51;//Command �Collective Servo adjust�
                    //value from 0x00 to 0x64 representing the desired percentage of collective.
                    OutGoingPacket[5] = AntiTorque;
                    //Calculate checksum high byte
                    OutGoingPacket[6] = Convert.ToByte(((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + AntiTorque) & 0xFF00) >> 8);
                    //Calculate checksum low byte
                    OutGoingPacket[7] = Convert.ToByte((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + AntiTorque) & 0x00FF);
                    OutGoingPacket[8] = 0xCC;//End of transmission 1
                    OutGoingPacket[9] = 0x33;//End of transmission 2
                    SendPacket("SetAntiTorque", ExpectedResponses.type.FullPacketResponse);
                }
                else
                {
                    throw new Exception("CommProtocol Error:  SetAntiTorque, specified AntiTorque value '" + AntiTorque +
                        "' is out of range.  Use a value >= 0 and <= 100");
                }
            }
        }
        /// <summary>
        /// Set tune points
        /// </summary>
        /// <param name="TuningPoint">Tuning set: 0x5A = Zero Points, 0x46 = 50% points, 0x4D = Max points</param>
        /// <param name="RPM">percentage RPM</param>
        /// <param name="PitchServo">percentage pitch servo</param>
        /// <param name="RollServo">percentage Roll servo</param>
        /// <param name="CollectiveServo">percentage collective servo</param>
        /// <param name="AntiTorqueServo">percentage anti-torque servo</param>
        public void SetTunePoints(byte TuningPoint, byte RPM, byte PitchServo, byte RollServo, byte CollectiveServo, byte AntiTorqueServo)
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                if (TuningPoint == 0x5A || TuningPoint == 0x46 || TuningPoint == 0x4D)
                {
                    OutGoingPacket = new byte[20];
                    OutGoingPacket[0] = 0xA5;//packet header
                    OutGoingPacket[1] = 0x08;
                    OutGoingPacket[2] = 0x30;//number of bytes in the data part
                    OutGoingPacket[3] = 0x54;//command type testing/tuning
                    OutGoingPacket[4] = TuningPoint;//packet command
                    OutGoingPacket[5] = RPM;
                    OutGoingPacket[6] = PitchServo;
                    OutGoingPacket[7] = RollServo;
                    OutGoingPacket[8] = CollectiveServo;
                    OutGoingPacket[9] = AntiTorqueServo;
                    OutGoingPacket[10] = 
                        (byte)(((
                        OutGoingPacket[2] +
                        OutGoingPacket[3] +
                        OutGoingPacket[4] +
                        OutGoingPacket[5] +
                        OutGoingPacket[6] +
                        OutGoingPacket[7] + 
                        OutGoingPacket[8] +
                        OutGoingPacket[9]
                        ) & 0xFF00) >> 8);//checksum high
                    OutGoingPacket[11] = 
                        (byte)((
                        OutGoingPacket[2] +
                        OutGoingPacket[3] +
                        OutGoingPacket[4] +
                        OutGoingPacket[5] +
                        OutGoingPacket[6] +
                        OutGoingPacket[7] +
                        OutGoingPacket[8] +
                        OutGoingPacket[9]
                        ) & 0x00FF); //checksum low
                    OutGoingPacket[12] = 0xCC;
                    OutGoingPacket[13] = 0x33;//packet footer
                    SendPacket("SetTunePoints", ExpectedResponses.type.FullPacketResponse);

                }
                else
                {
                    throw new Exception("SetTunePoints: Invalid tunining point specified must by one of: 0x5A, ox46 or 0x4D");
                }
            }
        }
        #endregion

        #region Flight operation commands
        /// <summary>
        /// Start the motor
        /// </summary>
        public void EngageEngine()
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                //packet formation: header(0xA55A, numbytes(0x02), command(0x4645), checksum(0x008D), footer(0xCC33)
                OutGoingPacket = new byte[]{ 0xA5, 0x5A, 0x02, 0x46, 0x45, 0x00, 0x8D, 0xCC, 0x33 };
                SendPacket("EngageEngine", ExpectedResponses.type.FullPacketResponse);
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
            if (ExpectedResponse.ResponseExpected == false)
            {
                if (mode == 0x50 || mode == 0x43)
                {
                    OutGoingPacket = new byte[10];
                    OutGoingPacket[0] = 0xA5;
                    OutGoingPacket[1] = 0x5A;//packet header
                    OutGoingPacket[2] = 0x03;//number bytes
                    OutGoingPacket[3] = 0x46;//Command type �Flight Ops�
                    OutGoingPacket[4] = 0x48;//Command �Hover�
                    OutGoingPacket[5] = mode;//Parameter set: 0x50 = Use preset altitude, 0x43 = hover at current altitude, 0x4D = use following altitude 
                    OutGoingPacket[6] = (byte)((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + mode) >> 8);//checksum high
                    OutGoingPacket[7] = (byte)((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + mode) & 0x00FF);//checksum low
                    OutGoingPacket[8] = 0xCC;
                    OutGoingPacket[9] = 0x33;//Footer
                    SendPacket("Hover", ExpectedResponses.type.FullPacketResponse);
                    
                }
                else if (mode == 0x4D)
                {
                    OutGoingPacket = new byte[12];
                    OutGoingPacket[0] = 0xA5;
                    OutGoingPacket[1] = 0x5A;//packet header
                    OutGoingPacket[2] = 0x05;//numbytes
                    OutGoingPacket[3] = 0x46;//Command type �Flight Ops�
                    OutGoingPacket[4] = 0x48;//Command �Hover�
                    OutGoingPacket[5] = mode;//Parameter set: 0x50 = Use preset altitude, 0x43 = hover at current altitude, 0x4D = use following altitude 
                    OutGoingPacket[6] = (byte)((Altitude & 0xFF00) >> 8);
                    OutGoingPacket[7] = (byte)(Altitude & 0x00FF);
                    OutGoingPacket[8] = (byte)((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + OutGoingPacket[5] + OutGoingPacket[6] + OutGoingPacket[7]) >> 8);//checksum high
                    OutGoingPacket[9] = (byte)((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + OutGoingPacket[5] + OutGoingPacket[6] + OutGoingPacket[7]) & 0x00FF);//checksum low
                    OutGoingPacket[10] = 0xCC;
                    OutGoingPacket[11] = 0x33;//Footer
                    SendPacket("Hover", ExpectedResponses.type.FullPacketResponse);                 

                }
                else
                {
                    throw new Exception("Hover(): Invalid mode argument. Must be either 0x50, 0x43 or 0x4D");
                }
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
           
            0x0F    number of bytes in data
            0x46	Command type �Flight Ops�
            0x47	Command �Go To�

          6 0xDD	Degrees (0-180) Latitude
            0xMM	Minutes (0-59) Latitude
            0xSSSS	Seconds(0-59) Latitude. Use upper six bits for integer portion of seconds, use remaining 10 bits for decimal portion of seconds. There are approximately 31 meters in one second of latitude (and slightly less in 1 minute of longitude at our distance from the equator). 31m / 2^10 = 3 cm accuracy, which is way more accuracy than the GPS will determine.
            0xNS	0x4E = North
	                0x53 = South

         11 0xDD	Degrees (0-180) Longitude
            0xMM	Minutes (0-59) Longitude
            0xSSSS	Seconds(0-59) Longitude. See above for more details.
            0xEW	0x45 = East
	                0x57 = West
            
         16 0xAA	Action on Arrival:
		            IF 0x48 Hover at given altitude ELSE IF 0x53 Circle at given altitude
            0xAAAA	Given altitude

         19 0xXX checksum High
            0xXX checksum Low
            0xCC
         22 0x33 footer
            */
            if (ExpectedResponse.ResponseExpected == false)
            {
                OutGoingPacket = new byte[22];
                OutGoingPacket[0] = 0xA5;
                OutGoingPacket[1] = 0x5A;
                OutGoingPacket[2] = 0x0F;//number of bytes
                OutGoingPacket[3] = 0x46;//Command type �Flight Ops�
                OutGoingPacket[4] = 0x47;
                OutGoingPacket[5] = Lat.Degrees;
                OutGoingPacket[6] = Lat.Minutes;
                //highest 2 bits of the first seconds packet were used for bits 9 and 10 of the secondsL value
                OutGoingPacket[7] = (byte)(Lat.SecondsH + (byte)((Lat.SecondsL & 0xFC00) >> 8));
                OutGoingPacket[8] = (byte)(Lat.SecondsL & 0x00FF);
                if (Lat.North)
                {
                    OutGoingPacket[9] = 0x4E;
                }
                else
                {
                    OutGoingPacket[9] = 0x53;
                }
                OutGoingPacket[10] = Long.Degrees;
                OutGoingPacket[11] = Long.Minutes;
                //highest 2 bits of the first seconds packet were used for bits 9 and 10 of the secondsL value
                OutGoingPacket[12] = (byte)(Long.SecondsH + (byte)((Long.SecondsL & 0xFC00) >> 8));
                OutGoingPacket[13] = (byte)(Long.SecondsL & 0x00FF);
                if (Long.East)
                {
                    OutGoingPacket[14] = 0x45;
                }
                else
                {
                    OutGoingPacket[14] = 0x57;
                }
                if (action == 0x48)
                {
                    OutGoingPacket[15] = 0x48;
                }
                else if (action == 0x53)
                {
                    OutGoingPacket[15] = 0x53;
                }
                else
                {
                    throw new Exception("CommProtocol.Goto(): invalid action specified.  Must be either 0x48 or 0x53");
                }
                OutGoingPacket[16] = (byte)((Altitude & 0xFF00) >> 8);//altitude high byte
                OutGoingPacket[17] = (byte)(Altitude & 0x00FF);//altitude low byte
                //calculate checksum
                short checksum = 0;
                for (int i = 2; i < 18; i++)//calculate checksum
                {
                    checksum += OutGoingPacket[i];
                }
                OutGoingPacket[18] = (byte)((checksum & 0xFF00) >> 8);//checksum high
                OutGoingPacket[19] = (byte)(checksum & 0x00FF);//checksum low
                OutGoingPacket[20] = 0xCC;
                OutGoingPacket[21] = 0x33;//footer

                SendPacket("Goto", ExpectedResponses.type.FullPacketResponse);

            }
        }
        /// <summary>
        /// instruct the helicpoter to return to its take off coordinate
        /// </summary>
        public void ReturnToBase()
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                //header(0xA55A), number bytes(0x02),command(0x4652),checksum(0x0098),footer(0xCC33)
                OutGoingPacket = new byte[] { 0xA5, 0x5A, 0x02, 0x46, 0x52, 0x00, 0x9A, 0xCC, 0x33 };
                SendPacket("ReturnToBase", ExpectedResponses.type.FullPacketResponse);
            }
        }
        /// <summary>
        /// Request a pre-flight packet, the helicopter should respond with a data packet
        /// </summary>
        public void RequestPreFlightPacket()
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                //header(0xA55A), number bytes(0x02),command(0x4650),checksum(0x0096),footer(0xCC33)
                OutGoingPacket = new byte[] { 0xA5, 0x5A, 0x02, 0x46, 0x50, 0x00, 0x98, 0xCC, 0x33 };
                SendPacket("RequestPreFlightPacket", ExpectedResponses.type.DataResponse);
            }
        }
        /// <summary>
        /// Move the helicopter a short distance the specified direction
        /// </summary>
        /// <param name="Direction">one of: 0x46(forward),0x52(reverse),0x50(Port) or 0x53(starboard)</param>
        public void DiscreetMovementControl(byte Direction)
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                if (Direction == 0x46 || Direction == 0x52 || Direction == 0x50 || Direction == 0x53)
                {
                    //header(0xA55A), number bytes(0x03),command(0x4650),direction(0xXX), checksum(0xXXXX),footer(0xCC33)
                    OutGoingPacket = new byte[10];
                    OutGoingPacket[0] = 0xA5;
                    OutGoingPacket[1] = 0x5A;
                    OutGoingPacket[2] = 0x03;
                    OutGoingPacket[3] = 0x46;
                    OutGoingPacket[4] = 0x50;
                    OutGoingPacket[5] = Direction;
                    OutGoingPacket[6] = (byte)(((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + OutGoingPacket[5]) & 0xFF00) >> 8);
                    OutGoingPacket[7] = (byte)((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + OutGoingPacket[5]) & 0x00FF);
                    OutGoingPacket[8] = 0xCC;
                    OutGoingPacket[9] = 0x33;
                    SendPacket("DiscreetMovementControl", ExpectedResponses.type.FullPacketResponse);
                }
                else
                {
                    throw new Exception(string.Format("Invalid direction arg: {0:X2} must be one of 0x46,0x52,0x50 or 0x53",Direction));
                }
            }
        }
        /// <summary>
        /// Request a data packet from the helicopter
        /// </summary>
        /// <param name="info">byte representing the desired information: 0x4C Location,
        /// 0x48 Heading/Speed/Altitude, 0x5A Attitude, 0x42 Battery Status</param>
        public void RequestForInformation(byte info)
        {
            if (ExpectedResponse.ResponseExpected == false)
            {
                if (info == 0x4C || info == 0x48 || info == 0x5A || info == 42)
                {
                    OutGoingPacket = new byte[10];
                    OutGoingPacket[0] = 0xA5;//packet header
                    OutGoingPacket[1] = 0x5A;
                    OutGoingPacket[2] = 0x05;//number of bytes
                    OutGoingPacket[3] = 0x46;//flight ops command
                    OutGoingPacket[4] = 0x49;//ROI
                    OutGoingPacket[5] = info;
                    OutGoingPacket[6] = (byte)(((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + OutGoingPacket[5]) & 0xFF00) >> 8);//checksum high
                    OutGoingPacket[7] = (byte)((OutGoingPacket[2] + OutGoingPacket[3] + OutGoingPacket[4] + OutGoingPacket[5]) & 0x00FF);//checksum low
                    OutGoingPacket[8] = 0xCC;
                    OutGoingPacket[9] = 0x33;
                    SendPacket("RequestForInformation", ExpectedResponses.type.DataResponse);
                }
            }
        }
        #endregion

        #endregion

        #region incoming packets

        private void SP_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            IncomingDataBuffer += SP.ReadExisting();
            if (ExpectedResponse.ResponseExpected == false)
            {
                //this means the helicopter has spoken without being asked to, probably an error message
                MatchIncomingPacket();
            }
            else if (ExpectedResponse.ResponseExpected == true && ExpectedResponse.ResponseType == ExpectedResponses.type.DataResponse)
            {
                //if WaitingForResponse is true then we are waiting for a response for a sent packet
                //otherwise it is probably a regular data packet

                //match the incoming packet against known data packets
                MatchIncomingPacket();
            }
            else if (ExpectedResponse.ResponseExpected == true && ExpectedResponse.ResponseType == ExpectedResponses.type.FullPacketResponse)
            {
                //a full packet response is requested
                if (IncomingDataBuffer == ExpectedResponse.ExpectedPacket)
                {
                    OnExpectedResponseRecieved(new ExpectedResponseRecievedEventArgs(ExpectedResponse.Name,IncomingDataBuffer));
                    //the response is successfully recieved
                }
                if (IncomingDataBuffer.Length >= ExpectedResponse.ExpectedPacket.Length)
                {
                    //throw the event
                    OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Recieved a bad packet response for command: " + ExpectedResponse.Name));
                    //the packet was bad dump it
                    ClearBuffer();
                }
            }
        }

        /// <summary>
        /// this is public only for debugging purposes
        /// </summary>
        public void MatchIncomingPacket()
        {
            if (IncomingDataBuffer.Length >= 5)//5 is the minimum size that contains the command 
            {
                if ((int)IncomingDataBuffer[0] == 0xA5 && (int)IncomingDataBuffer[1] == 0x5A)//packet header
                {
                    if ((int)IncomingDataBuffer[3] == 0x74)//telemetry command
                    {
                        if ((int)IncomingDataBuffer[4] == 0x4C)//location command
                        {
                            ParseLocationPacket();
                        }
                        else if ((int)IncomingDataBuffer[4] == 0x48)//heading/speed/altitude command
                        {
                            ParseHeadingSpeedAltitudePacket();
                        }
                        else if ((int)IncomingDataBuffer[4] == 0x5A)//attitude command
                        {
                            ParseAttitudePacket();
                        }
                        else if ((int)IncomingDataBuffer[4] == 0x42)//battery status
                        {
                            ParseBatteryStatusPacket();
                        }
                        else if ((int)IncomingDataBuffer[4] == 0x45)//onboard error
                        {
                            ParseOnBoardErrorPacket();
                        }
                        else if ((int)IncomingDataBuffer[4] == 0x50)//pre-flight packet
                        {
                            ParsePreFlightPacket();
                        }
                        else
                        {
                            OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Invalid packet: unknown telemetry command"));
                            ClearBuffer();
                        }
                    }
                    else if ((int)IncomingDataBuffer[3] == 0x43 && (int)IncomingDataBuffer[4] == 0x06)//comms handshake, ack recieved
                    {
                        ParseCommHandShakeAck();
                    }
                    else
                    {
                        OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Invalid packet: unknown command"));
                        ClearBuffer();
                    }
                }
                else
                {
                    OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Invalid packet: no packet header"));
                    ClearBuffer();
                }
            }
        }

       
        #region Location Packet recieved
        private void ParseLocationPacket()
        {
            /*
            Location Packet:
              1 0xA5    SOT1
                0x5A    SOT2
              
              3 0x0B    bytes
                0x74	Report type �Telemetry�
              5 0x4C	Report �Location�

                0xDD	Degrees (0-180) Latitude
                0xMM	Minutes (0-59) Latitude
                0xSSSS	Seconds (0-59) Latitude. Use first six bits for integer portion of seconds, use remaining 10 bits for decimal portion of seconds. There are approximately 31 meters in one second of latitude (and slightly less in 1 minute of longitude at our distance from the equator). 31m / 2^10 = 3 cm accuracy, which is way more accuracy than the GPS will determine.
             10 0xNS	0x4E = North or 0x53 = South

                0xDD	Degrees (0-180) Longitude
                0xMM	Minutes (0-59) Longitude
                0xSSSS	Seconds (0-59) Longitude. See above for more details.
             15 0xEW	0x45 = East or 0x57 = West
                    
                0xXX    Checksum 1
                0xXX    Checksum 2
                    
                0xCC    EOT1
             19 0x33    EOT2
            */
            if (IncomingDataBuffer.Length == 18 && (int)IncomingDataBuffer[17] == 0xCC && (int)IncomingDataBuffer[18] == 0x33)
            {
                //calculate checksum
                UInt16 sum = 0;
                for (int i = 3; i < 15; i++)
                {
                    sum += (ushort)IncomingDataBuffer[i];
                }
                byte chk1 = (byte)((sum & 0x0000FF00) >> 8);
                byte chk2 = (byte)(sum & 0x000000FF);
                if (chk1 != (int)IncomingDataBuffer[15] || chk2 != (int)IncomingDataBuffer[16])
                {
                    OnBadPacketReceived(
                        new BadPacketReceivedEventArgs(IncomingDataBuffer,
                        string.Format("Invalid Location packet: invalid checksum recieved: {0:x4}, expected: {1:x4}",
                        (Convert.ToUInt16((int)IncomingDataBuffer[15]) << 8) + (int)IncomingDataBuffer[16], sum))
                        );
                    ClearBuffer();
                }
                else
                {
                    //checksum ok
                    Latitude Lat = new Latitude();
                    Lat.Degrees = (byte)IncomingDataBuffer[5];
                    Lat.Minutes = (byte)IncomingDataBuffer[6];
                    Lat.SecondsH = (byte)((int)IncomingDataBuffer[7] & 0x00FC);//upper 6 bits for seconds
                    Lat.SecondsL = Convert.ToUInt16(((byte)IncomingDataBuffer[7] & 0x03) << 10 + Convert.ToUInt16((int)IncomingDataBuffer[8]));//10 bit decimal portion of seconds
                    if ((byte)IncomingDataBuffer[9] == 0x4E)
                    {
                        Lat.North = true;
                    }
                    else if ((byte)IncomingDataBuffer[9] == 0x4E)
                    {
                        Lat.North = false;
                    }
                    else
                    {
                        OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Invalid packet: The north south byte of the Latitude in a recieved location packet is invalid."));
                        ClearBuffer();
                        return;
                    }

                    Longitude Long = new Longitude();
                    Long.Degrees = (byte)IncomingDataBuffer[10];
                    Long.Minutes = (byte)IncomingDataBuffer[11];
                    Long.SecondsH = (byte)((int)IncomingDataBuffer[12] & 0x00FC);//upper 6 bits for seconds
                    Long.SecondsL = Convert.ToUInt16(((byte)IncomingDataBuffer[12] & 0x03) << 10 + Convert.ToUInt16((int)IncomingDataBuffer[13]));//10 bit decimal portion of seconds
                    if ((byte)IncomingDataBuffer[14] == 0x45)
                    {
                        Long.East = true;
                    }
                    else if ((byte)IncomingDataBuffer[14] == 0x57)
                    {
                        Long.East = false;
                    }
                    else
                    {
                        OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Invalid packet: The east west byte of the Longitude in a recieved location packet is invalid."));
                        ClearBuffer();
                        return;
                    }
                    //invoke the event
                    OnLocationPacketRecieved(new LocationPacketRecievedEventArgs(Lat, Long));
                    ClearBuffer();
                }
            }
            if (IncomingDataBuffer.Length >= 18)
            {
                OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Bad location packet recieved"));
                ClearBuffer();
            }
        }
        #endregion
       
        #region Heading Speed altitude recieved
        private void ParseHeadingSpeedAltitudePacket()
        {
            /*
                Heading/Speed/Altitude:
              1 0xA5    SOT1
                0x5A    SOT2

              3 0x07    Number of bytes
             
                0x74	Report type �Telemetry�
                0x48	Report �Heading, Speed, and Altitude�

              6 0xHHHH	Hex value (0x0000 � 0x0167) representing current heading. Can potentially use extra bits for partial degrees.

              8 0xSS	Hex value representing helicopter speed (m/s)

              9 0xAAAA	2-byte value representing altitude in meters
                    
             11 0xXX    Checksum 1
                0xXX    Checksum 2
                    
             13 0xCC    EOT1
             14 0x33    EOT2
             */
            if (IncomingDataBuffer.Length == 14 && (int)IncomingDataBuffer[12] == 0xCC && (int)IncomingDataBuffer[13] == 0x33)
            {
                //calculate checksum
                UInt16 sum = 0;
                for (int i = 3; i < 10; i++)
                {
                    sum += (ushort)IncomingDataBuffer[i];
                }
                byte chk1 = (byte)((sum & 0x0000FF00) >> 8);
                byte chk2 = (byte)(sum & 0x000000FF);
                if (chk1 != (int)IncomingDataBuffer[10] || chk2 != (int)IncomingDataBuffer[11])
                {
                    OnBadPacketReceived(
                       new BadPacketReceivedEventArgs(IncomingDataBuffer,
                       string.Format("Invalid heading speed altitude packet: invalid checksum: recieved {0:x4}, expected {1:x4}",
                       (Convert.ToUInt16((int)IncomingDataBuffer[10]) << 8) + (int)IncomingDataBuffer[11], sum))
                       );
                    ClearBuffer();
                }
                else
                {
                    //good checksum
                    HeadingSpeedAltitude HSA = new HeadingSpeedAltitude();
                    HSA.Heading = Convert.ToUInt16((int)IncomingDataBuffer[5] << 8 + (int)IncomingDataBuffer[6]);
                    HSA.Speed = Convert.ToByte((int)IncomingDataBuffer[7]);
                    HSA.Altitude = Convert.ToUInt16((int)IncomingDataBuffer[8] << 8 + (int)IncomingDataBuffer[9]);
                    //invoke the event
                    OnHeadingSpeedAltitudePacketRecieved(new HeadingSpeedAltitudePacketRecievedEventArgs(HSA));
                    ClearBuffer();
                }
            }
            else if (IncomingDataBuffer.Length >= 14)
            {
                OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Bad heading/speed/altitude packetrecieved"));
                ClearBuffer();
            }
        }
        #endregion
        
        #region Attitude packet received
        private void ParseAttitudePacket()
        {
            /*
               Attitude
                
               1 0xA5
                 0x5A    Packet Header
                
               3 0x08   bytes 
               
               4 0x74	Report type �Telemetry�
                 0x5A	Report �Attitude�

               6 0xRRRR	Hex value (0x0000 � 0x0167) representing current roll angle, can potentially use extra bits for partial degrees 
                 (increases as helicopter rolls in the starboard direction)
                 
               8 0xPPPP	Hex value (0x0000 � 0x0167) representing current pitch angle, can potentially use extra bits for partial degrees 
                 (increases as helicopter pitches forward)
                
              10 0xYYYY	Hex value (0x0000 � 0x0167) representing current yaw angle, can potentially use extra bits for partial degrees 
                 (increases as helicopter rotates counter clockwise, as seen from above the helicopter)

              12 0xXX //checksum high
                 0xXX //checksum low
                
              14 0xCC
              15 0x33 //footer
               */
            if (IncomingDataBuffer.Length == 14 && (int)IncomingDataBuffer[13] == 0xCC && (int)IncomingDataBuffer[14] == 0x33)
            {
                //calculate checksum
                UInt16 sum = 0;
                for (int i = 3; i < 11; i++)
                {
                    sum += (ushort)IncomingDataBuffer[i];
                }
                byte chk1 = (byte)((sum & 0xFF00) >> 8);
                byte chk2 = (byte)(sum & 0x00FF);
                if (chk1 != (int)IncomingDataBuffer[11] || chk2 != (int)IncomingDataBuffer[12])
                {
                    OnBadPacketReceived(
                       new BadPacketReceivedEventArgs(IncomingDataBuffer,
                       string.Format("Invalid attitude packet: invalid checksum: recieved {0:x4}, expected {1:x4}",
                       (Convert.ToUInt16((int)IncomingDataBuffer[11]) << 8) + (int)IncomingDataBuffer[12], sum))
                       );
                    ClearBuffer();
                }
                else
                {
                    //checksum ok
                    Attitude a = new Attitude();
                    a.Roll = (ushort)(((int)IncomingDataBuffer[5] << 8) + (int)IncomingDataBuffer[6]);
                    a.Pitch = (ushort)(((int)IncomingDataBuffer[7] << 8) + (int)IncomingDataBuffer[8]);
                    a.Yaw = (ushort)(((int)IncomingDataBuffer[9] << 8) + (int)IncomingDataBuffer[10]);
                    //invoke the event
                    OnAttitudePacketRecieved(new AttitudePacketRecievedEventArgs(a)); 
                    ClearBuffer();
                }
            }
            else if (IncomingDataBuffer.Length >= 14)
            {
                OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer,"Bad attitude packet recieved"));
                ClearBuffer();
            }


        }
        #endregion

        #region Battery status packet recieved
        private void ParseBatteryStatusPacket()
        {
            /*
           Battery Status
                        
           1 0xA5
             0x5A    Packet Header
                        
           3 0x08   bytes 
                       
           4 0x74	Report type �Telemetry�
             0x5A	Report �Attitude�

           6 0xVVVV	Hex Value representing battery voltage
                         
           8 0xIIII	Hex Value representing helicopter current draw
                        
          10 0xTTTT	Hex Value representing battery temperature

          12 0xXX //checksum high
             0xXX //checksum low
                        
          14 0xCC
          15 0x33 //footer
           */
            if (IncomingDataBuffer.Length == 14 && (int)IncomingDataBuffer[13] == 0xCC && (int)IncomingDataBuffer[14] == 0x33)
            {
                //calculate checksum
                UInt16 sum = 0;
                for (int i = 3; i < 11; i++)
                {
                    sum += (ushort)IncomingDataBuffer[i];
                }
                byte chk1 = (byte)((sum & 0xFF00) >> 8);
                byte chk2 = (byte)(sum & 0x00FF);
                if (chk1 != (int)IncomingDataBuffer[11] || chk2 != (int)IncomingDataBuffer[12])
                {
                    OnBadPacketReceived(
                       new BadPacketReceivedEventArgs(IncomingDataBuffer,
                       string.Format("Invalid battery status packet: invalid checksum: recieved {0:x4}, expected {1:x4}",
                       (Convert.ToUInt16((int)IncomingDataBuffer[11]) << 8) + (int)IncomingDataBuffer[12], sum))
                       );
                    ClearBuffer();
                }
                else
                {
                    //checksum ok
                    BatteryStatus b = new BatteryStatus();
                    b.Voltage = (ushort)(((int)IncomingDataBuffer[5] << 8) + (int)IncomingDataBuffer[6]);
                    b.CurrentDraw = (ushort)(((int)IncomingDataBuffer[7] << 8) + (int)IncomingDataBuffer[8]);
                    b.Temperature = (ushort)(((int)IncomingDataBuffer[9] << 8) + (int)IncomingDataBuffer[10]);
                    //invoke the event
                    OnBatteryStatusPacketRecieved(new BatteryStatusPacketRecievedEventArgs(b));
                    ClearBuffer();
                }
            }
            else if (IncomingDataBuffer.Length >= 14)
            {
                OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Bad battery status packet recieved"));
                ClearBuffer();
            }
        }
        #endregion

        #region on board error packet received
        private void ParseOnBoardErrorPacket()
        {
            /*
           On board error
                        
           1 0xA5
             0x5A    Packet Header
                        
           3 0x03   bytes 
                       
           4 0x74	Report type �Telemetry�
             0x45	Report �Attitude�

           6 0xEE   Error code

           7 0xXX   checksum high
             0xXX   checksum low
                        
           9 0xCC
          10 0x33   footer
           */
            if (IncomingDataBuffer.Length == 10 && (int)IncomingDataBuffer[8] == 0xCC && (int)IncomingDataBuffer[9] == 0x33)
            {
                //calculate checksum
                UInt16 sum = 0;
                for (int i = 3; i < 6; i++)
                {
                    sum += (ushort)IncomingDataBuffer[i];
                }
                byte chk1 = (byte)((sum & 0xFF00) >> 8);
                byte chk2 = (byte)(sum & 0x00FF);
                if (chk1 != (int)IncomingDataBuffer[6] || chk2 != (int)IncomingDataBuffer[7])
                {
                    OnBadPacketReceived(
                       new BadPacketReceivedEventArgs(IncomingDataBuffer,
                       string.Format("Invalid on board error packet: invalid checksum: recieved {0:x4}, expected {1:x4}",
                       (((int)IncomingDataBuffer[6]) << 8) + (int)IncomingDataBuffer[7], sum))
                       );
                    ClearBuffer();
                }
                else
                {
                    //checksum ok
                    //invoke the event
                    OnBoardErrorPacketRecieved(this, new OnBoardErrorPacketRecievedEventArgs((byte)IncomingDataBuffer[5]));
                    ClearBuffer();
                }
            }
            else if (IncomingDataBuffer.Length >= 10)
            {
                OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Bad on board error packet recieved"));
                ClearBuffer();
            }
        }
        #endregion

        #region pre-flight packet recieved
        private void ParsePreFlightPacket()
        {
            /*
           Pre flight packet
                        
           1 0xA5
             0x5A    Packet Header
                        
           3 0x14   bytes 
                       
           4 0x74	Report type �Telemetry�
             0x5A	Report �Attitude�
             
           6 0xDD	Degrees (0-180) Latitude
             0xMM	Minutes (0-59) Latitude
             0xSSSS	Seconds (0-59) Latitude. 
          10 0xNS	0x4E = North
	                0x53 = South

          11 0xDD	Degrees (0-180) Longitude
             0xMM	Minutes (0-59) Longitude
             0xSSSS	Seconds (0-59) Longitude. See above for more details.
          15 0xEW	0x45 = East
	                0x57 = West

          16 0xAAAA	Altitude reported by GPS

          18 0xSSSS	Altitude reported by Sonar

          20 0xVVVV	Battery Voltage

          22 0xTTTT	Battery Temperature

          23 0xSS	8-bit bit-field representing sensor status

          24 0xXX   checksum high
             0xXX   checksum low
                        
          26 0xCC
          27 0x33   footer
           */
            if (IncomingDataBuffer.Length == 27 && (int)IncomingDataBuffer[25] == 0xCC && (int)IncomingDataBuffer[26] == 0x33)
            {
                //calculate checksum
                UInt16 sum = 0;
                for (int i = 3; i < 23; i++)
                {
                    sum += (ushort)IncomingDataBuffer[i];
                }
                byte chk1 = (byte)((sum & 0xFF00) >> 8);
                byte chk2 = (byte)(sum & 0x00FF);
                if (chk1 != (int)IncomingDataBuffer[23] || chk2 != (int)IncomingDataBuffer[24])
                {
                    OnBadPacketReceived(
                       new BadPacketReceivedEventArgs(IncomingDataBuffer,
                       string.Format("Invalid battery status packet: invalid checksum: recieved {0:x4}, expected {1:x4}",
                       (Convert.ToUInt16((int)IncomingDataBuffer[23]) << 8) + (int)IncomingDataBuffer[24], sum))
                       );
                    ClearBuffer();
                }
                else
                {
                    //checksum ok
                    PreFlightPacketData PFP = new PreFlightPacketData();
                    PFP.Lat = new Latitude();
                    PFP.Lat.Degrees = (byte)IncomingDataBuffer[5];
                    PFP.Lat.Minutes = (byte)IncomingDataBuffer[6];
                    PFP.Lat.SecondsH = (byte)((int)IncomingDataBuffer[7] & 0x00FC);//upper 6 bits for seconds
                    PFP.Lat.SecondsL = Convert.ToUInt16(((int)IncomingDataBuffer[7] & 0x03) << 10 + (int)IncomingDataBuffer[8]);//10 bit decimal portion of seconds
                    if ((byte)IncomingDataBuffer[9] == 0x45)
                    {
                        PFP.Lat.North = true;
                    }
                    else if ((byte)IncomingDataBuffer[9] == 0x57)
                    {
                        PFP.Lat.North = false;
                    }
                    else
                    {
                        OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Invalid packet: The north south byte of the Latitude in a recieved pre-flight packet is invalid."));
                        ClearBuffer();
                        return;
                    }
                    PFP.Long = new Longitude();
                    PFP.Long.Degrees = (byte)IncomingDataBuffer[10];
                    PFP.Long.Minutes = (byte)IncomingDataBuffer[11];
                    PFP.Long.SecondsH = (byte)((int)IncomingDataBuffer[12] & 0x00FC);//upper 6 bits for seconds
                    PFP.Long.SecondsL = Convert.ToUInt16(((int)IncomingDataBuffer[12] & 0x03) << 10 + (int)IncomingDataBuffer[13]);//10 bit decimal portion of seconds
                    if ((byte)IncomingDataBuffer[14] == 0x4E)
                    {
                        PFP.Long.East = true;
                    }
                    else if ((byte)IncomingDataBuffer[14] == 0x4E)
                    {
                        PFP.Long.East = false;
                    }
                    else
                    {
                        OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Invalid packet: The east west byte of the Latitude in a recieved pre-flight packet is invalid."));
                        ClearBuffer();
                        return;
                    }
                    PFP.GPSAltitude = (ushort)(((int)IncomingDataBuffer[15]<<8) + (int)IncomingDataBuffer[16]);
                    PFP.SonarAltitude = (ushort)(((int)IncomingDataBuffer[17] << 8) + (int)IncomingDataBuffer[18]);
                    PFP.BatteryVoltage = (ushort)(((int)IncomingDataBuffer[19] << 8) + (int)IncomingDataBuffer[20]);
                    PFP.BatteryTemp = (ushort)(((int)IncomingDataBuffer[21] << 8) + (int)IncomingDataBuffer[22]);
                    PFP.SensorStatus = (byte)IncomingDataBuffer[23];
                    //invoke the event
                    OnPreFlightPacketRecieved(new PreFlightPacketRecievedEventArgs(PFP));
                    ClearBuffer();
                }
            }
            else if (IncomingDataBuffer.Length >= 27)
            {
                OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer, "Bad pre flight packet recieved"));
                ClearBuffer();
            }
        }
        #endregion

        #region communication handshake "ACK" recieved
        private void ParseCommHandShakeAck()
        {
            /*
           Comm handshake ack reply
                        
           1 0xA5
             0x5A    Packet Header
             
             0x02   2 bytes           
           4 0x43	Command type �Communications�
             0x06	ACK

           6 0xXX //checksum high
             0xXX //checksum low
                        
           8 0xCC
           9 0x33 //footer
           */
            if (IncomingDataBuffer.Length == 9 && (int)IncomingDataBuffer[7] == 0xCC && (int)IncomingDataBuffer[8] == 0x33)
            {               
                //calculate checksum
                UInt16 sum = 0;
                for (int i = 3; i < 10; i++)
                {
                    sum += (ushort)IncomingDataBuffer[i];
                }
                byte chk1 = (byte)((sum & 0xFF00) >> 8);
                byte chk2 = (byte)(sum & 0x00FF);
                if (chk1 != (int)IncomingDataBuffer[10] || chk2 != (int)IncomingDataBuffer[11])
                {
                    OnBadPacketReceived(
                       new BadPacketReceivedEventArgs(IncomingDataBuffer,
                       string.Format("Invalid attitude packet: invalid checksum: recieved {0:x4}, expected {1:x4}",
                       (Convert.ToUInt16((int)IncomingDataBuffer[10]) << 8) + (int)IncomingDataBuffer[11], sum))
                       );
                    ClearBuffer();
                }
                else
                {
                    //checksum ok
                    OnHandShakeAckRecieved(new EventArgs());
                    ClearBuffer();
                }
            }
            else if (IncomingDataBuffer.Length >= 9)
            {
                OnBadPacketReceived(new BadPacketReceivedEventArgs(IncomingDataBuffer,"Bad hand shake acknowledgement packet received"));
                ClearBuffer();
            }
        }
        #endregion

        #endregion

        #region Private class methods

        /// <summary>
        /// As the protocol specifies, the helicopter responds with non data requesting packets with a copy of that packet (with the ACK inserted).  
        /// This method builds the packet that should be replied by the helicopter for comparison
        /// </summary>
        /// <param name="OutgoingPacket">The Packet sent by the base station</param>
        /// <returns>the expected response by the helicopter</returns>
        private string BuildExpectedResponse(byte[] OutgoingPacket)
        {
            string ExpectedResponse = "";
            ExpectedResponse += 0xA5;//packet header high
            ExpectedResponse += 0x5A;//packet header low
            ExpectedResponse += OutGoingPacket[2];//length byte
            ExpectedResponse += 0x06;//ack
            for (int j = 4; j < OutGoingPacket.Length - 1; j++)
            {
                ExpectedResponse += OutGoingPacket[j - 1];//copy the sent packet to match with the recieved packet
            }
            return ExpectedResponse;
        }
        private void SendPacket(string Name, ExpectedResponses.type type)
        {
            if (type == ExpectedResponses.type.FullPacketResponse)
            {

                ExpectedResponse.ExpectedPacket = BuildExpectedResponse(OutGoingPacket);
                ExpectedResponse.ResponseExpected = true;
                ExpectedResponse.Name = Name;
                ExpectedResponse.ResponseType = type;
                SP.Write(OutGoingPacket, 0, OutGoingPacket.Length);
                ResponseTimer.Start();
            }
            else if (type == ExpectedResponses.type.DataResponse)
            {
                ExpectedResponse.ExpectedPacket = "UNDEFINED";//a data packet is expected, we can't define it here
                ExpectedResponse.ResponseExpected = true;
                ExpectedResponse.Name = Name;
                ExpectedResponse.ResponseType = type;
                SP.Write(OutGoingPacket, 0, OutGoingPacket.Length);
                ResponseTimer.Start();
            }
            else if (type == ExpectedResponses.type.none)
            {
                SP.Write(OutGoingPacket, 0, OutGoingPacket.Length);//no response is expected, just write the packet
            }

        }
        private void ResponseTimer_Elapsed(Object Sender, EventArgs e)
        {
            if (ExpectedResponse.ResponseExpected)
            {
                ResponseTimer.Stop();
                //this means a response has timed out
                //invoke the timeout event
                OnResponseTimeout(new ResponseTimeoutEventArgs(ExpectedResponse.Name));
                ClearBuffer();
                
            }
        }
        private void ClearBuffer()
        {
            IncomingDataBuffer = "";
            ExpectedResponse.ResponseExpected = false;
        }
        #endregion

        #region custom event code

        #region location Packet recieved
        public delegate void LocationPacketRecievedEventHandler(object sender, LocationPacketRecievedEventArgs e);
        public event LocationPacketRecievedEventHandler LocationPacketRecieved;
        /// <summary>
        /// Raised when a location packet is recieved by the base station
        /// </summary>
        /// <param name="e">This parameter stores the location data.</param>
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

        #region Hand shake ack recieved

        public delegate void HandShakeAckRecievedEventHandler(object sender, EventArgs e);
        public event HandShakeAckRecievedEventHandler HandShakeAckRecieved;
        protected virtual void OnHandShakeAckRecieved(EventArgs e)
        {
            if (HandShakeAckRecieved != null)
            {
                HandShakeAckRecieved(this, e);
            }
        }
        #endregion

        #region bad packet recieved
        public delegate void BadPacketRecievedEventHandler(object sender, BadPacketReceivedEventArgs e);
        /// <summary>
        /// Thrown when a bad packet is recieved, see BadPacketRecievedEventArgs.ErrorMessage for the details
        /// </summary>
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

        #region response timed out
        public delegate void ResponseTimeoutEventHandler(object sender, ResponseTimeoutEventArgs e);
        /// <summary>
        /// This event is invoked when a command was sent to the helicopter 
        /// and there was not a correct repsonse as defined in the protocol
        /// </summary>
        public event ResponseTimeoutEventHandler ResponseTimeout;
        protected virtual void OnResponseTimeout(ResponseTimeoutEventArgs e)
        {
            if (ResponseTimeout != null)
            {
                ResponseTimeout(this, e);
            }
        }

        public class ResponseTimeoutEventArgs : EventArgs
        {
            /// <summary>
            /// Name of the function that inititated the transaction, and for which the response was expected.
            /// </summary>
            public string Name;
            public ResponseTimeoutEventArgs(string Name)
            {
                this.Name = Name;
            }
        }
 
        #endregion

        #region battery status packet recieved
        public delegate void BatteryStatusPacketRecievedEventHandler(object sender, BatteryStatusPacketRecievedEventArgs e);
        public event BatteryStatusPacketRecievedEventHandler BatteryStatusPacketRecieved;
        protected virtual void OnBatteryStatusPacketRecieved(BatteryStatusPacketRecievedEventArgs e)
        {
            if (BatteryStatusPacketRecieved != null)
            {
                BatteryStatusPacketRecieved(this, e);
            }
        }
        public class BatteryStatusPacketRecievedEventArgs : EventArgs
        {
            public BatteryStatus BattStat;

            public BatteryStatusPacketRecievedEventArgs(BatteryStatus BattStat)
            {
                this.BattStat = BattStat;
            }
        }
        #endregion

        #region on board errror packet recieved
        public delegate void OnBoardErrorPacketRecievedEventHandler(object sender, OnBoardErrorPacketRecievedEventArgs e);
        public event OnBoardErrorPacketRecievedEventHandler OnBoardErrorPacketRecieved;
        protected virtual void OnOnBoardErrorPacketRecieved(OnBoardErrorPacketRecievedEventArgs e)
        {
            if (OnBoardErrorPacketRecieved != null)
            {
                OnBoardErrorPacketRecieved(this, e);
            }
        }

        public class OnBoardErrorPacketRecievedEventArgs : EventArgs
        {
            public byte ErrorCode;

            public OnBoardErrorPacketRecievedEventArgs(byte ErrorCode)
            {
                this.ErrorCode = ErrorCode;
            }
        }
        #endregion

        #region pre-flight packet recieved
        public delegate void PreFlightPacketRecievedEventHandler(object sender, PreFlightPacketRecievedEventArgs e);
        public event PreFlightPacketRecievedEventHandler PreFlightPacketRecieved;
        protected virtual void OnPreFlightPacketRecieved(PreFlightPacketRecievedEventArgs e)
        {
            if (PreFlightPacketRecieved != null)
            {
                PreFlightPacketRecieved(this, e);
            }
        }

        public class PreFlightPacketRecievedEventArgs : EventArgs
        {
            public PreFlightPacketData PFP;

            public PreFlightPacketRecievedEventArgs(PreFlightPacketData PFP)
            {
                this.PFP = PFP;
            }
        }

        #endregion

        #region Expected response recieved
        public delegate void ExpectedResponseRecievedEventHandler(object sender, ExpectedResponseRecievedEventArgs e);
        public event ExpectedResponseRecievedEventHandler ExpectedResponseRecieved;
        protected virtual void OnExpectedResponseRecieved(ExpectedResponseRecievedEventArgs e)
        {
            if (ExpectedResponseRecieved != null)
            {
                ExpectedResponseRecieved(this, e);
            }

        }

        public class ExpectedResponseRecievedEventArgs : EventArgs
        {
            public string RecievedPacket;
            public string Name;

            public ExpectedResponseRecievedEventArgs(string Name, string RecievedPacket)
            {
                this.RecievedPacket = RecievedPacket;
                this.Name = Name;
            }


        }
        #endregion
        #endregion
    }
}
