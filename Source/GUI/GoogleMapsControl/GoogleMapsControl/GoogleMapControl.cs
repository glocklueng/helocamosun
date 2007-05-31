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
using System.Net.NetworkInformation;

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
        public bool InternetConnected = false;

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
        bool setbasemode = false;
        public LatLong CurrentPosition = new LatLong();
        public LatLong BaseStation;
        public bool BaseStationSet = false;
        public GoogleMapControl()
        {

            InitializeComponent();
            heliIcon = new HelicopterIcon();
            webBrowser1.Controls.Add(this.heliIcon);

            TestForInternetConnection();

            heliIcon.Location = new Point(webBrowser1.Width / 2, webBrowser1.Height / 2);

            if(true)// (InternetConnected)
            {
                webBrowser1.AllowWebBrowserDrop = false;
                webBrowser1.IsWebBrowserContextMenuEnabled = false;
                webBrowser1.WebBrowserShortcutsEnabled = false;
                webBrowser1.ObjectForScripting = this;

                webBrowser1.Url = new Uri(@"E:\helo\Source\GUI\GoogleMapsControl\GoogleMapsControl\index.html");

                webBrowser1.DocumentCompleted += new WebBrowserDocumentCompletedEventHandler(webBrowser1_DocumentCompleted);
            }
        }

        public void GotoLoc(double latitude, double longitude)
        {
            
            if (!waypointmode && !setbasemode)
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
        void SetMarkerAtCurrentLoc()
        {
            webBrowser1.Document.InvokeScript("setMarkerAtCurrentLoc");
        }
        private void btnEnterWayPointMode_Click(object sender, EventArgs e)
        {
            if (!setbasemode)
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


        private void btnSetBase_Click(object sender, EventArgs e)
        {
            if (!waypointmode)
            {
                if (!setbasemode)
                {
                    ClearTracksAndMarkers();
                    setbasemode = true;
                    ToggleDynamicDragging();
                }
                else
                {
                    BaseStationSet = true;
                    BaseStation = GetCoords();
                    setbasemode = false;
                    ToggleDynamicDragging();
                    SetMarkerAtCurrentLoc();
                }
            }
        }
        public LatLong GetCoords()
        {
            HtmlElement latlong = webBrowser1.Document.GetElementById("message1");
            string[] splitLatLong = latlong.InnerHtml.Split(new string[] { " " }, StringSplitOptions.RemoveEmptyEntries);

            LatLong CurrentCoords;
            CurrentCoords.latitude = Convert.ToDouble(splitLatLong[4]);
            CurrentCoords.longitude = Convert.ToDouble(splitLatLong[6]);
            return CurrentCoords;
        }
        private void TestForInternetConnection()
        {
            Ping pingSender = new Ping();
            PingOptions options = new PingOptions();

            // Use the default Ttl value which is 128,
            // but change the fragmentation behavior.
            options.DontFragment = true;

            // Create a buffer of 32 bytes of data to be transmitted.
            string data = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
            byte[] buffer = Encoding.ASCII.GetBytes(data);
            int timeout = 120;
            try
            {
                PingReply reply = pingSender.Send("www.camosun.bc.ca", timeout, buffer, options);
                if (reply.Status == IPStatus.Success)
                {
                    InternetConnected = true;
                    long l = reply.RoundtripTime;
                }
            }
            catch (Exception ex)
            {
                string s = ex.Message;
                MessageBox.Show("There is no internet connection available.  The google maps control can not load.");
                InternetConnected = false;
            }


        }
    }
}