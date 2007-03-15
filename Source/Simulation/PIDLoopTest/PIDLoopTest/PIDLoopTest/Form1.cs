using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace PIDLoopTest
{
    public partial class Form1 : Form
    {
        DataTable Data;
        Grapher mygrapher;
        bool DataReady = false;
        double yshift = 0;
        double yvalue = 1;
        public Form1()
        {
            Data = new DataTable();
            DataColumn Time = new DataColumn("time", Type.GetType("System.Double"));
            DataColumn Position = new DataColumn("position", Type.GetType("System.Double"));
            Data.Columns.Add(Time);
            Data.Columns.Add(Position);
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DataReady = false;
            Kinematics k = new Kinematics();
            dataGridView1.DataSource = null;
            Force PIDCorrection = new Force();

            k.Forces.Add(PIDCorrection);
            PIDLoop pid = new PIDLoop(k);

            Force Disturbance = new Force();
            k.Forces.Add(Disturbance);

            k.mass = 1;
            Data.Clear();
            Double RunningTime = 0;
            k.XPosition = 0;
            k.XVelocity = -20;
            for (int i = 0; i < 10000; i++)//sim loop
            {
                RunningTime+= k.time;
                
                if (i > 50 && i< 3000)
                {
                    Disturbance.X = -100;
                    PIDCorrection.X = pid.calculateCorrection(50);
                }
                else if (i >= 3000)
                {
                    PIDCorrection.X = pid.calculateCorrection(25);
                }
                k.CalculateKinematics();
                DataRow newRow = Data.NewRow();
                newRow["time"] = RunningTime;
                newRow["position"] = k.XPosition;
                Data.Rows.Add(newRow);

            }
            DataReady = true;
            dataGridView1.DataSource = Data;
            this.Invalidate();
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            if (DataReady)//(DataReady)
            {
                string errstring;
                Graphics myGraphics = e.Graphics;
                mygrapher = new Grapher(new GraphDimension(25, this.Size.Width - 25, 200, this.Size.Height - 100), "Position versus time", yvalue);
                DataSeries series1 = new DataSeries(Data, "time", "position", Color.Red, 3.0f);

                mygrapher.AddDataSeries(series1);
                if ((errstring = mygrapher.PaintEvent(ref myGraphics, this.Font, yshift)) != "")
                {
                    MessageBox.Show(errstring);
                }
            }
            base.OnPaint(e);
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            if (textBox2.Text != "")
            {
                try
                {
                    yvalue = Convert.ToDouble(textBox2.Text);
                    this.Invalidate();
                }
                catch (Exception ex)
                { }
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (textBox1.Text != "")
            {
                try
                {
                    yshift = Convert.ToDouble(textBox1.Text);
                    this.Invalidate();
                }
                catch (Exception ex)
                { }
            }
        }



    }
}