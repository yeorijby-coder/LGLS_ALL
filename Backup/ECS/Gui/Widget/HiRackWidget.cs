using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Service.Movement;
using ECP.Db.Entity;

namespace HECS.Gui.Widget
{
    public partial class HiRackWidget : HECSBaseWidget
    {
        Timer refreshTimer = new Timer();
        private int totalCells = 0;
        private int totalEmptyCell = 0;
        private int totalReservedCell = 0;
        private int totalFullCell = 0;
        private int totalHoldCell = 0;
        public HiRackWidget()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.ContainerControl, false);
            refreshTimer.Interval = 5000;
            refreshTimer.Enabled = true;
            refreshTimer.Start();
            refreshTimer.Tick += new EventHandler(refreshTimer_Tick);
        }

        void refreshTimer_Tick(object sender, EventArgs e)
        {
            try
            {
                HirackData hiRack = new HirackData();
                totalCells = hiRack.getHirackDataCountByRackAndCellStatus("RACK:" + WidgetText, "%");
                totalEmptyCell = hiRack.getHirackDataCountByRackAndCellStatus("RACK:" + WidgetText, "Empty");
                totalReservedCell = hiRack.getHirackDataCountByRackAndCellStatus("RACK:" + WidgetText, "Reserved");
                totalHoldCell = hiRack.getHirackDataCountByRackAndCellStatus("RACK:" + WidgetText, "Hold");
                totalFullCell = hiRack.getHirackDataCountByRackAndCellStatus("RACK:" + WidgetText, "Full");
                Refresh();
            }
            catch (Exception)
            {
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Graphics graphics = e.Graphics;
            if (totalCells <= 0)
            {
                return;
            }


        

            int emptyHeight = (int)(totalEmptyCell * (this.Height - 10) / (double)totalCells);
            int reservedHeight = (int)(totalReservedCell * (this.Height - 10) / (double)totalCells);
            int fullHeight = (int)(totalFullCell * (this.Height - 10) / (double)totalCells);
            int holdHeight = (int)(totalHoldCell * (this.Height - 10) / (double)totalCells);

            if (totalEmptyCell > 0 && emptyHeight <=5)
            {
                emptyHeight = 1 * totalEmptyCell;
            }

            if (totalFullCell > 0 && fullHeight <= 5)
            {
                fullHeight = 1 * totalFullCell;
            }
            if (totalHoldCell > 0 && holdHeight <= 5)
            {
                holdHeight = 1 * totalHoldCell;
            }
            if (totalReservedCell > 0 && reservedHeight <= 5)
            {
                reservedHeight = 1 * totalReservedCell;
            }

            DrawFillRectangle(graphics, new Rectangle(5, this.Height - emptyHeight - 5, 3, emptyHeight), Color.Blue, Color.Blue);
            DrawFillRectangle(graphics, new Rectangle(9, this.Height - fullHeight - 5, 3, fullHeight), Color.Green, Color.Green);
            DrawFillRectangle(graphics, new Rectangle(13, this.Height - holdHeight - 5, 3, holdHeight), Color.Crimson, Color.Crimson);
            DrawFillRectangle(graphics, new Rectangle(17, this.Height - reservedHeight - 5, 3, reservedHeight), Color.OrangeRed , Color.OrangeRed);

            StringFormat stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;
            graphics.DrawString(this.WidgetText, WidgetTextFont, new SolidBrush(this.WidgetTextColor), new RectangleF(0, this.Height / 2, this.Width, this.Height / 2), stringFormat);

        }
    }
}
