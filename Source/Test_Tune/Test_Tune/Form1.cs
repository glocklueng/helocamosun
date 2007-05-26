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
        string text = "";
        string bad_text = "";
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

            pitchServo.MouseUp += new MouseEventHandler(pitchServo_MouseUp);
        }

        void pitchServo_MouseUp(object sender, MouseEventArgs e)
        {
            //throw new Exception("The method or operation is not implemented.");
            
            if (ckbTXPackets.Checked)
            {
                SP.SetCyclicPitch((byte)pitchServo.Value);

            }
        }

        void SP_ResponseTimeout(object sender, CommProtocol.ResponseTimeoutEventArgs e)
        {
            string hexbuffer = "";

            foreach (char c in e.BufferContents)
            { hexbuffer += string.Format("{0:x2} ", (int)c); }

            /*bad_text += hexbuffer + "\r\n";*/

            txt_bad.Text += hexbuffer + "\r\n";

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.KeyDown += new KeyEventHandler(Form1_KeyDown);
            this.KeyPress += new KeyPressEventHandler(Form1_KeyPress);
        }

        void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            throw new Exception("The method or operation is not implemented.");
        }

        void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            //throw new Exception("The method or operation is not implemented.");
            char c = e.KeyChar;
            switch (c)
            { 
                case 'w':
                case 'W':
                    txtColVal.Text = Convert.ToString(++colServo.Value);
                    if (ckbTXPackets.Checked)
                    {
                        SP.SetCollective((byte)colServo.Value);

                    }
                    break;
            }
        }

        private void pitchServo_Scroll(object sender, EventArgs e)
        {
            txtPitchVal.Text = Convert.ToString(pitchServo.Value);
        }

        private void rollServo_Scroll(object sender, EventArgs e)
        {
            txtRollVal.Text = Convert.ToString(rollServo.Value);
            if (ckbTXPackets.Checked)
            {
                SP.SetCyclicRoll((byte)rollServo.Value);
            }
        }

        private void yawServo_Scroll(object sender, EventArgs e)
        {
            txtYawVal.Text = Convert.ToString(yawServo.Value);
            if (ckbTXPackets.Checked)
            {
              
                SP.SetAntiTorque((byte)yawServo.Value);
            }
        }

        private void colServo_Scroll(object sender, EventArgs e)
        {
            txtColVal.Text = Convert.ToString(colServo.Value);
            if (ckbTXPackets.Checked)
            {
                SP.SetCollective((byte)colServo.Value);

            }
        }

        private void engineSpeed_Scroll(object sender, EventArgs e)
        {
            txtEngineSpeed.Text = Convert.ToString(engineSpeed.Value);
            if (ckbTXPackets.Checked)
            {
                SP.SetMotorRPM((byte)engineSpeed.Value);

            }
        }

        private void bnApply_Click(object sender, EventArgs e)
        {            
            
        }

        private void bnOpen_Click(object sender, EventArgs e)
        {
            Settings.BaudRate = Convert.ToInt32(cbBaudRate.Text);
            Settings.PortName = cbCommPort.Text;
            SP = new CommProtocol(Settings.PortName, Settings.BaudRate, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One, 1000, this);
            SP.ResponseTimeout += new CommProtocol.ResponseTimeoutEventHandler(SP_ResponseTimeout);
            SP.ExpectedResponseReceived += new CommProtocol.ExpectedResponseReceivedEventHandler(SP_ExpectedResponseReceived);
        }
        public void UpdateText(string text)
        {
            txt_rcvd.AppendText(text);
        }

        void SP_ExpectedResponseReceived(object sender, CommProtocol.ExpectedResponseReceivedEventArgs e)
        {
            string hexbuffer = "";
        
            foreach (char c in e.ReceivedPacket)
            { hexbuffer += string.Format("{0:x2} ", (int)c); }
            //text += e.Name + "\t" + hexbuffer + "\r\n";

            txt_rcvd.Text += e.Name + "\t" + hexbuffer + "\r\n";
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            //txt_rcvd.Text = text;
            //txt_bad.Text = bad_text;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            bad_text = "";
            text = "";
            txt_rcvd.Text = "";
            txt_bad.Text = "";

        }
    }
}