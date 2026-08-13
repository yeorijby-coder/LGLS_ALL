using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;
using System.Xml;
using System.IO;
using System.Windows.Forms;


using HECS.Gui.Monitor.Controls;
using HECS.Global;
 
using HECS.Util;
using HECS.Device;
using HECS.Device.Unit;

using ECP.Util.Client;
using HECS.Gui.Widget;
using System.Drawing;
using System.Drawing.Drawing2D;
 
using ECP.Service.Material;
using ECP.Global.Device;


namespace HECS.Gui.Monitor
{

    public static class MonitorEventHandler
    {

    //    private static List<string> copyRM = new List<string>();
    //    private static Dictionary<string, ControlInfo> widgetList = new Dictionary<string, ControlInfo>();
    //    private static bool isLoadControlInfo = false;
    //    private static double zoomRatio = 100;
    //    private static float rackmasterStep = 1F;
    //    private static int rackmasterOffset = 50;
    //    private static int zoneWidth = 0;
    //    private static int zoneLeft = 1;
    //    private static int widgetLeft = 50;
    //    private static bool isCalculatedRackmasterOffset = false;


    //    //private static int timerTickCount = 0;
    //    private static Panel panel = null;
    //    private static int prevMousePosition = 0;
    //    #region MAIN_FORM_EVENT_HANDLER

    //    /// <summary>
    //    /// Main Form의 종료 처리를 위함
    //    /// </summary>
    //    /// <param name="sender"></param>
    //    /// <param name="e"></param>
    //    public static void MainFormClosingEvent(object sender, FormClosingEventArgs e)
    //    {
    //        e.Cancel = true;

    //        ShutdownForm form = null;
    //        form = (ShutdownForm)SimpleCache.GetObject("Popup", "ShutDown");
    //        if (form == null)
    //        {
    //            form = new ShutdownForm();
    //            SimpleCache.AddObject("Popup", "ShutDown", form);


    //        }
    //        form.Show();

    //    }

    //    #endregion MAIN_FORM_EVENT_HANDLER


     
 
  
    //    private delegate void delegateWidgetContainerDraw();

  


    //    public static void PanelWidgetContainer_Scroll(object sender, MouseEventArgs e)
    //    {
    //        Panel panel = (Panel)sender;
    //        try
    //        {
    //            panel.HorizontalScroll.Value = e.X;
    //        }
    //        catch (Exception)
    //        {
    //        }

    //    }

    //    public static void PanelWidgetContainer_DragOver(object sender, DragEventArgs e)
    //    {
    //        if (sender as Panel == null)
    //        {
    //            return;
    //        }
    //        Panel panel = sender as Panel;
    //        Form stockerFormMain = (Form)SimpleCache.GetObject("FormMain", "StockerMainForm");

    //        int currentMouseX = e.X;
    //        int absoluteRight = stockerFormMain.Left + panel.ClientRectangle.Width;// +panel.DisplayRectangle.Left;
    //        int absoluteLeft = stockerFormMain.Left + panel.ClientRectangle.Left;
    //        if (currentMouseX >= absoluteRight - 30 && panel.HorizontalScroll.Maximum > panel.HorizontalScroll.Value)
    //        {
    //            panel.HorizontalScroll.Value = panel.HorizontalScroll.Value + 20;
    //        }
    //        else if (currentMouseX < absoluteLeft + 70 && panel.HorizontalScroll.Minimum < panel.HorizontalScroll.Value)
    //        {
    //            panel.HorizontalScroll.Value = panel.HorizontalScroll.Value - 20;
    //        }
    //        prevMousePosition = currentMouseX;


    //    }
    //    public static void SendOperationMessage(string message)
    //    {
    //        BottomNavigationPanel panel = (BottomNavigationPanel)SimpleCache.GetObject("FormMain", "BottomNavigationPanel");
    //    }






    //    private static void DoCarrierExport()
    //    {
    //        CarrierMove carrierMove = (CarrierMove)SimpleCache.GetObject("Popup", "CarrierMove");

    //        if (carrierMove == null)
    //        {
    //            carrierMove = new CarrierMove();
    //            SimpleCache.AddObject("Popup", "CarrierMove", carrierMove);
    //        }
    //        carrierMove.Show();
    //    }




    //    #region DrawFunctions
    //    private static void DrawEclipse(Graphics graphic, Rectangle rect, Color backColor, Color foreColor)
    //    {
    //        Pen fgColor = new Pen(foreColor);
    //        Brush backColorBrush = new SolidBrush(backColor);
    //        graphic.FillEllipse(backColorBrush, rect);
    //        graphic.DrawEllipse(fgColor, rect);
    //    }
    //    private static void DrawFillRectangle(Graphics graphic, Rectangle rect, Color foreColor, Color backColor)
    //    {
    //        int penWidth = 1;

    //        int pointX = rect.X;
    //        int pointY = rect.Y;
    //        int width = rect.Width;
    //        int height = rect.Height;
    //        Brush backColorBrush = new SolidBrush(backColor);
    //        Brush foreColorBrush = new SolidBrush(foreColor);

    //        graphic.FillRectangle(foreColorBrush, new Rectangle(pointX + (penWidth / 2), pointY + (penWidth / 2), width - penWidth, height - penWidth));
    //        Pen pen = new Pen(backColor, penWidth);
    //        graphic.DrawRectangle(pen, new Rectangle(pointX + (penWidth / 2), pointY + (penWidth / 2), width - penWidth, height - penWidth));
    //        pen.Dispose();
    //    }
    //    private static void DrawRoundFillRectangle(Graphics graphic, Rectangle rect, int rad, Color foreColor, Color backColor, int penWidth, Brush brush)
    //    {
    //        int pointX = rect.X;
    //        int pointY = rect.Y;
    //        int width = rect.Width;
    //        int height = rect.Height;
    //        GraphicsPath gp = new GraphicsPath();
    //        Pen pen = new Pen(foreColor, penWidth);   //랙마
    //        graphic.FillRectangle(brush, new Rectangle(pointX + (penWidth / 2), pointY + (penWidth / 2), width - penWidth, height - penWidth));
    //        gp.AddLine(pointX + rad, pointY, pointX + width - (rad * 2), pointY);
    //        gp.AddArc(pointX + width - (rad * 2), pointY, rad * 2, rad * 2, 270, 90);
    //        gp.AddLine(pointX + width, pointY + rad, pointX + width, pointY + height - (rad * 2));
    //        gp.AddArc(pointX + width - (rad * 2), pointY + height - (rad * 2), rad * 2, rad * 2, 0, 90);
    //        gp.AddLine(pointX + width - (rad * 2), pointY + height, pointX + rad, pointY + height);
    //        gp.AddArc(pointX, pointY + height - (rad * 2), rad * 2, rad * 2, 90, 90);
    //        gp.AddLine(pointX, pointY + height - (rad * 2), pointX, pointY + rad);
    //        gp.AddArc(pointX, pointY, rad * 2, rad * 2, 180, 90);
    //        gp.CloseFigure();

    //        graphic.DrawPath(pen, gp);
    //        gp.Dispose();
    //    }
    //    private static void DrawRoundRectangle(Graphics graphic, Rectangle rect, int rad, Color foreColor, Color backColor, int penWidth)
    //    {
    //        Pen pen = new Pen(backColor, penWidth);
    //        GraphicsPath gp = new GraphicsPath();
    //        int pointLeft = rect.Left;
    //        int pointTop = rect.Top;
    //        int width = rect.Width;
    //        int height = rect.Height;
    //        int degree = rad << 1;
    //        gp.AddArc(pointLeft, pointTop, degree, degree, 180, 90); // topleft
    //        gp.AddLine(pointLeft + rad, pointTop, pointLeft + width - rad, pointTop); // top
    //        gp.AddArc(pointLeft + width - degree, pointTop, degree, degree, 270, 90); // topright
    //        gp.AddLine(pointLeft + width, pointTop + rad, pointLeft + width, pointTop + height - rad); // right
    //        gp.AddArc(pointLeft + width - degree, pointTop + height - degree, degree, degree, 0, 90); // bottomright
    //        gp.AddLine(pointLeft + width - rad, pointTop + height, pointLeft + rad, pointTop + height); // bottom
    //        gp.AddArc(pointLeft, pointTop + height - degree, degree, degree, 90, 90); // bottomleft
    //        gp.AddLine(pointLeft, pointTop + height - rad, pointLeft, pointTop + rad); // left
    //        gp.CloseFigure();
    //        graphic.DrawPath(pen, gp);
    //        gp.Dispose();
    //    }
    //    private static void DrawTopRoundRectangle(Graphics graphic, Rectangle rect, int rad, Color foreColor, Color backColor, int penWidth)
    //    {
    //        Pen pen = new Pen(backColor, penWidth);
    //        GraphicsPath gp = new GraphicsPath();
    //        int pointLeft = rect.Left;
    //        int pointTop = rect.Top;
    //        int width = rect.Width;
    //        int height = rect.Height;
    //        int degree = rad << 1;
    //        gp.AddArc(pointLeft, pointTop, degree, degree, 180, 90); // topleft
    //        gp.AddLine(pointLeft + rad, pointTop, pointLeft + width - rad, pointTop); // top
    //        gp.AddArc(pointLeft + width - degree, pointTop, degree, degree, 270, 90); // topright
    //        gp.AddLine(pointLeft + width, pointTop + rad, pointLeft + width, pointTop + height); // right
    //        gp.AddLine(pointLeft + width, pointTop + height, pointLeft, pointTop + height); // bottom
    //        gp.AddLine(pointLeft, pointTop + height, pointLeft, pointTop + rad); // left
    //        gp.CloseFigure();
    //        graphic.DrawPath(pen, gp);
    //        gp.Dispose();
    //    }
    //    private static void DrawBottomRadialPath(Graphics graphic, Rectangle rect, Color foreColor, Color backColor, int penWidth)
    //    {
    //        Pen pen = new Pen(backColor, penWidth);
    //        GraphicsPath gp = new GraphicsPath();
    //        RectangleF rectangle = rect;
    //        rectangle.X -= rectangle.Width * .35f;
    //        rectangle.Y -= rectangle.Height * .15f;
    //        rectangle.Width *= 1.7f;
    //        rectangle.Height *= 2.3f;
    //        gp.AddEllipse(rect);
    //        gp.CloseFigure();
    //        graphic.DrawPath(pen, gp);
    //        gp.Dispose();
    //    }


    //    private static GraphicsPath CreateRoundRectangle(Rectangle rectangle, int radius)
    //    {
    //        GraphicsPath path = new GraphicsPath();
    //        int l = rectangle.Left;
    //        int t = rectangle.Top;
    //        int w = rectangle.Width;
    //        int h = rectangle.Height;
    //        int d = radius << 1;
    //        path.AddArc(l, t, d, d, 180, 90); // topleft
    //        path.AddLine(l + radius, t, l + w - radius, t); // top
    //        path.AddArc(l + w - d, t, d, d, 270, 90); // topright
    //        path.AddLine(l + w, t + radius, l + w, t + h - radius); // right
    //        path.AddArc(l + w - d, t + h - d, d, d, 0, 90); // bottomright
    //        path.AddLine(l + w - radius, t + h, l + radius, t + h); // bottom
    //        path.AddArc(l, t + h - d, d, d, 90, 90); // bottomleft
    //        path.AddLine(l, t + h - radius, l, t + radius); // left
    //        path.CloseFigure();
    //        return path;
    //    }
    //    private static GraphicsPath CreateTopRoundRectangle(Rectangle rectangle, int radius)
    //    {
    //        GraphicsPath path = new GraphicsPath();
    //        int l = rectangle.Left;
    //        int t = rectangle.Top;
    //        int w = rectangle.Width;
    //        int h = rectangle.Height;
    //        int d = radius << 1;
    //        path.AddArc(l, t, d, d, 180, 90); // topleft
    //        path.AddLine(l + radius, t, l + w - radius, t); // top
    //        path.AddArc(l + w - d, t, d, d, 270, 90); // topright
    //        path.AddLine(l + w, t + radius, l + w, t + h); // right
    //        path.AddLine(l + w, t + h, l, t + h); // bottom
    //        path.AddLine(l, t + h, l, t + radius); // left
    //        path.CloseFigure();
    //        return path;
    //    }


    //    #endregion
    //}
    //class ControlInfo
    //{
    //    private int left;

    //    public int Left
    //    {
    //        get
    //        {
    //            return left;
    //        }
    //        set
    //        {
    //            left = value;
    //        }
    //    }
    //    private int width;

    //    public int Width
    //    {
    //        get
    //        {
    //            return width;
    //        }
    //        set
    //        {
    //            width = value;
    //        }
    //    }
    //    private int height;

    //    public int Height
    //    {
    //        get
    //        {
    //            return height;
    //        }
    //        set
    //        {
    //            height = value;
    //        }
    //    }
    //    private int top;

    //    public int Top
    //    {
    //        get
    //        {
    //            return top;
    //        }
    //        set
    //        {
    //            top = value;
    //        }
    //    }
    //    private Font font;

    //    public Font Font
    //    {
    //        get
    //        {
    //            return font;
    //        }
    //        set
    //        {
    //            font = value;
    //        }
    //    }
    }
}
