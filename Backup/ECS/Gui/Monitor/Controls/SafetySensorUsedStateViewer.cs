using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Controls
{
    public partial class SafetySensorUsedStateViewer : UserControl
    {
        public SafetySensorUsedStateViewer()
        {
            InitializeComponent();
        }
        private bool isHPSensorOn = false;
        public bool IsHPSensorOn
        {
            get { return isHPSensorOn; }
            set { isHPSensorOn = value; Invalidate(); }
        }

        private bool isMDSensorOn = false;
        public bool IsMDSensorOn
        {
            get { return isMDSensorOn; }
            set { isMDSensorOn = value; Invalidate(); }
        }

        private bool isOPSensorOn = false;
        public bool IsOPSensorOn
        {
            get { return isOPSensorOn; }
            set { isOPSensorOn = value; Invalidate(); }
        }

        private bool isPortSideSensorOn = false;
        public bool IsPortSideSensorOn
        {
            get { return isPortSideSensorOn; }
            set { isPortSideSensorOn = value; Invalidate(); }
        }

        private bool isEqpSideSensorOn = false;
        public bool IsEqpSideSensorOn
        {
            get { return isEqpSideSensorOn; }
            set { isEqpSideSensorOn = value; Invalidate(); }
        }

        private bool isDual = true;
        public bool IsDual
        {
            get { return isDual; }
            set { isDual = value; Invalidate(); }
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

        }
        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);
        }

        private void DrawDualMode()
        {
            panelHPSensor.Visible = true;
            panelMidSensor.Visible = true;
            panelOPSensor.Visible = true;
            panelPortSensor.Visible = true;
            panelEQPSensor.Visible = true;
            panelHomeRM.Visible = true;
            panelRearRM.Visible = true;
            panelPoint7.Visible = true;
            panelPoint8.Visible = true;
            panelHPSensor.BackColor = (this.isHPSensorOn) ? Color.Lime: Color.Yellow;
            panelMidSensor.BackColor = (this.isMDSensorOn) ? Color.Lime : Color.Yellow;
            panelOPSensor.BackColor = (this.isOPSensorOn) ? Color.Lime : Color.Yellow;
            panelPortSensor.BackColor = (this.isPortSideSensorOn) ? Color.Lime : Color.Yellow;
            panelEQPSensor.BackColor = (this.isEqpSideSensorOn) ? Color.Lime : Color.Yellow;
            //STKPORT SAFETY SENSOR
            panelPoint.Size = new Size(31, 41);
            panelPoint.Location = new Point(panelPoint.Width / 2, panelPoint.Width / 2); 
            panelPoint2.Size = new Size(31, 41);
            panelPoint2.Location = new Point(panelViewerBack.Width - panelPoint2.Width - (panelPoint2.Width / 2), panelPoint2.Width / 2);
            panelPortSensor.Location = new Point(panelPoint.Left + (panelPoint.Width / 2), panelPoint.Top + (panelPoint.Height / 4));
            panelPortSensor.Size = new Size(panelPoint2.Left + (panelPoint2.Width / 2) - panelPortSensor.Left, panelPoint.Height / 2);
            //EQPPORT SAFETY SENSOR
            panelPoint3.Size = new Size(31, 41);
            panelPoint3.Location = new Point(panelPoint3.Width / 2, panelViewerBack.Height - panelPoint3.Height - (panelPoint3.Height / 2));
            panelPoint4.Size = new Size(31, 41);
            panelPoint4.Location = new Point(panelViewerBack.Width - panelPoint4.Width - (panelPoint4.Width / 2), panelViewerBack.Height - panelPoint4.Height - (panelPoint4.Height / 2));
            panelEQPSensor.Location = new Point(panelPoint3.Left + (panelPoint3.Width / 2), panelPoint3.Top + (panelPoint3.Height / 4));
            panelEQPSensor.Size = new Size(panelPoint4.Left + (panelPoint4.Width / 2) - panelEQPSensor.Left, panelPoint3.Height / 2);
            
            //RM#1 LOC
            panelHomeRM.Size = new Size(96, 86);
            panelHomeRM.Location = new Point((panelViewerBack.Width / 3) - (panelHomeRM.Width / 2), (panelViewerBack.Height / 2) - (panelHomeRM.Height/2));
            //RM#2 LOC
            panelRearRM.Size = new Size(96, 86);
            panelRearRM.Location = new Point((panelViewerBack.Width - (panelViewerBack.Width / 3)) - (panelRearRM.Width / 2), (panelViewerBack.Height / 2) - (panelRearRM.Height / 2));
            //RM HOME SIDE SAFETY SENSOR
            panelPoint5.Size = new Size(31, 41);
            panelPoint5.Location = new Point(panelPoint5.Width / 2, ((panelViewerBack.Height) / 2) - (panelPoint5.Height / 2));
            panelPoint6.Size = new Size(31, 41);
            panelPoint6.Location = new Point(panelHomeRM.Left - panelPoint6.Width + 1, panelPoint5.Top);
            panelHPSensor.Location = new Point(panelPoint5.Left + (panelPoint5.Width / 2), panelPoint5.Top + (panelPoint5.Height / 4));
            panelHPSensor.Size = new Size(panelPoint6.Left + (panelPoint6.Width / 2) - panelHPSensor.Left, panelPoint5.Height / 2);
            //RM MID SIDE SAFETY SENSOR
            panelPoint7.Size = new Size(31, 41);
            panelPoint7.Location = new Point(panelHomeRM.Left + panelHomeRM.Width - 1, ((panelViewerBack.Height) / 2) - (panelPoint7.Height / 2));
            panelPoint8.Size = new Size(31, 41);
            panelPoint8.Location = new Point(panelRearRM.Left - panelPoint8.Width + 1, panelPoint7.Top);
            panelMidSensor.Location = new Point(panelPoint7.Left + (panelPoint7.Width / 2), panelPoint7.Top + (panelPoint7.Height / 4));
            panelMidSensor.Size = new Size(panelPoint8.Left + (panelPoint8.Width / 2) - panelMidSensor.Left, panelPoint7.Height / 2);
            //RM REAR SIDE SAFETY SENSOR
            panelPoint9.Size = new Size(31, 41);
            panelPoint9.Location = new Point(panelRearRM.Left + panelRearRM.Width - 1, ((panelViewerBack.Height) / 2) - (panelPoint9.Height / 2));
            panelPoint10.Size = new Size(31, 41);
            panelPoint10.Location = new Point(panelViewerBack.Width - panelPoint10.Width - (panelPoint10.Width / 2), panelPoint9.Top);
            panelOPSensor.Location = new Point(panelPoint9.Left + (panelPoint9.Width / 2), panelPoint9.Top + (panelPoint9.Height / 4));
            panelOPSensor.Size = new Size(panelPoint10.Left + (panelPoint10.Width / 2) - panelOPSensor.Left, panelPoint9.Height / 2);
        }

        private void DrawSingleMode()
        {
            panelHPSensor.Visible = true;
            panelMidSensor.Visible = false;
            panelOPSensor.Visible = true;
            panelPortSensor.Visible = true;
            panelEQPSensor.Visible = true;
            panelHomeRM.Visible = true;
            panelRearRM.Visible = false;
            panelPoint7.Visible = false;
            panelPoint8.Visible = false;
            panelHPSensor.BackColor = (this.isHPSensorOn) ? Color.Lime : Color.Yellow;            
            panelOPSensor.BackColor = (this.isOPSensorOn) ? Color.Lime : Color.Yellow;
            panelPortSensor.BackColor = (this.isPortSideSensorOn) ? Color.Lime : Color.Yellow;
            panelEQPSensor.BackColor = (this.isEqpSideSensorOn) ? Color.Lime : Color.Yellow;
            //STKPORT SAFETY SENSOR
            panelPoint.Size = new Size(31, 41);
            panelPoint.Location = new Point(panelPoint.Width / 2, panelPoint.Width / 2);
            panelPoint2.Size = new Size(31, 41);
            panelPoint2.Location = new Point(panelViewerBack.Width - panelPoint2.Width - (panelPoint2.Width / 2), panelPoint2.Width / 2);
            panelPortSensor.Location = new Point(panelPoint.Left + (panelPoint.Width / 2), panelPoint.Top + (panelPoint.Height / 4));
            panelPortSensor.Size = new Size(panelPoint2.Left + (panelPoint2.Width / 2) - panelPortSensor.Left, panelPoint.Height / 2);
            //EQPPORT SAFETY SENSOR
            panelPoint3.Size = new Size(31, 41);
            panelPoint3.Location = new Point(panelPoint.Width / 2, panelViewerBack.Height - panelPoint3.Height - (panelPoint3.Height / 2));
            panelPoint4.Size = new Size(31, 41);
            panelPoint4.Location = new Point(panelViewerBack.Width - panelPoint4.Width - (panelPoint4.Width / 2), panelViewerBack.Height - panelPoint4.Height - (panelPoint4.Height / 2));
            panelEQPSensor.Location = new Point(panelPoint3.Left + (panelPoint3.Width / 2), panelPoint3.Top + (panelPoint3.Height / 4));
            panelEQPSensor.Size = new Size(panelPoint4.Left + (panelPoint4.Width / 2) - panelEQPSensor.Left, panelPoint3.Height / 2);
            //RM#1 LOC
            panelHomeRM.Size = new Size(96, 86);
            panelHomeRM.Location = new Point((panelViewerBack.Width / 2) - (panelHomeRM.Width / 2), (panelViewerBack.Height / 2) - (panelHomeRM.Height / 2));
            //RM HOME SIDE SAFETY SENSOR
            panelPoint5.Size = new Size(31, 41);
            panelPoint5.Location = new Point(panelPoint5.Width / 2, ((panelViewerBack.Height) / 2) - (panelPoint5.Height / 2));
            panelPoint6.Size = new Size(31, 41);
            panelPoint6.Location = new Point(panelHomeRM.Left - panelPoint6.Width + 1, panelPoint5.Top);
            panelHPSensor.Location = new Point(panelPoint5.Left + (panelPoint5.Width / 2), panelPoint5.Top + (panelPoint5.Height / 4));
            panelHPSensor.Size = new Size(panelPoint6.Left + (panelPoint6.Width / 2) - panelHPSensor.Left, panelPoint5.Height / 2);
            //RM REAR SIDE SAFETY SENSOR
            panelPoint9.Size = new Size(31, 41);
            panelPoint9.Location = new Point(panelHomeRM.Left + panelHomeRM.Width - 1, ((panelViewerBack.Height) / 2) - (panelPoint9.Height / 2));
            panelPoint10.Size = new Size(31, 41);
            panelPoint10.Location = new Point(panelViewerBack.Width - panelPoint10.Width - (panelPoint10.Width / 2), panelPoint9.Top);
            panelOPSensor.Location = new Point(panelPoint9.Left + (panelPoint9.Width / 2), panelPoint9.Top + (panelPoint9.Height / 4));
            panelOPSensor.Size = new Size(panelPoint10.Left + (panelPoint10.Width / 2) - panelOPSensor.Left, panelPoint9.Height / 2);

        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (IsDual)
            {
                this.DrawDualMode();
            }
            else
            {
                this.DrawSingleMode();
            }
        }
    }
}
