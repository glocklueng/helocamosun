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
    /// Parent class for guages
    /// </summary>
    public partial class Gauge : UserControl
    {
        /// <summary>
        /// Background color of the gauge
        /// </summary>
        public Color BGColor = Color.Silver;
        /// <summary>
        /// Color of ring surrounding gauge
        /// </summary>
        public Color DialOutlineColor = Color.DarkGray;
        //color of the 4 corner screws
        public Color ScrewColor = Color.Black;
        /// <summary>
        /// Color of gauge information surface
        /// </summary>
        public Color GaugeSurfaceColor = Color.Black;
        
        private float _DialOutlineWidth = 10F;

        //Gauge surface dimensions
        /// <summary>
        /// x coordinate of corner where the guage surface is rendered from
        /// </summary>
        protected float UpperLeftCornerX;
        /// <summary>
        /// y coordinate of corner where the guage surface is rendered from
        /// </summary>
        protected float UpperLeftCornerY;
        /// <summary>
        /// Width of gauge, this value is dynamically adjusted depending on the size of the parent control
        /// </summary>
        protected float GaugeWidth;
        /// <summary>
        /// Height of gauge, this value is dynamically adjusted depending on the size of the parent control
        /// </summary>
        protected float GaugeHeight;
        /// <summary>
        /// Thickness of gauge outline ring, this value is dynamically adjusted depending on the size of the parent control
        /// </summary>
        public float DialOutlineWidth
        {
            set { _DialOutlineWidth = value; }
            get { return _DialOutlineWidth; }
        }
        /// <summary>
        /// Create a new gauge with the default parameters
        /// </summary>
        public Gauge()
        {
            InitializeComponent();
        }
        /// <summary>
        /// Draw the basic gauge with 4 screws, a ring and the guage surface.
        /// </summary>
        /// <param name="e">paint event args</param>
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            e.Graphics.Clear(BGColor);
            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(Color.Black);

            DrawGuageSurface(myGraphics, myPen);
            DrawGaugeOutline(myGraphics, myPen);

            DrawScrews(myGraphics, myPen);
            myPen.Dispose();
        }
        private void DrawGuageSurface(Graphics myGraphics, Pen myPen)
        {
            myPen.Color = GaugeSurfaceColor;
            UpperLeftCornerX = this.Size.Height/10 + _DialOutlineWidth / 2 * this.Size.Height / 150;
            UpperLeftCornerY = this.Size.Height / 10 + _DialOutlineWidth / 2 * this.Size.Height / 150;
            GaugeWidth = this.Size.Width - (this.Size.Height / 5 + _DialOutlineWidth * this.Size.Height / 150);
            GaugeHeight = GaugeWidth;

            myGraphics.FillEllipse(myPen.Brush, UpperLeftCornerX, UpperLeftCornerY, GaugeWidth, GaugeHeight);
        }
        private void DrawGaugeOutline(Graphics myGraphics, Pen myPen)
        {
            myPen.Width = _DialOutlineWidth * this.Size.Width / 150;
            float GaugeOutLineWidth = GaugeWidth + myPen.Width;
            float GaugeOutLineHeight = GaugeOutLineWidth;
            myPen.Color = DialOutlineColor;

            myGraphics.DrawEllipse(myPen, UpperLeftCornerX - myPen.Width / 2, UpperLeftCornerY - myPen.Width / 2, GaugeOutLineWidth, GaugeOutLineHeight);
        }
        private void DrawScrews(Graphics myGraphics, Pen myPen)
        {
            float ScrewWidth = this.Size.Width / 7.5F;
            float ScrewHeight = ScrewWidth;
            myPen.Color = ScrewColor;
            myGraphics.FillEllipse(myPen.Brush,
                4,
                4,
                ScrewWidth,
                ScrewHeight);//upper left corner screw

            myGraphics.FillEllipse(myPen.Brush,
                this.Size.Width - (4 + this.Size.Width / 7.5F),
                4,
                ScrewWidth,
                ScrewHeight);//upper right corner screw

            myGraphics.FillEllipse(myPen.Brush,
                this.Size.Width - (4 + this.Size.Width / 7.5F),
                this.Size.Height - (4 + this.Size.Height / 7.5F),
                ScrewWidth,
                ScrewHeight);//lower right corner screw

            myGraphics.FillEllipse(myPen.Brush, 4,
                this.Size.Height - (4 + this.Size.Height / 7.5F),
                ScrewWidth,
                ScrewHeight);//lower left corner screw
        }
    }
    
}