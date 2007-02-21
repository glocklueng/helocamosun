using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace helopanel
{
    public class CenterDial : Gauge
    {
        private int MajorTickDegrees;
        private int MinorTickDegrees;
        private int MajorTickLength;
        private int MinorTickLength;

        private int min;
        private int max;

        private int ZeroAngle;

        private int value = 100;

        private Color tickColor = Color.YellowGreen;
        private Color NeedleColor = Color.Orange;

        private bool RotationDirection = true; //clockwise
        public CenterDial()
        {
            this.MajorTickDegrees = 30;
            this.MinorTickDegrees = 5;
            this.min = 0;
            this.max = 120;
            this.ZeroAngle = 180;
        }
        public CenterDial(int max, int min, int MajorTickDegrees, int MinorTickDegrees, int ZeroAngle, bool RotationDirection)
        {
            this.MajorTickDegrees = MajorTickDegrees;
            this.MinorTickDegrees = MinorTickDegrees;
            this.min = min;
            this.max = max;
            this.RotationDirection = RotationDirection;
            this.ZeroAngle = ZeroAngle;
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            MinorTickLength = 5 * this.Size.Width / 150;
            MajorTickLength = 10 * this.Size.Width / 150;
            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(DialOutlineColor, 1.0f);
            DrawMajorTicks(myGraphics, myPen);
            DrawMinorTicks(myGraphics, myPen);
            DrawNeedle(GetAngleFromValue(value), myGraphics, myPen);

            myPen.Dispose();
        }
        private void DrawMajorTicks(Graphics myGraphics, Pen myPen)
        {
            float Diameter = GaugeWidth;//assumes circle
            for (int CurrentAngle = ZeroAngle; CurrentAngle < 360 + ZeroAngle; CurrentAngle += MajorTickDegrees)
            {
                myPen.Color = tickColor;
                myPen.Width = 2.0f;
                Point InnerPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                            Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                Point OuterPoint = new Point(-Convert.ToInt32((Diameter / 2 - 0.5 * MajorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                            Convert.ToInt32((Diameter / 2 - 0.5 * MajorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                Point RotatedPoint = new Point(-Convert.ToInt32((Diameter / 2 - 0.8 * MajorTickLength) * Math.Cos((CurrentAngle - MajorTickDegrees / 3) * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                            Convert.ToInt32((Diameter / 2 - 0.8 * MajorTickLength) * Math.Sin((CurrentAngle - MajorTickDegrees / 3) * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);


                //draw the labels on the major ticks
                StringFormat stringFormat = new StringFormat();
                stringFormat.Alignment = StringAlignment.Center;
                stringFormat.LineAlignment = StringAlignment.Center;
                myGraphics.DrawString(GetValueFromAngle(CurrentAngle).ToString(), new Font(FontFamily.GenericSansSerif, 5f * this.Size.Width / 150, FontStyle.Regular), myPen.Brush, RotatedPoint.X, RotatedPoint.Y, stringFormat);

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
            Point InnerPoint = new Point(Convert.ToInt32(UpperLeftCornerX + Diameter / 2), Convert.ToInt32(UpperLeftCornerY + Diameter / 2));//centre
            Point OuterPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength + MinorTickLength) * Math.Cos(angle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                            Convert.ToInt32((Diameter / 2 - 2 * MajorTickLength + MinorTickLength) * Math.Sin(angle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
            myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);
        }
        private int GetAngleFromValue(int value)
        {
            int angle = 0;
            float intercept = (float)360 - (float)360 / (float)(max - min) * (float)max + (float)ZeroAngle;
            if (RotationDirection)
            {
                angle = (int)(((float)360 / (float)(max - min)) * (float)value + intercept);

            }
            else
            {
                angle = 360-(int)(((float)360 / (float)(max - min)) * (float)value + intercept);

            }
            return angle;
        }
        private int GetValueFromAngle(int angle)
        {
            int value = 0;
            float intercept = (float)360 - (float)360 / (float)(max - min) * (float)max + (float)ZeroAngle;
            if (RotationDirection)
            {
                value = (int)(((float)angle - intercept) * (float)(max - min) / (float)360);
            }
            else
            {
                value = max-(int)(((float)angle - intercept) * (float)(max - min) / (float)360);
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
