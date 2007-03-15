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

        }
        public void LoadMap(string MapFilePath)
        {
            Map = Image.FromFile(MapFilePath);
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Graphics myGraphics = e.Graphics;
            Pen myPen = new Pen(Color.Black);

            Rectangle rect = new Rectangle(new Point(0,0), this.Size);
            if (Map != null)
            {
                myGraphics.DrawImage(Map, rect, ImageTopLeftX, ImageTopLeftY, this.Size.Width, this.Size.Height, GraphicsUnit.Pixel);//overload 23
            }
            else
            {
                MessageBox.Show("You must load a map file using LoadMap() before adding this user control to your form");
            }
            myPen.Dispose();
        }
    }
}
