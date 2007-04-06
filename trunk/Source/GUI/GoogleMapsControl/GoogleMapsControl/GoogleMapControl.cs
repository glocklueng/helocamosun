using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Security.Permissions;

namespace GoogleMapsControl
{
    [PermissionSet(SecurityAction.Demand, Name = "FullTrust")]
    [System.Runtime.InteropServices.ComVisibleAttribute(true)]

    public partial class GoogleMapControl : UserControl
    {
        public GoogleMapControl()
        {
            InitializeComponent();

            webBrowser1.AllowWebBrowserDrop = false;
            webBrowser1.IsWebBrowserContextMenuEnabled = false;
            webBrowser1.WebBrowserShortcutsEnabled = false;
            webBrowser1.ObjectForScripting = this;
            webBrowser1.Url = new Uri(@"..\index.html");
            webBrowser1.DocumentCompleted += new WebBrowserDocumentCompletedEventHandler(webBrowser1_DocumentCompleted);

        }

        public void GotoLoc(double latitude, double longitude)
        {
            HtmlElement LatitudeTextBox;
            if ((LatitudeTextBox = webBrowser1.Document.GetElementById("gotoLatitude")) != null)
            {

                LatitudeTextBox.OuterHtml = "<INPUT id=gotoLatitude size=8 value=" + latitude + ">";


                HtmlElement LongitudeTextBox = webBrowser1.Document.GetElementById("gotoLongitude");
                LongitudeTextBox.OuterHtml = "<INPUT id=gotoLongitude size=8 value=" + longitude + ">";
                webBrowser1.Document.InvokeScript("gotoLoc");
            }
        }

        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            webBrowser1.Document.Window.ScrollTo(new Point(50, 100));
        }

        private void button1_Click(object sender, EventArgs e)
        {
            GotoLoc(48.490928 , -123.415958);
        }
    }
}