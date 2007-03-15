using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Collections;
using System.Drawing;
using System.Drawing.Text;
using System.Windows.Forms;
namespace PIDLoopTest
{
    class Grapher
    {

        private int GraphLeftSide;
        private int GraphRightSide;
        private int GraphTop;
        private int GraphBottom;
        ArrayList DataSeriesCollection;
        private string GraphTitle;

        public double PixelsPerUnit;
        double rightXbounds;
        double leftXbounds;
        /// <summary>
        /// Grapher constructor.  Create a new graph object specifiying the dimensions, visible title, y - axis shit, and pixels per unit
        /// </summary>
        /// <param name="graphdimension">valid graphdimension class</param>
        /// <param name="GraphTitle">title that will appear at the top of the graph</param>
        /// <param name="pixelsperunit">number of pixels per unit of graphed value</param>
        public Grapher(GraphDimension graphdimension, string GraphTitle, double pixelsperunit)
        {
            this.PixelsPerUnit = pixelsperunit;
            this.GraphTitle = GraphTitle;
            DataSeriesCollection = new ArrayList();

            this.GraphLeftSide = graphdimension.x1;
            this.GraphRightSide = graphdimension.x2;
            this.GraphTop = graphdimension.y1;
            this.GraphBottom = graphdimension.y2;
        }
        public void AddDataSeries(DataSeries dataseries)
        {
            if (dataseries.seriesRows[0].x != null)
            {
                DataSeriesCollection.Add(dataseries);
            }
        }
        public string PaintEvent(ref Graphics graphics,Font f, double shift)
        {
            Pen myPen;

            if(DataSeriesCollection.Count>0)
            {
                DrawBackGround(ref graphics, f, (DataSeries)DataSeriesCollection[0], shift);

                bool firstSeries = true;
                foreach (DataSeries dseries in DataSeriesCollection)
                {

                    myPen = new Pen(dseries.color, dseries.LineWidth);

                    float thisXpoint;
                    float thisYpoint;
                    float lastXpoint = 0;
                    float lastYpoint = 0;

                    int firstpoint = 1;

                    if (dseries.seriesRows != null)
                    {
                        foreach (DataSeries.SeriesRow d in dseries.seriesRows)
                        {
                            if (firstSeries)
                            {   //scale the graph for the first data series
                                rightXbounds = CastValue(dseries.Xmax, dseries.Xtype);
                                leftXbounds = CastValue(dseries.Xmin, dseries.Xtype);
                                if (rightXbounds == leftXbounds)
                                {
                                    return "all X values are the same" ;
                                }
                                firstSeries = false;
                            }
                            thisXpoint = (float)scaleXValue(CastValue(d.x, dseries.Xtype));
                            thisYpoint = (float)scaleYValue(CastValue(d.y, dseries.Ytype), shift);
                            //thispoint = new Point(scaleXValue(CastValue(d.x, dseries.Xtype)), );
                            if (firstpoint != 1)
                            {
                                if (thisXpoint <= GraphRightSide - 25 && thisYpoint >= GraphTop + 25 && thisYpoint <= GraphBottom - 25)//set graph line constraints
                                {
                                    myPen.Color = dseries.color;
                                    graphics.DrawLine(myPen, lastXpoint, lastYpoint, thisXpoint, thisYpoint);
                                }
                                else
                                {
                                    myPen.Color = Color.Transparent;
                                    graphics.DrawLine(myPen, lastXpoint, lastYpoint, thisXpoint, thisYpoint);
                                }
                            }
                            firstpoint = 0;
                            lastXpoint = thisXpoint;
                            lastYpoint = thisYpoint;
                        }
                    }
                    myPen.Dispose();
                }
            }
            return "";
        }
        double scaleXValue(double xVal)
        {

            xVal = ((float)(GraphRightSide - GraphLeftSide - 50) / ((float)rightXbounds - (float)leftXbounds) * (float)(xVal - leftXbounds)) + 50;
            return xVal;
        }
        double scaleYValue(double yVal, double shift)
        {
            return -(yVal * PixelsPerUnit) + GraphBottom -25 - shift*PixelsPerUnit;
        }
        double CastValue(object value, Type type)
        {

            double CastVal = 0;
            if (type == Type.GetType("System.DateTime"))
            {
                CastVal = Convert.ToDateTime(value).Second + Convert.ToDateTime(value).Minute * 60 + Convert.ToDateTime(value).Hour * 3600;
            }
            else if (type == Type.GetType("System.Int32"))
            {
                CastVal = Convert.ToDouble(value);
            }
            else if (type == Type.GetType("System.Double"))
            {
                CastVal = Convert.ToDouble(value);
            }

            return CastVal;
        }
        private void DrawBackGround(ref Graphics graphics, Font f, DataSeries D, double shift)
        {

            int vertical_divisions = 10;
            int horizontal_divisions = 8;

            int Hpixelsperdivision = (GraphBottom - GraphTop - 50) / horizontal_divisions;

            SolidBrush mybrush = new SolidBrush(Color.White);
            graphics.FillRectangle(mybrush, GraphLeftSide, GraphTop, GraphRightSide - GraphLeftSide, GraphBottom - GraphTop);
            mybrush.Dispose();

            TextFormatFlags flags = TextFormatFlags.Bottom | TextFormatFlags.EndEllipsis;
            TextRenderer.DrawText(graphics, GraphTitle, f,
                new Rectangle(GraphLeftSide + 2, GraphTop + 2, GraphRightSide - GraphLeftSide, 15), SystemColors.ControlText, flags);

            Pen myPen = new Pen(Color.Black, 1.0f);
            graphics.DrawRectangle(myPen, GraphLeftSide, GraphTop, GraphRightSide - GraphLeftSide, GraphBottom - GraphTop);
            graphics.DrawRectangle(myPen, GraphLeftSide + 25, GraphTop + 25, GraphRightSide - GraphLeftSide - 50, GraphBottom - GraphTop - 50);
            //draw horizontal lines and labels
            for (int i = 0; i <= horizontal_divisions; i++)
            {
                double maxVal = (double)(GraphBottom - GraphTop - 50) / PixelsPerUnit;
                double horizontal_value = maxVal / (horizontal_divisions) * (horizontal_divisions - i) - shift;
                string horizontal_value_str = horizontal_value.ToString();
                int LineLeft = GraphLeftSide + 25;
                int LineRight = GraphRightSide - 25;
                int LineHeight = GraphTop + ((GraphBottom - GraphTop - 50) / horizontal_divisions) * i + 25;
                graphics.DrawLine(myPen, LineLeft, LineHeight, LineRight, LineHeight);
                TextRenderer.DrawText(graphics, horizontal_value_str, f,
                    new Rectangle(LineLeft - 25, LineHeight - 4, 50, 12), SystemColors.ControlText, flags);
            }
            for (int i = 0; i < vertical_divisions; i++)//draw vertical lines and labels
            {

                if (D.Xtype == Type.GetType("System.DateTime"))
                {
                    double totalseconds = CastValue(D.Xmax, D.Xtype) - CastValue(D.Xmin, D.Xtype);
                    double secondsPerDiv = totalseconds / vertical_divisions;
                    DateTime minval = (DateTime)D.Xmin;
                    string vertical_value_str = String.Format("{0:HH}:{0:mm}:{0:ss}", minval.AddSeconds(secondsPerDiv * i));
                    int LineTop = GraphTop + 25;
                    int LineBottom = GraphBottom - 25;
                    int LinePos = GraphLeftSide + ((GraphRightSide - GraphLeftSide - 50) / (vertical_divisions)) * i + 25;
                    graphics.DrawLine(myPen, LinePos, LineTop, LinePos, LineBottom);
                    TextRenderer.DrawText(graphics, vertical_value_str, f,
                        new Rectangle(LinePos - 4, LineBottom + 10, 50, 12), SystemColors.ControlText, flags);
                }
                else if (D.Xtype == Type.GetType("System.Int32") || D.Xtype == Type.GetType("System.Int16") || D.Xtype == Type.GetType("System.Double"))
                {
                    double maxVal = CastValue(D.Xmax, D.Xtype) - CastValue(D.Xmin, D.Xtype);
                    double vertical_value = maxVal / (vertical_divisions) * i;// -shift;
                    string vertical_value_str = vertical_value.ToString();
                    int LineTop = GraphTop + 25;
                    int LineBottom = GraphBottom - 25;
                    int LinePos = GraphLeftSide + ((GraphRightSide - GraphLeftSide - 50) / vertical_divisions) * i + 25;
                    graphics.DrawLine(myPen, LinePos, LineTop, LinePos, LineBottom);
                    TextRenderer.DrawText(graphics, vertical_value_str, f,
                        new Rectangle(LinePos - 4, LineBottom + 10, 50, 12), SystemColors.ControlText, flags);

                }
            }
            myPen.Dispose();
            mybrush.Dispose();
        }
    }
    class GraphDimension
    {
        public int x1, x2, y1, y2;
        public GraphDimension(int x1, int x2, int y1, int y2)
        {
            this.x1 = x1;
            this.x2 = x2;
            this.y1 = y1;
            this.y2 = y2;
        }
    }
    class DataSeries
    {
        public DataTable DataSource;
        public object Xmax;
        public object Xmin;
        public object Ymax;
        public object Ymin;
        public Type Xtype;
        public Type Ytype;
        public struct SeriesRow
        {
            public object x;
            public object y;
        }
        public Color color;
        public SeriesRow[] seriesRows;
        public float LineWidth;

        public DataSeries(DataTable DataSource, string Xcolname, string Ycolname, Color C, float linewidth)
        {
            //int numberpoints = 0;
            int MaxRows = 500;
            LineWidth = linewidth;
            color = C;
            this.DataSource = DataSource;
            int numrows = DataSource.Rows.Count;
            if (DataSource.Columns.Contains(Xcolname) && DataSource.Columns.Contains(Ycolname) && DataSource.Rows.Count > 0)
            {


                Xtype = DataSource.Columns[Xcolname].DataType;
                Ytype = DataSource.Columns[Ycolname].DataType;

                
                int rowCount = 0;
                
                if (DataSource.Rows.Count < MaxRows)
                {
                    seriesRows = new SeriesRow[numrows];

                    foreach (DataRow Dr in DataSource.Rows)
                    {
                        seriesRows[rowCount].x = Dr[Xcolname];
                        seriesRows[rowCount].y = Dr[Ycolname];
                        rowCount++;
                    }
                }
                else//reduce the number of datapoints
                {
                    seriesRows = new SeriesRow[MaxRows+1];
                    int DistanceBetweenRows = DataSource.Rows.Count / MaxRows;
                    int contiguousRows = 0;
                    foreach (DataRow Dr in DataSource.Rows)
                    {
                        if ((rowCount == DistanceBetweenRows || contiguousRows == 0 || contiguousRows ==MaxRows) && contiguousRows<=MaxRows)
                        {
                            seriesRows[contiguousRows].x = Dr[Xcolname];
                            seriesRows[contiguousRows].y = Dr[Ycolname];
                            contiguousRows++;
                            rowCount = 0;
                        }
                        rowCount++;
                    }

                   // numberpoints
            
                }
                Xmax = DataSource.Rows[DataSource.Rows.Count-1][Xcolname];// DataSource.Select("[" + Xcolname + "] = MAX([" + Xcolname + "])")[0][Xcolname];
                Ymax = DataSource.Rows[DataSource.Rows.Count - 1][Ycolname]; ;//DataSource.Select("[" + Ycolname + "] = MAX([" + Ycolname + "])")[0][Ycolname];
                Xmin = DataSource.Rows[0][Xcolname];// DataSource.Select("[" + Xcolname + "] = MIN([" + Xcolname + "])")[0][Xcolname];
                Ymin = 1.05;// DataSource.Select("[" + Ycolname + "] = MIN([" + Ycolname + "])")[0][Ycolname];

            }
        }
    }
}
