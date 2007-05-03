using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace helopanel
{
    public partial class Compass : Gauge
    {
        /// <summary>
        /// This is the reference, a picture of an airplane
        /// </summary>
        private Image AirPlane;

        private float angle = 0;
        /// <summary>
        /// The compass angle, a value between 0 and 360, rotating towards east as the value increases
        /// </summary>
        public float Angle
        {
            set
            { 
                this.Invalidate();
                angle = value;
            }
            get 
            {
                return angle;
            }
        }
        public Compass()
        {
            InitializeComponent();
            this.Size = new Size(350,350);
            AirPlane = Image.FromFile("CompassAirplane.jpg");
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(DialOutlineColor, 1.0f);

            DrawReference(myGraphics, myPen);
            DrawPerimeterLabels(myGraphics, myPen);
            DrawPerimeterTicks(myGraphics, myPen);

            myPen.Dispose();
        }
        private void DrawReference(Graphics myGraphics, Pen myPen)
        {
            Rectangle rect = new Rectangle(
                new Point(Convert.ToInt32(this.Width/4.2f),Convert.ToInt32(this.Height/5)), 
                new Size(Convert.ToInt32(this.Width/2f),Convert.ToInt32(this.Height/1.8f))
                );
            myGraphics.DrawImage(AirPlane, rect);
        }
        private void DrawPerimeterTicks(Graphics myGraphics, Pen myPen)
        {
            GraphicsPath gp;
            Matrix RotationTransform;
            PointF RotationPoint;
            myPen.Color = Color.White;

            float centerX = UpperLeftCornerX + GaugeWidth / 2;
            float centerY = UpperLeftCornerY + GaugeHeight / 2;
            float ScaleFactor = (float)this.Size.Width / 150;

            gp = new GraphicsPath();
            myPen.Width = 2f * ScaleFactor;


            for (int i = 0; i < 36; i++)
            {
                if (i % 3 > 0)
                {
                    gp.AddLine(
                        new Point(Convert.ToInt32(centerX), Convert.ToInt32(GaugeWidth / 10f)),
                        new Point(Convert.ToInt32(centerX), Convert.ToInt32(GaugeWidth / 6.75f))
                        );
                }
                else
                {
                    gp.AddLine(
                        new Point(Convert.ToInt32(centerX), Convert.ToInt32(GaugeWidth / 10f)),
                        new Point(Convert.ToInt32(centerX), Convert.ToInt32(GaugeWidth / 5.75f))
                        );
                }
                RotationTransform = new Matrix(1, 0, 0, 1, 0, 0); // rotation matrix
                RotationPoint = new PointF(centerX, centerY); // rotation point
                RotationTransform.RotateAt(10, RotationPoint);
                gp.Transform(RotationTransform);

                gp.CloseFigure();
            }

            RotationTransform = new Matrix(1, 0, 0, 1, 0, 0); // rotation matrix
            RotationPoint = new PointF(centerX, centerY); // rotation point
            RotationTransform.RotateAt(-angle, RotationPoint);
            gp.Transform(RotationTransform);
            myGraphics.DrawPath(myPen, gp);
            gp.Dispose();
        }
        private void DrawPerimeterLabels(Graphics myGraphics, Pen myPen )
        {
            GraphicsPath gp;
            Matrix RotationTransform;
            PointF RotationPoint;
            myPen.Color = Color.White;
            
            string[] Labels = {"N","33","30","W","24","21","S","15","12","E","6","3" };
            float centerX = UpperLeftCornerX + GaugeWidth / 2;
            float centerY = UpperLeftCornerY + GaugeHeight / 2;
            float ScaleFactor = (float)this.Size.Width / 150;

            gp = new GraphicsPath();


            StringFormat stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;
            
            for (int i = 0; i < 12; i++)
            {
                gp.AddString(Labels[i], FontFamily.GenericSansSerif, 0, 12.0f * ScaleFactor,
                    new Point(Convert.ToInt32(centerX), Convert.ToInt32(GaugeHeight / 4.25f)),
                    stringFormat
                    );

                RotationTransform = new Matrix(1, 0, 0, 1, 0, 0); // rotation matrix
                RotationPoint = new PointF(centerX, centerY); // rotation point
                RotationTransform.RotateAt(30, RotationPoint);
                gp.Transform(RotationTransform);

            }
            RotationTransform = new Matrix(1, 0, 0, 1, 0, 0); // rotation matrix
            RotationPoint = new PointF(centerX, centerY); // rotation point
            RotationTransform.RotateAt(-angle, RotationPoint);
            gp.Transform(RotationTransform);

            myGraphics.FillPath(myPen.Brush, gp);
            gp.Dispose();


        }
    }
}
