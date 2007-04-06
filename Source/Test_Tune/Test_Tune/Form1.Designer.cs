namespace Test_Tune
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.pitchServo = new System.Windows.Forms.TrackBar();
            this.rollServo = new System.Windows.Forms.TrackBar();
            this.yawServo = new System.Windows.Forms.TrackBar();
            this.colServo = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.txtPitchVal = new System.Windows.Forms.TextBox();
            this.txtColVal = new System.Windows.Forms.TextBox();
            this.txtYawVal = new System.Windows.Forms.TextBox();
            this.txtRollVal = new System.Windows.Forms.TextBox();
            this.txtEngineSpeed = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.engineSpeed = new System.Windows.Forms.TrackBar();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.ckbTXPackets = new System.Windows.Forms.CheckBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.bnOpen = new System.Windows.Forms.Button();
            this.bnApply = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.cbBaudRate = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.cbCommPort = new System.Windows.Forms.ComboBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.txtFBVoltage = new System.Windows.Forms.TextBox();
            this.txtFBTemp = new System.Windows.Forms.TextBox();
            this.txtFBCurrent = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pitchServo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.rollServo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.yawServo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.colServo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.engineSpeed)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pitchServo
            // 
            this.pitchServo.Location = new System.Drawing.Point(27, 51);
            this.pitchServo.Maximum = 100;
            this.pitchServo.Name = "pitchServo";
            this.pitchServo.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.pitchServo.Size = new System.Drawing.Size(45, 227);
            this.pitchServo.TabIndex = 0;
            this.pitchServo.Scroll += new System.EventHandler(this.pitchServo_Scroll);
            // 
            // rollServo
            // 
            this.rollServo.Location = new System.Drawing.Point(127, 51);
            this.rollServo.Maximum = 100;
            this.rollServo.Name = "rollServo";
            this.rollServo.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.rollServo.Size = new System.Drawing.Size(45, 227);
            this.rollServo.TabIndex = 1;
            this.rollServo.Scroll += new System.EventHandler(this.rollServo_Scroll);
            // 
            // yawServo
            // 
            this.yawServo.Location = new System.Drawing.Point(227, 51);
            this.yawServo.Maximum = 100;
            this.yawServo.Name = "yawServo";
            this.yawServo.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.yawServo.Size = new System.Drawing.Size(45, 227);
            this.yawServo.TabIndex = 2;
            this.yawServo.Scroll += new System.EventHandler(this.yawServo_Scroll);
            // 
            // colServo
            // 
            this.colServo.Location = new System.Drawing.Point(327, 51);
            this.colServo.Maximum = 100;
            this.colServo.Name = "colServo";
            this.colServo.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.colServo.Size = new System.Drawing.Size(45, 227);
            this.colServo.TabIndex = 3;
            this.colServo.Scroll += new System.EventHandler(this.colServo_Scroll);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(62, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Pitch Servo";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(307, 21);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(84, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Collective Servo";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(220, 21);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Yaw Servo";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(121, 21);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(56, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Roll Servo";
            // 
            // txtPitchVal
            // 
            this.txtPitchVal.Enabled = false;
            this.txtPitchVal.Location = new System.Drawing.Point(32, 282);
            this.txtPitchVal.Name = "txtPitchVal";
            this.txtPitchVal.Size = new System.Drawing.Size(34, 20);
            this.txtPitchVal.TabIndex = 8;
            // 
            // txtColVal
            // 
            this.txtColVal.Enabled = false;
            this.txtColVal.Location = new System.Drawing.Point(332, 282);
            this.txtColVal.Name = "txtColVal";
            this.txtColVal.Size = new System.Drawing.Size(34, 20);
            this.txtColVal.TabIndex = 9;
            // 
            // txtYawVal
            // 
            this.txtYawVal.Enabled = false;
            this.txtYawVal.Location = new System.Drawing.Point(232, 282);
            this.txtYawVal.Name = "txtYawVal";
            this.txtYawVal.Size = new System.Drawing.Size(34, 20);
            this.txtYawVal.TabIndex = 10;
            // 
            // txtRollVal
            // 
            this.txtRollVal.Enabled = false;
            this.txtRollVal.Location = new System.Drawing.Point(132, 282);
            this.txtRollVal.Name = "txtRollVal";
            this.txtRollVal.Size = new System.Drawing.Size(34, 20);
            this.txtRollVal.TabIndex = 11;
            // 
            // txtEngineSpeed
            // 
            this.txtEngineSpeed.Enabled = false;
            this.txtEngineSpeed.Location = new System.Drawing.Point(432, 282);
            this.txtEngineSpeed.Name = "txtEngineSpeed";
            this.txtEngineSpeed.Size = new System.Drawing.Size(34, 20);
            this.txtEngineSpeed.TabIndex = 14;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(407, 21);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(74, 13);
            this.label5.TabIndex = 13;
            this.label5.Text = "Engine Speed";
            // 
            // engineSpeed
            // 
            this.engineSpeed.Location = new System.Drawing.Point(427, 51);
            this.engineSpeed.Maximum = 100;
            this.engineSpeed.Name = "engineSpeed";
            this.engineSpeed.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.engineSpeed.Size = new System.Drawing.Size(45, 227);
            this.engineSpeed.TabIndex = 12;
            this.engineSpeed.Scroll += new System.EventHandler(this.engineSpeed_Scroll);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(39, 27);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(522, 391);
            this.tabControl1.TabIndex = 16;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.ckbTXPackets);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.txtEngineSpeed);
            this.tabPage1.Controls.Add(this.pitchServo);
            this.tabPage1.Controls.Add(this.label5);
            this.tabPage1.Controls.Add(this.rollServo);
            this.tabPage1.Controls.Add(this.engineSpeed);
            this.tabPage1.Controls.Add(this.yawServo);
            this.tabPage1.Controls.Add(this.txtRollVal);
            this.tabPage1.Controls.Add(this.colServo);
            this.tabPage1.Controls.Add(this.txtYawVal);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.txtColVal);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.txtPitchVal);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(514, 365);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Control";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // ckbTXPackets
            // 
            this.ckbTXPackets.AutoSize = true;
            this.ckbTXPackets.Location = new System.Drawing.Point(195, 318);
            this.ckbTXPackets.Name = "ckbTXPackets";
            this.ckbTXPackets.Size = new System.Drawing.Size(108, 17);
            this.ckbTXPackets.TabIndex = 15;
            this.ckbTXPackets.Text = "Transmit Packets";
            this.ckbTXPackets.UseVisualStyleBackColor = true;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.bnOpen);
            this.tabPage2.Controls.Add(this.bnApply);
            this.tabPage2.Controls.Add(this.label7);
            this.tabPage2.Controls.Add(this.cbBaudRate);
            this.tabPage2.Controls.Add(this.label6);
            this.tabPage2.Controls.Add(this.cbCommPort);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(514, 365);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Configuration";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // bnOpen
            // 
            this.bnOpen.Location = new System.Drawing.Point(178, 257);
            this.bnOpen.Name = "bnOpen";
            this.bnOpen.Size = new System.Drawing.Size(75, 23);
            this.bnOpen.TabIndex = 5;
            this.bnOpen.Text = "Open Port\r\n";
            this.bnOpen.UseVisualStyleBackColor = true;
            this.bnOpen.Click += new System.EventHandler(this.bnOpen_Click);
            // 
            // bnApply
            // 
            this.bnApply.Location = new System.Drawing.Point(68, 256);
            this.bnApply.Name = "bnApply";
            this.bnApply.Size = new System.Drawing.Size(75, 23);
            this.bnApply.TabIndex = 4;
            this.bnApply.Text = "Apply";
            this.bnApply.UseVisualStyleBackColor = true;
            this.bnApply.Click += new System.EventHandler(this.bnApply_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(68, 138);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(61, 13);
            this.label7.TabIndex = 3;
            this.label7.Text = "Baud Rate:";
            // 
            // cbBaudRate
            // 
            this.cbBaudRate.FormattingEnabled = true;
            this.cbBaudRate.Items.AddRange(new object[] {
            "2400",
            "4800",
            "9600",
            "14400",
            "19200",
            "38400",
            "57600",
            "115200"});
            this.cbBaudRate.Location = new System.Drawing.Point(68, 156);
            this.cbBaudRate.Name = "cbBaudRate";
            this.cbBaudRate.Size = new System.Drawing.Size(147, 21);
            this.cbBaudRate.TabIndex = 2;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(68, 45);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(61, 13);
            this.label6.TabIndex = 1;
            this.label6.Text = "Comm Port:";
            // 
            // cbCommPort
            // 
            this.cbCommPort.FormattingEnabled = true;
            this.cbCommPort.Items.AddRange(new object[] {
            "COM1",
            "COM2",
            "COM3",
            "COM4",
            "COM5",
            "COM6",
            "COM7",
            "COM8",
            "COM9",
            "COM10",
            "COM11",
            "COM12",
            "COM13",
            "COM14",
            "COM15",
            "COM16"});
            this.cbCommPort.Location = new System.Drawing.Point(68, 61);
            this.cbCommPort.Name = "cbCommPort";
            this.cbCommPort.Size = new System.Drawing.Size(147, 21);
            this.cbCommPort.TabIndex = 0;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label13);
            this.groupBox1.Controls.Add(this.label12);
            this.groupBox1.Controls.Add(this.label11);
            this.groupBox1.Controls.Add(this.txtFBVoltage);
            this.groupBox1.Controls.Add(this.txtFBTemp);
            this.groupBox1.Controls.Add(this.txtFBCurrent);
            this.groupBox1.Controls.Add(this.label10);
            this.groupBox1.Controls.Add(this.label9);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Location = new System.Drawing.Point(608, 50);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(241, 149);
            this.groupBox1.TabIndex = 17;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Feed Back";
            // 
            // txtFBVoltage
            // 
            this.txtFBVoltage.Enabled = false;
            this.txtFBVoltage.Location = new System.Drawing.Point(107, 78);
            this.txtFBVoltage.Name = "txtFBVoltage";
            this.txtFBVoltage.Size = new System.Drawing.Size(47, 20);
            this.txtFBVoltage.TabIndex = 5;
            // 
            // txtFBTemp
            // 
            this.txtFBTemp.Enabled = false;
            this.txtFBTemp.Location = new System.Drawing.Point(107, 109);
            this.txtFBTemp.Name = "txtFBTemp";
            this.txtFBTemp.Size = new System.Drawing.Size(47, 20);
            this.txtFBTemp.TabIndex = 4;
            // 
            // txtFBCurrent
            // 
            this.txtFBCurrent.Enabled = false;
            this.txtFBCurrent.Location = new System.Drawing.Point(107, 47);
            this.txtFBCurrent.Name = "txtFBCurrent";
            this.txtFBCurrent.Size = new System.Drawing.Size(47, 20);
            this.txtFBCurrent.TabIndex = 3;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(20, 113);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(70, 13);
            this.label10.TabIndex = 2;
            this.label10.Text = "Temperature:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(20, 82);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(82, 13);
            this.label9.TabIndex = 1;
            this.label9.Text = "Battery Voltage:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(20, 51);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(72, 13);
            this.label8.TabIndex = 0;
            this.label8.Text = "Current Draw:";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(158, 51);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(22, 13);
            this.label11.TabIndex = 6;
            this.label11.Text = "mA";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(158, 82);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(14, 13);
            this.label12.TabIndex = 7;
            this.label12.Text = "V";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(158, 113);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(14, 13);
            this.label13.TabIndex = 8;
            this.label13.Text = "C";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(933, 458);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.tabControl1);
            this.Name = "Form1";
            this.Text = "Airwulf Testing and Tuning Application";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pitchServo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.rollServo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.yawServo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.colServo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.engineSpeed)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TrackBar pitchServo;
        private System.Windows.Forms.TrackBar rollServo;
        private System.Windows.Forms.TrackBar yawServo;
        private System.Windows.Forms.TrackBar colServo;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtPitchVal;
        private System.Windows.Forms.TextBox txtColVal;
        private System.Windows.Forms.TextBox txtYawVal;
        private System.Windows.Forms.TextBox txtRollVal;
        private System.Windows.Forms.TextBox txtEngineSpeed;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TrackBar engineSpeed;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox cbBaudRate;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox cbCommPort;
        private System.Windows.Forms.Button bnApply;
        private System.Windows.Forms.Button bnOpen;
        private System.Windows.Forms.CheckBox ckbTXPackets;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox txtFBCurrent;
        private System.Windows.Forms.TextBox txtFBVoltage;
        private System.Windows.Forms.TextBox txtFBTemp;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label11;

    }
}

