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
        public Color BGColor = Color.Silver;
        public Color DialOutlineColor = Color.DarkGray;
        public Color ScrewColor = Color.Black;
        public Color GaugeSurfaceColor = Color.Black;
        
        private float _DialOutlineWidth = 10F;

        //Gauge surface dimensions
        protected float UpperLeftCornerX;
        protected float UpperLeftCornerY;
        protected float GaugeWidth;
        protected float GaugeHeight;
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
    public class BCDIndicator : Gauge
    {
        public BCDIndicator()
        {

        }

    }
    public class CenterDial : Gauge
    {
        private int MajorTickDegrees;
        private int MinorTickDegrees;
        private int MajorTickLength;
        private int MinorTickLength;

        private int min;
        private int max;

        private int value = 100;

        private Color tickColor = Color.YellowGreen;
        private Color NeedleColor = Color.Orange;

        private bool RotationDirection = true; //clockwise
        public CenterDial()
        {
            this.MajorTickDegrees = 30;
            this.MinorTickDegrees = 5;
            this.min = 0;
            this.max = 200;
        }
        public CenterDial(int max, int min, int MajorTickDegrees, int MinorTickDegrees, int ZeroAngle, bool RotationDirection)
        {
            this.MajorTickDegrees = MajorTickDegrees;
            this.MinorTickDegrees = MinorTickDegrees;
            this.min = min;
            this.max = max;
            this.RotationDirection = RotationDirection;
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            MinorTickLength = 5 * this.Size.Width / 150;
            MajorTickLength = 10 * this.Size.Width / 150;
            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(DialOutlineColor,1.0f);
            DrawMajorTicks(myGraphics, myPen);
            DrawMinorTicks(myGraphics, myPen);
            DrawNeedle(GetAngleFromValue(value), myGraphics, myPen);

            myPen.Dispose();
        }
        private void DrawMajorTicks(Graphics myGraphics, Pen myPen)
        {
            float Diameter = GaugeWidth;//assumes circle
            for (int CurrentAngle = 0; CurrentAngle < 360; CurrentAngle += MajorTickDegrees)
            {
                myPen.Color = tickColor;
                myPen.Width = 2.0f;
                Point InnerPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                            Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                Point OuterPoint = new Point(-Convert.ToInt32((Diameter / 2 - 0.5 * MajorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                            Convert.ToInt32((Diameter / 2 - 0.5*MajorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                Point RotatedPoint = new Point(-Convert.ToInt32((Diameter / 2 - 0.8 * MajorTickLength) * Math.Cos((CurrentAngle-MajorTickDegrees/3) * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                            Convert.ToInt32((Diameter / 2 - 0.8 * MajorTickLength) * Math.Sin((CurrentAngle - MajorTickDegrees / 3) * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);
                StringFormat stringFormat = new StringFormat();
                stringFormat.Alignment = StringAlignment.Center;
                stringFormat.LineAlignment = StringAlignment.Center;
                myGraphics.DrawString(GetValueFromAngle(CurrentAngle).ToString(), new Font(FontFamily.GenericSansSerif, 5f * this.Size.Width / 150, FontStyle.Regular), myPen.Brush,RotatedPoint.X,RotatedPoint.Y, stringFormat);
            }
        }
        private void DrawMinorTicks(Graphics myGraphics, Pen myPen)
        {
            float Diameter = GaugeWidth;//assumes circle
            for (int CurrentAngle = 0; CurrentAngle < 360; CurrentAngle += MinorTickDegrees)
            {
                myPen.Color = tickColor;
                myPen.Width = 1.0f;
                Point InnerPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                            Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));

                Point OuterPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength + MinorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                            Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength + MinorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);
            }
        }
        private void DrawNeedle(int angle, Graphics myGraphics, Pen myPen)
        {
            myPen.Color = NeedleColor;
            myPen.Width = 3.0f;
            float Diameter = GaugeWidth;
            Point InnerPoint = new Point(Convert.ToInt32(UpperLeftCornerX + Diameter / 2),Convert.ToInt32(UpperLeftCornerY + Diameter / 2));//centre
            Point OuterPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength + MinorTickLength) * Math.Cos(angle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                            Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength + MinorTickLength) * Math.Sin(angle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
            myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);
        }
        private int GetAngleFromValue(int value)
        {
            int angle = 0;
            if (RotationDirection)
            {
                angle = (int)(((float)360/ (float)(max - min)) * (float)value + (float)180);

            }
            else
            {
                angle = -(int)(((float)360 / (float)(max - min)) * (float)value + (float)180);

            }
            return angle;
        }
        private int GetValueFromAngle(int angle)
        {
            int value = 0;
            if (RotationDirection)
            {
                value = (int)(((float)angle - (float)180) * (float)(max - min) / (float)360);
            }
            else
            {
                value = -(int)(((float)angle - (float)180) * (float)(max - min) / (float)360);
            }
            return value;
        }
        public void SetValue(int value)
        {
            this.value = value;
            this.Invalidate();

        }
    }
}