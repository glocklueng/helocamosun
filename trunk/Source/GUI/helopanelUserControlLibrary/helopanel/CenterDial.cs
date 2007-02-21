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
        /// <summary>
        /// Number of degrees between major ticks.
        /// </summary>
        public int MajorTickDegrees = 30;
        /// <summary>
        /// Number of degrees between minor ticks.
        /// </summary>
        public int MinorTickDegrees = 6;
        /// <summary>
        /// Length, in pixels of major ticks, at the default control size.
        /// </summary>
        public int MajorTickLength = 10;
        private int ScaledMajorTickLength;
        /// <summary>
        /// Length, in pixels of minor ticks, at the default control size.
        /// </summary>
        public int MinorTickLength = 5;
        private int ScaledMinorTickLength;
        /// <summary>
        /// Minumum value supported by the dial
        /// </summary>
        public int min = -360;
        /// <summary>
        /// Maximum value supported by the dial
        /// </summary>
        public int max = 360;
        /// <summary>
        /// Large label above the center of the dial gauge
        /// </summary>
        public string MajorLabel = "Temperature";
        /// <summary>
        /// Small label under the MajorLabel
        /// </summary>
        public string MinorLabel = "deg C";
        /// <summary>
        /// Angle where the lowest value is shown.
        /// </summary>
        public int LowestValueAngle
        {
            set { LowAngle = value + 180; }
            get { return LowAngle -180; }
        }
        private int LowAngle = 0;
        private float value = 5.7f;
        /// <summary>
        /// Value after which no ticks or tick labels will be drawn.
        /// </summary>
        public int MaxDisplayedValue = 360;
        /// <summary>
        /// Value before which no ticks or tick labels will be drawn.
        /// </summary>
        public int MinDisplayedValue = -250;
        /// <summary>
        /// Value after which the ticks and tick lables will be drawn in the RedLineColor.
        /// </summary>
        public int RedLineThreshold = 180;
        /// <summary>
        /// Color of the major ticks
        /// </summary>
        public Color MajorTickColor = Color.White;
        /// <summary>
        /// Color of the minor ticks
        /// </summary>
        public Color MinorTickColor = Color.Green;
        /// <summary>
        /// color of the needle
        /// </summary>
        public Color NeedleColor = Color.Orange;
        /// <summary>
        /// Color of the dial's label
        /// </summary>
        public Color LabelColor = Color.Yellow;
        /// <summary>
        /// Color of the ticks after the red-line threshold
        /// </summary>
        public Color RedLineColor = Color.Red;
        /// <summary>
        /// Color of the tick base arc
        /// </summary>
        public Color TickBaseArcColor = Color.Green;     
        /// <summary>
        /// If true the dial values increase in the counter-clockwise direction, otherwise clockwise.
        /// </summary>
        public bool CounterClockWise = true;

        public CenterDial()
        {
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            ScaledMinorTickLength = MinorTickLength * this.Size.Width / 150;
            ScaledMajorTickLength = MajorTickLength * this.Size.Width / 150;
            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(DialOutlineColor, 1.0f);
           // DrawTickBaseArc(myGraphics, myPen);
            DrawMinorTicks(myGraphics, myPen);
            DrawMajorTicks(myGraphics, myPen);
            DrawLabels(myGraphics, myPen);
            DrawNeedle(GetAngleFromValue(value), myGraphics, myPen);


            myPen.Dispose();
        }
        private void DrawLabels(Graphics myGraphics, Pen myPen)
        {
            myPen.Color = LabelColor;
            //draw the major label
            StringFormat stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;
            myGraphics.DrawString(MajorLabel, new Font(FontFamily.GenericSansSerif, 6f * this.Size.Width / 150, FontStyle.Bold), myPen.Brush, UpperLeftCornerY + GaugeWidth / 2, UpperLeftCornerY + GaugeWidth / 3, stringFormat);

            //draw the minor label
            stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;
            myGraphics.DrawString(MinorLabel, new Font(FontFamily.GenericSansSerif, 5f * this.Size.Width / 150, FontStyle.Bold), myPen.Brush, UpperLeftCornerY + GaugeWidth / 2, UpperLeftCornerY + GaugeWidth / 2.5f, stringFormat);

        }
        private void DrawTickBaseArc(Graphics myGraphics, Pen myPen)
        {
            myPen.Color = TickBaseArcColor;
            myPen.Width = 1f;
            myGraphics.DrawArc(myPen, UpperLeftCornerX + ScaledMajorTickLength*2, UpperLeftCornerY + ScaledMajorTickLength*2, GaugeWidth - 4*ScaledMajorTickLength, GaugeWidth - 4*ScaledMajorTickLength, 90f, 180f);

        }
        private void DrawMajorTicks(Graphics myGraphics, Pen myPen)
        {
            float Diameter = GaugeWidth;//assumes circle
            for (int CurrentAngle = LowAngle; CurrentAngle <= 360 + LowAngle; CurrentAngle += MajorTickDegrees)
            {
                float Value = GetValueFromAngle(CurrentAngle);
                if (Value <= MaxDisplayedValue && Value >= MinDisplayedValue)
                {

                    string sValue;
                    if (Math.Abs(Value % 1) >= 0.05)
                    {
                        sValue = string.Format("{0:0.0}", Value);
                    }
                    else
                    {
                        sValue = string.Format("{0:0}", Value);
                    }
                    if (Value >= RedLineThreshold)
                    {
                        myPen.Color = RedLineColor;
                        myPen.Width = 2.0f;

                    }
                    else
                    {
                        myPen.Color = MajorTickColor;
                        myPen.Width = 2.0f;
                    }

                    Point InnerPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                                Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                    Point OuterPoint = new Point(-Convert.ToInt32((Diameter / 2 - 0.5 * ScaledMajorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                                Convert.ToInt32((Diameter / 2 - 0.5 * ScaledMajorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                    Point RotatedPoint = new Point(-Convert.ToInt32((Diameter / 2 - 0.8 * ScaledMajorTickLength) * Math.Cos((CurrentAngle - MajorTickDegrees / 3) * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                                Convert.ToInt32((Diameter / 2 - 0.8 * ScaledMajorTickLength) * Math.Sin((CurrentAngle - MajorTickDegrees / 3) * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                    myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);

                    //draw the labels on the major ticks
                    StringFormat stringFormat = new StringFormat();
                    stringFormat.Alignment = StringAlignment.Center;
                    stringFormat.LineAlignment = StringAlignment.Center;
                    myGraphics.DrawString(sValue, new Font(FontFamily.GenericSansSerif, 5f * this.Size.Width / 150, FontStyle.Regular), myPen.Brush, RotatedPoint.X, RotatedPoint.Y, stringFormat);
                }
            }

        }
        private void DrawMinorTicks(Graphics myGraphics, Pen myPen)
        {
            float Diameter = GaugeWidth;//assumes circle
            for (int CurrentAngle = LowAngle; CurrentAngle < 360 + LowAngle; CurrentAngle += MinorTickDegrees)
            {
                float Value = GetValueFromAngle(CurrentAngle);
                if (Value <= MaxDisplayedValue && Value >= MinDisplayedValue)
                {
                    if (Value >= RedLineThreshold)
                    {
                        myPen.Color = RedLineColor;
                        myPen.Width = 1.0f;

                    }
                    else
                    {
                        myPen.Color = MinorTickColor;
                        myPen.Width = 1.0f;
                    }

                    Point InnerPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                                Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));

                    Point OuterPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength + ScaledMinorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                                Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength + ScaledMinorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                    myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);
                }
            }
        }
        private void DrawNeedle(float angle, Graphics myGraphics, Pen myPen)
        {
            myPen.Color = NeedleColor;
            myPen.Width = 3.0f;
            float Diameter = GaugeWidth;
            Point InnerPoint = new Point(Convert.ToInt32(UpperLeftCornerX + Diameter / 2), Convert.ToInt32(UpperLeftCornerY + Diameter / 2));//centre
            Point OuterPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength + ScaledMinorTickLength) * Math.Cos(angle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                            Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength + ScaledMinorTickLength) * Math.Sin(angle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
            myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);
        }
        private float GetAngleFromValue(float value)
        {
            float angle = 0;
            float intercept = (float)360 - (float)360 / (float)(max - min) * (float)max + (float)LowAngle;
            if (CounterClockWise)
            {
                angle = ((float)360 / (float)(max - min)) * (float)value + intercept;

            }
            else
            {
                angle = -((float)360 / (float)(max - min)) * (float)value + intercept;

            }
            return angle;
        }
        private float GetValueFromAngle(int angle)
        {
            float value = 0;
            float intercept = (float)360 - (float)360 / (float)(max - min) * (float)max + (float)LowAngle;
            if (CounterClockWise)
            {
                value = ((float)angle - intercept) * (float)(max - min) / (float)360;
            }
            else
            {
                value = -((float)angle - intercept) * (float)(max - min) / (float)360;

            }

            return value;
        }
        /// <summary>
        /// Set the current value that the needle points to
        /// </summary>
        /// <param name="value">Value to set the needle to</param>
        public void SetValue(float value)
        {
            this.value = value;
            this.Invalidate();
        }
    }
}
