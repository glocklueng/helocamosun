using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Security.Permissions;
using System.Collections;

namespace GoogleMapsControl
{

    public struct LatLong
    {
        public double latitude;
        public double longitude;
    }
    
    [PermissionSet(SecurityAction.Demand, Name = "FullTrust")]
    [System.Runtime.InteropServices.ComVisibleAttribute(true)]
    public partial class GoogleMapControl : UserControl
    {
        private HelicopterIcon heliIcon;
 
        public float HeadingAngle
        {
            set
            {
                heading = value;
            }
            get
            {
                return heading;
            }
        }
        private float heading;
        bool waypointmode = false;
        public LatLong CurrentPosition = new LatLong();
        public GoogleMapControl()
        {

            InitializeComponent();
            heliIcon = new HelicopterIcon();
            webBrowser1.Controls.Add(this.heliIcon);

            heliIcon.Location = new Point(webBrowser1.Width/2, webBrowser1.Height/2);

            webBrowser1.AllowWebBrowserDrop = false;
            webBrowser1.IsWebBrowserContextMenuEnabled = false;
            webBrowser1.WebBrowserShortcutsEnabled = false;
            webBrowser1.ObjectForScripting = this;
            webBrowser1.Url = new Uri(@"E:\helo\Source\GUI\GoogleMapsControl\GoogleMapsControl\index.html");
            webBrowser1.DocumentCompleted += new WebBrowserDocumentCompletedEventHandler(webBrowser1_DocumentCompleted);
        }

        public void GotoLoc(double latitude, double longitude)
        {
            
            if (!waypointmode)
            {
                CurrentPosition.latitude = latitude;
                txtLatitude.Text = latitude.ToString();
                CurrentPosition.longitude = longitude;
                txtLongitude.Text = longitude.ToString();
                HtmlElement LatitudeTextBox;
                if ((LatitudeTextBox = webBrowser1.Document.GetElementById("gotoLatitude")) != null)
                {
                    LatitudeTextBox.OuterHtml = "<INPUT id=gotoLatitude size=8 value=" + latitude + ">";
                    
                    HtmlElement LongitudeTextBox = webBrowser1.Document.GetElementById("gotoLongitude");
                    LongitudeTextBox.OuterHtml = "<INPUT id=gotoLongitude size=8 value=" + longitude + ">";
                    webBrowser1.Document.InvokeScript("gotoLoc");
                }
            }
        }
        public void ToggleDynamicDragging()
        {
            HtmlElement DragEnableBox;
            if ((DragEnableBox = webBrowser1.Document.GetElementById("enableDragging")) != null)
            {
                if (DragEnableBox.OuterHtml.Contains("CHECKED"))
                {
                    DragEnableBox.OuterHtml = "<INPUT id=enableDragging onclick=\"toggler('enableDragging')\" type=checkbox >";
                }
                else
                {
                    DragEnableBox.OuterHtml = "<INPUT id=enableDragging onclick=\"toggler('enableDragging')\" type=checkbox CHECKED>";
                }
            }
            webBrowser1.Document.InvokeScript("toggler", new string[] { "enableDragging" });
        }
        public void ToggleMapTrack()
        {
            HtmlElement MapTrackEnableBox;
            if ((MapTrackEnableBox = webBrowser1.Document.GetElementById("mapTrack")) != null)
            {
                if (MapTrackEnableBox.OuterHtml.Contains("CHECKED"))
                {
                    MapTrackEnableBox.OuterHtml = "<INPUT id=mapTrack onclick=\"toggler('mapTrack')\" type=checkbox >";
                }
                else
                {
                    MapTrackEnableBox.OuterHtml = "<INPUT id=mapTrack onclick=\"toggler('mapTrack')\" type=checkbox CHECKED>";
                }
            }
            webBrowser1.Document.InvokeScript("toggler", new string[] { "mapTrack" });

        }
        public LatLong[] GetWayPointCoords()
        {
            ArrayList latlongArray = new ArrayList();
            string[] LATLONGS = webBrowser1.Document.GetElementById("message3").InnerHtml.Split(new string[] { "\"" }, StringSplitOptions.RemoveEmptyEntries);
            if (LATLONGS.Length > 2)
            {
                for (int i = 1; i < LATLONGS.Length - 1; i += 2)
                {
                    latlongArray.Add(LATLONGS[i]);
                }

                LatLong[] latlong = new LatLong[latlongArray.Count/2];
                int arrayCount = 0;
                for (int i = 0; i < latlongArray.Count; i++)
                {
                    latlong[arrayCount].latitude = Convert.ToDouble(latlongArray[i]);
                    latlong[arrayCount].longitude = Convert.ToDouble(latlongArray[++i]);
                    arrayCount++;
                }
                return latlong;
            }
            return null;
        }
        public void ToggleWaypointCoords()
        {
            HtmlElement WaypointCoordsBox;
            if ((WaypointCoordsBox = webBrowser1.Document.GetElementById("waypointCoords")) != null)
            {
                if (WaypointCoordsBox.OuterHtml.Contains("CHECKED"))
                {
                    WaypointCoordsBox.OuterHtml = "<INPUT id=waypointCoords onclick=\"toggler('waypointCoords')\" type=checkbox >";
                }
                else
                {
                    WaypointCoordsBox.OuterHtml = "<INPUT id=waypointCoords onclick=\"toggler('waypointCoords')\" type=checkbox CHECKED>";
                }
            }
            webBrowser1.Document.InvokeScript("toggler", new string[] { "waypointCoords" });

        }
        public void ClearTracksAndMarkers()
        {
            webBrowser1.Document.InvokeScript("resetMapTrack");
        }
        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            OnGoogleMapLoaded(new EventArgs());
            webBrowser1.Document.Window.ScrollTo(new Point(50, 105));

        }

        public delegate void GoogleMapLoadedEventHandler(object sender, EventArgs e);
        public event GoogleMapLoadedEventHandler GoogleMapLoaded;
        protected virtual void OnGoogleMapLoaded(EventArgs e)
        {
            if (GoogleMapLoaded != null)
            {
                GoogleMapLoaded(this, e);
            }
        }

        private void btnEnterWayPointMode_Click(object sender, EventArgs e)
        {
            if (!waypointmode)
            {
                ClearTracksAndMarkers();
                waypointmode = true;
            }
            else
            {
                LatLong[] latlongs = GetWayPointCoords();
                string waypoints = "";
                foreach (LatLong latlong in latlongs)
                {
                    waypoints += " Lat: " + latlong.latitude + " Long: " + latlong.longitude + "\r\n";
                }
                if (waypoints != "")
                {
                    MessageBox.Show(waypoints, "Waypoints");
                }
                waypointmode = false;
            }
            ToggleWaypointCoords();
            ToggleDynamicDragging();
            ToggleMapTrack();
        }


    }
}