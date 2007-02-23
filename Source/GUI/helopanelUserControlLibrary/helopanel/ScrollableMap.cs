using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace helopanel
{
    public partial class ScrollableMap : UserControl
    {
        Image Map;
        private float _ImageTopLeftX = 0;
        public float ImageTopLeftX
        {
            set { this.Invalidate(); _ImageTopLeftX = value; }
            get { return _ImageTopLeftX; }
        }
        public float _ImageTopLeftY = 0;
        public float ImageTopLeftY
        {
            set { this.Invalidate(); _ImageTopLeftY = value; }
            get { return _ImageTopLeftY; }
        }
        public ScrollableMap()
        {
            InitializeComponent();

            // Activates double buffering
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true);
            Map = Image.FromFile(@"c:\cc.jpg");
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(Color.Black);

            Rectangle rect = new Rectangle(new Point(0,0), this.Size);
            myGraphics.DrawImage(Map, rect, ImageTopLeftX, ImageTopLeftY, this.Size.Width+100, this.Size.Height, GraphicsUnit.Pixel);//overload 23
            myPen.Dispose();
        }
    }
}
