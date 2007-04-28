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
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(213, 12);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox1.Size = new System.Drawing.Size(438, 499);
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
            this.btnRequestLocation.Location = new System.Drawing.Point(6, 30);
            this.btnRequestLocation.Name = "btnRequestLocation";
            this.btnRequestLocation.Size = new System.Drawing.Size(174, 23);
            this.btnRequestLocation.TabIndex = 7;
            this.btnRequestLocation.Text = "RequestLocation";
            this.btnRequestLocation.UseVisualStyleBackColor = true;
            this.btnRequestLocation.Click += new System.EventHandler(this.btnRequestLocation_Click);
            // 
            // btnRequestPreFlightPacket
            // 
            this.btnRequestPreFlightPacket.Location = new System.Drawing.Point(12, 349);
            this.btnRequestPreFlightPacket.Name = "btnRequestPreFlightPacket";
            this.btnRequestPreFlightPacket.Size = new System.Drawing.Size(174, 23);
            this.btnRequestPreFlightPacket.TabIndex = 8;
            this.btnRequestPreFlightPacket.Text = "RequestPreFlightPacket";
            this.btnRequestPreFlightPacket.UseVisualStyleBackColor = true;
            this.btnRequestPreFlightPacket.Click += new System.EventHandler(this.btnRequestPreFlightPacket_Click);
            // 
            // btnReturnToBase
            // 
            this.btnReturnToBase.Location = new System.Drawing.Point(12, 378);
            this.btnReturnToBase.Name = "btnReturnToBase";
            this.btnReturnToBase.Size = new System.Drawing.Size(174, 23);
            this.btnReturnToBase.TabIndex = 9;
            this.btnReturnToBase.Text = "ReturnToBase";
            this.btnReturnToBase.UseVisualStyleBackColor = true;
            this.btnReturnToBase.Click += new System.EventHandler(this.btnReturnToBase_Click);
            // 
            // btnSetAntiTorque
            // 
            this.btnSetAntiTorque.Location = new System.Drawing.Point(12, 407);
            this.btnSetAntiTorque.Name = "btnSetAntiTorque";
            this.btnSetAntiTorque.Size = new System.Drawing.Size(174, 23);
            this.btnSetAntiTorque.TabIndex = 10;
            this.btnSetAntiTorque.Text = "SetAntiTorque";
            this.btnSetAntiTorque.UseVisualStyleBackColor = true;
            this.btnSetAntiTorque.Click += new System.EventHandler(this.btnSetAntiTorque_Click);
            // 
            // btnSetCollective
            // 
            this.btnSetCollective.Location = new System.Drawing.Point(12, 437);
            this.btnSetCollective.Name = "btnSetCollective";
            this.btnSetCollective.Size = new System.Drawing.Size(174, 23);
            this.btnSetCollective.TabIndex = 11;
            this.btnSetCollective.Text = "SetCollective";
            this.btnSetCollective.UseVisualStyleBackColor = true;
            this.btnSetCollective.Click += new System.EventHandler(this.btnSetCollective_Click);
            // 
            // btnSetCyclicPitch
            // 
            this.btnSetCyclicPitch.Location = new System.Drawing.Point(12, 466);
            this.btnSetCyclicPitch.Name = "btnSetCyclicPitch";
            this.btnSetCyclicPitch.Size = new System.Drawing.Size(174, 23);
            this.btnSetCyclicPitch.TabIndex = 12;
            this.btnSetCyclicPitch.Text = "SetCyclicPitch";
            this.btnSetCyclicPitch.UseVisualStyleBackColor = true;
            this.btnSetCyclicPitch.Click += new System.EventHandler(this.btnSetCyclicPitch_Click);
            // 
            // btnSetCyclicRoll
            // 
            this.btnSetCyclicRoll.Location = new System.Drawing.Point(12, 495);
            this.btnSetCyclicRoll.Name = "btnSetCyclicRoll";
            this.btnSetCyclicRoll.Size = new System.Drawing.Size(174, 23);
            this.btnSetCyclicRoll.TabIndex = 13;
            this.btnSetCyclicRoll.Text = "SetCyclicRoll";
            this.btnSetCyclicRoll.UseVisualStyleBackColor = true;
            this.btnSetCyclicRoll.Click += new System.EventHandler(this.btnSetCyclicRoll_Click);
            // 
            // btnSetMotorRPM
            // 
            this.btnSetMotorRPM.Location = new System.Drawing.Point(12, 524);
            this.btnSetMotorRPM.Name = "btnSetMotorRPM";
            this.btnSetMotorRPM.Size = new System.Drawing.Size(174, 23);
            this.btnSetMotorRPM.TabIndex = 14;
            this.btnSetMotorRPM.Text = "SetMotorRPM";
            this.btnSetMotorRPM.UseVisualStyleBackColor = true;
            this.btnSetMotorRPM.Click += new System.EventHandler(this.btnSetMotorRPM_Click);
            // 
            // btnSetTunePoints
            // 
            this.btnSetTunePoints.Location = new System.Drawing.Point(12, 553);
            this.btnSetTunePoints.Name = "btnSetTunePoints";
            this.btnSetTunePoints.Size = new System.Drawing.Size(174, 23);
            this.btnSetTunePoints.TabIndex = 15;
            this.btnSetTunePoints.Text = "SetTunePoints";
            this.btnSetTunePoints.UseVisualStyleBackColor = true;
            this.btnSetTunePoints.Click += new System.EventHandler(this.btnSetTunePoints_Click);
            // 
            // btnRequestBatteryStatus
            // 
            this.btnRequestBatteryStatus.Location = new System.Drawing.Point(6, 59);
            this.btnRequestBatteryStatus.Name = "btnRequestBatteryStatus";
            this.btnRequestBatteryStatus.Size = new System.Drawing.Size(174, 23);
            this.btnRequestBatteryStatus.TabIndex = 16;
            this.btnRequestBatteryStatus.Text = "RequestBatteryStatus";
            this.btnRequestBatteryStatus.UseVisualStyleBackColor = true;
            this.btnRequestBatteryStatus.Click += new System.EventHandler(this.btnRequestBatteryStatus_Click);
            // 
            // btnRequestHSA
            // 
            this.btnRequestHSA.Location = new System.Drawing.Point(6, 88);
            this.btnRequestHSA.Name = "btnRequestHSA";
            this.btnRequestHSA.Size = new System.Drawing.Size(174, 23);
            this.btnRequestHSA.TabIndex = 17;
            this.btnRequestHSA.Text = "RequestHeadingSpeedAltitude";
            this.btnRequestHSA.UseVisualStyleBackColor = true;
            this.btnRequestHSA.Click += new System.EventHandler(this.btnRequestHSA_Click);
            // 
            // btnRequestAttitude
            // 
            this.btnRequestAttitude.Location = new System.Drawing.Point(6, 117);
            this.btnRequestAttitude.Name = "btnRequestAttitude";
            this.btnRequestAttitude.Size = new System.Drawing.Size(174, 23);
            this.btnRequestAttitude.TabIndex = 18;
            this.btnRequestAttitude.Text = "RequestAttitude";
            this.btnRequestAttitude.UseVisualStyleBackColor = true;
            this.btnRequestAttitude.Click += new System.EventHandler(this.btnRequestAttitude_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnRequestLocation);
            this.groupBox1.Controls.Add(this.btnRequestAttitude);
            this.groupBox1.Controls.Add(this.btnRequestBatteryStatus);
            this.groupBox1.Controls.Add(this.btnRequestHSA);
            this.groupBox1.Location = new System.Drawing.Point(12, 186);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(185, 157);
            this.groupBox1.TabIndex = 19;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Info requests";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(663, 582);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnSetTunePoints);
            this.Controls.Add(this.btnSetMotorRPM);
            this.Controls.Add(this.btnSetCyclicRoll);
            this.Controls.Add(this.btnSetCyclicPitch);
            this.Controls.Add(this.btnSetCollective);
            this.Controls.Add(this.btnSetAntiTorque);
            this.Controls.Add(this.btnReturnToBase);
            this.Controls.Add(this.btnRequestPreFlightPacket);
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

    }
}

