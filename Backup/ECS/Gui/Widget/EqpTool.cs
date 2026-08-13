using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

using ECP.Gui.Widget;

namespace HECS.Gui.Widget
{
    public class EqpTool : BaseWidget
    {
        public EqpTool()
        {
            Invalidate();
        }

        Rectangle eqpTool = new Rectangle();
        Pen pen = new Pen(Color.Blue, 1);

        private string eqpName = "EqpName";

        public string EqpName
        {
            get { return eqpName; }
            set
            {
                eqpName = value;
                Invalidate();
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            DrawRectangleInt(e.Graphics);
        }


        private void DrawRectangleInt(Graphics g)
        {
            eqpTool.Width = this.Width - 1;
            eqpTool.Height = this.Height - 1;

            //g.SmoothingMode = SmoothingMode.AntiAlias;

            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighSpeed;

            g.FillRectangle(Brushes.Aquamarine, eqpTool);
            g.DrawRectangle(pen, eqpTool);
            TextFormatFlags flags = TextFormatFlags.HorizontalCenter;
            TextRenderer.DrawText(g, this.eqpName, new Font("Arial", 8), eqpTool, Color.Blue, flags);

        }
    }
}