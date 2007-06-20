using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace helopanel
{
    public enum NeedleType
    {
        Simple

    }
    public class RedLineRange
    {
        public float LowerBound = 40;
        public float UpperBound = 50;
        public Color MajorTickColor = Color.Red;
        public Color MinorTickColor = Color.Red;
        public Color TickLabelColor = Color.Red;
        public bool OverrideMinorTickColor = true;
        public bool OverrideMajorTickColor = true;
        public bool OverrideTickLabelColor = true;
    }
    /// <summary>
    /// A dynamic, GDI rendered dial gauge with many features.
    /// </summary>
    public class CenterDial : Gauge
    {
        #region data members
        private float MajorTickWidth = 1.5f;
        private float MinorTickWidth = 1f;
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
        public int Min
        {
            set { this.min = value; }
            get { return min; }
        }
        private int min = -360;
        /// <summary>
        /// Maximum value supported by the dial
        /// </summary>
        public int Max
        {
            set { this.max = value; }
            get { return max; }
        }
        private int max = 360;
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
            set { LowAngle = value; }
            get { return LowAngle; }
        }
        private int LowAngle = 0;
        private float value = 5.7f;
        public float Value
        {
            set { this.Invalidate(); this.value = value; ValueIndicator.SetToolTip(this, value.ToString()); }
            get { return value; }
        }
        public float TickBaseArcMin = 0;
        public float TickBaseArcMax = 0;
        /// <summary>
        /// Value after which no ticks or tick labels will be drawn.
        /// </summary>
        public int MaxDisplayedValue = 360;
        /// <summary>
        /// Value before which no ticks or tick labels will be drawn.
        /// </summary>
        public int MinDisplayedValue = -250;
        /// <summary>
        /// Color of the major ticks
        /// </summary>
        public Color MajorTickColor = Color.White;
        /// <summary>
        /// Color of the major tick labels
        /// </summary>
        public Color TickLabelColor = Color.White;
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
        /// Color of the tick base arc (not yet implemented)
        /// </summary>
        private Color TickBaseArcColor = Color.Green;     
        /// <summary>
        /// If true the dial values increase in the counter-clockwise direction, otherwise clockwise.
        /// </summary>
        public bool CounterClockWise = false;
        private ArrayList RedLineRanges = new ArrayList();
        
        public NeedleType needleType = NeedleType.Simple;

        private ToolTip ValueIndicator = new ToolTip();
        #endregion
        #region contstructors
        /// <summary>
        /// Create a new Centerdial with the default settings
        /// </summary>
        public CenterDial()
        {
            ValueIndicator.SetToolTip(this, Value.ToString());


        }
        /// <summary>
        /// Create a new Centerdial with the default color and needle style and the specified settings
        /// </summary>
        /// <param name="MajorTickDegrees">Degrees in between major ticks</param>
        /// <param name="MajorTickLength">Length of major tick marks, in pixels, at the default control size (150 by 150)</param>
        /// <param name="MinorTickDegrees">Degrees in between minor ticks</param>
        /// <param name="MinorTickLength">Length of minor tick marks, in pixels, at the default control size (150 by 150)</param>
        /// <param name="Min">The dial's lowest value</param>
        /// <param name="MinDisplayedValue">The dial's lowest displayed value</param>
        /// <param name="Max">The dial's highest value</param>
        /// <param name="MaxDisplayedValue">The dial's highest displayed value</param>
        /// <param name="LowestValueAngle">The angle at which the lowest displayed value is rendered</param>
        /// <param name="MajorLabel">The major text label to show above the center of the gauge (e.g. Temperature)</param>
        /// <param name="MinorLabel">The minor text label to show below the major label (e.g. deg Kelvin)</param>
        /// <param name="CounterClockWise">if True the dial's value increases in the counterclockwise direction, otherwise it decreases</param>
        /// <param name="ranges">Array of RedLineRanges to define tick values that have a different color</param>
        public CenterDial(int MajorTickDegrees, int MajorTickLength, int MinorTickDegrees, int MinorTickLength,
                            int Min, int MinDisplayedValue, int Max, int MaxDisplayedValue, int LowestValueAngle,
                            string MajorLabel, string MinorLabel, bool CounterClockWise, RedLineRange[] ranges
                            )
        {
            this.MajorTickDegrees = MajorTickDegrees;
            this.MajorTickLength = MajorTickLength;
            this.MinorTickDegrees = MinorTickDegrees;
            this.MinorTickLength = MinorTickLength;
            this.Min = Min;
            this.MinDisplayedValue = MinDisplayedValue;
            this.Max = Max;
            this.MaxDisplayedValue = MaxDisplayedValue;
            this.LowestValueAngle = LowestValueAngle;
            this.MajorLabel = MajorLabel;
            this.MinorLabel = MinorLabel;
            this.CounterClockWise = CounterClockWise;
            foreach (RedLineRange r in ranges)
            {
                RedLineRanges.Add(r);
            }
            ValueIndicator.SetToolTip(this, Value.ToString());
        }
        /// <summary>
        /// Create a new Centerdial with specified settings
        /// </summary>
        /// <param name="MajorTickDegrees">Degrees in between major ticks</param>
        /// <param name="MajorTickLength">Length of major tick marks, in pixels, at the default control size (150 by 150)</param>
        /// <param name="MinorTickDegrees">Degrees in between minor ticks</param>
        /// <param name="MinorTickLength">Length of minor tick marks, in pixels, at the default control size (150 by 150)</param>
        /// <param name="Min">The dial's lowest value</param>
        /// <param name="MinDisplayedValue">The dial's lowest displayed value</param>
        /// <param name="Max">The dial's highest value</param>
        /// <param name="MaxDisplayedValue">The dial's highest displayed value</param>
        /// <param name="LowestValueAngle">The angle at which the lowest displayed value is rendered</param>
        /// <param name="MajorLabel">The major text label to show above the center of the gauge (e.g. Temperature)</param>
        /// <param name="MinorLabel">The minor text label to show below the major label (e.g. deg Kelvin)</param>
        /// <param name="CounterClockWise">if True the dial's value increases in the counterclockwise direction, otherwise it decreases</param>
        /// <param name="ranges">Array of RedLineRanges to define tick values that have a different color</param>
        /// <param name="needleType">Type of needle to render</param>
        /// <param name="LabelColor">Color of the Major and minor labels</param>
        /// <param name="MajorTickColor">Color of the major ticks</param>
        /// <param name="MinorTickColor">Color of the minor ticks</param>
        /// <param name="NeedleColor">Color of the needle</param>
        /// <param name="BackGroundColor">Background color of the gauge</param>
        /// <param name="GaugeRingColor">Color of ring surrounding gauge</param>
        /// <param name="ScrewColor">color of the 4 corner screws</param>
        /// <param name="GaugeSurfaceColor">Color of gauge information surface</param>

        public CenterDial(  int MajorTickDegrees, int MajorTickLength, int MinorTickDegrees, int MinorTickLength,
                            int Min, int MinDisplayedValue, int Max, int MaxDisplayedValue, int LowestValueAngle,
                            string MajorLabel, string MinorLabel,bool CounterClockWise, RedLineRange[] ranges, NeedleType needleType,
                            Color LabelColor, Color MajorTickColor, Color MinorTickColor,Color NeedleColor, Color BackGroundColor,
                            Color GaugeRingColor, Color ScrewColor, Color GaugeSurfaceColor
                            )
                            
            
        {
            this.MajorTickDegrees = MajorTickDegrees;
            this.MajorTickLength = MajorTickLength;
            this.MinorTickDegrees =MinorTickDegrees;
            this.MinorTickLength=MinorTickLength;
            this.Min=Min;
            this.MinDisplayedValue=MinDisplayedValue;
            this.Max=Max;
            this.MaxDisplayedValue=MaxDisplayedValue;
            this.LowestValueAngle=LowestValueAngle;
            this.MajorLabel=MajorLabel;
            this.MinorLabel=MinorLabel;
            this.CounterClockWise = CounterClockWise;
            foreach (RedLineRange r in ranges)
            {
                RedLineRanges.Add(r);
            }
            this.needleType = needleType;
            this.LabelColor = LabelColor;
            this.MajorTickColor = MajorTickColor;
            this.MinorTickColor = MinorTickColor;
            this.NeedleColor = NeedleColor;
            this.BGColor = BackGroundColor;
            this.DialOutlineColor = GaugeRingColor;
            this.ScrewColor = ScrewColor;
            this.GaugeSurfaceColor = GaugeSurfaceColor;

            ValueIndicator.SetToolTip(this, Value.ToString());
        }


        #endregion
        #region methods
        /// <summary>
        /// This event draws the ticks, labels and needle of the dial gauge.
        /// </summary>
        /// <param name="e">paint event args</param>
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            ScaledMinorTickLength = MinorTickLength * this.Size.Width / 150;
            ScaledMajorTickLength = MajorTickLength * this.Size.Width / 150;
            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(DialOutlineColor, 1.0f);

            DrawMinorTicks(myGraphics, myPen);
            DrawMajorTicks(myGraphics, myPen);
            DrawLabels(myGraphics, myPen);
            DrawNeedle(GetAngleFromValue(value), myGraphics, myPen);


            myPen.Dispose();
        }
        /// <summary>
        /// Add a RedLineRange that defines values where ticks have a different color
        /// </summary>
        /// <param name="r">RedLineRange that defines tick values that have a different color</param>
        public void AddRedLineRange(RedLineRange r)
        {
            RedLineRanges.Add(r);
        }
        /// <summary>
        /// Remove a RedLineRange
        /// </summary>
        /// <param name="r">RedLineRange to remove</param>
        public void RemoveRedLineRange(RedLineRange r)
        {
            RedLineRanges.Remove(r);
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
            float ScaleFactor = this.Size.Width / 150;
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
                    Point InnerPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                                Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                    Point OuterPoint = new Point(-Convert.ToInt32((Diameter / 2 - 0.5 * ScaledMajorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                                Convert.ToInt32((Diameter / 2 - 0.5 * ScaledMajorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                    Point RotatedPoint = new Point(-Convert.ToInt32((Diameter / 2 - 0.8 * ScaledMajorTickLength) * Math.Cos((CurrentAngle - MajorTickDegrees / 3) * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                                Convert.ToInt32((Diameter / 2 - 0.8 * ScaledMajorTickLength) * Math.Sin((CurrentAngle - MajorTickDegrees / 3) * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));

                    StringFormat stringFormat = new StringFormat();
                    stringFormat.Alignment = StringAlignment.Center;
                    stringFormat.LineAlignment = StringAlignment.Center;

                    bool TickDrawn = false;
                    bool LabelDrawn = false;
                    foreach (RedLineRange r in RedLineRanges)
                    {
  
                        if (Value >= r.LowerBound && Value <= r.UpperBound)
                        {
                            if (r.OverrideMajorTickColor && !TickDrawn)
                            {
                                myPen.Color = r.MajorTickColor;
                                myPen.Width = MajorTickWidth * ScaleFactor;
                                myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);
                                TickDrawn = true;
                            }
                            if (r.OverrideTickLabelColor && !LabelDrawn)
                            {
                                myPen.Color = r.TickLabelColor;
                                myGraphics.DrawString(sValue, new Font(FontFamily.GenericSansSerif, 5f * this.Size.Width / 150, FontStyle.Regular), myPen.Brush, RotatedPoint.X, RotatedPoint.Y, stringFormat);
                                LabelDrawn = true;
                            }
                        }
                        if (TickDrawn && LabelDrawn)
                        {
                            break;
                        }
                    }
                    if (!TickDrawn)
                    {
                        //draw the default tick
                        myPen.Color = MajorTickColor;
                        myPen.Width = MajorTickWidth * ScaleFactor;
                        myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);
                    }
                    if (!LabelDrawn)
                    {
                        //draw the default label
                        myPen.Color = TickLabelColor;
                        myGraphics.DrawString(sValue, new Font(FontFamily.GenericSansSerif, 5f * this.Size.Width / 150, FontStyle.Regular), myPen.Brush, RotatedPoint.X, RotatedPoint.Y, stringFormat);

                    }

                }
            }

        }
        private void DrawMinorTicks(Graphics myGraphics, Pen myPen)
        {
            float ScaleFactor = this.Size.Width / 150;
            float Diameter = GaugeWidth;//assumes circle
            for (int CurrentAngle = LowAngle; CurrentAngle < 360 + LowAngle; CurrentAngle += MinorTickDegrees)
            {
                float Value = GetValueFromAngle(CurrentAngle);
                if (Value <= MaxDisplayedValue && Value >= MinDisplayedValue)
                {

                    Point InnerPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                            Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));

                    Point OuterPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength + ScaledMinorTickLength) * Math.Cos(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                                Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength + ScaledMinorTickLength) * Math.Sin(CurrentAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));


                    bool TickDrawn = false;
                    foreach (RedLineRange r in RedLineRanges)
                    {
                        if (Value >= r.LowerBound && Value <= r.UpperBound)
                        {
                            if (r.OverrideMajorTickColor && !TickDrawn)
                            {
                                myPen.Color = r.MinorTickColor;
                                myPen.Width = MinorTickWidth * ScaleFactor;
                                myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);
                                TickDrawn = true;
                                break;
                            }
                        }
                    }
                    if (!TickDrawn)
                    {
                        //draw the default tick
                        myPen.Color = MajorTickColor;
                        myPen.Width = MinorTickWidth * ScaleFactor;
                        myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);
                    }


                }
            }
        }
        private void DrawNeedle(float angle, Graphics myGraphics, Pen myPen)
        {
            float RotatedAngle = angle + LowestValueAngle;
            myPen.Color = NeedleColor;

            if (needleType == NeedleType.Simple)
            {
                myPen.Width = 2.0f * this.Size.Width /150;
                float Diameter = GaugeWidth;
                Point InnerPoint = new Point(Convert.ToInt32(UpperLeftCornerX + Diameter / 2), Convert.ToInt32(UpperLeftCornerY + Diameter / 2));//centre
                Point OuterPoint = new Point(-Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength + ScaledMinorTickLength) * Math.Cos(RotatedAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerX + Diameter / 2),
                                                Convert.ToInt32((Diameter / 2 - 2 * ScaledMajorTickLength + ScaledMinorTickLength) * Math.Sin(RotatedAngle * 2 * Math.PI / 360)) + Convert.ToInt32(UpperLeftCornerY + Diameter / 2));
                myGraphics.DrawLine(myPen, InnerPoint, OuterPoint);
            }
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
                angle = -(float)360 / (float)(max - min)*(float)value + (float)360 / (float)(max - min) * (float)max;

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
                value = max + min - ((float)angle - intercept) * (float)(max - min) / (float)360;
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
        #endregion
    }
        
}
