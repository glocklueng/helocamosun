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
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void pitchServo_Scroll(object sender, EventArgs e)
        {
            txtPitchVal.Text = Convert.ToString(pitchServo.Value);
        }

        private void rollServo_Scroll(object sender, EventArgs e)
        {
            txtRollVal.Text = Convert.ToString(rollServo.Value);
        }

        private void yawServo_Scroll(object sender, EventArgs e)
        {
            txtYawVal.Text = Convert.ToString(yawServo.Value);
        }

        private void colServo_Scroll(object sender, EventArgs e)
        {
            txtColVal.Text = Convert.ToString(colServo.Value);

        }

        private void engineSpeed_Scroll(object sender, EventArgs e)
        {
            txtEngineSpeed.Text = Convert.ToString(engineSpeed.Value);
        }

        private void bnApply_Click(object sender, EventArgs e)
        {            
            Settings.BaudRate = Convert.ToInt32(cbBaudRate.Text);
            Settings.PortName = cbCommPort.Text;
        }

        private void bnOpen_Click(object sender, EventArgs e)
        {
            SP = new CommProtocol(Settings.PortName, Settings.BaudRate);
        }

    }
}