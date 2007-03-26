using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;

namespace CommProtocolLib
{
    public class CommProtocol
    {
        ArrayList ClassExceptions = new ArrayList();
        SerialPort SP;
        public CommProtocol(string PortName, int BaudRate)
        {
            try
            {
                SP = new SerialPort(PortName, BaudRate);
                SP.Open();
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
            }
            catch (Exception ex)
            {
                ClassExceptions.Add(ex);
            }
        }
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
                throw new Exception("CommProtocol Error:  SendTTCEngineRPM, specified RPM value '" + RPM +
                                    "' is out of range.  Use a value >= 0 and <= 100");
            }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="PitchCyclic"></param>
        public void SetCyclicPitch(int CyclicPitch)
        {


        }
    }
}
