using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
namespace helopanel
{
    /// <summary>
    /// An artificial horizon gauge, for reading roll and tilt.
    /// </summary>
    public class ArtificialHorizon : Gauge
    {
        private Color SkyColor = Color.SkyBlue;
        private Color GroundColor = Color.FromArgb(150, 55, 15);
        private Color OutLineColor = Color.White;
        private Color ReferenceColor = Color.Gray;
        private float pitch = 10f;
        public float Pitch
        {
            set { this.Invalidate(); pitch = value; }
            get { return pitch; }
        }
        private float roll = 55.4f;
        public float Roll
        {
            set { this.Invalidate(); roll = value; }
            get { return roll; }
        }
        private float RingWidth = 10f;
        private float ScaledRingWidth;
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(Color.Black, 1.0f);
            ScaledRingWidth = RingWidth * this.Size.Width / 150f;
            DrawPitchBall(myGraphics, myPen);
            DrawUpperRollRing(myGraphics, myPen);
            DrawLowerRollRing(myGraphics, myPen);
            DrawRollRingTicks(myGraphics, myPen);
            DrawReference(myGraphics, myPen);
            myPen.Dispose();
        }
        private void DrawReference(Graphics myGraphics, Pen myPen)
        {
            float centerX = UpperLeftCornerX + GaugeWidth / 2;
            float centerY = UpperLeftCornerY + GaugeHeight / 2;
            float ScaleFactor = (float)this.Size.Width / 150f;

            myPen.Color = ReferenceColor;
            myPen.Width = 5f;
            myGraphics.DrawArc(myPen, centerX - 10 * ScaleFactor, centerY - 10 * ScaleFactor, 20 * ScaleFactor, 20 * ScaleFactor, 0, 180);
            myGraphics.DrawLine(myPen, centerX, centerY + 10 * ScaleFactor, centerX, centerY + GaugeHeight / 2);

        }
        private void DrawUpperRollRing(Graphics myGraphics, Pen myPen)
        {
            myPen.Color = SkyColor;
            myPen.Width = ScaledRingWidth;
            myGraphics.DrawArc(myPen, UpperLeftCornerX + ScaledRingWidth / 2,
                UpperLeftCornerY + ScaledRingWidth / 2,
                GaugeWidth - ScaledRingWidth,
                GaugeWidth - ScaledRingWidth,
                roll,
                -180f);

        }
        private void DrawLowerRollRing(Graphics myGraphics, Pen myPen)
        {
            myPen.Color = GroundColor;
            myPen.Width = ScaledRingWidth;
            myGraphics.DrawArc(myPen, UpperLeftCornerX + ScaledRingWidth / 2,
                UpperLeftCornerY + ScaledRingWidth / 2,
                GaugeWidth - ScaledRingWidth,
                GaugeWidth - ScaledRingWidth,
                roll,
                180f);

        }
        private void DrawRollRingTicks(Graphics myGraphics, Pen myPen)
        {
            myPen.Color = OutLineColor;
            myPen.Width = 3f * this.Size.Width / 150;
            for (float CurrentAngle = roll; CurrentAngle <= 360 + roll; CurrentAngle += 20)
            {                   
                float InnerPointX = -((GaugeWidth / 2 - ScaledRingWidth) * (float)Math.Cos(-CurrentAngle * 2 * Math.PI / 360)) + (UpperLeftCornerX + GaugeWidth / 2);
                float InnerPointY = ((GaugeWidth / 2 - ScaledRingWidth) * (float)Math.Sin(-CurrentAngle * 2 * Math.PI / 360)) + (UpperLeftCornerY + GaugeWidth / 2);
                float OuterPointX = -((GaugeWidth / 2) * (float)Math.Cos(-CurrentAngle * 2 * Math.PI / 360)) + (UpperLeftCornerX + GaugeWidth / 2);
                float OuterPointY = ((GaugeWidth / 2) * (float)Math.Sin(-CurrentAngle * 2 * Math.PI / 360)) + (UpperLeftCornerY + GaugeWidth / 2);
                myGraphics.DrawLine(myPen,InnerPointX,InnerPointY,OuterPointX,OuterPointY);
            }
        }
        private void DrawPitchBall(Graphics myGraphics, Pen myPen)
        {
            GraphicsPath gp;
            Matrix RotationTransform;
            Matrix TranslationTransform;
            PointF RotationPoint;

            float centerX = UpperLeftCornerX + GaugeWidth / 2;
            float centerY = UpperLeftCornerY + GaugeHeight / 2;
            float ScaleFactor = (float)this.Size.Width / 150;
            //draw the bottom half
            gp = new GraphicsPath();
            gp.AddPie(UpperLeftCornerX + ScaledRingWidth/2, UpperLeftCornerY + GaugeWidth / 4f, GaugeWidth - ScaledRingWidth, GaugeWidth / 2f, 0, 180);

            TranslationTransform = new Matrix(1, 0, 0, 1, 0, 0); // translation matrix
            TranslationTransform.Translate(0, -pitch);
            gp.Transform(TranslationTransform);

            RotationTransform = new Matrix(1, 0, 0, 1, 0, 0); // rotation matrix
            RotationPoint = new PointF(centerX, centerY); // rotation point
            RotationTransform.RotateAt(roll, RotationPoint);
            gp.Transform(RotationTransform);

            myPen.Color = OutLineColor;
            myPen.Width = 4f * this.Size.Width / 150;
            myGraphics.DrawPath(myPen, gp);
            myPen.Color = GroundColor;
            myGraphics.FillPath(myPen.Brush, gp);
            gp.Dispose();
            
            //draw the top half
            gp = new GraphicsPath();
            gp.AddPie(UpperLeftCornerX + ScaledRingWidth / 2, UpperLeftCornerY + GaugeWidth / 4f, GaugeWidth - ScaledRingWidth, GaugeWidth / 2f, 0, -180);

            TranslationTransform = new Matrix(1, 0, 0, 1, 0, 0); // translation matrix
            TranslationTransform.Translate(0, -pitch);
            gp.Transform(TranslationTransform);

            RotationTransform = new Matrix(1, 0, 0, 1, 0, 0); // rotation matrix
            RotationPoint = new PointF(centerX, centerY); // rotation point
            RotationTransform.RotateAt(roll, RotationPoint);
            gp.Transform(RotationTransform);

            myPen.Color = OutLineColor;
            myPen.Width = 4f * this.Size.Width / 150;
            myGraphics.DrawPath(myPen, gp);
            myPen.Color = SkyColor;
            myGraphics.FillPath(myPen.Brush, gp);
            gp.Dispose();

            //draw the lines
            gp = new GraphicsPath();

            gp.AddLine(centerX + 20 * ScaleFactor, centerY - 24 * ScaleFactor, centerX - 20 * ScaleFactor, centerY - 24 * ScaleFactor);
            gp.CloseFigure();
            gp.AddLine(centerX + 32 * ScaleFactor, centerY - 16 * ScaleFactor, centerX - 32 * ScaleFactor, centerY - 16 * ScaleFactor);
            gp.CloseFigure();
            gp.AddLine(centerX + 45 * ScaleFactor, centerY - 8 * ScaleFactor, centerX - 45 * ScaleFactor, centerY - 8 * ScaleFactor);
            gp.CloseFigure();

            gp.AddLine(centerX + 45 * ScaleFactor, centerY + 8 * ScaleFactor, centerX - 45 * ScaleFactor, centerY + 8 * ScaleFactor);
            gp.CloseFigure();
            gp.AddLine(centerX + 32 * ScaleFactor, centerY + 16 * ScaleFactor, centerX - 32 * ScaleFactor, centerY + 16 * ScaleFactor);
            gp.CloseFigure();
            gp.AddLine(centerX + 20 * ScaleFactor, centerY + 24 * ScaleFactor, centerX - 20 * ScaleFactor, centerY + 24 * ScaleFactor);
            gp.CloseFigure();
            
            TranslationTransform = new Matrix(1, 0, 0, 1, 0, 0); // translation matrix
            TranslationTransform.Translate(0, -pitch);
            gp.Transform(TranslationTransform);

            RotationTransform = new Matrix(1, 0, 0, 1, 0, 0); // rotation matrix
            RotationPoint = new PointF(centerX, centerY); // rotation point
            RotationTransform.RotateAt(roll, RotationPoint);
            gp.Transform(RotationTransform);

            myPen.Color = OutLineColor;
            myPen.Width = 2f * this.Size.Width / 150;
            myGraphics.DrawPath(myPen, gp);
            gp.Dispose();


        }

    }
}
