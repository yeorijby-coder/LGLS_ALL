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

namespace ECP.Gui.Widget
{
    public partial class LineWidgetWithArrow : LineWidget
    {
        public LineWidgetWithArrow() :base()
        {
            InitializeComponent();
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

                buildPolygonWithArrow(widgetData.x - originX, widgetData.y - originY,
                    widgetData.width - originX, widgetData.height - originY);
            }
        }

        public override void SetPoint(int whichPoint, Point point)
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

            buildPolygonWithArrow(widgetData.x - originX, widgetData.y - originY,
             widgetData.width - originX, widgetData.height - originY);

        }
    }
}
