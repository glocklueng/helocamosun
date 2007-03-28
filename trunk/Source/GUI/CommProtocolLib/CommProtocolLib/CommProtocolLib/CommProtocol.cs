using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.Timers;

namespace CommProtocolLib
{
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
            if (IncomingDataBuffer.Length >= 2)//start of transmission bytes are 0 and 1
            {
                if (IncomingDataBuffer[0] == 0xA5 && IncomingDataBuffer[1] == 0x5A)
                {
                    if (IncomingDataBuffer.Length >= 4)//command bytes are 2 and 3
                    {
                        if (IncomingDataBuffer[3] == 0x74)//telemetry
                        {
                            if (IncomingDataBuffer.Length >= 6)
                            {
                                if (IncomingDataBuffer[4] == 0x4C)//Report “Location”
                                {

                                }
                                else if (IncomingDataBuffer[4] == 0x48)//Report “Heading, Speed, and Altitude”
                                {

                                }
                            }
                        }
                        else
                        {
                            IncomingDataBuffer = "";//garbage data, blank the buffer
                            throw new Exception("Received unknown command from helicopter: " + string.Format("0:X2", IncomingDataBuffer[3]));
                        }
                    }
                }
                else
                {
                    IncomingDataBuffer = "";//garbage data, blank the buffer
                    throw new Exception("Received unknown packet header from helicopter: byte 1:" + string.Format("0:X2", IncomingDataBuffer[3]) + "byte2: " + string.Format("0:X", IncomingDataBuffer[3]));
                }
            }
        }
    }
}
