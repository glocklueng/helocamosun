using System;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;

namespace cartesian_plotter
{
    public partial class Form1 : Form
    {

        SerialPort sp;
        string data = "";
        struct points
        {
            public int x;
            public int y;
            //public bool drawn;
        }
        ArrayList pointsArray = new ArrayList();
        public Form1()
        {
            InitializeComponent();
 
            this.Resize += new EventHandler(Form1_Resize);
            this.Size = new Size(500, 500);

            sp = new SerialPort("COM11",19200);
            sp.Open();
            timer1.Tick += new EventHandler(timer1_Tick);
            timer1.Start();
        }

        void timer1_Tick(object sender, EventArgs e)
        {
            for (int i = 0; i < sp.BytesToRead; i++ )
            {
                data += sp.ReadByte() + " ";
            }
            string[] splitData = data.Split(new string[] { "67" }, StringSplitOptions.RemoveEmptyEntries);

            foreach (string dat in splitData)
            {

                string[] numbers = dat.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                if (numbers.Length == 6)
                {

                    int xval = (Convert.ToInt32(numbers[2]) << 8) + Convert.ToInt32(numbers[3]);
                    if (xval > 1023)
                    {
                        xval = -(65535 - xval);
                    }
                    int yval = (Convert.ToInt32(numbers[4]) << 8) + Convert.ToInt32(numbers[5]);
                    if (yval > 1023)
                    {
                        yval = -(65535 - yval);
                    }
                    points p;
                    p.x = xval;
                    p.y = yval;
                    pointsArray.Add(p);
                    this.Invalidate();
                }
            }
        }

      

        void Form1_Resize(object sender, EventArgs e)
        {
            this.Invalidate();
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            Pen myPen = new Pen(Color.Black);

            Graphics myGraphics = e.Graphics;
            
            myGraphics.DrawLine(myPen, new Point(this.Width / 2, 0), new Point(this.Width / 2, this.Height));
            myGraphics.DrawLine(myPen, new Point(0, this.Height / 2), new Point(this.Width, this.Height / 2));
            for (int q = 0; q < 25; q++)
            {
                myGraphics.DrawLine(myPen, new Point((this.Width / 2) - 5, (this.Height / 2) - (10 * q)), new Point((this.Width / 2) + 5, (this.Height / 2) - (10 * q)));
                myGraphics.DrawLine(myPen, new Point((this.Width / 2) - 5, (this.Height / 2) + (10 * q)), new Point((this.Width / 2) + 5, (this.Height / 2) + (10 * q)));
                myGraphics.DrawLine(myPen, new Point((this.Width / 2) - (10 * q), (this.Height / 2) - 5), new Point((this.Width / 2) - (10 * q), (this.Height / 2) + 5));
                myGraphics.DrawLine(myPen, new Point((this.Width / 2) + (10 * q), (this.Height / 2) - 5), new Point((this.Width / 2) + (10 * q), (this.Height / 2) + 5));

            }
            foreach (points p in pointsArray)
            {
                PlotPoint(p.x, p.y, myPen, myGraphics);
            }
            myPen.Dispose();
        }
        public void PlotPoint(int x, int y, Pen myPen, Graphics myGraphics)
        {
            myPen.Width = 2;
            x = x + 250;
            y = -y + 250;
            myGraphics.DrawLine(myPen, new Point(x - 2, y - 2), new Point(x + 2, y + 2));
            myGraphics.DrawLine(myPen, new Point(x - 2, y + 2), new Point(x + 2, y - 2));

        }
    }
}