namespace commprotocoltester
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
            this.components = new System.ComponentModel.Container();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.btnCommsHandShakeInitiate = new System.Windows.Forms.Button();
            this.btnCommsHandShakeTerminate = new System.Windows.Forms.Button();
            this.btnDiscreetMovementControl = new System.Windows.Forms.Button();
            this.btnEngageEngine = new System.Windows.Forms.Button();
            this.btnGoto = new System.Windows.Forms.Button();
            this.btnHover = new System.Windows.Forms.Button();
            this.btnRequestLocation = new System.Windows.Forms.Button();
            this.btnRequestPreFlightPacket = new System.Windows.Forms.Button();
            this.btnReturnToBase = new System.Windows.Forms.Button();
            this.btnSetAntiTorque = new System.Windows.Forms.Button();
            this.btnSetCollective = new System.Windows.Forms.Button();
            this.btnSetCyclicPitch = new System.Windows.Forms.Button();
            this.btnSetCyclicRoll = new System.Windows.Forms.Button();
            this.btnSetMotorRPM = new System.Windows.Forms.Button();
            this.btnSetTunePoints = new System.Windows.Forms.Button();
            this.btnRequestBatteryStatus = new System.Windows.Forms.Button();
            this.btnRequestHSA = new System.Windows.Forms.Button();
            this.btnRequestAttitude = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnRequestMotorRPM = new System.Windows.Forms.Button();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.txtYaw = new System.Windows.Forms.TextBox();
            this.txtCollective = new System.Windows.Forms.TextBox();
            this.txtPitch = new System.Windows.Forms.TextBox();
            this.txtRoll = new System.Windows.Forms.TextBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.btnConnect = new System.Windows.Forms.Button();
            this.btnstop = new System.Windows.Forms.Button();
            this.txtRPM = new System.Windows.Forms.TextBox();
            this.btnGPSCorrection = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.btnInitPWMS = new System.Windows.Forms.Button();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.btnSetOpsMode = new System.Windows.Forms.Button();
            this.cbxCtrlMode = new System.Windows.Forms.ComboBox();
            this.btnKillMotor = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(213, 12);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox1.Size = new System.Drawing.Size(358, 617);
            this.textBox1.TabIndex = 0;
            // 
            // btnCommsHandShakeInitiate
            // 
            this.btnCommsHandShakeInitiate.Location = new System.Drawing.Point(12, 12);
            this.btnCommsHandShakeInitiate.Name = "btnCommsHandShakeInitiate";
            this.btnCommsHandShakeInitiate.Size = new System.Drawing.Size(174, 23);
            this.btnCommsHandShakeInitiate.TabIndex = 1;
            this.btnCommsHandShakeInitiate.Text = "CommsHandShakeInitiate";
            this.btnCommsHandShakeInitiate.UseVisualStyleBackColor = true;
            this.btnCommsHandShakeInitiate.Click += new System.EventHandler(this.btnCommsHandShakeInitiate_Click);
            // 
            // btnCommsHandShakeTerminate
            // 
            this.btnCommsHandShakeTerminate.Location = new System.Drawing.Point(12, 41);
            this.btnCommsHandShakeTerminate.Name = "btnCommsHandShakeTerminate";
            this.btnCommsHandShakeTerminate.Size = new System.Drawing.Size(174, 23);
            this.btnCommsHandShakeTerminate.TabIndex = 2;
            this.btnCommsHandShakeTerminate.Text = "CommsHandShakeTerminate";
            this.btnCommsHandShakeTerminate.UseVisualStyleBackColor = true;
            this.btnCommsHandShakeTerminate.Click += new System.EventHandler(this.btnCommsHandShakeTerminate_Click);
            // 
            // btnDiscreetMovementControl
            // 
            this.btnDiscreetMovementControl.Location = new System.Drawing.Point(12, 70);
            this.btnDiscreetMovementControl.Name = "btnDiscreetMovementControl";
            this.btnDiscreetMovementControl.Size = new System.Drawing.Size(174, 23);
            this.btnDiscreetMovementControl.TabIndex = 3;
            this.btnDiscreetMovementControl.Text = "DiscreetMovementControl";
            this.btnDiscreetMovementControl.UseVisualStyleBackColor = true;
            this.btnDiscreetMovementControl.Click += new System.EventHandler(this.btnDiscreetMovementControl_Click);
            // 
            // btnEngageEngine
            // 
            this.btnEngageEngine.Location = new System.Drawing.Point(12, 99);
            this.btnEngageEngine.Name = "btnEngageEngine";
            this.btnEngageEngine.Size = new System.Drawing.Size(174, 23);
            this.btnEngageEngine.TabIndex = 4;
            this.btnEngageEngine.Text = "EngageEngine";
            this.btnEngageEngine.UseVisualStyleBackColor = true;
            this.btnEngageEngine.Click += new System.EventHandler(this.btnEngageEngine_Click);
            // 
            // btnGoto
            // 
            this.btnGoto.Location = new System.Drawing.Point(12, 128);
            this.btnGoto.Name = "btnGoto";
            this.btnGoto.Size = new System.Drawing.Size(174, 23);
            this.btnGoto.TabIndex = 5;
            this.btnGoto.Text = "Goto";
            this.btnGoto.UseVisualStyleBackColor = true;
            this.btnGoto.Click += new System.EventHandler(this.btnGoto_Click);
            // 
            // btnHover
            // 
            this.btnHover.Location = new System.Drawing.Point(12, 157);
            this.btnHover.Name = "btnHover";
            this.btnHover.Size = new System.Drawing.Size(174, 23);
            this.btnHover.TabIndex = 6;
            this.btnHover.Text = "Hover";
            this.btnHover.UseVisualStyleBackColor = true;
            this.btnHover.Click += new System.EventHandler(this.btnHover_Click);
            // 
            // btnRequestLocation
            // 
            this.btnRequestLocation.Location = new System.Drawing.Point(6, 19);
            this.btnRequestLocation.Name = "btnRequestLocation";
            this.btnRequestLocation.Size = new System.Drawing.Size(174, 23);
            this.btnRequestLocation.TabIndex = 7;
            this.btnRequestLocation.Text = "RequestLocation";
            this.btnRequestLocation.UseVisualStyleBackColor = true;
            this.btnRequestLocation.Click += new System.EventHandler(this.btnRequestLocation_Click);
            // 
            // btnRequestPreFlightPacket
            // 
            this.btnRequestPreFlightPacket.Location = new System.Drawing.Point(5, 164);
            this.btnRequestPreFlightPacket.Name = "btnRequestPreFlightPacket";
            this.btnRequestPreFlightPacket.Size = new System.Drawing.Size(174, 23);
            this.btnRequestPreFlightPacket.TabIndex = 8;
            this.btnRequestPreFlightPacket.Text = "RequestPreFlightPacket";
            this.btnRequestPreFlightPacket.UseVisualStyleBackColor = true;
            this.btnRequestPreFlightPacket.Click += new System.EventHandler(this.btnRequestPreFlightPacket_Click);
            // 
            // btnReturnToBase
            // 
            this.btnReturnToBase.Location = new System.Drawing.Point(12, 434);
            this.btnReturnToBase.Name = "btnReturnToBase";
            this.btnReturnToBase.Size = new System.Drawing.Size(174, 23);
            this.btnReturnToBase.TabIndex = 9;
            this.btnReturnToBase.Text = "ReturnToBase";
            this.btnReturnToBase.UseVisualStyleBackColor = true;
            this.btnReturnToBase.Click += new System.EventHandler(this.btnReturnToBase_Click);
            // 
            // btnSetAntiTorque
            // 
            this.btnSetAntiTorque.Location = new System.Drawing.Point(12, 463);
            this.btnSetAntiTorque.Name = "btnSetAntiTorque";
            this.btnSetAntiTorque.Size = new System.Drawing.Size(135, 23);
            this.btnSetAntiTorque.TabIndex = 10;
            this.btnSetAntiTorque.Text = "SetAntiTorque [a/d]";
            this.btnSetAntiTorque.UseVisualStyleBackColor = true;
            this.btnSetAntiTorque.Click += new System.EventHandler(this.btnSetAntiTorque_Click);
            // 
            // btnSetCollective
            // 
            this.btnSetCollective.Location = new System.Drawing.Point(12, 493);
            this.btnSetCollective.Name = "btnSetCollective";
            this.btnSetCollective.Size = new System.Drawing.Size(135, 23);
            this.btnSetCollective.TabIndex = 11;
            this.btnSetCollective.Text = "SetCollective [w/s]";
            this.btnSetCollective.UseVisualStyleBackColor = true;
            this.btnSetCollective.Click += new System.EventHandler(this.btnSetCollective_Click);
            // 
            // btnSetCyclicPitch
            // 
            this.btnSetCyclicPitch.Location = new System.Drawing.Point(12, 522);
            this.btnSetCyclicPitch.Name = "btnSetCyclicPitch";
            this.btnSetCyclicPitch.Size = new System.Drawing.Size(135, 23);
            this.btnSetCyclicPitch.TabIndex = 12;
            this.btnSetCyclicPitch.Text = "SetCyclicPitch [i/k]";
            this.btnSetCyclicPitch.UseVisualStyleBackColor = true;
            this.btnSetCyclicPitch.Click += new System.EventHandler(this.btnSetCyclicPitch_Click);
            // 
            // btnSetCyclicRoll
            // 
            this.btnSetCyclicRoll.Location = new System.Drawing.Point(12, 551);
            this.btnSetCyclicRoll.Name = "btnSetCyclicRoll";
            this.btnSetCyclicRoll.Size = new System.Drawing.Size(135, 23);
            this.btnSetCyclicRoll.TabIndex = 13;
            this.btnSetCyclicRoll.Text = "SetCyclicRoll [j/l]";
            this.btnSetCyclicRoll.UseVisualStyleBackColor = true;
            this.btnSetCyclicRoll.Click += new System.EventHandler(this.btnSetCyclicRoll_Click);
            // 
            // btnSetMotorRPM
            // 
            this.btnSetMotorRPM.Location = new System.Drawing.Point(12, 580);
            this.btnSetMotorRPM.Name = "btnSetMotorRPM";
            this.btnSetMotorRPM.Size = new System.Drawing.Size(135, 23);
            this.btnSetMotorRPM.TabIndex = 14;
            this.btnSetMotorRPM.Text = "SetMotorRPM[t/g]";
            this.btnSetMotorRPM.UseVisualStyleBackColor = true;
            this.btnSetMotorRPM.Click += new System.EventHandler(this.btnSetMotorRPM_Click);
            // 
            // btnSetTunePoints
            // 
            this.btnSetTunePoints.Location = new System.Drawing.Point(12, 638);
            this.btnSetTunePoints.Name = "btnSetTunePoints";
            this.btnSetTunePoints.Size = new System.Drawing.Size(174, 23);
            this.btnSetTunePoints.TabIndex = 15;
            this.btnSetTunePoints.Text = "SetTunePoints";
            this.btnSetTunePoints.UseVisualStyleBackColor = true;
            this.btnSetTunePoints.Click += new System.EventHandler(this.btnSetTunePoints_Click);
            // 
            // btnRequestBatteryStatus
            // 
            this.btnRequestBatteryStatus.Location = new System.Drawing.Point(6, 48);
            this.btnRequestBatteryStatus.Name = "btnRequestBatteryStatus";
            this.btnRequestBatteryStatus.Size = new System.Drawing.Size(174, 23);
            this.btnRequestBatteryStatus.TabIndex = 16;
            this.btnRequestBatteryStatus.Text = "RequestBatteryStatus";
            this.btnRequestBatteryStatus.UseVisualStyleBackColor = true;
            this.btnRequestBatteryStatus.Click += new System.EventHandler(this.btnRequestBatteryStatus_Click);
            // 
            // btnRequestHSA
            // 
            this.btnRequestHSA.Location = new System.Drawing.Point(6, 77);
            this.btnRequestHSA.Name = "btnRequestHSA";
            this.btnRequestHSA.Size = new System.Drawing.Size(174, 23);
            this.btnRequestHSA.TabIndex = 17;
            this.btnRequestHSA.Text = "RequestHeadingSpeedAltitude";
            this.btnRequestHSA.UseVisualStyleBackColor = true;
            this.btnRequestHSA.Click += new System.EventHandler(this.btnRequestHSA_Click);
            // 
            // btnRequestAttitude
            // 
            this.btnRequestAttitude.Location = new System.Drawing.Point(6, 106);
            this.btnRequestAttitude.Name = "btnRequestAttitude";
            this.btnRequestAttitude.Size = new System.Drawing.Size(174, 23);
            this.btnRequestAttitude.TabIndex = 18;
            this.btnRequestAttitude.Text = "RequestAttitude";
            this.btnRequestAttitude.UseVisualStyleBackColor = true;
            this.btnRequestAttitude.Click += new System.EventHandler(this.btnRequestAttitude_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnRequestMotorRPM);
            this.groupBox1.Controls.Add(this.btnRequestLocation);
            this.groupBox1.Controls.Add(this.btnRequestAttitude);
            this.groupBox1.Controls.Add(this.btnRequestBatteryStatus);
            this.groupBox1.Controls.Add(this.btnRequestHSA);
            this.groupBox1.Controls.Add(this.btnRequestPreFlightPacket);
            this.groupBox1.Location = new System.Drawing.Point(12, 208);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(185, 191);
            this.groupBox1.TabIndex = 19;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Info requests";
            // 
            // btnRequestMotorRPM
            // 
            this.btnRequestMotorRPM.Location = new System.Drawing.Point(5, 135);
            this.btnRequestMotorRPM.Name = "btnRequestMotorRPM";
            this.btnRequestMotorRPM.Size = new System.Drawing.Size(174, 23);
            this.btnRequestMotorRPM.TabIndex = 19;
            this.btnRequestMotorRPM.Text = "Request Motor RPM";
            this.btnRequestMotorRPM.UseVisualStyleBackColor = true;
            this.btnRequestMotorRPM.Click += new System.EventHandler(this.btnRequestMotorRPM_Click);
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(577, 15);
            this.textBox2.Multiline = true;
            this.textBox2.Name = "textBox2";
            this.textBox2.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox2.Size = new System.Drawing.Size(438, 548);
            this.textBox2.TabIndex = 20;
            // 
            // txtYaw
            // 
            this.txtYaw.Location = new System.Drawing.Point(156, 463);
            this.txtYaw.Name = "txtYaw";
            this.txtYaw.Size = new System.Drawing.Size(41, 20);
            this.txtYaw.TabIndex = 21;
            this.txtYaw.Text = "0";
            // 
            // txtCollective
            // 
            this.txtCollective.Location = new System.Drawing.Point(156, 493);
            this.txtCollective.Name = "txtCollective";
            this.txtCollective.Size = new System.Drawing.Size(41, 20);
            this.txtCollective.TabIndex = 22;
            this.txtCollective.Text = "0";
            // 
            // txtPitch
            // 
            this.txtPitch.Location = new System.Drawing.Point(156, 522);
            this.txtPitch.Name = "txtPitch";
            this.txtPitch.Size = new System.Drawing.Size(41, 20);
            this.txtPitch.TabIndex = 23;
            this.txtPitch.Text = "0";
            // 
            // txtRoll
            // 
            this.txtRoll.Location = new System.Drawing.Point(156, 548);
            this.txtRoll.Name = "txtRoll";
            this.txtRoll.Size = new System.Drawing.Size(41, 20);
            this.txtRoll.TabIndex = 24;
            this.txtRoll.Text = "0";
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(213, 637);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(121, 21);
            this.comboBox1.TabIndex = 26;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(340, 635);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 23);
            this.btnConnect.TabIndex = 27;
            this.btnConnect.Text = "Connnect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // btnstop
            // 
            this.btnstop.Location = new System.Drawing.Point(439, 637);
            this.btnstop.Name = "btnstop";
            this.btnstop.Size = new System.Drawing.Size(75, 23);
            this.btnstop.TabIndex = 28;
            this.btnstop.Text = "stop";
            this.btnstop.UseVisualStyleBackColor = true;
            this.btnstop.Click += new System.EventHandler(this.btnstop_Click);
            // 
            // txtRPM
            // 
            this.txtRPM.Location = new System.Drawing.Point(156, 580);
            this.txtRPM.Name = "txtRPM";
            this.txtRPM.Size = new System.Drawing.Size(41, 20);
            this.txtRPM.TabIndex = 29;
            this.txtRPM.Text = "0";
            // 
            // btnGPSCorrection
            // 
            this.btnGPSCorrection.Location = new System.Drawing.Point(12, 405);
            this.btnGPSCorrection.Name = "btnGPSCorrection";
            this.btnGPSCorrection.Size = new System.Drawing.Size(174, 23);
            this.btnGPSCorrection.TabIndex = 30;
            this.btnGPSCorrection.Text = "Send GPS correction";
            this.btnGPSCorrection.UseVisualStyleBackColor = true;
            this.btnGPSCorrection.Click += new System.EventHandler(this.btnGPSCorrection_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // btnInitPWMS
            // 
            this.btnInitPWMS.Location = new System.Drawing.Point(12, 609);
            this.btnInitPWMS.Name = "btnInitPWMS";
            this.btnInitPWMS.Size = new System.Drawing.Size(174, 23);
            this.btnInitPWMS.TabIndex = 31;
            this.btnInitPWMS.Text = "Init PWMs";
            this.btnInitPWMS.UseVisualStyleBackColor = true;
            this.btnInitPWMS.Click += new System.EventHandler(this.btnInitPWMS_Click);
            // 
            // timer2
            // 
            this.timer2.Tick += new System.EventHandler(this.timer2_Tick);
            // 
            // btnSetOpsMode
            // 
            this.btnSetOpsMode.Location = new System.Drawing.Point(12, 186);
            this.btnSetOpsMode.Name = "btnSetOpsMode";
            this.btnSetOpsMode.Size = new System.Drawing.Size(85, 23);
            this.btnSetOpsMode.TabIndex = 32;
            this.btnSetOpsMode.Text = "Set Ops Mode";
            this.btnSetOpsMode.UseVisualStyleBackColor = true;
            this.btnSetOpsMode.Click += new System.EventHandler(this.btnSetOpsMode_Click);
            // 
            // cbxCtrlMode
            // 
            this.cbxCtrlMode.FormattingEnabled = true;
            this.cbxCtrlMode.Items.AddRange(new object[] {
            "Testing/tuning",
            "Fuzzy"});
            this.cbxCtrlMode.Location = new System.Drawing.Point(103, 186);
            this.cbxCtrlMode.Name = "cbxCtrlMode";
            this.cbxCtrlMode.Size = new System.Drawing.Size(104, 21);
            this.cbxCtrlMode.TabIndex = 33;
            // 
            // btnKillMotor
            // 
            this.btnKillMotor.BackColor = System.Drawing.Color.Red;
            this.btnKillMotor.Font = new System.Drawing.Font("Microsoft Sans Serif", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnKillMotor.Location = new System.Drawing.Point(577, 569);
            this.btnKillMotor.Name = "btnKillMotor";
            this.btnKillMotor.Size = new System.Drawing.Size(438, 113);
            this.btnKillMotor.TabIndex = 34;
            this.btnKillMotor.Text = "STOP MOTOR";
            this.btnKillMotor.UseVisualStyleBackColor = false;
            this.btnKillMotor.Click += new System.EventHandler(this.btnKillMotor_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1038, 694);
            this.Controls.Add(this.btnKillMotor);
            this.Controls.Add(this.cbxCtrlMode);
            this.Controls.Add(this.btnSetOpsMode);
            this.Controls.Add(this.btnInitPWMS);
            this.Controls.Add(this.btnGPSCorrection);
            this.Controls.Add(this.txtRPM);
            this.Controls.Add(this.btnstop);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.txtRoll);
            this.Controls.Add(this.txtPitch);
            this.Controls.Add(this.txtCollective);
            this.Controls.Add(this.txtYaw);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnSetTunePoints);
            this.Controls.Add(this.btnSetMotorRPM);
            this.Controls.Add(this.btnSetCyclicRoll);
            this.Controls.Add(this.btnSetCyclicPitch);
            this.Controls.Add(this.btnSetCollective);
            this.Controls.Add(this.btnSetAntiTorque);
            this.Controls.Add(this.btnReturnToBase);
            this.Controls.Add(this.btnHover);
            this.Controls.Add(this.btnGoto);
            this.Controls.Add(this.btnEngageEngine);
            this.Controls.Add(this.btnDiscreetMovementControl);
            this.Controls.Add(this.btnCommsHandShakeTerminate);
            this.Controls.Add(this.btnCommsHandShakeInitiate);
            this.Controls.Add(this.textBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button btnCommsHandShakeInitiate;
        private System.Windows.Forms.Button btnCommsHandShakeTerminate;
        private System.Windows.Forms.Button btnDiscreetMovementControl;
        private System.Windows.Forms.Button btnEngageEngine;
        private System.Windows.Forms.Button btnGoto;
        private System.Windows.Forms.Button btnHover;
        private System.Windows.Forms.Button btnRequestLocation;
        private System.Windows.Forms.Button btnRequestPreFlightPacket;
        private System.Windows.Forms.Button btnReturnToBase;
        private System.Windows.Forms.Button btnSetAntiTorque;
        private System.Windows.Forms.Button btnSetCollective;
        private System.Windows.Forms.Button btnSetCyclicPitch;
        private System.Windows.Forms.Button btnSetCyclicRoll;
        private System.Windows.Forms.Button btnSetMotorRPM;
        private System.Windows.Forms.Button btnSetTunePoints;
        private System.Windows.Forms.Button btnRequestBatteryStatus;
        private System.Windows.Forms.Button btnRequestHSA;
        private System.Windows.Forms.Button btnRequestAttitude;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox txtYaw;
        private System.Windows.Forms.TextBox txtCollective;
        private System.Windows.Forms.TextBox txtPitch;
        private System.Windows.Forms.TextBox txtRoll;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Button btnRequestMotorRPM;
        private System.Windows.Forms.Button btnstop;
        private System.Windows.Forms.TextBox txtRPM;
        private System.Windows.Forms.Button btnGPSCorrection;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Button btnInitPWMS;
        private System.Windows.Forms.Timer timer2;
        private System.Windows.Forms.Button btnSetOpsMode;
        private System.Windows.Forms.ComboBox cbxCtrlMode;
        private System.Windows.Forms.Button btnKillMotor;

    }
}

