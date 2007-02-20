using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace helopanel
{
    public partial class Gauge : UserControl
    {
        public enum GaugeType
        {
            CentralDial,
            OffSetDial,
            BCD,
            Compass
        }
        public Color BGColor = Color.Silver;
        public Color DialOutlineColor = Color.Black;
        public Color ScrewColor = Color.Black;
        public Color GaugeSurfaceColor = Color.Black;
        private float _DialOutlineWidth = 10F;
        public float DialOutlineWidth
        {
            set { _DialOutlineWidth = value; }
            get { return _DialOutlineWidth; }
        }
        public Gauge()
        {
            InitializeComponent();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            e.Graphics.Clear(BGColor);
            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(DialOutlineColor, _DialOutlineWidth*this.Size.Height/150);

            DrawGaugeOutline(myGraphics, myPen);
            DrawGuageSurface(myGraphics, myPen);
            DrawScrews(myGraphics, myPen);
        }
        private void DrawGuageSurface(Graphics myGraphics, Pen myPen)
        {
            myPen.Color = GaugeSurfaceColor;
            float UpperLeftCornerX = 10 + _DialOutlineWidth / 2 * this.Size.Height / 150;
            float UpperLeftCornerY = 10 + _DialOutlineWidth / 2 * this.Size.Height / 150;
            float Width =this.Size.Width - (20 + _DialOutlineWidth * this.Size.Height / 150);
            float Height = this.Size.Height - (20 + _DialOutlineWidth * this.Size.Height / 150);

            myGraphics.FillEllipse(myPen.Brush, UpperLeftCornerX, UpperLeftCornerY, Width, Height);

        }
        private void DrawGaugeOutline(Graphics myGraphics, Pen myPen)
        {
            myPen.Color = DialOutlineColor;
            myGraphics.DrawEllipse(myPen, 10, 10, this.Size.Width - 20, this.Size.Height - 20);
        }
        private void DrawScrews(Graphics myGraphics, Pen myPen)
        {
            myPen.Color = ScrewColor;
            myGraphics.FillEllipse(myPen.Brush,
                4,
                4,
                this.Size.Width / 7.5F,
                this.Size.Height / 7.5F);//upper left corner screw

            myGraphics.FillEllipse(myPen.Brush,
                this.Size.Width - (4 + this.Size.Width / 7.5F),
                4,
                this.Size.Width / 7.5F,
                this.Size.Height / 7.5F);//upper right corner screw

            myGraphics.FillEllipse(myPen.Brush,
                this.Size.Width - (4 + this.Size.Width / 7.5F),
                this.Size.Height - (4 + this.Size.Height / 7.5F),
                this.Size.Width / 7.5F,
                this.Size.Height / 7.5F);//lower right corner screw

            myGraphics.FillEllipse(myPen.Brush, 4,
                this.Size.Height - (4 + this.Size.Height / 7.5F),
                this.Size.Width / 7.5F,
                this.Size.Height / 7.5F);//lower left corner screw
        }
    }
    public class BCDIndicator
    {


    }
}