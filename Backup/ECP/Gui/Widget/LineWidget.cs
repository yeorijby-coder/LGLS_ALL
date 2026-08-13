using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using System.Drawing.Drawing2D;
using ECP.Global.Gui;
using ECP.Gui.Util;
using ECP.Global;

namespace ECP.Gui.Widget
{
    public partial class LineWidget : BaseWidget
    {
        private static Dictionary<string, LineWidget> lineWidgetByName =
             new Dictionary<string, LineWidget>();

        public const int POINT_1 = 1;

        public const int POINT_2 = 2;            

        GraphicsPath linePath = null;
     
        protected float THETA = 0.5f;

        protected float ARROW_WIDTH = 12;

        protected const int LINEWIDTH = 7;

        protected int axOffset = 0;
        protected int ayOffset = 0;

        protected int originX, originY;
     
        public LineWidget()
        {
            InitializeComponent();
           this.SetStyle(ControlStyles.OptimizedDoubleBuffer |
             ControlStyles.AllPaintingInWmPaint |
             ControlStyles.UserPaint, true);
        }

        override public WidgetData WidgetData
        {
            get { return widgetData; }
            set
            {
                this.widgetData = value;

                base.InitializeWidgetProperties();
      
                originX = (widgetData.x < widgetData.width) ? widgetData.x : widgetData.width;
                originY = (widgetData.y < widgetData.height) ? widgetData.y : widgetData.height;

                buildPolygon(widgetData.x - originX, widgetData.y - originY, 
                   widgetData.width- originX, widgetData.height - originY);
             }
        }

     

        public bool Contains(int x, int y)
        {
            return this.linePath.IsVisible(x, y);
        }

        public Rectangle GetLineBounds()
        {
            return this.Bounds;
        }

        public virtual void SetPoint(int whichPoint, Point point)
        {
            switch (whichPoint)
            {
                case LineWidget.POINT_1:
                    this.widgetData.x = point.X;
                    this.widgetData.y = point.Y; 
                    break;
                case LineWidget.POINT_2:
                    this.widgetData.width = point.X;
                    this.widgetData.height = point.Y;
                    break;
            }

            originX = (widgetData.x < widgetData.width) ? widgetData.x : widgetData.width;
            originY = (widgetData.y < widgetData.height) ? widgetData.y : widgetData.height;

        
            buildPolygon(widgetData.x - originX, widgetData.y - originY,
            widgetData.width - originX, widgetData.height - originY);
            
        }

        public Point GetPoint(int whichPoint)
        {
            switch (whichPoint)
            {
                case LineWidget.POINT_1 :
                    return new Point(widgetData.x, widgetData.y);
                case LineWidget.POINT_2 :
                    return new Point(widgetData.width, widgetData.height);
                default :
                    return Point.Empty;
            }
        }

        public override void Subscribe()
        {
            
        }

        private delegate void delegateUpdateWidget();

        public void UpdateWidgetDelegate()
        {
            //if(this.InvokeRequired)
            this.BeginInvoke(new delegateUpdateWidget(UpdateWidget));
        }

        public void UpdateWidget()
        {

        }


        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            Brush brush;
     
            if (linePath != null)
            {
                if (selected)
                {
                    brush = Brushes.Red;
                }
                else
                {
                  //  brush = Brushes.Blue;
                    if(this.WidgetForeColor == Color.Transparent ||
                        this.WidgetForeColor == Color.FromArgb(0,255,255,255))
                        brush = Brushes.Black;
                    else
                        brush = new SolidBrush(this.WidgetForeColor);
                }
                g.SmoothingMode = SmoothingMode.AntiAlias;
                g.FillPath(brush, linePath);            
                g.DrawPath(new Pen(brush, 1), linePath);
              }
        }

        protected void buildPolygon(int x, int y, int w, int h)
        {

            Rectangle oldBounds = Bounds;

            int x1, y1, x2, y2;

            x1 = x;
            y1 = y;
            x2 = w;
            y2 = h;

            double angle;
            double halfWidth = ((double)LINEWIDTH) / 2.0;
            double deltaX = (double)(x2 - x1);
            double deltaY = (double)(y2 - y1);

            if (x1 == x2)
                angle = Math.PI;

            else
                angle = Math.Atan(deltaY / deltaX) + Math.PI / 2;


            int xOffset = (int)(halfWidth * Math.Cos(angle));
            int yOffset = (int)(halfWidth * Math.Sin(angle));

            axOffset = Math.Abs(xOffset)+1;
            ayOffset = Math.Abs(yOffset)+1;
            
            
            Point[] corners = new Point[]
            {
                new Point(x1-xOffset+axOffset, y1-yOffset+ayOffset),
                new Point(x2-xOffset+axOffset, y2-yOffset+ayOffset),
                new Point(x2+xOffset+axOffset, y2+yOffset+ayOffset),
                new Point(x1+xOffset+axOffset, y1+yOffset+ayOffset)
            };
            
            linePath = new GraphicsPath();
                   
            linePath.StartFigure();
            linePath.AddPolygon(corners);
            linePath.CloseFigure();

            this.Region = new Region(linePath);            
            RectangleF b = linePath.GetBounds();
     
            this.SetBounds(originX - axOffset, originY - ayOffset, (int)b.Width + axOffset, (int)b.Height + ayOffset);

            Invalidate(oldBounds);
            Invalidate(); 
        }
        protected void buildPolygonWithArrow(int x, int y, int w, int h)
        {

            Rectangle oldBounds = Bounds;

            Point[] aptArrowHead = new Point[3];

            aptArrowHead[0] = new Point(x, y);

            Vector vecLine =  new Vector( x-w,  y-h);
            Vector vecLeft =  new Vector(-vecLine[1], vecLine[0]);

            float lineLength = vecLine.Length;
            float th = ARROW_WIDTH / (1.0f * lineLength);
            float ta = ARROW_WIDTH / (1.0f * ((float)Math.Tan(THETA / 1.0f)) * lineLength);

            // find the base of the arrow
            PointF pBase = new PointF((aptArrowHead[0].X + -ta * vecLine[0]), 
                (aptArrowHead[0].Y + -ta * vecLine[1])); //base of the arrow

            aptArrowHead[1] = new Point((int)(pBase.X + th * vecLeft[0]), (int)(pBase.Y + th * vecLeft[1]));
            aptArrowHead[2] = new Point((int)(pBase.X + -th * vecLeft[0]), (int)(pBase.Y + -th * vecLeft[1]));

            x = (int)pBase.X;
            y = (int)pBase.Y;

            int x1, y1, x2, y2;

            x1 = x;
            y1 = y;
            x2 = w;
            y2 = h;

            double angle;
            double halfWidth = ((double)LINEWIDTH) / 2.0;
            double deltaX = (double)(x2 - x1);
            double deltaY = (double)(y2 - y1);

            if (x1 == x2)
                angle = Math.PI;

            else
                angle = Math.Atan(deltaY / deltaX) + Math.PI / 2;


            int xOffset = (int)(halfWidth * Math.Cos(angle));
            int yOffset = (int)(halfWidth * Math.Sin(angle));

            //int axOffset = Math.Abs(xOffset) + 1;
            //int ayOffset = Math.Abs(yOffset) + 1;

             axOffset = Math.Abs((int)(th * vecLeft[0]));
             ayOffset = Math.Abs((int)(th * vecLeft[1]));

             Point[] corners;


             if (x1 - xOffset + axOffset < x2 - xOffset + axOffset)
             {

                corners = new Point[]
                {
                    new Point(x1-xOffset+axOffset, y1-yOffset+ayOffset),
                    new Point(x2-xOffset+axOffset, y2-yOffset+ayOffset),
                    new Point(x2+xOffset+axOffset, y2+yOffset+ayOffset),
                    new Point(x1+xOffset+axOffset, y1+yOffset+ayOffset),

                    new Point(aptArrowHead[2].X+axOffset, aptArrowHead[2].Y+ayOffset),
                    new Point(aptArrowHead[0].X+axOffset, aptArrowHead[0].Y+ayOffset),
                    new Point(aptArrowHead[1].X+axOffset, aptArrowHead[1].Y+ayOffset),
                };
             }
             else
             {
                 corners = new Point[]
                {
                    new Point(x1+xOffset+axOffset, y1+yOffset+ayOffset),
                    new Point(x2+xOffset+axOffset, y2+yOffset+ayOffset),
                    new Point(x2-xOffset+axOffset, y2-yOffset+ayOffset),
                    new Point(x1-xOffset+axOffset, y1-yOffset+ayOffset),

                    new Point(aptArrowHead[2].X+axOffset, aptArrowHead[2].Y+ayOffset),
                    new Point(aptArrowHead[0].X+axOffset, aptArrowHead[0].Y+ayOffset),
                    new Point(aptArrowHead[1].X+axOffset, aptArrowHead[1].Y+ayOffset),
                };
             }

            linePath = new GraphicsPath();

            linePath.StartFigure();
            linePath.AddPolygon(corners);
            linePath.CloseFigure();

            this.Region = new Region(linePath);
            RectangleF b = linePath.GetBounds();

            this.SetBounds(originX-axOffset, originY-ayOffset, (int)b.Width + axOffset, (int)b.Height + ayOffset);

            Invalidate(oldBounds);
            Invalidate();
        }
    }
}
