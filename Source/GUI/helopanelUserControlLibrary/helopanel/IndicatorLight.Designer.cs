namespace helopanel
{
    partial class IndicatorLight
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.BlinkTimer = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // BlinkTimer
            // 
            this.BlinkTimer.Enabled = true;
            this.BlinkTimer.Interval = 500;
            this.BlinkTimer.Tick += new System.EventHandler(this.BlinkTimer_Tick);
            // 
            // IndicatorLight
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.Name = "IndicatorLight";
            this.Size = new System.Drawing.Size(100, 33);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Timer BlinkTimer;
    }
}
