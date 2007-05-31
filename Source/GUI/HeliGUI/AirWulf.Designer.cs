namespace HeliGui
{
    partial class frmAirWulf
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
            this.btnConnect = new System.Windows.Forms.Button();
            this.cbxComPortSelect = new System.Windows.Forms.ComboBox();
            this.lblComPort = new System.Windows.Forms.Label();
            this.btnReloadGMC = new System.Windows.Forms.Button();
            this.RequestInfoTimer = new System.Windows.Forms.Timer(this.components);
            this.PacketRXTimer = new System.Windows.Forms.Timer(this.components);
            this.txtLatCorrection = new System.Windows.Forms.TextBox();
            this.txtLongCorrection = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txtAltitudeCorrection = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(228, 6);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 23);
            this.btnConnect.TabIndex = 0;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // cbxComPortSelect
            // 
            this.cbxComPortSelect.FormattingEnabled = true;
            this.cbxComPortSelect.Location = new System.Drawing.Point(101, 6);
            this.cbxComPortSelect.Name = "cbxComPortSelect";
            this.cbxComPortSelect.Size = new System.Drawing.Size(121, 21);
            this.cbxComPortSelect.TabIndex = 1;
            // 
            // lblComPort
            // 
            this.lblComPort.AutoSize = true;
            this.lblComPort.Location = new System.Drawing.Point(12, 9);
            this.lblComPort.Name = "lblComPort";
            this.lblComPort.Size = new System.Drawing.Size(83, 13);
            this.lblComPort.TabIndex = 2;
            this.lblComPort.Text = "Select Com Port";
            // 
            // btnReloadGMC
            // 
            this.btnReloadGMC.Location = new System.Drawing.Point(12, 37);
            this.btnReloadGMC.Name = "btnReloadGMC";
            this.btnReloadGMC.Size = new System.Drawing.Size(163, 23);
            this.btnReloadGMC.TabIndex = 3;
            this.btnReloadGMC.Text = "Reload Google Maps control";
            this.btnReloadGMC.UseVisualStyleBackColor = true;
            this.btnReloadGMC.Click += new System.EventHandler(this.btnReloadGMC_Click);
            // 
            // RequestInfoTimer
            // 
            this.RequestInfoTimer.Tick += new System.EventHandler(this.RequestInfoTimer_Tick);
            // 
            // PacketRXTimer
            // 
            this.PacketRXTimer.Enabled = true;
            this.PacketRXTimer.Interval = 500;
            this.PacketRXTimer.Tick += new System.EventHandler(this.PacketRXTimer_Tick);
            // 
            // txtLatCorrection
            // 
            this.txtLatCorrection.Location = new System.Drawing.Point(267, 37);
            this.txtLatCorrection.Name = "txtLatCorrection";
            this.txtLatCorrection.Size = new System.Drawing.Size(48, 20);
            this.txtLatCorrection.TabIndex = 4;
            // 
            // txtLongCorrection
            // 
            this.txtLongCorrection.Location = new System.Drawing.Point(409, 39);
            this.txtLongCorrection.Name = "txtLongCorrection";
            this.txtLongCorrection.Size = new System.Drawing.Size(48, 20);
            this.txtLongCorrection.TabIndex = 5;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(188, 40);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(73, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Lat Correction";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(321, 40);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(82, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Long Correction";
            // 
            // txtAltitudeCorrection
            // 
            this.txtAltitudeCorrection.Location = new System.Drawing.Point(562, 40);
            this.txtAltitudeCorrection.Name = "txtAltitudeCorrection";
            this.txtAltitudeCorrection.Size = new System.Drawing.Size(40, 20);
            this.txtAltitudeCorrection.TabIndex = 8;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(463, 42);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(93, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Altitude Correction";
            // 
            // frmAirWulf
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1016, 741);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.txtAltitudeCorrection);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtLongCorrection);
            this.Controls.Add(this.txtLatCorrection);
            this.Controls.Add(this.btnReloadGMC);
            this.Controls.Add(this.lblComPort);
            this.Controls.Add(this.cbxComPortSelect);
            this.Controls.Add(this.btnConnect);
            this.MinimumSize = new System.Drawing.Size(1024, 768);
            this.Name = "frmAirWulf";
            this.Text = "AirWulf";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.ComboBox cbxComPortSelect;
        private System.Windows.Forms.Label lblComPort;
        private System.Windows.Forms.Button btnReloadGMC;
        private System.Windows.Forms.Timer RequestInfoTimer;
        private System.Windows.Forms.Timer PacketRXTimer;
        private System.Windows.Forms.TextBox txtLatCorrection;
        private System.Windows.Forms.TextBox txtLongCorrection;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtAltitudeCorrection;
        private System.Windows.Forms.Label label3;
    }
}

