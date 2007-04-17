using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using Tao.Platform.Windows;
using Tao.OpenGl;

namespace GLUserControl
{
    public partial class UserControl1 : UserControl
    {
        SimpleOpenGlControl s;
        float rotation = 0;
        public UserControl1()
        {
            InitializeComponent();
            s = new SimpleOpenGlControl();
            s.Paint += new PaintEventHandler(s_Paint);
            dosomething();
        }

        void s_Paint(object sender, PaintEventArgs e)
        {
            rotation = rotation + 1.0f;
            Gl.glClear(Gl.GL_COLOR_BUFFER_BIT);
            Gl.glPushMatrix();
            Gl.glRotatef(rotation, 0f, 0f, 1f);
            Gl.glBegin(Gl.GL_TRIANGLES);
            Gl.glVertex3f(0.5f, 1.0f, 0.0f);
            Gl.glVertex3f(0.0f, 0.0f, 0.0f);
            Gl.glVertex3f(1.0f, 0.0f, 0.0f);
            Gl.glEnd();
            Gl.glPopMatrix();
       
        }
        public void dosomething()
        {
            s.Dock = DockStyle.Fill;
            s.InitializeContexts();
            this.Controls.Add(s);
            s.Show();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            s.Invalidate();
        }
    }
}