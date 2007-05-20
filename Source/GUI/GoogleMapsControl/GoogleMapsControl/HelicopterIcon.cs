using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace GoogleMapsControl
{
    public partial class HelicopterIcon : UserControl
    {
        public float Heading
        {
            set
            {
                heading = value;
                this.Invalidate();
            }
            get
            {
                return heading;
            }
        }
        private float heading;
        public HelicopterIcon()
        {

            InitializeComponent();

            this.SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            this.BackColor = Color.Transparent;


        }
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(Color.Black, 1.0f);
            GraphicsPath gp = new GraphicsPath();
            Matrix RotationTransform;

            float centerX = this.Size.Width / 2f;
            float centerY = this.Size.Height / 2f;

            gp.AddPolygon(new PointF[] { 
                new PointF(centerX, centerY - 5), 
                new PointF(centerX-5, centerY + 5), 
                new PointF(centerX+5, centerY + 5) 
            });

            RotationTransform = new Matrix(1, 0, 0, 1, 0, 0); // rotation matrix
            PointF RotationPoint = new PointF(centerX, centerY); // rotation point
            RotationTransform.RotateAt(heading, RotationPoint);
            gp.Transform(RotationTransform);

            myGraphics.FillPath(myPen.Brush, gp);
            myGraphics.DrawPath(myPen, gp);
            myPen.Dispose();
            gp.Dispose();
        }
        
    }
}
