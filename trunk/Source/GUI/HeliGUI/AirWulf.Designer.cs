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
            // frmAirWulf
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1016, 741);
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
    }
}

