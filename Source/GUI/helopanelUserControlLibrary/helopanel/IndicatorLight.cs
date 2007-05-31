using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace helopanel
{
    /// <summary>
    /// This is a common indicator light, normally it is grey, but it will change colour and show text when turned on
    /// </summary>
    public partial class IndicatorLight : UserControl
    {
        /// <summary>
        /// The background color of the control in the on state
        /// </summary>
        public Color OnColor = Color.Orange;
        /// <summary>
        /// the background color of the control in the off state
        /// </summary>
        public Color OffColor = Color.Black;
        /// <summary>
        /// The text color when the control is on, should have good contrast with the OnColor property
        /// </summary>
        public Color TextColor = Color.Black;
        /// <summary>
        /// The text to display when the IndicatorLight's enabled property is set to true;
        /// </summary>
        public string WarningText = "Warning";
        private bool oneshot = false;
        /// <summary>
        /// If set true the indicator will blink only once if the Blink property is set true
        /// </summary>
        public bool OneShot
        {
            set { oneshot = value; }
            get { return oneshot; }
        }
        /// <summary>
        /// If set to true the control will light up and show the Warning text, or it will blink if the Blink property is set to true. 
        /// If it is set to false it will be blank.
        /// </summary>
        public bool On
        {
            set 
            {
                on = value; 
                this.Invalidate();
            }
            get { return on; }
        }
        private bool on = false;
        /// <summary>
        /// If set to true the control will blink on and off at the period specified by BlinkRate
        /// </summary>
        public bool Blink
        {
            set
            {
                blink = value;
                if (blink)
                {
                    On = true;
                }
            }
            get { return blink; }
        }
        private bool blink = false;

        /// <summary>
        /// The rate, in milliseconds that the control will blink if its On property is set to true.
        /// </summary>
        public int BlinkRate
        {
            set
            {
                blinkRate = value;
                BlinkTimer.Interval = value;
            }
            get 
            { 
                return blinkRate;
            }
        }
        private int blinkRate = 500;
        /// <summary>
        /// Standard constructor, you will need to set the parameters manually if you use this.
        /// </summary>
        public IndicatorLight()
        {
            InitializeComponent();
            // Activates double buffering so the controls dont flicker
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true); 
        }
        public IndicatorLight(string WarningText)
        {
            InitializeComponent();

            this.WarningText = WarningText;
            // Activates double buffering so the controls dont flicker
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true); 
        }

        private void BlinkTimer_Tick(object sender, EventArgs e)
        {
            BlinkTimer.Interval = blinkRate;
            if (blink)
            {
                if (OneShot)
                {
                    blink = false;
                }
               on ^= true;

               this.Invalidate();

            }
            
        }
        /// <summary>
        /// The indicator light's overrided paint event
        /// </summary>
        /// <param name="e">paint event args</param>
        protected override void OnPaint(PaintEventArgs e)
        {

            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(Color.Black);


            if (on)
            {
                myPen.Color = OnColor;
                myGraphics.FillRectangle(myPen.Brush,new Rectangle(new Point(0,0),this.Size));
                myPen.Color = TextColor;
                StringFormat stringFormat = new StringFormat();
                stringFormat.Alignment = StringAlignment.Center;
                stringFormat.LineAlignment = StringAlignment.Center;

                myGraphics.DrawString(
                    WarningText,
                    new Font(FontFamily.GenericSansSerif, 8.5f * this.Size.Width / 100, FontStyle.Bold),
                    myPen.Brush,
                    this.Width/2.0f,
                    this.Height/2.0f,
                    stringFormat
                    );

            }
            else
            {
                myPen.Color = OffColor;
                myGraphics.FillRectangle(myPen.Brush, new Rectangle(new Point(0, 0), this.Size));
            }
            base.OnPaint(e);
        }
    }
}
