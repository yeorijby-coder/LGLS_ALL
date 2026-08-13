using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace HECS.Gui.Monitor.Controls
{
    public partial class BaseControl : Control 
    {
        protected bool selected = false;

        protected bool errorState = false;




        public BaseControl()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            this.PerformLayout();
        }

        protected void DrawEclipse(Graphics g, Rectangle rect, Pen drawColor, Brush fillColor)
        {
            g.FillEllipse(fillColor, rect);
            g.DrawEllipse(drawColor, rect); ;
        }
        protected void DrawRect(Graphics g, Rectangle rect, Brush drawColor, Brush fillColor)
        {
            int penWidth = 1;

            int X = rect.X;
            int Y = rect.Y;
            int width = rect.Width;
            int height = rect.Height;

            g.FillRectangle(drawColor, new Rectangle(X + (penWidth / 2), Y + (penWidth / 2), width - penWidth, height - penWidth));
            Pen p = new Pen(fillColor, penWidth);
            g.DrawRectangle(p, new Rectangle(X + (penWidth / 2), Y + (penWidth / 2), width - penWidth, height - penWidth));

        }
        protected void DrawRoundRect(Graphics g, Rectangle rect, int radius, Brush drawColor, Brush fillColor, int penWidth)
        {
            //int penWidth = 2;

            int X = rect.X;
            int Y = rect.Y;
            int width = rect.Width;
            int height = rect.Height;

            Pen p = new Pen(fillColor, penWidth);   //·¢¸¶
            g.FillRectangle(drawColor, new Rectangle(X + (penWidth / 2), Y + (penWidth / 2), width - penWidth, height - penWidth));

            GraphicsPath gp = new GraphicsPath();
            gp.AddLine(X + radius, Y, X + width - (radius * 2), Y);
            gp.AddArc(X + width - (radius * 2), Y, radius * 2, radius * 2, 270, 90);
            gp.AddLine(X + width, Y + radius, X + width, Y + height - (radius * 2));
            gp.AddArc(X + width - (radius * 2), Y + height - (radius * 2), radius * 2, radius * 2, 0, 90);
            gp.AddLine(X + width - (radius * 2), Y + height, X + radius, Y + height);
            gp.AddArc(X, Y + height - (radius * 2), radius * 2, radius * 2, 90, 90);
            gp.AddLine(X, Y + height - (radius * 2), X, Y + radius);
            gp.AddArc(X, Y, radius * 2, radius * 2, 180, 90);
            gp.CloseFigure();
            g.DrawPath(p, gp);
            gp.Dispose();
        }

        protected static GraphicsPath CreateRoundRectangle(Rectangle rectangle, int radius)
        {
            GraphicsPath path = new GraphicsPath();
            int l = rectangle.Left;
            int t = rectangle.Top;
            int w = rectangle.Width;
            int h = rectangle.Height;
            int d = radius << 1;
            path.AddArc(l, t, d, d, 180, 90); // topleft
            path.AddLine(l + radius, t, l + w - radius, t); // top
            path.AddArc(l + w - d, t, d, d, 270, 90); // topright
            path.AddLine(l + w, t + radius, l + w, t + h - radius); // right
            path.AddArc(l + w - d, t + h - d, d, d, 0, 90); // bottomright
            path.AddLine(l + w - radius, t + h, l + radius, t + h); // bottom
            path.AddArc(l, t + h - d, d, d, 90, 90); // bottomleft
            path.AddLine(l, t + h - radius, l, t + radius); // left
            path.CloseFigure();
            return path;
        }

        protected static GraphicsPath CreateTopRoundRectangle(Rectangle rectangle, int radius)
        {
            GraphicsPath path = new GraphicsPath();
            int l = rectangle.Left;
            int t = rectangle.Top;
            int w = rectangle.Width;
            int h = rectangle.Height;
            int d = radius << 1;
            path.AddArc(l, t, d, d, 180, 90); // topleft
            path.AddLine(l + radius, t, l + w - radius, t); // top
            path.AddArc(l + w - d, t, d, d, 270, 90); // topright
            path.AddLine(l + w, t + radius, l + w, t + h); // right
            path.AddLine(l + w, t + h, l, t + h); // bottom
            path.AddLine(l, t + h, l, t + radius); // left
            path.CloseFigure();
            return path;
        }

        protected static GraphicsPath CreateBottomRadialPath(Rectangle rectangle)
        {
            GraphicsPath path = new GraphicsPath();
            RectangleF rect = rectangle;
            rect.X -= rect.Width * .35f;
            rect.Y -= rect.Height * .15f;
            rect.Width *= 1.7f;
            rect.Height *= 2.3f;
            path.AddEllipse(rect);
            path.CloseFigure();
            return path;
        }
    }
}
