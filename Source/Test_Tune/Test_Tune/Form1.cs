using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using CommProtocolLib;



namespace Test_Tune
{
    public struct CommSettings
    {
        public string PortName;
        public Int32 BaudRate;
    }

    public partial class Form1 : Form
    {
        

        CommSettings Settings;
        CommProtocol SP;
        public Form1()
        {
            
            
            InitializeComponent();
            ckbTXPackets.Checked = true;
            txtColVal.Text = "0";
            txtEngineSpeed.Text = "0";
            txtPitchVal.Text = "0";
            txtRollVal.Text = "0";
            txtYawVal.Text = "0";

            txtFBCurrent.Text = "UNK";
            txtFBTemp.Text = "UNK";
            txtFBVoltage.Text = "UNK";
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void pitchServo_Scroll(object sender, EventArgs e)
        {
            txtPitchVal.Text = Convert.ToString(pitchServo.Value);
            if (ckbTXPackets.Checked)
            {
                SP.SetCyclicPitch((ushort)pitchServo.Value);
               
            }
        }

        private void rollServo_Scroll(object sender, EventArgs e)
        {
            txtRollVal.Text = Convert.ToString(rollServo.Value);
            if (ckbTXPackets.Checked)
            {
                SP.SetCyclicRoll((ushort)rollServo.Value);
            }
        }

        private void yawServo_Scroll(object sender, EventArgs e)
        {
            txtYawVal.Text = Convert.ToString(yawServo.Value);
            if (ckbTXPackets.Checked)
            {
                SP.SetYaw((ushort)yawServo.Value);
               
            }
        }

        private void colServo_Scroll(object sender, EventArgs e)
        {
            txtColVal.Text = Convert.ToString(colServo.Value);
            if (ckbTXPackets.Checked)
            {
                SP.SetCollective((ushort)colServo.Value);

            }
        }

        private void engineSpeed_Scroll(object sender, EventArgs e)
        {
            txtEngineSpeed.Text = Convert.ToString(engineSpeed.Value);
            if (ckbTXPackets.Checked)
            {
                SP.SetMotorRPM((ushort)engineSpeed.Value);

            }
        }

        private void bnApply_Click(object sender, EventArgs e)
        {            
            Settings.BaudRate = Convert.ToInt32(cbBaudRate.Text);
            Settings.PortName = cbCommPort.Text;
        }

        private void bnOpen_Click(object sender, EventArgs e)
        {
            SP = new CommProtocol(Settings.PortName, Settings.BaudRate);
            //SP = new CommProtocol("COM6", 9600);
        }

     

      

    }
}